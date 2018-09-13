#pragma once
#ifndef download_h
#define download_h
#include "stdafx.h"
#include <stdio.h>
#include <string> 
#include <iostream>
#include <sstream>  
#include <vector>
#include <locale>
#include <iomanip>
#include "curl.h"
#include "readdata.h"

using namespace std;

struct MemoryStruct {
	char *memory;
	size_t size;
};

void *myrealloc(void *ptr, size_t size)
{
	/* There might be a realloc() out there that doesn't like reallocing
	NULL pointers, so we take care of it here */
	if (ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}


size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)data;

	mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory) {
		memcpy(&(mem->memory[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;
	}
	return realsize;
}

string getTimeinSeconds(string Time)
{
	std::tm t = { 0 };
	std::istringstream ssTime(Time);
	char time[100];
	memset(time, 0, 100);
	if (ssTime >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S"))
	{
		sprintf(time, "%ld", mktime(&t));
		return string(time);
	}
	else
	{
		cout << "Parse failed\n";
		return "";
	}
}

int yahoo(map<string, vector<string>> map3, map<string, vector<double>> &mapofprice, map<string, vector<double>> &mapofsp)
{
	vector<double> adjprice;
	vector<string> symbolList;
	vector<string> startdate;
	vector<string> enddate;

	for (map<string, vector<string>>::iterator it = map3.begin(); it != map3.end(); it++)
	{
		string startTime = getTimeinSeconds(it->second[0]);
		string endTime = getTimeinSeconds(it->second[1]);
		symbolList.push_back(it->first);
		startdate.push_back(startTime);
		enddate.push_back(endTime);
	}

	struct MemoryStruct data;
	data.memory = NULL;
	data.size = 0;
	//file pointer to create file that store the data
	FILE *fp;

	/* declaration of an object CURL */
	CURL *handle;

	/* result of the whole process */
	CURLcode result;

	/* the first functions */
	/* set up the program environment that libcurl needs */
	curl_global_init(CURL_GLOBAL_ALL);

	/* curl_easy_init() returns a CURL easy handle that you're gonna reuse in other easy functions*/
	handle = curl_easy_init();
	/* if everything's all right with the easy handle... */
	if (handle)
	{
		string sCookies, sCrumb;
		if (sCookies.length() == 0 || sCrumb.length() == 0)
		{
			curl_easy_setopt(handle, CURLOPT_URL, "https://finance.yahoo.com/quote/A/history");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
			curl_easy_setopt(handle, CURLOPT_COOKIEJAR, "cookies.txt");
			curl_easy_setopt(handle, CURLOPT_COOKIEFILE, "cookies.txt");

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);

			/* perform, then store the expected code in 'result'*/
			result = curl_easy_perform(handle);

			/* Check for errors */
			if (result != CURLE_OK)
			{
				/* if errors have occured, tell us what is wrong with 'result'*/
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}

			char cKey[] = "CrumbStore\":{\"crumb\":\"";
			char *ptr1 = strstr(data.memory, cKey);
			char *ptr2 = ptr1 + strlen(cKey);
			char *ptr3 = strstr(ptr2, "\"}");
			if (ptr3 != NULL)
				*ptr3 = NULL;

			sCrumb = ptr2;

			fp = fopen("cookies.txt", "r");
			char cCookies[100];
			if (fp) {
				while (fscanf(fp, "%s", cCookies) != EOF);
				fclose(fp);
			}
			else
				cerr << "cookies.txt does not exists" << endl;

			sCookies = cCookies;
			free(data.memory);
			data.memory = NULL;
			data.size = 0;
		}
		for (int i = 0; i<symbolList.size(); i++)
		{
			// stocks
			string urlA = "https://query1.finance.yahoo.com/v7/finance/download/";
			string symbol = symbolList[i];
			//cout << "symbol: " << symbolList[i] << endl;
			string urlB = "?period1=";
			string urlC = "&period2=";
			string urlD = "&interval=1d&events=history&crumb=";
			string url = urlA + symbol + urlB + startdate[i] + urlC + enddate[i] + urlD + sCrumb;
			const char * cURL = url.c_str();
			const char * cookies = sCookies.c_str();
			curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
			curl_easy_setopt(handle, CURLOPT_URL, cURL);

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
			result = curl_easy_perform(handle);

			/* Check for errors */
			if (result != CURLE_OK)
			{
				/* if errors have occurred, tell us what is wrong with 'result'*/
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}

			stringstream sData;
			sData.str(data.memory);
			string sValue, sDate;
			double dValue = 0;
			string line;
			getline(sData, line);
			while (getline(sData, line)) {
				sDate = line.substr(0, line.find_first_of(','));
				line.erase(line.find_last_of(','));
				sValue = line.substr(line.find_last_of(',') + 1);
				dValue = strtod(sValue.c_str(), NULL);
				adjprice.push_back(dValue);
				//cout << sDate << " " << std::fixed << ::setprecision(6) << dValue << endl;
			}
			//cout << "  " << adjprice.size() << "  ";

			mapofprice.insert(pair<string, vector<double>>(symbolList[i], adjprice));
			adjprice.clear();
			// free data
			free(data.memory);
			data.memory = NULL;
			data.size = 0;

			// S&P 500
			string symbolsp = "%5EGSPC";
			string urlsp = urlA + symbolsp + urlB + startdate[i] + urlC + enddate[i] + urlD + sCrumb;
			const char * cURLsp = urlsp.c_str();
			//const char * cookies = sCookies.c_str();
			curl_easy_setopt(handle, CURLOPT_COOKIE, cookies);   // Only needed for 1st stock
			curl_easy_setopt(handle, CURLOPT_URL, cURLsp);

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
			result = curl_easy_perform(handle);

			/* Check for errors */
			if (result != CURLE_OK)
			{
				/* if errors have occurred, tell us what is wrong with 'result'*/
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				return 1;
			}

			stringstream sDatasp;
			sDatasp.str(data.memory);
			string sValuesp, sDatesp;
			double dValuesp = 0;
			string linesp;
			getline(sDatasp, linesp);
			while (getline(sDatasp, linesp)) {
				sDatesp = linesp.substr(0, linesp.find_first_of(','));
				linesp.erase(linesp.find_last_of(','));
				sValuesp = linesp.substr(linesp.find_last_of(',') + 1);
				dValuesp = strtod(sValuesp.c_str(), NULL);
				adjprice.push_back(dValuesp);
				//cout << sDate << " " << std::fixed << ::setprecision(6) << dValue << endl;
			}

			mapofsp.insert(pair<string, vector<double>>(symbolList[i], adjprice));
			cout << i;
			adjprice.clear();
			// free data
			free(data.memory);
			data.memory = NULL;
			data.size = 0;
		}
		free(data.memory);
		data.size = 0;
	}
	else
	{
		fprintf(stderr, "Curl init failed!\n");
		return 1;
	}

	/* cleanup since you've used curl_easy_init */
	curl_easy_cleanup(handle);

	/* this function releases resources acquired by curl_global_init() */
	curl_global_cleanup();
	return 0;
}

#endif // !download_h
