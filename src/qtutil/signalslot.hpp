#ifndef CVVISUAL_SIGNALEMITTER_HPP
#define CVVISUAL_SIGNALEMITTER_HPP

//std
#include <functional>
//QT
#include <QObject>

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
	Signal(QObject* parent = nullptr);
	/**
	 * @brief Emits the signal.
	 * @param args The arguments
	 */
	void emitSignal();
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
	 * @param parent The parent
	 */
	Slot(const std::function<void()>& f, QObject* parent = nullptr);
public slots:
	/**
	 * @brief The slot calling function()
	 */
	void slot();
private:
	/**
	 * @brief The function called by the slot slot()
	 */
	std::function<void()> function_;
};

}} // end namespaces qtutil, cvv
#endif //CVVISUAL_SIGNALEMITTER_HPP
