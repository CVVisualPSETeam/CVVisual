#include <QVBoxLayout>

#include "keypointsettingsselector.hpp"
#include "../../util/util.hpp"

namespace cvv{ namespace qtutil{

KeyPointSettingsSelector::KeyPointSettingsSelector(const std::vector<cv::KeyPoint> &univers, QWidget *parent):
	KeyPointSettings{parent},
	RegisterHelper<KeyPointSettings,std::vector<cv::KeyPoint>>{},
	univers_{univers}
{
	auto layout=util::make_unique<QVBoxLayout>();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(comboBox_);

	connect(&signalElementSelected(),SIGNAL(signal(QString)),this,SLOT(changedSetting()));
	layout_=layout.get();
	setLayout(layout.release());
	TRACEPOINT;
	if(this->has(this->selection())){
		TRACEPOINT;
		changedSetting();
	}
	TRACEPOINT;
}

void KeyPointSettingsSelector::setSettings(CVVKeyPoint &key)
{
	setting_->setSettings(key);
}

void KeyPointSettingsSelector::changedSetting()
{
	TRACEPOINT;
	auto setting=(*this)()(univers_);
	TRACEPOINT;
	if(setting){
		TRACEPOINT;
		if(setting_){
			TRACEPOINT;
			layout_->removeWidget(setting_);
			disconnect(setting_,SIGNAL(settingsChanged(KeyPointSettings&)),
				   this,SIGNAL(settingsChanged(KeyPointSettings&)));
		}
		TRACEPOINT;
		setting_->deleteLater();
		TRACEPOINT;
		setting_=setting.get();
		TRACEPOINT;
		layout_->addWidget(setting.release());
		TRACEPOINT;
		connect(setting_,SIGNAL(settingsChanged(KeyPointSettings&)),
		this,SIGNAL(settingsChanged(KeyPointSettings&)));
	}
}

}}
