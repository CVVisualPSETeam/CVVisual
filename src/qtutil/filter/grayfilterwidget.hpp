#ifndef CVVISUAL_GRAYFILTERWIDGET_HPP
#define CVVISUAL_GRAYFILTERWIDGET_HPP



#include <vector>

#include <QVBoxLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QWidget>
#include <QObject>
#include <QString>

#include "opencv2/core/core.hpp"

#include "../filterfunctionwidget.hpp"
#include "../../dbg/dbg.hpp"

namespace cvv { namespace qtutil {

/**
 * @brief Represents a gray filter.
 */
class GrayFilterWidget: public FilterFunctionWidget<1,1>
{
	Q_OBJECT
public:
	/**
	 * @brief The input type.
	 */
	using InputArray  = FilterFunctionWidget<1,1>::InputArray;

	/**
	 * @brief The output type.
	 */
	using OutputArray = FilterFunctionWidget<1,1>::OutputArray;

	/**
	 * @brief Constructor
	 */
	GrayFilterWidget(QWidget* parent = nullptr);

	/**
	 * @brief Applys the filter to in and saves the result in out.
	 * @param in The input images.
	 * @param out The output images.
	 */
	virtual void applyFilter(InputArray in,OutputArray out) const override;

	/**
	 * @brief Checks whether input can be progressed by the applyFilter function.
	 * @param in The input images.
	 * @return bool = true: the filter can be executed.
	 *		bool = false: the filter cant be executed (e.g. images have wrong depth)
	 *		QString = message for the user (e.g. why the filter can't be progressed.)
	 */
	virtual std::pair<bool, QString> checkInput(InputArray) const override;

private slots:
	/**
	 * @brief Sets the number of channels.
	 * @param n The number of channels.
	 */
	void setChannel(int n)
	{
		TRACEPOINT;
		setChannel(static_cast<std::size_t>(n));
		TRACEPOINT;
	}

	/**
	 * @brief Sets the number of channels.
	 * @param n The number of channels.
	 */
	void setChannel(std::size_t n);

	/**
	 * @brief Sets the standard gray filter.
	 */
	void setStd();

private:
	/**
	 * @brief The layout.
	 */
	QVBoxLayout* layout_;
	/**
	 * @brief The spinbox to select the number of channels.
	 */
	QSpinBox* channel_;
	/**
	 * @brief Spin boxes for the channels.
	 */
	std::vector<QDoubleSpinBox*> chanValues_;
};

/**
 * @brief Registers the gray filter. (name=Gray filter)
 */
void registerGray();

}}

#endif // CVVISUAL_GRAYFILTERWIDGET_HPP