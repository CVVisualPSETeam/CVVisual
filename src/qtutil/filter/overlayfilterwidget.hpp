#ifndef CVVISUAL_OVERLAY_FILTER_WIDGET_HPP
#define CVVISUAL_OVERLAY_FILTER_WIDGET_HPP

#include <unordered_map>

#include "../../util/observer_ptr.hpp"
#include "../filterselectorwidget.hpp"

namespace cvv {
namespace qtutil {

class OverlayFilterWidget: public FilterFunctionWidget<2,1>
{
Q_OBJECT
public:
	/**
	 * @brief The input type.
	 */
	using InputArray  = FilterFunctionWidget<2,1>::InputArray;
	//std::array<util::Reference<const cv::Mat>,2>

	/**
	 * @brief The output type.
	 */
	using OutputArray = FilterFunctionWidget<2,1>::OutputArray;
	//std::array<util::Reference<cv::Mat>,1>
	
	/**
	* @brief Constructs OverlayFilterWidget with default opacity 0,5.
	* @param parent The parent of the widget
	*/
	OverlayFilterWidget(QWidget* parent = nullptr);
	
	/**
	* @brief Default destructuor.
	*/
	~OverlayFilterWidget()
	{
		TRACEPOINT;
	}

	/**
	* The opacity of the first image while overlaying is indicated by
	* opacityOfOriginalImg_.
	* @brief Overlays the original image
	* @param in array of input matrices
	* @param out array of output matrices
	*/
	void applyFilter(InputArray in,OutputArray out) const;

	/**
	* Checks whether the matrices have the same size and same number of channels.
	* @brief Checks whether matrices in in can be processed by Overlayfilter
	* @param in array of input matrices
	*/
	std::pair<bool, QString> checkInput(InputArray in) const;
	
private:
	double opacityOfOriginalImg_;
		//< opacity of the first input image when ovelaying
	
private slots:
	/**
	* @brief Sets opacityOfOriginalImg_ and emits signFilterSettingsChanged.
	* @param op new opacity
	*/
	void updateOpacity(int op);
};

}}

#endif
