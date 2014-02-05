#include "../../src/qtutil/collapsable.hpp"
#include "../../src/util/util.hpp"

#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	auto l = cvv::util::make_unique<QLabel>("some \ntext \"with \nnewlines\n\n\n\nloooooooooooooooooooooooooong line");

	cvv::qtutil::Collapsable w("TITLE GOES HERE", std::move(l));

	w.show();
	return a.exec();
}
