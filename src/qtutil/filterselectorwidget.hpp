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

	using InputArray  = typename FilterFunctionWidget<In,Out>::InputArray;
	using OutputArray = typename FilterFunctionWidget<In,Out>::OutputArray;

	/**
	 * @brief Constuctor
	 * @param parent The parent widget.
	 */
	FilterSelectorWidget(QWidget *parent = nullptr):
		QWidget{parent},
		RegisterHelper<FilterFunctionWidget<In,Out>, QWidget*>{},
		currentFilter_{nullptr},
		layout_{new QVBoxLayout{}},
		slotFilterSelected_{[this](){this->updatedSelectedFilter();}
}
	{
		TRACEPOINT;
		this->layout_->addWidget((this->comboBox_));
		//connect elem selected with update for it
		QObject::connect(&(this->signElementSelected_),SIGNAL(signal(QString)),
				 &(this->slotFilterSelected_), SLOT(slot()));
		this->setLayout((this->layout_));
		//update for initial selection (if it is valid)
		if(this->has(this->selection())){updatedSelectedFilter();}
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
	virtual void applyFilter(InputArray in,OutputArray out) const override
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
	virtual std::pair<bool, QString> checkInput(InputArray in) const override
	{
		TRACEPOINT;
		if(currentFilter_)
			{return {false, "No entry selected."};}
		TRACEPOINT;
		return currentFilter_->checkInput(in);
	}


private:
	void updatedSelectedFilter()
	{
		TRACEPOINT;
		if((this->currentFilter_))
		{
			TRACEPOINT;
			layout_->removeWidget((this->currentFilter_));
			//disconnect
			QObject::disconnect(&(this->currentFilter_->signFilterSettingsChanged_),0,
						&(this->signFilterSettingsChanged_),0);
			delete (this->currentFilter_);
		}
		this->currentFilter_= ((*this)()(nullptr)).release();
		this->layout_->addWidget((this->currentFilter_));
		//pass signal
		QObject::connect(&(this->currentFilter_->signFilterSettingsChanged_),
				 SIGNAL(signal()),
				 &(this->signFilterSettingsChanged_),SIGNAL(signal()));
		//settings changed
		this->signFilterSettingsChanged_.emitSignal();
		TRACEPOINT;
	}

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
}; //FilterSelectorWidget

}} // end namespaces qtutil, cvv
#endif // CVVISUAL_FILTERSELECTORWIDGET_HPP
