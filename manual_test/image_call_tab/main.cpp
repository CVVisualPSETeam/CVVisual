#include <sstream>

#include <QWidget>
#include <QApplication>

#include "../../src/gui/image_call_tab.hpp"
#include "../../src/impl/single_image_call.hpp"
#include "../../src/controller/view_controller.hpp"
#include "../../include/opencv2/call_meta_data.hpp"
#include "../../src/util/util.hpp"

#include <opencv2/core/core.hpp>

int main(int argc, char *argv[])
{

	/* Create some data for the ImageCallTab: */
	cv::Mat mat{ 100, 100, CV_8U };
	cvv::impl::CallMetaData data{};
	QString type{ "test_type" };
	cvv::impl::SingleImageCall call{
		mat, data, type, "some description", ""
	};
	QApplication vc{ argc, argv };

	cvv::gui::ImageCallTab u{ call };
	cvv::gui::ImageCallTab v{ "TestITab", call };
	u.show();
	v.show();
	vc.exec();
	return 0;
}
