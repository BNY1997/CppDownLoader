#include "DownLoad.h"
int currThreadCnt;
int totalThreadNum;
long totalDownloadSize;
bool errorFlag;
using std::map;
using std::cout;
using std::endl;
using std::string;
using std::vector;
map <int, long> downloadMap;
std::mutex m;

struct downloadNode
{
	FILE *fp;
	long startPos;
	long endPos;
	CURL *curl;
	int index;
};

static size_t writeFunc(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	downloadNode *node = (downloadNode *)userdata;
	size_t written = 0;
	m.lock();
	if (node->startPos + size * nmemb <= node->endPos)
	{
		fseek(node->fp, node->startPos, SEEK_SET);
		written = fwrite(ptr, size, nmemb, node->fp);
		node->startPos += size * nmemb;
	}
	else
	{
		fseek(node->fp, node->startPos, SEEK_SET);
		written = fwrite(ptr, 1, node->endPos - node->startPos + 1, node->fp);
		node->startPos = node->endPos;
	}
	m.unlock();
	return written;
}

static int progressFunction(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
	if (totalToDownload > 0 && nowDownloaded > 0)
	{
		m.lock();
		long size = 0L;
		downloadNode *pNode = (downloadNode*)ptr;
		downloadMap[pNode->index] = nowDownloaded;
		map <int, long>::iterator i = downloadMap.begin();
		while (i != downloadMap.end())
		{
			size += i->second;
			++i;
		}
		size = size - long(totalThreadNum) + 1L;
		cout << "currentDownloadSize: " << size << " tototalSize:" << totalDownloadSize << endl;
		m.unlock();
	}
	return 0;
}

long getDownloadFileLenth(const char *url)
{
	double downloadFileLenth = 0;
	CURL *handle = curl_easy_init();
	curl_easy_setopt(handle, CURLOPT_URL, url);
	curl_easy_setopt(handle, CURLOPT_HEADER, 1);
	curl_easy_setopt(handle, CURLOPT_NOBODY, 1);
	if (curl_easy_perform(handle) == CURLE_OK)
	{
		curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth);
	}
	else
	{
		downloadFileLenth = -1;
	}
	return downloadFileLenth;
}

void downloadFinish(FILE *fp)
{
	m.lock();
	currThreadCnt--;
	m.unlock();
	if (currThreadCnt == 0)
	{
		fclose(fp);
		cout << "download succed......" << endl;
	}
}

void downloadError(string errorString)
{
	if (errorFlag)
		return;
	errorFlag = true;
	cout << "download error " << errorString << endl;
}

void workThread(void *pData)
{
	downloadNode *pNode = (downloadNode *)pData;

	CURLcode curlcode = curl_easy_perform(pNode->curl);
	if (curlcode == CURLE_OK)
	{
		cout << "####thread " << pNode->index << "###Downlaod ok" << endl;
		downloadFinish(pNode->fp);
	}
	else
	{
		cout << "####thread " << pNode->index << "#########Downlaod error,Error code:" + curlcode << endl;
		downloadError("Downlaod error, Error code : " + curlcode);
	}
	curl_easy_cleanup(pNode->curl);

	delete pNode;
}


bool download(int threadNum, string url, string path, string fileName)
{
	totalThreadNum = threadNum;
	long fileLength = getDownloadFileLenth(url.c_str());
	if (fileLength <= 0)
	{
		cout << "get the file length error...";
		return false;
	}
	totalDownloadSize = fileLength;
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>totalDownloadsize:" << totalDownloadSize << endl;
	downloadMap.clear();
	errorFlag = false;
	const string outFileName = path + fileName;
	FILE *fp = fopen(outFileName.c_str(), "wb");
	if (!fp)
	{
		return false;
	}
	vector<std::thread> threadList;
	ThreadPool threads(threadNum);
	for (int i = 0; i < threadNum; i++)
	{
		downloadNode *pNode = new downloadNode();
		pNode->startPos = fileLength * i / threadNum;
		pNode->endPos = fileLength * (i + 1) / threadNum;
		CURL *curl = curl_easy_init();

		pNode->curl = curl;
		pNode->fp = fp;
		pNode->index = i + 1;

		char range[64] = { 0 };

#ifdef _WIN32
		_snprintf(range, sizeof(range), "%ld-%ld", pNode->startPos, pNode->endPos);
#else
		snprintf(range, sizeof(range), "%ld-%ld", pNode->startPos, pNode->endPos);
#endif
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)pNode);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progressFunction);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, (void *)pNode);;
		curl_easy_setopt(curl, CURLOPT_RANGE, range);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0L);
		curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1L);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		m.lock();
		currThreadCnt++;
		m.unlock();
		threads.AddTask(workThread, pNode);
	}

	return true;

}