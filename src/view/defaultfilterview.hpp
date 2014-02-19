#ifndef CVVISUAL_DEFAULT_FILTER_VIEW
#define CVVISUAL_DEFAULT_FILTER_VIEW

#include <QWidget>

#include "opencv2/core/core.hpp"

#include "filter_view.hpp"
#include "../dbg/dbg.hpp"


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
	DefaultFilterView(const std::vector<cv::Mat>& images,QWidget *parent=nullptr);

	~DefaultFilterView(){TRACEPOINT;}
};
}}//namespaces
#endif
