#include <sstream>

#include <QWidget>
#include <QApplication>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../../src/impl/init.hpp"
#include "../../src/view/image_view.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	if(argc < 2)
	{
		std::cerr << "Only execute this with filenames of images as arguments!" << std::endl;
		return -1;
	}
	auto src = cv::imread(argv[1]);

	cvv::impl::initializeFilterAndViews();
	cvv::view::ImageView view{src};
	view.setWindowTitle("Image View Test");
	view.show();

	return a.exec();
}
