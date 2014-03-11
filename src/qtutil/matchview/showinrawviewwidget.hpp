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

protected:
	virtual void hideEvent(QHideEvent * );
	
	virtual void showEvent(QShowEvent *);

private slots:

	void getcurrentSelection();

private:
	MatchManagement* matchmnt_;
	KeyPointManagement* keymnt_;
	RawviewWindow* rawViewWindow_;
	std::vector<cv::KeyPoint> left_key_;
	std::vector<cv::KeyPoint> right_key_;
	std::vector<cv::DMatch> matches_;
};

}}

#endif
