#ifndef CVVISUAL_KEYPOINT_SELECTOR
#define CVVISUAL_KEYPOINT_SELECTOR

#include "opencv2/features2d/features2d.hpp"

#include "keypointsettings.hpp"
#include "cvvkeypoint.hpp"

namespace cvv{ namespace qtutil{

class KeyPointSelector:public KeyPointSettings{

	Q_OBJECT

public:
	KeyPointSelector(QWidget* parent=nullptr);

	virtual void setSettings(CVVKeyPoint& key);

public slots:

	void addToSelection(const cv::KeyPoint& keypoint);

	void singleSelection(const cv::KeyPoint& keypoint);

	void setSelection(const std::vector<cv::KeyPoint>& selection);

signals:

	void updateSelection(const std::vector<cv::KeyPoint>& selection);

private:
	std::vector<cv::KeyPoint> selection_;
};
}}
#endif
