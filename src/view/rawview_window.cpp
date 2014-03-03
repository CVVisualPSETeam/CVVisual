#include "rawview_window.hpp"

namespace cvv { namespace view {

RawviewWindow::RawviewWindow(QString title,
				 const std::vector<cv::KeyPoint>& keypoints1,
				 const std::vector<cv::KeyPoint>& keypoints2,
				 const std::vector<cv::DMatch>& matches):
	RawviewWindow(title, keypoints1, keypoints2)
{
	TRACEPOINT;
	setWindowTitle(title);
	setMinimumWidth(600);
	setMinimumHeight(600);
	view = new Rawview(keypoints1, keypoints2, matches, true);
	setCentralWidget(view);
	connect(view, SIGNAL(matchesSelected(std::vector<cv::DMatch>)),
			this, SIGNAL(matchesSelected(std::vector<cv::DMatch>)));
	connect(view, SIGNAL(keyPointsSelected(std::vector<cv::KeyPoint>)),
			this, SIGNAL(keyPointsSelected(std::vector<cv::KeyPoint>)));
	TRACEPOINT;
}

RawviewWindow::RawviewWindow(QString title,
				 const std::vector<cv::KeyPoint>& keypoints1,
				 const std::vector<cv::KeyPoint>& keypoints2):
	keypoints1{keypoints1}, keypoints2{keypoints2}
{
	TRACEPOINT;
	setWindowTitle(title);
	TRACEPOINT;
}

void RawviewWindow::selectMatches(const std::vector<cv::DMatch> &matches)
{
	if (view == nullptr)
	{
		view = new Rawview(keypoints1, keypoints2, matches, true);
		setCentralWidget(view);
		connect(view, SIGNAL(matchesSelected(const std::vector<cv::DMatch>&)),
				this, SIGNAL(matchesSelected(const std::vector<cv::DMatch>&)));
		setMinimumWidth(600);
		setMinimumHeight(600);
	}
	view->selectMatches(keypoints1, keypoints2, matches);
}

void RawviewWindow::selectKeyPoints(const std::vector<cv::KeyPoint> &keyPoints)
{
	view->selectKeyPoints(keyPoints);
}

}}
