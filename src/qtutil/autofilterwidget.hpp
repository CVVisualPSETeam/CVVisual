#ifndef CVVISUAL_AUTOFILTERWIDGET_HPP
#define CVVISUAL_AUTOFILTERWIDGET_HPP

#include <array>
#include <vector>
#include <chrono>

#include "opencv2/core/core.hpp"

#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>

#include "filterselectorwidget.hpp"
#include "signalslot.hpp"
#include "../util/util.hpp"
#include "../dbg/dbg.hpp"

namespace cvv { namespace qtutil{

template< std::size_t In, std::size_t Out> class AutoFilterWidget;

/**
 * @brief Contains internal structures or classes.
 *
 * Stores the image input/output, the name and update signals for all output images.
 * Also provides the label to pass messages from the filter and provides a check box to select
 * the input to be filtered (can be deactivated).
 */
namespace structures{

/**
 * @brief Represents an entry of an autofilterwidget.
 */
template< std::size_t In, std::size_t Out>
class AutoFilterWidgetEntry: public QWidget
{
public:
	/**
	 * The input type for a filter.
	 */
	using InputArray= typename AutoFilterWidget<In,Out>::InputArray;
	/**
	 * The type of an output parameter of a filter.
	 */
	using OutputArray= typename AutoFilterWidget<In,Out>::OutputArray;

	/**
	 * @brief Constructor
	 * @param name The name shown to the user.
	 * @param in Image input
	 * @param out Image output
	 * @param parent Parent widget
	 */
	AutoFilterWidgetEntry(const QString& name, InputArray in, OutputArray out,
			QWidget* parent = nullptr):
		QWidget{parent},
		name_{name},
		checkBox_{new QCheckBox{name}},
		message_{new QLabel{}},
		in_{in},
		out_{out},
		signals_{}
	{
		TRACEPOINT;
		auto lay=util::make_unique<QVBoxLayout>();
		lay->addWidget(checkBox_);
		lay->addWidget(message_);
		message_->setVisible(false);
		setLayout(lay.release());
		enableUserSelection(true);
		TRACEPOINT;
	}

	/**
	 * @brief Checks wheather the check box is checked.
	 */
	operator bool()
		{TRACEPOINT;return checkBox_->isChecked();}

	/**
	 * @brief Returns the image input.
	 * @return The image input.
	 */
	InputArray input()
		{TRACEPOINT;return in_;}

	/**
	 * @brief Returns the image output.
	 * @return The image output.
	 */
	OutputArray output()
		{TRACEPOINT;return out_;}

	/**
	 * @brief Returns references to the update signals.
	 * @return References to the update signals.
	 */
	std::vector<util::Reference<SignalMatRef>> signalsRef()
	{
		TRACEPOINT;
		std::vector<util::Reference<SignalMatRef>> result{};
		for(auto& elem:signals_)
		{
			result.emplace_back(elem);
		}
		TRACEPOINT;
		return result;
	}

	/**
	 * @brief Emits all update signals.
	*/
	void emitAll()
	{
		TRACEPOINT;
		for(std::size_t i=0;i<Out;i++)
		{
			signals_.at(i).emitSignal(out_.at(i).get());
		}
		TRACEPOINT;
	}

	/**
	 * @brief Sets the message to display.
	 * @param msg The message to display (if msg == "" no message will be shown.
	 */
	void setMessage(const QString& msg = "")
	{
		TRACEPOINT;
		if(msg=="")
		{
			message_->setVisible(false);
			TRACEPOINT;
			return;
		}
		message_->setVisible(true);
		message_->setText(QString("<font color='red'>")+name_+QString(": ")
				+msg+QString("</font>"));
		TRACEPOINT;
	}

	/**
	 * @brief Enables/disables the checkbox.
	 * @param enabled If true the box will be enabled.
	 * If false the box will be disabled and checked.
	 */
	void enableUserSelection(bool enabled = true)
	{
		TRACEPOINT;
		if(!enabled)
		{
			checkBox_->setChecked(true);
		}
		checkBox_->setVisible(enabled);
		TRACEPOINT;
	}

	/**
	 * @brief The display name.
	 */
	QString name_;
	/**
	 * @brief The check box.
	 */
	QCheckBox* checkBox_;
	/**
	 * @brief The label to display messages.
	 */
	QLabel* message_;
	/**
	 * @brief Image input.
	 */
	InputArray in_;
	/**
	 * @brief Image output.
	 */
	OutputArray out_;
	/**
	 * @brief The update signals for the output.
	 */
	std::array<SignalMatRef, Out> signals_;
};

}//structures

/**
 * @brief The AutoFilterWidget class automatically applies the selected filter to all added entries.
 */
template< std::size_t In, std::size_t Out>
class AutoFilterWidget: public FilterSelectorWidget<In,Out>
{
public:
	/**
	 * The input type for a filter.
	 */
	using InputArray= typename FilterSelectorWidget<In,Out>::InputArray;
	/**
	 * The type of an output parameter of a filter.
	 */
	using OutputArray= typename FilterSelectorWidget<In,Out>::OutputArray;

