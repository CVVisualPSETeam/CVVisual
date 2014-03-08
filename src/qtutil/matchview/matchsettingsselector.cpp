#include <QVBoxLayout>
#include <QPushButton>

#include "matchsettingsselector.hpp"
#include "../../util/util.hpp"

namespace cvv{ namespace qtutil{

MatchSettingsSelector::MatchSettingsSelector(const std::vector<cv::DMatch> &univers, QWidget *parent):
	MatchSettings{parent},
	RegisterHelper<MatchSettings,std::vector<cv::DMatch>>{},
	univers_{univers}
{
	auto layout=util::make_unique<QVBoxLayout>();
	auto headerLayout=util::make_unique<QHBoxLayout>();
	auto closebutton=util::make_unique<QPushButton>("-");
	closebutton->setMaximumWidth(30);

	connect(closebutton.get(),SIGNAL(clicked()),this,SLOT(removeMe()));
	connect(&signalElementSelected(),SIGNAL(signal(QString)),this,SLOT(changedSetting()));

	headerLayout->addWidget(closebutton.release());
	headerLayout->addWidget(comboBox_);

	layout->setContentsMargins(0, 0, 0, 0);
	layout->addLayout(headerLayout.release());
	layout_=layout.get();
	setLayout(layout.release());
	TRACEPOINT;
	if(this->has(this->selection())){
		TRACEPOINT;
		changedSetting();
	}
	TRACEPOINT;
}

void MatchSettingsSelector::setSettings(CVVMatch &match)
{
	setting_->setSettings(match);
}

void MatchSettingsSelector::changedSetting()
{
	TRACEPOINT;
	auto setting=(*this)()(univers_);
	TRACEPOINT;
	if(setting){
		TRACEPOINT;
		if(setting_){
			TRACEPOINT;
			layout_->removeWidget(setting_);
			disconnect(setting_,SIGNAL(settingsChanged(MatchSettings&)),
				   this,SIGNAL(settingsChanged(MatchSettings&)));
		}
		TRACEPOINT;
		setting_->deleteLater();
		TRACEPOINT;
		setting_=setting.get();
		TRACEPOINT;
		layout_->addWidget(setting.release());
		TRACEPOINT;
		connect(setting_,SIGNAL(settingsChanged(MatchSettings&)),
			this,SIGNAL(settingsChanged(MatchSettings&)));
	}
}

}}
