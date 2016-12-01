#ifndef MYQAPP_H_
#define MYQAPP_H_

#include <QApplication>

class MYQapp
{
		static MYQapp* instance;
	protected:
		MYQapp();
	public:
		virtual ~MYQapp();
		static MYQapp* getInstance();

		void processEvents();
	private:
		QApplication* app;
};

#endif