	/**
	 * @brief Constructor.
	 * @param parent The parent widget.
	 */
	AutoFilterWidget(QWidget* parent = nullptr):
		FilterSelectorWidget<In,Out>{parent},
		entryLayout_{new QVBoxLayout},
		applyFilterIndividually_{false},
		entries_{},
		earliestActivationTime_{},
		slotApplyFilter_{[this](){this->autoApplyFilter();}}
	{
		TRACEPOINT;
		//add sublayout
		this->layout_->insertLayout(0,entryLayout_);
		//connect auto filter slot
		QObject::connect(&(this->signFilterSettingsChanged_),
				 SIGNAL(signal()),//&AutoFilterWidget<In,Out>::signFilterSettingsChanged_::signal,
				 &(this->slotApplyFilter_),
				 SLOT(slot())//&AutoFilterWidget<In,Out>::slotApplyFilter_::slot
				 );
		TRACEPOINT;
	}

	/**
	 * @brief Adds an entry.
	 * @param name The name of the enty.
	 * @param in The image input.
	 * @param out The image output.
	 * @return The update signals for all output images.
	 */
	std::vector<util::Reference<SignalMatRef>>
		addEntry(const QString& name, InputArray in, OutputArray out)
	{
		TRACEPOINT;
		auto elem=util::make_unique<structures::AutoFilterWidgetEntry<In,Out>>(name,in,out);
		auto result = elem->signalsRef();
		//store element
		entries_.emplace_back(*elem);
		//add it to the widget
		entryLayout_->addWidget(elem.release());
		TRACEPOINT;
		return result;
	}

	/**
	 * @brief Removes all entries.
	 */
	void removeAll()
	{
		TRACEPOINT;
		structures::AutoFilterWidgetEntry<In,Out>* elemToDelete;
		for(auto& elem: entries_)
		{
			elemToDelete = elem.getPtr();
			//remove from layout
			entryLayout_->removeWidget(elemToDelete);
			//delete the element
			elemToDelete->deleteLater();
		}
		entries_.clear();
		TRACEPOINT;
	}

public slots:
	/**
	 * @brief Enabels / disables the user to select entries to filter per combo boxes.
	 * @param enabled If true it will be enabled.
	 */
	void enableUserSelection(bool enabled = true)
	{
		TRACEPOINT;
		for(auto& elem:entries_)
		{
			elem.get().enableUserSelection(enabled);
		}
		TRACEPOINT;
	}

	/**
	 * @brief Sets wheather the filter will be applied to entries it can be applied to
	 * even when one other entry cant apply the filter.
	 * @param individually If true each entry that can apply the filter does so.
	 */
	void useFilterIndividually(bool individually = true)
		{TRACEPOINT;applyFilterIndividually_=individually;TRACEPOINT;}
private:
	/**
	 * @brief Applies the filter wen some settings where changed.
	 */
	void autoApplyFilter()
	{
		TRACEPOINT;
		auto start=std::chrono::high_resolution_clock::now();
		//activate again?
		if(start<earliestActivationTime_){return;}
		//apply filter
		if(!applyFilterIndividually_)
		{
			TRACEPOINT;
			//check wheather all filters can be applied
			std::size_t failed = 0;
			for(auto& elem:entries_)
			{
				//activated?
				if(elem.get())
				{
					auto check=this->checkInput(elem.get().input());

					if(!check.first)
					{
						failed++;
						elem.get().setMessage(check.second);
					}else{
						elem.get().setMessage("");
					}
				}
			}
			if(failed){TRACEPOINT; return;}
			//apply filters
			for(auto& elem:entries_)
			{
				//activated?
				if(elem.get())
				{
					this->applyFilter(elem.get().input(),elem.get().output());
					elem.get().emitAll();
				};
			}
		} else{
			TRACEPOINT;
			//filters can be applied individually
			for(auto& elem:entries_)
			{

				//activated?
				if(elem.get())
				{
					auto check=this->checkInput(elem.get().input());
					if(!check.first)
					{
						elem.get().setMessage(check.second);
					}else{
						//apply filter+set message
						elem.get().setMessage("");
						this->applyFilter(elem.get().input(),
								  elem.get().output());
						elem.get().emitAll();
					}
				}
			}
		}
		//update activation time
		earliestActivationTime_=std::chrono::high_resolution_clock::now()+
			(std::chrono::high_resolution_clock::now()-start);//duration
		TRACEPOINT;
	}

	/**
	 * @brief The layout containing the entries.
	 */
	QVBoxLayout* entryLayout_;
	/**
	 * @brief Wheater each entry that can apply the filter does so.
	 */
	bool applyFilterIndividually_;
	/**
	 * @brief The entries.
	 */
	std::vector<util::Reference<structures::AutoFilterWidgetEntry<In,Out>>> entries_;
	/**
	 * @brief Time for the earliest next activation for the filter.
	 */
	std::chrono::time_point<std::chrono::high_resolution_clock> earliestActivationTime_;
	/**
	 * @brief Slot called when filter settings change.
	 */
	Slot slotApplyFilter_;
};
}}
#endif // CVVISUAL_AUTOFILTERWIDGET_HPP