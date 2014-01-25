#ifndef CVVISUAL_FILTER_VIEW_HPP
#define CVVISUAL_FILTER_VIEW_HPP

#include <memory>

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
	virtual void updateLeftFooter(QString newText) = 0;

	/**
	 * @brief update right foooter
	 * Signal to update the right side of the footer with newText.
	 * @param newText
	 */
	virtual void updateRightFoooter(QString newText) = 0;

public:
	/**
	 * @brief default constructor
	 **/
	FilterView():FilterView{0}{};
	
	virtual ~FilterView() = default;
	
	/**
	 * @brief factory method to create a filter view
	 * Factory method to create FilterViews.
	 * @param originalImg
	 * @param filteredImg
	 * @return an instance of FilterView
	 */
	virtual std::unique_ptr<FilterView> createFilterView(const cv::Mat& originalImg,
			const cv::Mat& filteredImg) = 0;
protected:
	/**
	 * @brief constructor of QWidget(parent)
	 * @param parent the parent of this view
	 **/
	FilterView(QWidget *parent):QWidget{parent}{};
};

}} //namespaces

#endif
