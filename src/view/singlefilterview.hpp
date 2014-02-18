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
	 * @brief the constructor
	 * @param lefKeyPoints (queryindx) the keypoint from the left image
	 * @param rightKeyPoint (trainIdx/imIdx) the keypoints from the right Image
	 * @param matches the matches between the images
	 * @param usetrainIdx if true the trainIdx will be taken for rightKeyPoint if false
	 *	the imIdx will be taken
	 * @param parent the parent widget
	 */
	SingleFilterView(const std::vector<cv::Mat>& images,QWidget *parent=nullptr);
};

}}//namespaces
#endif
