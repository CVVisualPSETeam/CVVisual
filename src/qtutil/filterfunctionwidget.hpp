#ifndef CVVISUAL_FILTERFUNCTIONWIDGET_HPP
#define CVVISUAL_FILTERFUNCTIONWIDGET_HPP
//STD
#include <array>
#include <type_traits>

//QT
#include <QWidget>
#include <QString>

//OCV
#include "opencv2/core/core.hpp"

//cvv
#include "signalslot.hpp"

namespace cvv { namespace qtutil{

/**
 * @brief The type for the input of the filter.
 *
 * Inherit from it if you want to provide a new image filter.
 * Use the widget to let the user choose parameters.
 * Emit stateChanged when user input leads to different parameters.
 *
 * @tparam In The number of input images.
 * @tparam Out The number of output images.
 */
template< std::size_t In, std::size_t Out>
class FilterFunctionWidget: public virtual QWidget
{
	static_assert( Out > 0, "Out must not be 0!");
public:
	/**
	 * @brief virtual destructor.
	 */
	virtual ~FilterFunctionWidget(){}

	/**
	 * @brief Applys the filter to in and saves the result in out.
	 * @param in The input images.
	 * @param out The output images.
	 * @return Returns out.
	 */
	virtual const std::array<cv::Mat&,Out>&
		applyFilter(const std::array<const cv::Mat&,Out>& in,
				const std::array<cv::Mat&,Out>& out) const = 0;

	/**
	 * @brief Checks whether input can be progressed by the applyFilter function.
	 * @param in The input images.
	 * @return bool = true: the filter can be executed.
	 *		bool = false: the filter cant be executed (e.g. images have wrong depth)
	 *		QString = message for the user (e.g. why the filter can't be progressed.)
	 */
	virtual std::pair<bool, QString> checkInput(const std::array<const cv::Mat&,Out>& in) const
												= 0;

	/**
	 * @brief Signal to emit when user input leads to different parameters.
	 */
	Signal sigFilterSettingsChanged_;
};

}} // end namespaces qtutil, cvv
#endif // CVVISUAL_FILTERFUNCTIONWIDGET_HPP
