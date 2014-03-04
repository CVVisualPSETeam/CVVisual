#ifndef CVVISUAL_MATCH_SELECTOR
#define CVVISUAL_MATCH_SELECTOR

#include "opencv2/features2d/features2d.hpp"

#include "matchsettings.hpp"
#include "cvvmatch.hpp"

namespace cvv
{
namespace qtutil
{

class MatchSelector : public MatchSettings
{

	Q_OBJECT

      public:
	MatchSelector(QWidget *parent = nullptr);

	virtual void setSettings(CVVMatch &match);

      public
slots:

	void addToSelection(const cv::DMatch &match);

	void singleSelection(const cv::DMatch &match);

	void setSelection(const std::vector<cv::DMatch> &selection);

signals:

	void updateSelection(const std::vector<cv::DMatch> &selection);

      private:
	std::vector<cv::DMatch> selection_;
};
}
}
#endif
