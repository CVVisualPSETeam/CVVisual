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
#include "../../src/controller/view_controller.hpp"
#include "../../include/opencv2/call_meta_data.hpp"
#include "../../src/view/linematchview.hpp"
#include "../../src/view/rawview.hpp"
#include "../../src/view/match_view.hpp"

#include "../../src/util/util.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

std::unique_ptr<cvv::view::MatchView> makeLineMatchView(const cv::Mat& img1,
		const std::vector<cv::KeyPoint>& key1, const cv::Mat& img2,
		const std::vector<cv::KeyPoint>& key2, const std::vector<cv::DMatch>& match, QWidget* parent)
{
	return cvv::util::make_unique<cvv::view::LineMatchView>(key1, key2, match, img1, img2, parent);
}

int main(/*int argc, char *argv[]*/)
{

	/* Create some data for the MatchCallTab: */
	cv::Mat src{1000,1000,CV_8U};
	cv::Mat train{1000,1000,CV_8U};
	cvv::impl::CallMetaData data{};
	QString type{"test_type"};

	cvv::controller::ViewController vc{};

	std::vector<cv::KeyPoint> key1;

	for(int i=0;i<std::min(src.rows,src.cols);i+=30)
	{
		cv::Point2f pt{static_cast<float>(i),static_cast<float>(i)};
		key1.emplace_back(pt,0.0f);
	}
	std::vector<cv::KeyPoint> key2;
	for(int i=0;i<std::min(train.rows,train.cols);i+=30)
	{
		cv::Point2f pt{static_cast<float>(i),static_cast<float>(i)};
		key2.emplace_back(pt,0.0f);
	}
	std::vector<cv::DMatch> match;
	for(size_t i=0;i<std::min(key1.size(),key2.size());i++)
	{
		match.emplace_back(i,i,1.0f);
	}

	cvv::impl::MatchCall mc{src, key1, train, key2, match, data, type, "some description", ""};

	std::function<std::unique_ptr<cvv::view::MatchView>(const cv::Mat&, const std::vector<cv::KeyPoint>&, const cv::Mat&,
			const std::vector<cv::KeyPoint>&, const std::vector<cv::DMatch>&, QWidget*)> mlmv = makeLineMatchView;
	cvv::gui::MatchCallTab::addMatchViewToMap("LineMatchView", mlmv);

	cvv::gui::MatchCallTab::addMatchViewToMap("RawView",
						[] (const cv::Mat& img1, const std::vector<cv::KeyPoint>& key1,
							const cv::Mat& img2, const std::vector<cv::KeyPoint>& key2,
							const std::vector<cv::DMatch>& match, QWidget* parent)
						{	(void) img1; (void) img2; (void) parent;
							std::vector<std::vector<cv::DMatch>> vect{};
							vect.push_back(match);
							return cvv::util::make_unique<cvv::view::Rawview>(key1, key2, vect); });

	cvv::gui::MatchCallTab v{mc, vc};
	v.show();
	vc.exec();
	return 0;
}
