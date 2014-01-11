#include "../src/qutil/collapsable.hpp"

#include <QLabel>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QLabel l("some \ntext \”with \nnewlines\n\n\n\nloooooooooooooooooooooooooong line");

	cvv::qtutil::Collapsable w("TITLE GOES HERE", l);

	w.show();
	return a.exec();
}
