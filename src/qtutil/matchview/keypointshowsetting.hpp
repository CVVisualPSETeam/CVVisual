#ifndef CVVISUAL_KEY_POINT_SHOW_SETTING
#define CVVISUAL_KEY_POINT_SHOW_SETTING

#include <vector>

#include <QPushButton>

#include "opencv2/features2d/features2d.hpp"
#include "keypointsettings.hpp"

namespace cvv{ namespace qtutil{

class KeyPointShowSetting:public KeyPointSettings{

	Q_OBJECT

public:
	KeyPointShowSetting(std::vector<cv::KeyPoint>,QWidget* parent=nullptr);

	virtual void setSettings(CVVKeyPoint &key) override
		{key.setShow(button_->isChecked());}

	virtual void setUnSelectedSettings(CVVKeyPoint &key) override
		{key.setShow(!(button_->isChecked()));}
public slots:

	void updateButton();
private:
	QPushButton *button_;
};

}}

#endif
