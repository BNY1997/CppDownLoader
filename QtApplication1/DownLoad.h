#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <map>
#include <thread>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <vector>
#include "FDnThreadPool.h"

//size_t writeFunc(void *ptr, size_t size, size_t nmemb, void *userdata);

int progressFunction(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded);

//long getDownloadFileLenth(const char *url);
//
//void downloadFinish(FILE *fp);
//
//void downloadError(std::string errorString);
//
//void workThread(void *pData);

bool download(int threadNum, std::string url, std::string path, std::string fileName);

