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
	void updateLeftFooter(const QString &newText);

	/**
	 * @brief update right foooter
	 * Signal to update the right side of the footer with newText.
	 * @param newText
	 */
	void updateRightFoooter(const QString& newText);

public:
	/**
	 * @brief default constructor
	 **/
	FilterView():FilterView{0}{};
	
	virtual ~FilterView() = default;
	
protected:
	/**
	 * @brief constructor of QWidget(parent)
	 * @param parent the parent of this view
	 **/
	FilterView(QWidget *parent):QWidget{parent}{};
};

}} //namespaces

#endif
