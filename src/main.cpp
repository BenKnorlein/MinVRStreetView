///\file main.cpp
///\author Benjamin Knorlein
///\date 08/10/2016

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS

#ifndef WITH_CONSOLE
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <windows.h>
#endif



#include "QTImageDownload.h"
#include <iostream>
#include <chrono>
#include <ctime>
std::vector<ImageGrab*> grabber;


int main(int argc, char** argv)
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	PanoIDGrabber * panograb = new PanoIDGrabber(51.5991335, -2.0558616);
	while (!panograb->hasDataReady())
	{
		MYQapp::getInstance()->processEvents();
	}

	QString panoID = panograb->getPanoID();
	
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cerr << "Time to get PanoID in sec" << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << std::endl;

	std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();

	for (int y = 0; y < 13; y++){
		for (int x = 0; x < 26; x++){
			grabber.push_back(new ImageGrab(panoID, x, y));
		}
	}

	while (grabber.size() != 0)
	{
		MYQapp::getInstance()->processEvents();
		for (int i = grabber.size() - 1; i >= 0; i--) 
		{
			if (grabber[i]->hasDataReady())
			{
				delete grabber[i];
				grabber.erase(grabber.begin() + i);
			}
		}
	}
	
	std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
	std::cerr << "Time to download and save images in sec" << std::chrono::duration_cast<std::chrono::seconds>(end2 - begin2).count() << std::endl;

	delete MYQapp::getInstance();
	return 1;
}
