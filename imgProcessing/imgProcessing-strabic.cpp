/**
 *  @project: Strabic
 *	@author: Brieuc DANIEL - ESIR2 IN
 *	@date: 2015
 */
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <curl/curl.h>
#include <fstream>
#include <ios>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <sstream>
#include <string>
#include <stdio.h>
#include <sys/stat.h>

/**
 *	###########################################
 *	######     DOWNLOADIMG FUNCTIONS     ######
 *	###########################################
 */
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}

void downloadFile(std::string path, std::string fileName = "")
{
	CURL *curl;
	FILE *fp;
	CURLcode imgRes;
	
	std::stringstream sstm;
	// Assigning the output path in tmp directory
	if(fileName == "")
	{
		sstm << "tmp/" << path.substr(path.find_last_of("/") + 1);
	}
	else
	{
		sstm << "tmp/" << fileName;
	}
	std::string sTemp = sstm.str();
	char *output = (char *) sTemp.c_str();
	char *url = (char *)path.c_str();

	curl = curl_easy_init();
	if(curl)
	{
		std::cout << output << std::endl;
		// Open image and give the write permission
		fp = fopen(output, "w+");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		imgRes = curl_easy_perform(curl);
		// Cleanup
		curl_easy_cleanup(curl); 
		fclose(fp);
	}
}
/* ---------------------------------------------------------------------------- */

/**
 *	#########################################
 *	######     PROCESSIMG FUNCTION     ######
 *	#########################################
 */
void processImg(std::string filePath)
{
	cv::Mat imgSource = cv::imread(filePath, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat imgOutput;
	cv::Mat imgMat;
	
	int h_source = imgSource.rows;
	int w_source = imgSource.cols;
	int origin;

	// Horizontal crop
	if(h_source > w_source)
	{
		origin = (h_source - w_source) / 2;
		imgMat = cv::Mat(imgSource, cv::Rect(0, origin,  w_source-1, w_source-1));
		resize(imgMat, imgOutput, cvSize(150, 150));
	}
	// Vertical crop
	else if(h_source < w_source)
	{
		origin = (w_source - h_source) / 2;
		imgMat = cv::Mat(imgSource, cv::Rect(origin, 0,  h_source-1, h_source-1));
	}
	
	// Resizing image
	resize(imgMat, imgOutput, cvSize(150, 150));

	// Iniatializing image compression
	cv::vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(9);
	// Substract the initial extension
	int extIndex = filePath.find_last_of(".");
	std::string newPath = filePath.substr(0, extIndex);
	// Writing image as png
	imwrite("img/" + filePath.substr(filePath.find_last_of("/") + 1), imgOutput, compression_params);
}

/**
 *	############################################
 *	######    PROCESSFROMURL FUNCTION     ######
 *	############################################
 */
void processFromURL(std::string path)
{
	std::ifstream fileImg(path, std::ios::in);
	// File Opening
	if(fileImg)
	{
		std::string imgPath;
		int nb_error = 0;
		// Create directories
		boost::filesystem::create_directory("tmp");
		boost::filesystem::create_directory("img");
		// Scan all lines in the file
		while(getline(fileImg, imgPath))
		{
			// Dowload the image
			downloadFile(imgPath);
			// Processing the image
			if((std::string)imgPath.substr(imgPath.find_last_of(".") + 1) != "gif")
			{
				processImg("tmp/" + imgPath.substr(imgPath.find_last_of("/") + 1));
			}
			else
			{
				std::cout << "Sorry can't process 'gif' files !" << std::endl;
				nb_error++;
			}
			boost::filesystem::remove("tmp/" + imgPath.substr(imgPath.find_last_of("/") + 1));
		}
		std::cout << "Download Finish (" << nb_error << " errors) !" << std::endl;
		fileImg.close();
		boost::filesystem::remove("tmp");
	}
	else
	{
		std::cout << "Error can't open the file !" << std::endl;
	}
}

/**
 *	######################################################
 *	######     EXTRACTIMAGEFROMWEBPAGE FUNCTION     ######
 *	######################################################
 */

void extractImageFromWebPage(std::string path)
{
	std::ifstream pageFile(path, std::ios::in);
	if(pageFile)
	{
		std::string pagePath;
		// Create directories
		boost::filesystem::create_directory("tmp");
		boost::filesystem::create_directory("img");
		while(getline(pageFile, pagePath))
		{
			pagePath = pagePath.substr(0, pagePath.size() - 1);
			// Download the html file
			downloadFile(pagePath);
			// Opening the local html file
			std::string pageCurrent;
			pageCurrent = pagePath.substr(pagePath.find_last_of("/") + 1);
			std::ifstream webPage("tmp/" + pageCurrent, std::ios::in);
			if(webPage)
			{
				std::string line;				
				boost::regex exp_src(".*(src=('|\"))([a-zA-Z0-9_/ .-]*)(\"|').*", boost::regex::extended);
				boost::regex exp_href(".*(href=('|\"))([a-zA-Z0-9_/ #&;.!?:,()+=*-]*)('|\").*", boost::regex::extended);
				std::string previousLine = "";
				std::string memo_href;
				int nb_error = 0;
				while(getline(webPage, line))
				{		
					std::stringstream sstm;
					sstm << "http://strabic.fr/";
					boost::cmatch result_src, result_href;
					if(previousLine == "<a" && boost::regex_match(line.c_str(), result_href, exp_href))
					{
						memo_href = result_href[3];
					}
					else if(previousLine == "<img" && boost::regex_match(line.c_str(), result_src, exp_src))
					{
						std:: string pathImg = result_src[3];
						sstm << pathImg;
						std::string extension = pathImg.substr(pathImg.find_last_of("."));
						downloadFile(sstm.str(), memo_href + extension);
						if((std::string) pathImg.substr(pathImg.find_last_of(".") + 1) != "gif")
						{
							processImg("tmp/" + memo_href + extension);
							boost::filesystem::remove("tmp/" + memo_href + extension);
						}
						else
						{
							std::cout << "Sorry can't process 'gif' files" << std::endl;
							nb_error++;
						}
					}
					else
					{
						previousLine = line;
					}
				}
				std::cout << std::endl;
			}
			else
			{
				std::cout << "Can't open the online file !" << std::endl;
			}
			boost::filesystem::remove("tmp/" + pageCurrent);
		}
		boost::filesystem::remove("tmp");
	}
	else
	{
		std::cout << "Can't open the file !" << std::endl;
	}
}

/**	
 *	###################################
 *	######     MAIN FUNCTION     ######
 *	###################################
 */
int main(int argc, char *argv[])
{ 	
	std::cout << "#### Strabic Image Processing MENU ####" << std::endl;
	int choice = 1;
	while(choice != 0)
	{
		std::cout << "1. Process images from URL in a file .txt" << std::endl;
		std::cout << "2. Extract images from a webpage (.html)" << std::endl;
		std::cout << "0. Exit program" << std::endl;
		std::cout << std::endl << "Please select a process (number) : ";
		std::cin >> choice;
		// Selecting the file to process
		std::string filePath;
		if(choice == 1 || choice == 2)
		{
			std::cout << "Enter the .txt file with the urls" << std::endl;
			std::cin >> filePath;
		}
		switch(choice)
		{
			case 1:
				processFromURL(filePath);
			break;
			case 2:
				extractImageFromWebPage(filePath);
			break;
			case 0:
				std::cout << "End of programm - Thanks for the use" << std::endl;
			break;
			default:
				std::cout << "Please be sure to make a valid choice !" << std::endl;
			break;
		}
		std::cout << std::endl;
	}
}
