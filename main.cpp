#include <QApplication>
#include "Notepad.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	Notepad::instance()->show();
    return app.exec();
}
