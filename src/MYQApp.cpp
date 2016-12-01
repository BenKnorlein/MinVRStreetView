#include "MYQApp.h"
#include <iostream>
#include <QThread>

MYQapp* MYQapp::instance = NULL;

MYQapp::MYQapp()
{
	static int argc = 1;
	static char* argv[] = { (char*)"dummy.exe", NULL };
	app = new QApplication(argc, argv);
}

MYQapp::~MYQapp()
{
	delete app;
}

MYQapp* MYQapp::getInstance()
{
	if (!instance)
	{
		instance = new MYQapp();
	}
	return instance;
}

void MYQapp::processEvents()
{
	app->processEvents();
}
