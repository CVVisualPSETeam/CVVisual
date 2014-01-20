#ifndef CVVISUAL_SIGNALEMITTER_HPP
#define CVVISUAL_SIGNALEMITTER_HPP

//std
#include <functional>
#include <stdexcept>

//QT
#include <QObject>
#include <QString>

namespace cvv { namespace qtutil{

/**
 * @brief The Signal class can be used for privat or static signals and in case of
 * conflicts between templates and Q_OBJECT.
 */
class Signal: public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief Constructor
	 * @param parent The parent
	 */
	Signal(QObject* parent = nullptr):QObject{parent}{}

	/**
	 * @brief Emits the signal.
	 * @param args The arguments
	 */
	void emitSignal(){emit signal();}
signals:
	/**
	 * @brief The signal emited by emitSignal.
	 */
	void signal();
};

/**
 * @brief The Slot class can be used for static slots and in case of conflicts between
 * templates and Q_OBJECT.
 */
class Slot: public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief Constructor
	 * @param f Function called by the slot slot()
	 * @throw std::invalid_argument If f is invalide
	 * @param parent The parent
	 */
	Slot(const std::function<void()>& f, QObject* parent = nullptr):
		QObject{parent}, function_{f}
		{if(!f)throw std::invalid_argument{"invalide function"};}

public slots:
	/**
	 * @brief The slot calling function()
	 */
	void slot(){function_();}
private:
	/**
	 * @brief The function called by the slot slot()
	 */
	std::function<void()> function_;
};

// ///////////////////////////////////////////////////////////////
// manual "templating" for classes Signal and Slot
// ///////////////////////////////////////////////////////////////

/**
 * @brief The SignalQString class similar to Signal but takes a QString
 */
class SignalQString: public QObject
{
	Q_OBJECT
public:
	SignalQString(QObject* parent = nullptr):QObject{parent}{}

	void emitSignal(const QString& s){emit signal(s);}
signals:
	void signal(QString s);
};

/**
 * @brief The SlotQString class similar to Slot but accepts a QString
 */
class SlotQString: public QObject
{
	Q_OBJECT
public:
	SlotQString(const std::function<void(QString)>& f, QObject* parent = nullptr):
		QObject{parent}, function_{f}
		{if(!f) throw std::invalid_argument{"invalide function"};}

public slots:
	void slot(QString s){function_(s);}
private:
	std::function<void(QString)> function_;
};

}} // end namespaces qtutil, cvv
#endif //CVVISUAL_SIGNALEMITTER_HPP
