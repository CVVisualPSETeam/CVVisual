#ifndef CVVISUAL_REGISTERHELPER_HPP
#define CVVISUAL_REGISTERHELPER_HPP
//std
#include <map>
#include <vector>
#include <stdexcept>
#include <memory>
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
 * The content of the QComboBox is updated whenever a new function is registered.
 *
 * @todo SYNCHRONIZE
 */
template<class Value, class...Args>
class RegisterHelper
{
public:
	/**
	 * @brief Constructor
	 * @param parent The parent
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
		QObject::connect(&(this->signElementRegistered_),
			SIGNAL(signal(QString)),
			&(this->slotElementRegistered_),
			SLOT(slot(QString)));
		for(auto& elem: RegisterHelper<Value,Args...>::registeredElements_)
			{comboBox_->addItem(elem.first);}
		select(selection());
		TRACEPOINT;
	}

	~RegisterHelper(){TRACEPOINT;}

	/**
	 * @brief Returns the current selection from the QComboBox
	 * @return The current selection from the QComboBox
	 */
	QString selection() const
		{TRACEPOINT;return comboBox_->currentText();}

	/**
	 * @brief Checks whether a function was registered with the name.
	 * @param name The name to look up
	 * @return true if there is a function. false otherwise
	 */
	static bool has(const QString& name)
		{TRACEPOINT;return registeredElements_.find(name) != registeredElements_.end();}

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
	 * @brief Registers a new function.
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
			{TRACEPOINT;return false;};

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
		{TRACEPOINT;return (*this)(selection());}

	/**
	 * @brief Returns the function according to name.
	 * @param The name of a registered function.
	 * @throw std::out_of_range If there is no such function.
	 * @return The function according to name.
	 */
	std::function<std::unique_ptr<Value>(Args...)> operator()(const QString& name)
		{TRACEPOINT;return registeredElements_.at(name);}

	/**
	 *@brief Signal emitted whenever a new function is registered.
	 *@todo SYNCHRONIZE
	 */
	//thread_local
	static SignalQString signElementRegistered_;

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
	 * @brief Slot called whenever a new function is registered
	 */
	SlotQString slotElementRegistered_;
};
/**
 * @todo SYNCHRONIZE
 */
template<class Value, class...Args>
	//thread_local
	std::map<QString,std::function<std::unique_ptr<Value>(Args...)>>
		RegisterHelper<Value,Args...>::registeredElements_{};

/**
 * @todo SYNCHRONIZE
 */
template<class Value, class...Args>
	//thread_local
	SignalQString RegisterHelper<Value,Args...>::signElementRegistered_{};
}} // end namespaces qtutil, cvv

#endif //CVVISUAL_REGISTERHELPER_HPP
