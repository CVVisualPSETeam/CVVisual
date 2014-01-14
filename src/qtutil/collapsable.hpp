#ifndef CVVISUAL_COLLAPSABLE_H
#define CVVISUAL_COLLAPSABLE_H

//QT
#include <QString>
#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

namespace cvv{ namespace qtutil{

/**
 * @brief Contains a widget and a title.
 *
 * The widget can be collapsed and expanded with a button.
 * If the widget is collapsed only button and title are shown.
 *
 *
 * @TODO fix crash during destruction (illegal free)
 */
class Collapsable : public QWidget
{
	Q_OBJECT
public:
	/**
	 * @brief constructs a collapsable
	 * @param title 	The title above the widget.
	 * @param widget The widget to store.
	 * @param isCollapsed If true the contained widget will be collapsed. (It will be shown
	 * otherwise.)
	 */
	explicit Collapsable(const QString& title,QWidget& widget, bool isCollapsed = true,
			QWidget *parent = 0);

	/**
	 * @brief collapses the contained widget.
	 * @param b
	 * @parblock
	 * 		true: collapses the widget
	 * 		false: expands the widget
	 * @endparblock
	 */
	void collapse(bool b = true);

	/**
	 * @brief expands the contained widget.
	 * @param b
	 * @parblock
	 * 		true: expands the widget
	 * 		false: collapses the widget
	 * @endparblock
	*/
	void expand(bool b = true){collapse(!b);}

	/**
	* @brief Sets the title above the widget.
	*/
	void setTitle(const QString& title);

	/**
	 * @brief Returns the current title above the widget.
	 * @return the current title above the widget
	 */
	QString title();

	/**
	 * @brief Returns a reference to the contained widget.
	 * @return a reference to the contained widget.
	 */
	QWidget& widget();

private slots:
	/**
	 * @brief Toggles the visibility.
	 */
	void toggleVisibility();

private:
	/**
	 * @brief the title above the widget
	 */
	QLabel title_;

	/**
	 * @brief the contained widget
	 */
	QWidget& widget_;

	/**
	 * @brief the button to toggle the widget
	 */
	QToolButton button;

	/**
	 * @brief the layout for the widget and the header
	 */
	QVBoxLayout layoutHeaderAndWidget;

	/**
	 * @brief layoutHeader the layout for the button and the title (the header)
	 */
	QHBoxLayout layoutHeader;
}; //Collapsable
}} // end namespaces qtutil, cvv

#endif // CVVISUAL_COLLAPSABLE_H
