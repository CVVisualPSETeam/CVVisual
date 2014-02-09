#ifndef CVVISUAL_SINGLE_FILTER_VIEW
#define CVVISUAL_SINGLE_FILTER_VIEW

#include <QWidget>

#include "opencv2/core/core.hpp"

#include "filter_view.hpp"
#include "../dbg/dbg.hpp"


namespace cvv{ namespace view{

/*
 * @brief This Filterview applies the same filter for all given images.
*/
class SingleFilterView : public cvv::view::FilterView{
	Q_OBJECT
public:
	/*
	 * @brief Standart constructor for SingleFilterView
	 * @param images A List of images
	 * @param parent The parent of this QWidget
	 */
	SingleFilterView(std::vector<cv::Mat> images,QWidget *parent=nullptr);
}}//namespaces
#endif
