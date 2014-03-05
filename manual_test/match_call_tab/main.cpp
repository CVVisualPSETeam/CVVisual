#include <sstream>
#include <vector>
#include <functional>
#include <memory>

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QApplication>

#include "../../src/gui/match_call_tab.hpp"
#include "../../src/impl/match_call.hpp"
#include "../../include/opencv2/call_meta_data.hpp"
#include "../../src/view/linematchview.hpp"
#include "../../src/view/rawview.hpp"
#include "../../src/view/match_view.hpp"

#include "../../src/util/util.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
	cvv::dbg::setLoggingState(false); // Disable tracepoints.

	/* Create some data for the MatchCallTab: */
	cv::Mat src{ 1000, 1000, CV_8U };
	cv::Mat train{ 1000, 1000, CV_8U };
	cvv::impl::CallMetaData data{};
	QString type{ "test_type" };

	QApplication vc{ argc, argv };

	std::vector<cv::KeyPoint> key1;

	for (int i = 0; i < std::min(src.rows, src.cols); i += 30)
	{
		cv::Point2f pt{ static_cast<float>(i), static_cast<float>(i) };
		key1.emplace_back(pt, 0.0f);
	}
	std::vector<cv::KeyPoint> key2;
	for (int i = 0; i < std::min(train.rows, train.cols); i += 30)
	{
		cv::Point2f pt{ static_cast<float>(i), static_cast<float>(i) };
		key2.emplace_back(pt, 0.0f);
	}
	std::vector<cv::DMatch> match;
	for (size_t i = 0; i < std::min(key1.size(), key2.size()); i++)
	{
		match.emplace_back(i, i, 1.0f);
	}

	cvv::impl::MatchCall mc{ src,  key1, train,              key2, match,
		                 data, type, "some description", "",   true };

	cvv::gui::MatchCallTab::registerMatchView<cvv::view::LineMatchView>(
	    "LineMatchView");
	cvv::gui::MatchCallTab::registerMatchView<cvv::view::Rawview>(
	    "RawView");

	cvv::gui::MatchCallTab v{ mc };
	cvv::gui::MatchCallTab w{ mc, "RawView" };
	v.show();
	w.show();
	vc.exec();
	return 0;
}
