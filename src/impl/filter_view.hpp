#ifndef CVVISUAL_FILTER_VIEW_HPP
#define CVVISUAL_FILTER_VIEW_HPP

#include <QString>
#include <QWidget>
#include <vector>

#include <opencv2/core/core.hpp>

namespace cvv {
namespace impl {

class FilterView: public QWidget
{
Q_OBJECT
signals:
	void updateLeftFooter(QString newText);
	void updateRightFoooter(QString newText);
public:
    //virtual FilterView createFilterView(const std::vector<cv::InputArray>& img1);
};

}} //namespaces

#endif
