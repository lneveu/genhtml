/**
 *  @project: Strabic
 *	@author: Brieuc DANIEL - ESIR2 IN
 *	@date: 2015
 */

#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>

using namespace std;

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

void downloadImg(string path)
{
	CURL *curl;
	FILE *fp;
	CURLcode imgRes;
	
	stringstream sstm;
	sstm << path.substr(path.find_last_of("/") + 1);
	string sTemp = sstm.str();
	char *output = (char *) sTemp.c_str();
	char *url = (char *)path.c_str();

	curl = curl_easy_init();
	if(curl)
	{
		cout << output << endl;
		fp = fopen(output, "w+");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		imgRes = curl_easy_perform(curl);
		// Cleanup !!!
		curl_easy_cleanup(curl); 
		fclose(fp);
	}
}

int main(int argc, char *argv[])
{ 	
	cout << argv[1];
	if(argc == 2)
	{
		// Loop extract url from .txt
		ifstream file(argv[1], ios::in);
		if(file)
		{
			cout << endl;
			string imgPath;
			int i = 1;
			while(getline(file, imgPath))
			{
				// Download the image
				downloadImg(imgPath);
				cout << "Download successfully done !" << endl;
			}
			file.close();
		}
	}
	else
	{
		cout << "You must enter a single parameter the path of the file you must read !" << endl;
	}

	return 0;
}
