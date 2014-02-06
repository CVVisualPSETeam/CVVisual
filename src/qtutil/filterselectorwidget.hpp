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
#include "../dbg/dbg.hpp"

namespace cvv { namespace qtutil{

/**
 * @brief The FilterSelectorWidget class
 */
template< std::size_t In, std::size_t Out>
class FilterSelectorWidget : public RegisterHelper<FilterFunctionWidget<In,Out>, QWidget*>,
			public FilterFunctionWidget<In,Out>
{
	static_assert( Out > 0, "Out must not be 0!");
public:
	/**
	 * @brief Constuctor
	 * @param parent The parent widget.
	 */
	explicit FilterSelectorWidget(QWidget *parent = nullptr):QWidget{parent},
		RegisterHelper<FilterFunctionWidget<In,Out>, QWidget*>{},
		currentFilter_{nullptr}, layout_{new QVBoxLayout{}},
		slotFilterSelected_{[this](){TRACEPOINT;
			if(this->currentFilter_)
			{
				TRACEPOINT;
				this->layout_->removeWidget(this->currentFilter_);
				delete this->currentFilter_;
				//disconnect
				QObject::disconnect(&(this->currentFilter_->sigFilterSettingsChanged_),
							0,
							&(this->slotInternalFilterChanged_),
							0);
			}
			this->currentFilter_= ((*this)()(nullptr)).release();
			this->layout_->addWidget(this->currentFilter_);
			//connect signals and slots
			QObject::connect(&(this->currentFilter_->sigFilterSettingsChanged_),
					SIGNAL(signal()),
					&(this->slotInternalFilterChanged_),
					SLOT(slot()));
			TRACEPOINT;
			this->sigSelect_.emitSignal();
			DEBUGF(1, "Signal was emmitted");
		}},
		slotInternalFilterChanged_{[this](){TRACEPOINT;
			this->sigFilterSettingsChanged_.emitSignal();TRACEPOINT;}}
	{
		TRACEPOINT;
		layout_->addWidget((this->comboBox_));
		QObject::connect((this->comboBox_),SIGNAL(currentTextChanged(const QString &)),
			&slotFilterSelected_, SLOT(slot()));
		this->setLayout(layout_);
		TRACEPOINT;
	}

	~FilterSelectorWidget(){TRACEPOINT;}

	/**
	 * @brief Applies the selected filter.
	 * @param in Input images.
	 * @param out Output images.
	 * @throw std::invalid_argument checkInput(in).first==false
	 * @return parameter out
	 */
	virtual const std::array<cv::Mat,Out>& applyFilter(const std::array<cv::Mat,In>& in,
					std::array<cv::Mat,Out>& out) const override
	{
		TRACEPOINT;
		auto check = checkInput(in);
		if(!check.first)
			{throw std::invalid_argument{check.second.toStdString()};}
		TRACEPOINT;
		return currentFilter_->applyFilter(in,out);
	}

	/**
	 * @brief Checks whether input can be progressed by the selected filter.
	 * @param in The input images.
	 * @return bool = true: the filter can be executed.
	 *		bool = false: the filter cant be executed (e.g. images have wrong depth)
	 *		QString = message for the user (e.g. why the filter can't be progressed.)
	 */
	virtual std::pair<bool, QString> checkInput(const std::array<cv::Mat,In>& in) const override
	{
		TRACEPOINT;
		if(currentFilter_)
			{return {false, "No entry selected."};}
		TRACEPOINT;
		return currentFilter_->checkInput(in);
	}

	Signal sigSelect_;

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
