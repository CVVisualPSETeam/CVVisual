#ifndef CVVISUAL_LINE_MATCH_VIEW
#define CVVISUAL_LINE_MATCH_VIEW

#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "match_view.hpp"
namespace cvv { namespace view{

class LineMatchView:public MatchView{
public:
	LineMatchView(std::vector<cv::KeyPoint> leftKeyPoints,std::vector<cv::KeyPoint> rightKeyPoints,std::vector<cv::DMatch> matches,cv::Mat leftIm,cv::Mat rightIm,QWidget *parent=nullptr);
};
}}
#endif
