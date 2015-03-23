/**
 *  @project: Strabic
 *	@author: Brieuc DANIEL - ESIR2 IN
 *	@date: 2015
 */

#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <sstream>
#include <string>
#include <stdio.h>

using namespace cv;
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
	sstm << "tmp/" << path.substr(path.find_last_of("/") + 1);
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

void processImg(string filePath)
{
	Mat imgSource = imread(filePath, CV_LOAD_IMAGE_GRAYSCALE);
	Mat imgOutput;
	int h_source = imgSource.rows;
	int w_source = imgSource.cols;
	int origin; 
	if(h_source > w_source)
	{
		origin = (h_source - w_source) / 2;
		resize(imgSource, imgOutput, cvSize(200, 200));
	}
	else if(h_source < w_source)
	{
		origin = (w_source - h_source) / 2;
		resize(imgSource, imgOutput, cvSize(200, 200));
	}
	else
	{
		resize(imgSource, imgOutput, cvSize(200,200));
	}
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	// Substract the initial extension
	int extIndex = filePath.find_last_of(".");
	string newPath = filePath.substr(0, extIndex);
	imwrite("img/" + filePath.substr(filePath.find_last_of("/") + 1), imgOutput, compression_params);
}

int main(int argc, char *argv[])
{ 	
	// Loop extract url from .txt
	ifstream file(argv[1], ios::in);
	if(file)
	{
		string imgPath;
		while(getline(file, imgPath))
		{
			// Download the image
			downloadImg(imgPath);
			// Processing the image
			if((string) imgPath.substr(imgPath.find_last_of(".") + 1) != "gif")
			{
				processImg("tmp/" + imgPath.substr(imgPath.find_last_of("/") + 1));
			}
			else
			{
				cout << "Sorry can't process 'gif' files !" << endl;
			}
		}
		cout << "Download succesfully done !" << endl;
		file.close();
	}
	else
	{
		cout << "Error: Can't open the file !";
	}


	return 0;
}
