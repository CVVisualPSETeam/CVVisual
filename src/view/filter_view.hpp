#ifndef CVVISUAL_FILTER_VIEW_HPP
#define CVVISUAL_FILTER_VIEW_HPP

#include <QString>
#include <QWidget>
#include <vector>

#include <opencv2/core/core.hpp>

namespace cvv {
namespace view {

class FilterView: public QWidget
{
Q_OBJECT

signals:

	/**
	 * @brief update left Ffooter
	 * Signal to update the left side of the footer with newText.
	 * @param newText
	 */
	void updateLeftFooter(QString newText);

	/**
	 * @brief update right foooter
	 * Signal to update the right side of the footer with newText.
	 * @param newText
	 */
	void updateRightFoooter(QString newText);

public:
	/**
	 * @brief default constructor
	 **/
	FilterView():FilterView{0}{};
	/**
	 * @brief factory method to create a filter view
	 * Factory method to create FilterViews.
	 * @param originalImg
	 * @param filteredImg
	 * @return an instance of FilterView
	 */
	//virtual FilterView createFilterView(const cv::Mat& originalImg, const cv::Mat& filteredImg);
protected:
	/**
	 * @brief constructor of QWidget(parent)
	 * @param parent the parent of this view
	 **/
	FilterView(QWidget *parent):QWidget{parent}{};
};

}} //namespaces

#endif
