#ifndef CVVISUAL_MATCH_PORTION_SELECTOR
#define CVVISUAL_MATCH_PORTION_SELECTOR

#include <vector>

#include "opencv2/features2d/features2d.hpp"

#include "matchselection.hpp"
#include "../portionselector.hpp"

namespace cvv {namespace qtutil{

class MatchPortionSelection:public MatchSelection{
public:
	MatchPortionSelection(std::vector<cv::DMatch> univers,QWidget * parent=nullptr);


	virtual std::vector<cv::DMatch> select(const std::vector<cv::DMatch>& selection)override;

private:
	std::vector<cv::DMatch> univers_;
	PortionSelector* selector_;
};

}}

#endif
