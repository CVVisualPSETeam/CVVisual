#ifndef CVVISUAL_MATCH_SHOW_SETTING
#define CVVISUAL_MATCH_SHOW_SETTING

#include <vector>

#include <QPushButton>

#include "opencv2/features2d/features2d.hpp"
#include "matchsettings.hpp"
#include "cvvmatch.hpp"

namespace cvv{ namespace qtutil{

class MatchShowSetting:public MatchSettings{

	Q_OBJECT

public:
	MatchShowSetting(std::vector<cv::DMatch>,QWidget* parent=nullptr);

	virtual void setSettings(CVVMatch &match) override
		{match.setShow(button_->isChecked());}

	virtual void setUnSelectedSettings(CVVMatch &match) override
		{match.setShow(!(button_->isChecked()));}
public slots:

	void updateButton();
private:
	QPushButton *button_;
};

}}

#endif
