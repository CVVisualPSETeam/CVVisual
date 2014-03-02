#ifndef CVVISUAL_REGISTERHELPER_HPP
#define CVVISUAL_REGISTERHELPER_HPP
//std
#include <map>
#include <vector>
#include <stdexcept>
#include <memory>
#include <functional>
//QT
#include <QWidget>
#include <QString>
#include <QComboBox>
#include <QVBoxLayout>
//cvv
#include "signalslot.hpp"
#include "../dbg/dbg.hpp"

namespace cvv { namespace qtutil{
/**
 * @brief The RegisterHelper class can be inherited to gain a mechanism to register fabric functions
 * for QWidgets.
 *
 * The registered functions are shared between all instances of a class.
 * A QComboBox is provided for user selection.
 * The content of the QComboBox is updated whenever a function is registered.
 *
 * Inheriting classes have to delete the member comboBox_ on destruction!
 * (e.g. by putting it into a layout)
 */
template<class Value, class...Args>
class RegisterHelper
{
public:
	/**
	 * @brief Constructor
	 */
	RegisterHelper()
		:comboBox_{new QComboBox{}},
		slotElementRegistered_{[&](const QString& name){
			TRACEPOINT;
			comboBox_->addItem(name);
			TRACEPOINT;
		}}
	{
		TRACEPOINT;
		//elem registered
		QObject::connect(&signElementRegistered_,&SignalQString::signal,
				 &slotElementRegistered_,&SlotQString::slot);
		//connect
		QObject::connect(comboBox_, &QComboBox::currentTextChanged,
				 &signElementSelected_, &SignalQString::signal);
		//add current list of elements
		for(auto& elem: RegisterHelper<Value,Args...>::registeredElements_)
			{comboBox_->addItem(elem.first);}

		TRACEPOINT;
	}

	~RegisterHelper(){TRACEPOINT;}

	/**
	 * @brief Returns the current selection from the QComboBox
	 * @return The current selection from the QComboBox
	 */
	QString selection() const
	{
		TRACEPOINT;
		return comboBox_->currentText();
	}

	/**
	 * @brief Checks whether a function was registered with the name.
	 * @param name The name to look up
	 * @return true if there is a function. false otherwise
	 */
	static bool has(const QString& name)
	{
		TRACEPOINT;
		return registeredElements_.find(name) != registeredElements_.end();
	}

	/**
	 * @brief Returns the names of all registered functions.
	 * @return The names of all registered functions.
	 */
	static std::vector<QString> registeredElements()
	{
		TRACEPOINT;
		std::vector<QString> result{};
		for(auto& elem:registeredElements_)
		{
			result.push_back(elem.first);
		};
		TRACEPOINT;
		return result;
	}

	/**
	 * @brief Registers a function.
	 * @param name The name.
	 * @param fabric The fabric function.
	 * @return true if the function was registered. false if the name was taken
	 * (the function was not registered!)
	 */
	static bool registerElement(const QString& name,
				const std::function< std::unique_ptr<Value>(Args...)>& fabric)
	{
		TRACEPOINT;
		if(has(name))
		{
			TRACEPOINT;
			return false;
		};

		registeredElements_.emplace(name, fabric);

		signElementRegistered_.emitSignal(name);

		TRACEPOINT;
		return true;
	}

	/**
	 * @brief Selects an function according to name.
	 * @param name The name of the function to select.
	 * @return true if the function was selected. false if no function has name.
	 */
	bool select(const QString& name)
	{
		TRACEPOINT;
		if(!has(name))
			{return false;}
		comboBox_->setCurrentText(name);
		return true;
		TRACEPOINT;
	}

	/**
	 * @brief Returns the function according to the current selection of the QComboBox.
	 * @throw std::out_of_range If there is no such function.
	 * @return The function according to the current selection of the QComboBox.
	 */
	std::function<std::unique_ptr<Value>(Args...)> operator()()
	{
		TRACEPOINT;
		return (*this)(selection());
	}

	/**
	 * @brief Returns the function according to name.
	 * @param The name of a registered function.
	 * @throw std::out_of_range If there is no such function.
	 * @return The function according to name.
	 */
	std::function<std::unique_ptr<Value>(Args...)> operator()(const QString& name)
	{
		TRACEPOINT;
		return registeredElements_.at(name);
	}

	/**
	 *@brief Signal emitted whenever a function is registered.
	 *@todo SYNCHRONIZE
	 */
	//thread_local
	static const SignalQString signElementRegistered_;

	const SignalQString signElementSelected_;

protected:
	/**
	 * @brief Map of registered functions and their names.
	 *@todo SYNCHRONIZE
	 */
	//thread_local
	static std::map<QString,std::function<std::unique_ptr<Value>(Args...)>>
									registeredElements_;

	/**
	 * @brief QComboBox containing all names of registered functions
	 */
	QComboBox* comboBox_;

	/**
	 * @brief Slot called whenever a function is registered
	 */
	SlotQString slotElementRegistered_;
};

template<class Value, class...Args>
	std::map<QString,std::function<std::unique_ptr<Value>(Args...)>>
		RegisterHelper<Value,Args...>::registeredElements_{};


template<class Value, class...Args>
	const  SignalQString RegisterHelper<Value,Args...>::signElementRegistered_{};
}} // end namespaces qtutil, cvv

#endif //CVVISUAL_REGISTERHELPER_HPP
