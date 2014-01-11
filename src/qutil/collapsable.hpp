#ifndef CVVISUAL_COLLAPSABLE_H
#define CVVISUAL_COLLAPSABLE_H

//QT
#include <qt5/QtWidgets/QWidget>
#include <qt5/QtCore/QString>
#include <qt5/QtWidgets/QToolButton>
#include <qt5/QtWidgets/QHBoxLayout>
#include <qt5/QtWidgets/QVBoxLayout>
#include <qt5/QtWidgets/QLabel>

namespace cvv{ namespace qtutil{

/**
* Contains a widget and a title.
* The widget can be collapsed and expanded with a button.
* If the widget is collapsed only button and title are shown.
*/
class Collapsable : public QWidget
{
	Q_OBJECT
public:
	/**
	* @param title 	The title above the widget.
	* @param widget The widget to store.
	* @param isCollapsed If true the contained widget will be collapsed. (It will be shown
	* otherwise.)
	*/
	explicit Collapsable(const QString& title,QWidget& widget, bool isCollapsed = true,
			QWidget *parent = 0);

	~Collapsable() = default;

	/**
	* @param b
	* @parblock
	* 		true: collapses the widget
	* 		false: expands the widget
	* @endparblock
	*/
	void collapse(bool b = true);

	/**
	 * @param b
	 * @parblock
	 * 		true: expands the widget
	 * 		false: collapses the widget
	 * @endparblock
	*/
	void expand(bool b = true){collapse(!b);}

	/**
	* Sets the title above the widget.
	*/
	void setTitle(const QString& title);

	/**
	 * @brief title Returns the current title above the widget.
	 * @return the current title above the widget
	 */
	QString title();

	/**
	 * @brief widget Returns a reference to the contained widget.
	 * @return a reference to the contained widget.
	 */
	QWidget& widget();

private slots:
	/**
	 * @brief toggleVisibility Toggles the visibility.
	 */
	void toggleVisibility();

private:
	/**
	 * @brief title_ the title above the widget
	 */
	QLabel title_;

	/**
	 * @brief widget_ the contained widget
	 */
	QWidget& widget_;

	/**
	 * @brief button the button to toggle the widget
	 */
	QToolButton button;

	/**
	 * @brief layoutHeader the layout for the button and the title (the header)
	 */
	QVBoxLayout layoutHeader;

	/**
	 * @brief layoutHeaderAndWidget the layout for the widget and the header
	 */
	QHBoxLayout layoutHeaderAndWidget;
}; //Collapsable
}} // end namespaces qtutil, cvv

#endif // CVVISUAL_COLLAPSABLE_H
