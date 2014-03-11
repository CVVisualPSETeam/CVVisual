#ifndef CVVISUAL_SHOW_IN_RAWVIEW
#define CVVISUAL_SHOW_IN_RAWVIEW

#include <QWidget>

#include "opencv2/features2d/features2d.hpp"

#include "matchmanagement.hpp"
#include "keypointmanagement.hpp"
#include "rawview_window.hpp"

namespace  cvv {namespace qtutil{

class ShowInRawView:public QWidget{

	Q_OBJECT

public:
	ShowInRawView(std::vector<cv::KeyPoint> left_key,
		      std::vector<cv::KeyPoint> right_key,
		      std::vector<cv::DMatch> matches,
		      MatchManagement* matchmnt,
		      KeyPointManagement* keymnt,
		      QWidget*parent=nullptr);

	~ShowInRawView();
private slots:

	void showHideRawviewWindow()
	{rawViewWindow_->setVisible(!rawViewWindow_->isVisible());}

private:
	MatchManagement* matchmnt_;
	KeyPointManagement* keymnt_;
	RawviewWindow* rawViewWindow_;
};

}}

#endif
