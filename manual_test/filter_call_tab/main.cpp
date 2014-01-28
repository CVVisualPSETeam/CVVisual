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

int main(/*int argc, char *argv[]*/)
{

	/* Create some data for the FilterCallTab: */
	cv::Mat in{100,100,CV_8U};
	cv::Mat out{100,100,CV_8U};
	cvv::impl::CallMetaData data{};
	QString type{"test_type"};
	cvv::impl::FilterCall fc{in, out, data, type};
	cvv::controller::ViewController vc{};
	cvv::gui::FilterCallTab v{"TestFTab", fc, vc};
	cvv::gui::FilterCallTab w{"TestFTab", fc, vc, "test_view"};

	v.show();
	w.show();
	vc.exec();
	return 0;
}
