#ifndef CVVISUAL_COLLAPSABLE_H
#define CVVISUAL_COLLAPSABLE_H
//std
#include <cstddef>
//QT
#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

#include "../util/util.hpp"
#include "../util/observer_ptr.hpp"
#include "../dbg/dbg.hpp"

namespace cvv{ namespace qtutil{

/**
 * @brief Contains a widget and a title.
 *
 * The widget can be collapsed and expanded with a button.
 * If the widget is collapsed only button and title are shown.
 */
class Collapsable : public QFrame
{
	Q_OBJECT
public:
	/**
	 * @brief Constructs a collapsable
	 * @param title The title above the widget.
	 * @param widget The widget to store.
	 * @param isCollapsed If true the contained widget will be collapsed. (It will be shown
	 * otherwise.)
	 */
	//explicit Collapsable(const QString& title, QWidget& widget, bool isCollapsed = true,
	//		QWidget *parent = 0);
	explicit Collapsable(const QString& title, std::unique_ptr<QWidget> widget, bool isCollapsed = true,
			QWidget *parent = 0);

	~Collapsable(){TRACEPOINT;}

	/**
	 * @brief Collapses the contained widget.
	 * @param b
	 * @parblock
	 * 		true: collapses the widget
	 * 		false: expands the widget
	 * @endparblock
	 */
	void collapse(bool b = true);

	/**
	 * @brief Expands the contained widget.
	 * @param b
	 * @parblock
	 * 		true: expands the widget
	 * 		false: collapses the widget
	 * @endparblock
	*/
	void expand(bool b = true)
	{
		TRACEPOINT;
		collapse(!b);
		TRACEPOINT;
	}

	/**
	* @brief Sets the title above the widget.
	*/
	void setTitle(const QString& title)
	{
		TRACEPOINT;
		button_->setText(title);
		TRACEPOINT;
	}

	/**
	 * @brief Returns the current title above the widget.
	 * @return The current title above the widget
	 */
	QString title() const
	{
		TRACEPOINT;
		return button_->text();
	}

	/**
	 * @brief Returns a reference to the contained widget.
	 * @return A reference to the contained widget.
	 */
	QWidget& widget()
	{
		TRACEPOINT;return *widget_;
	}


	const QWidget& widget() const
		{TRACEPOINT;return *widget_;}

	/**
	 * @brief Detaches the contained widget. (ownership remains)
	 * @return The contained widget
	 */
	QWidget* detachWidget();

private slots:
	/**
	 * @brief Toggles the visibility.
	 */
	void toggleVisibility()
	{
		TRACEPOINT;
		collapse(widget_->isVisible());
		TRACEPOINT;
	}

private:
	/**
	 * @brief The contained widget
	 */
	QWidget* widget_;

	/**
	 * @brief The button to toggle the widget
	 */
	QPushButton* button_;

	/**
	 * @brief The layout containing the header and widget
	 */
	util::ObserverPtr<QVBoxLayout> layout_;
}; //Collapsable
}} // end namespaces qtutil, cvv

#endif // CVVISUAL_COLLAPSABLE_H
