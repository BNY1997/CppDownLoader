
#include <iostream>
#include <curl/curl.h>

using namespace std;

int main()
{

	CURL* curl = nullptr;
	CURLcode res;

	curl = curl_easy_init();
	if (curl != nullptr) {

		curl_easy_setopt(curl, CURLOPT_URL, "https://fanyi.baidu.com/");

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		curl_easy_cleanup(curl);
	}

	system("pause");
	return 0;
}

