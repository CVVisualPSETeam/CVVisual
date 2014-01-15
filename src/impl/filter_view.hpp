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

    /**
     * @brief updateLeftFooter
     * Signal to update the left side of the footer with newText.
     * @param newText
     */
	void updateLeftFooter(QString newText);

    /**
     * @brief updateRightFoooter
     * Signal to update the right side of the footer with newText.
     * @param newText
     */
	void updateRightFoooter(QString newText);

public:

    /**
     * @brief createFilterView
     * Factory method to create FilterViews.
     * @param img1
     * @return
     */
    //virtual FilterView createFilterView(const std::vector<cv::InputArray>& img1);
};

}} //namespaces

#endif
