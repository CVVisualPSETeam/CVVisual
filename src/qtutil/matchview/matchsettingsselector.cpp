
#include <QVBoxLayout>

#include "matchsettingsselector.hpp"
#include "../../util/util.hpp"


namespace cvv{ namespace qtutil{

MatchSettingsSelector::MatchSettingsSelector(const std::vector<cv::DMatch> &univers, QWidget *parent):
	MatchSettings{parent},
	univers_{univers}
{
	auto layout=util::make_unique<QVBoxLayout>();
	layout->addWidget(comboBox_);

	connect(&signalElementSelected(),SIGNAL(signal(QString)),this,SLOT(changedSetting()));
	setLayout(layout.release());
	changedSetting();
}

void MatchSettingsSelector::setSettings(CVVMatch &match)
{
	setting_->setSettings(match);
}

void MatchSettingsSelector::changedSetting()
{
	auto setting=(*this)()(univers_);
	layout_->removeWidget(setting_);
	setting_=setting.get();
	layout_->addWidget(setting.release());
}

}}
