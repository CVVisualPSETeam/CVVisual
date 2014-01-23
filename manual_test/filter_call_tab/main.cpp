#include <sstream>

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>

#include "../../src/gui/filter_call_tab.hpp"
#include "../../src/impl/filter_call.hpp"
#include "../../src/controller/view_controller.hpp"
#include "../../include/opencv2/call_meta_data.hpp"

#include <opencv2/core/core.hpp>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	cv::InputArray in{};
	cv::InputArray out{};
	cvv::impl::CallMetaData data{};
	QString type{"test_type"};
	cvv::impl::FilterCall fc{in, out, data, type};
	cvv::gui::FilterCallTab w{"TestFTab", fc, cvv::controller::ViewController{}};
	//QLabel w{"FilterCallTab"};

	w.show();
	return a.exec();
}
