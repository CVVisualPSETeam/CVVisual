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

namespace structures{

template< std::size_t In, std::size_t Out>
class AutoFilterWidgetEntry: public QWidget
{
	using InputArray= typename AutoFilterWidget<In,Out>::InputArray;
	using OutputArray= typename AutoFilterWidget<In,Out>::OutputArray;

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

	operator bool()
		{TRACEPOINT;return checkBox_->isChecked();}

	InputArray input()
		{TRACEPOINT;return in_;}

	OutputArray output()
		{TRACEPOINT;return out_;}

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

	void emitAll()
	{
		TRACEPOINT;
		for(auto& elem:signals_)
		{
			elem.emitSignal();
		}
		TRACEPOINT;
	}

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
		message_->setText(QString("<font color='red'>")+msg+QString("</font>"));
		TRACEPOINT;
	}

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

	QString name_;
	QCheckBox* checkBox_;
	QLabel* message_;
	InputArray in_;
	OutputArray out_;
	std::array<SignalMatRef, Out> signals_;
};

}//structures

template< std::size_t In, std::size_t Out>
class AutoFilterWidget: public FilterSelectorWidget<In,Out>
{
	using InputArray= typename FilterSelectorWidget<In,Out>::InputArray;
	using OutputArray= typename FilterSelectorWidget<In,Out>::OutputArray;

	AutoFilterWidget(QWidget* parent = nullptr):
		QWidget{parent},
		entryLayout_{new QVBoxLayout},
		applyFilterIndividually_{false},
		entries_{},
		earliestActivationTime_{},
		slotApplyFilter_{[this](){this->autoApplyFilter();}}
	{
		//add sublayout
		this->layout_->insertLayout(0,entryLayout_);
		//connect auto filter slot
		QObject::connect(this, &AutoFilterWidget<In,Out>::signFilterSettingsChanged_,
				 this, &AutoFilterWidget<In,Out>::slotApplyFilter_);

	}

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
	void enableUserSelection(bool enabled = true)
	{
		TRACEPOINT;
		for(auto& elem:entries_)
		{
			elem.enableUserSelection(enabled);
		}
		TRACEPOINT;
	}

	void useFilterIndividually(bool individually = true)
		{TRACEPOINT;applyFilterIndividually_=individually;TRACEPOINT;}
						   // std::chrono::high_resolution_clock::now();
private:
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
					auto check=checkInput(elem.input());

					if(!check.first)
					{
						failed++;
						elem.setMessage(check.second);
					}else{
						elem.setMessage("");
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
					applyFilter(elem.input(),elem.output());
					elem.emitAll();
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
					auto check=checkInput(elem.input());
					if(!check.first)
					{
						elem.setMessage(check.second);
					}else{
						//apply filter+set message
						elem.setMessage("");
						applyFilter(elem.input(),elem.output());
						elem.emitAll();
					}
				}
			}
		}
		//update activation time
		earliestActivationTime_=std::chrono::high_resolution_clock::now()+
			(std::chrono::high_resolution_clock::now()-start);//duration
		TRACEPOINT;
	}

	QVBoxLayout* entryLayout_;
	bool applyFilterIndividually_;
	std::vector<util::Reference<structures::AutoFilterWidgetEntry<In,Out>>> entries_;
	std::chrono::time_point<std::chrono::high_resolution_clock> earliestActivationTime_;
	Slot slotApplyFilter_;
};

}}
#endif // CVVISUAL_AUTOFILTERWIDGET_HPP
