#ifndef CVVISUAL_MULTIVIEW_CALL_TAB_HPP
#define CVVISUAL_MULTIVIEW_CALL_TAB_HPP

#include <vector>
#include <memory>

#include <QObject>
#include <QString>
#include <QMap>
#include <QPushButton>
#include <QComboBox>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "call_tab.hpp"
#include "../util/util.hpp"
#include "../qtutil/registerhelper.hpp"
#include "../qtutil/signalslot.hpp"
#include "../qtutil/util.hpp"
#include "../dbg/dbg.hpp"

namespace cvv
{
namespace gui
{

/** Call Tab for multiple views.
 * @brief Inner part of a tab, contains a View.
 * The inner part of a tab or window
 * containing a View.
 * Allows to switch between different views and to access the help.
 */
template <class ViewType, class CallType>
class MultiViewCallTab
    : public CallTab,
      public cvv::qtutil::RegisterHelper<ViewType, const CallType &, QWidget *>
{
      public:
	MultiViewCallTab(const CallType &call)
	    : MultiViewCallTab{ call.description(), call }
	{
		TRACEPOINT;
	}

	MultiViewCallTab(const QString &tabName, const CallType &call)
	    : call_{ call }, currentIndexChanged{ [&]()
	{
		vlayout_->removeWidget(view_);
		view_->setVisible(false);
		setView();
	} },
	      helpButtonClicked{ [&]()
	{ qtutil::openHelpBrowser(viewId_); } },
	      setAsDefaultButtonClicked{ [&]()
	{ qtutil::setSetting(default_scope_, default_key_, viewId_); } }
	{
		TRACEPOINT;
		setName(tabName);
		TRACEPOINT;
	}

	~MultiViewCallTab()
	{
		TRACEPOINT;
	}

	/**
	 * @brief get ID.
	 * @return the ID of the CallTab.
	 * (ID is equal to the ID of the associated call).
	 * Overrides CallTab's getId.
	 */
	size_t getId() const override
	{
		TRACEPOINT;
		return call_->getId();
	}

	/**
	 * @brief Register the template class to the map of Views.
	 * View needs to offer a constructor of the form View(const
	 * cvv::impl::CallType&, QWidget*).
	 * @param name to register the class under.
	 * @tparam View - Class to register.
	 * @return true when the view was registered and false when the name was
	 * already taken.
	 */
	template <class View> static bool registerView(const QString &name)
	{
		TRACEPOINT;
		return MultiViewCallTab<ViewType, CallType>::registerElement(
		    name, [](const CallType &call, QWidget *parent)
		{
			    TRACEPOINT;
			    return cvv::util::make_unique<View>(call, parent);
		    });
	}

      protected:
	/**
	 * @brief Scope to search the default view in.
	 */
	QString default_scope_;
	/**
	 * @brief Key under which the default view is saved.
	 */
	QString default_key_;
	/**
	 * @brief standard default view.
	 */
	QString standard_default_;

	/**
	 * @brief Sets up the visible parts.
	 * Called by the constructors.
	 */
	void createGui()
	{
		TRACEPOINT;
		// Sets standard_default_ as default in case no other default is
		// set:
		qtutil::setDefaultSetting(default_scope_, default_key_,
		                          standard_default_);
		viewId_ = qtutil::getSetting(default_scope_, default_key_);

		if (!this->select(viewId_))
		{
			this->select(standard_default_);
			viewId_ = this->selection();
			setAsDefaultButtonClicked.slot(); // Set as default.
			/* If viewId_ does not name a valid View, it will be
			 * attempted to set standard_default_.
			 * If that was not registered either, the current
			 * selection of the ComboBox will be used automatically.
			 * Whichever was chosen will be set as the new default.
			 */
		}
		hlayout_ = new QHBoxLayout{};
		hlayout_->setAlignment(Qt::AlignTop);
		hlayout_->addWidget(new QLabel{ "View:" });
		hlayout_->addWidget(this->comboBox_);
		setAsDefaultButton_ = new QPushButton{ "Set as default", this };
		hlayout_->addWidget(setAsDefaultButton_);
		helpButton_ = new QPushButton{ "Help", this };
		hlayout_->addWidget(helpButton_);

		upperBar_ = new QWidget{ this };
		upperBar_->setLayout(hlayout_);

		vlayout_ = new QVBoxLayout{};

		vlayout_->addWidget(upperBar_);
		setView();

		setLayout(vlayout_);

		QObject::connect(setAsDefaultButton_, SIGNAL(clicked()),
		                 &setAsDefaultButtonClicked, SLOT(slot()));
		QObject::connect(helpButton_, SIGNAL(clicked()),
		                 &helpButtonClicked, SLOT(slot()));
		QObject::connect(&this->signalElementSelected(),
		                 SIGNAL(signal(QString)), &currentIndexChanged,
		                 SLOT(slot()));
		TRACEPOINT;
	}

      private:
	/**
	 * @brief sets up the View currently selected in the ComboBox inherited
	 * from RegisterHelper.
	 */
	void setView()
	{
		TRACEPOINT;
		viewId_ = this->selection();
		if (viewHistory_.count(this->selection()))
		{
			view_ = viewHistory_.at(this->selection());
			vlayout_->addWidget(view_);
			view_->setVisible(true);
		}
		else
		{
			viewHistory_.emplace(
			    this->selection(),
			    ((*this)()(*call_, this).release()));
			view_ = viewHistory_.at(this->selection());
			vlayout_->addWidget(view_);
		}
		TRACEPOINT;
	}

	util::Reference<const CallType> call_;
	QString viewId_;
	ViewType *view_;
	std::map<QString, ViewType *> viewHistory_;

	QPushButton *helpButton_;
	QPushButton *setAsDefaultButton_;
	QHBoxLayout *hlayout_;
	QVBoxLayout *vlayout_;
	QWidget *upperBar_;

	// slots:
	/**
	* @brief View selection change.
	* Slot called when the index of the view selection changes.
	*/
	qtutil::Slot currentIndexChanged;
	/**
	 * @brief Help Button clicked.
	 * Called when the help button is clicked.
	 */
	const qtutil::Slot helpButtonClicked;
	/**
	 * @brief setAsDefaultButton clicked.
	 * Called when the setAsDefaultButton,which sets the current view as
	 * default, is clicked.
	 */
	qtutil::Slot setAsDefaultButtonClicked;
};
}
} // namespaces

#endif
