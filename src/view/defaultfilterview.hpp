#ifndef CVVISUAL_DEFAULT_FILTER_VIEW
#define CVVISUAL_DEFAULT_FILTER_VIEW

#include <QWidget>

#include "opencv2/core/core.hpp"

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
	
	/**
	 * This method is used if you cannot use pointer for constructors
	 * @return a new instanz of this class with the given images
	 */
	 
	//static DefaultFilterView create(const std::vecotr<InputArray>& input);
private: 
	std::vector<cv::Mat> images_;
};	
}}//namespaces
#endif
