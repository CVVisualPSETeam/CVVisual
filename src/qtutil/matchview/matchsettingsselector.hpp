#ifndef CVVISUAL_MATCH_SETTINGS_SELECTOR
#define CVVISUAL_MATCH_SETTINGS_SELECTOR

#include <vector>

#include "opencv2/features2d/features2d.hpp"

#include "matchsettings.hpp"
#include "../registerhelper.hpp"

namespace cvv{ namespace qtutil{

class MatchSettingsSelector:public MatchSettings, public RegisterHelper<MatchSettings,std::vector<cv::DMatch>>{

	Q_OBJECT
public:
	MatchSettingsSelector(const std::vector<cv::DMatch>& univers,QWidget * parent=nullptr);

	virtual void setSettings(CVVMatch &match)override;

private slots:

	virtual void changedSetting();

private:
	MatchSettings * setting_;
	std::vector<cv::DMatch> univers_;
	QLayout * layout_;

};

}}

#endif
