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

	/**
	 * @brief the constructor
	 * @param images a vector of images which will be shown
	 * @param parent the parent Widget
	 */
	SingleFilterView(const std::vector<cv::Mat>& images,QWidget *parent=nullptr);

	~SingleFilterView(){TRACEPOINT;}
};

}}//namespaces
#endif
