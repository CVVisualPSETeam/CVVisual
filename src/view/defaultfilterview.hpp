#ifndef CVVISUAL_DEFAULT_FILTER_VIEW
#define CVVISUAL_DEFAULT_FILTER_VIEW

#include <QWidget>
#include <QImage>
#include <QString>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

#include "filter_view.hpp"

namespace cvv{ namespace view{

/*
 * @brief This Filterview shows only the given images and has no other options.
*/
class DefaultFilterView : public cvv::view::FilterView{
	Q_OBJECT
public:
	/*
	 * @brief Standart constructor for FilterView
	 * @param images A List of images
	 * @param parent The parent of this QWidget
	 */
	DefaultFilterView(std::vector<cv::Mat> images,QWidget *parent);

private: 
	/*
	 *@brief the images of this view
	 */
	std::vector<cv::Mat> images_;
};	
}}//namespaces
#endif
