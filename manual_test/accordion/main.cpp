#include <sstream>

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>

#include "acctester.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Acctester w{};

	w.show();
	return a.exec();
}
