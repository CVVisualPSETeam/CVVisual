#ifndef CVVISUAL_FILTERSELECTORWIDGET_HPP
#define CVVISUAL_FILTERSELECTORWIDGET_HPP
//STD
#include <stdexcept>
#include <array>
#include <type_traits>

//QT
#include <QWidget>
#include <QString>
#include <QComboBox>

//OCV
#include "opencv2/core/core.hpp"

//CVV
#include "signalslot.hpp"
#include "registerhelper.hpp"
#include "filterfunctionwidget.hpp"

	//makro damit filter nicht hinzugefügt werden???
	//layout auf referenz

namespace cvv { namespace qtutil{

/**
 * @brief The FilterSelectorWidget class
 */
template< std::size_t In, std::size_t Out>
class FilterSelectorWidget : public RegisterHelper<FilterFunctionWidget<In,Out>>
{
public:
	/**
	 * @brief Constuctor
	 * @param parent The parent widget.
	 */
	explicit FilterSelectorWidget(QWidget *parent = 0):
		RegisterHelper<FilterFunctionWidget<In,Out>>{parent},
		sigFilterSettingsChanged_{}, currentFilter_{nullptr}, layout_{new QVBoxLayout{}},
		slotFilterSelected_{[this](){
			if(this->currentFilter_)
			{
				this->layout_.removeWidget(this->currentFilter_);
				delete this->currentFilter_;
				//disconnect
				disconnect(&(this->currentFilter_->sigStateChanged_),
						SIGNAL(signal()), 0, 0);
			}
			this->currentFilter_= (this->operator ()()).release;
			this->layout_->addWidget(this->currentFilter_);
			//connect signals and slots
			this->connect(&(this->currentFilter_->sigStateChanged_), SIGNAL(signal())
					&(this->slotInternalFilterChanged_), SLOT(slot()));
		}},
		slotInternalFilterChanged_{[this](){this->sigFilterSettingsChanged_.emitSignal();}}
	{
		layout_->addWidget(comboBox_);
		connect(comboBox_,SIGNAL(currentTextChanged(const QString &)),
			&slotFilterSelected_, SLOT(slot()));
	}

	/**
	 * @brief Applies the selected filter.
	 * @param in Input images.
	 * @param out Output images.
	 * @throw std::invalid_argument checkInput(in).first==false
	 * @return parameter out
	 */
	const std::array<cv::Mat&,Out>& applyFilter(const std::array<const cv::Mat&,Out>& in,
							const std::array<cv::Mat&,Out>& out) const
	{
		auto check = checkInput(in);
		if(!check.first)
			{throw std::invalid_argument(check.second);}
		return currentFilter_->applyFilter(in,out);
	}

	/**
	 * @brief Checks whether input can be progressed by the selected filter.
	 * @param in The input images.
	 * @return bool = true: the filter can be executed.
	 *		bool = false: the filter cant be executed (e.g. images have wrong depth)
	 *		QString = message for the user (e.g. why the filter can't be progressed.)
	 */
	std::pair<bool, QString> checkInput(const std::array<const cv::Mat&,Out>& in)
	{
		if(currentFilter_)
			{return {false, "No entry selected."};}
		return currentFilter_->checkInput(in);
	}

	/**
	 * @brief Signal to emit when user input leads to different parameters.
	 */
	Signal sigFilterSettingsChanged_;

private:
	/**
	 * @brief the current filter
	 */
	FilterFunctionWidget<In, Out>* currentFilter_;

	/**
	 * @brief The layout
	 */
	QVBoxLayout* layout_;

	/**
	 * @brief Slot called when user changes selection
	 */
	Slot slotFilterSelected_;

	/**
	 * @brief Slot called whenever the current filter emits sigStateChanged_
	 */
	Slot slotInternalFilterChanged_;
}; //FilterSelectorWidget

}} // end namespaces qtutil, cvv
#endif // CVVISUAL_FILTERSELECTORWIDGET_HPP
