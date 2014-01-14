#ifndef CVVISUAL_ACCORDION_HPP
#define CVVISUAL_ACCORDION_HPP
//STD
#include <memory>
#include <stdexcept>
#include <map>
#include <limits>
//QT
#include <QWidget>
#include <QString>
#include <QVBoxLayout>
//CVV
#include "collapsable.hpp"
#include "../util/util.hpp"

namespace cvv{ namespace qtutil
{
/**
 * @brief The Accordion class
 *
 * Contains multiple widgets an their title. These get stored in collapsables.
 * The collapsables are stored in a collumn.
 */
class Accordion : public QWidget
{
	Q_OBJECT
public:
	/**
	 * @brief the handle type to access elements
	 */
	typedef std::size_t Handle;

	/**
	 * @brief Constructs an empty accordion.
	 * @param parent the paren widget
	 */
	explicit Accordion(QWidget *parent = 0);

	/**
	 * @brief returns the element corrsponding to handle
	 * @throw std::out_of_range if there is no element corresponding to handle
	 * @return the element corrsponding to handle
	 */
	Collapsable& element(Handle handle)
		{return *elements.at(handle);}

	/**
	 * @brief collapses an element
	 * @param handle the element to collapse
	 * @param b
	 * @parblock
	 * 		true: collapses the widget
	 * 		false: expands the widget
	 * @endparblock
	 * @throw std::out_of_range if there is no element corresponding to handle
	 */
	void collapseElement(Handle handle, bool b = true)
		{element(handle).collapse(b);}

	/**
	 * @brief expands an element
	 * @param handle element to expand
	 * @param b
	 * @parblock
	 * 		true: expands the widget
	 * 		false: collapses the widget
	 * @endparblock
	 * @throw std::out_of_range if there is no element corresponding to handle
	 */
	void expandElement(Handle handle , bool b = true)
		{collapseElement(handle, !b);}

	/**
	 * @brief collapses all elements
	 * @param b
	 * @parblock
	 * 		true: collapses all elements
	 * 		false: expands all elements
	 * @endparblock
	 */
	void collapseAll(bool b = true);

	/**
	 * @brief expands all elements
	 * @param b
	 * @parblock
	 * 		true: expands all elements
	 * 		false: collapses all elements
	 * @endparblock
	 */
	void expandAll(bool b = true)
		{collapseAll(!b);}

	/**
	 * @brief makes the element invisible
	 * @param handle the element
	 * @param b
	 * @parblock
	 * 		true: makes the element invisible
	 * 		false: makes the element visible
	 * @endparblock
	 * @throw std::out_of_range if there is no element corresponding to handle
	 */
	void hide(Handle handle, bool b = true)
		{element(handle).setVisible(!b);}

	/**
	 * @brief makes the element visible
	 * @param handle the element
	 * @param b
	 * @parblock
	 * 		true: makes the element visible
	 * 		false: makes the element invisible
	 * @endparblock
	 * @throw std::out_of_range if there is no element corresponding to handle
	 */
	void show(Handle handle, bool b = true)
		{hide(handle,!b);}

	/**
	 * @brief sets all elements visibility to !b
	 * @param b
	 * @parblock
	 * 		true: makes all elements invisible
	 * 		false: makes all elements visible
	 * @endparblock
	 */
	void hideAll(bool b = true);

	/**
	 * @brief sets all elements visibility to b
	 * @param b
	 * @parblock
	 * 		true: makes all elements visible
	 * 		false: makes all elements invisible
	 * @endparblock
	 */
	void showAll(bool b = true)
		{hideAll(!b);}

	/**
	 * @brief inserts a widget at the given position
	 * @param title the title to display
	 * @param widget the widget to display
	 * @param isCollapsed whether the widget is collapsed after creation
	 * @param position the position. if it is greater than the number of elements the widget
	 *	will be added to the end
	 * @throw std::length_error if there is no free handle
	 * @return the handle to access the element
	 */
	Handle insert(const QString& title,QWidget& widget, bool isCollapsed = true,
			std::size_t position = std::numeric_limits<std::size_t>::max());


	/**
	 * @brief adds a widget to the end of the Accordion
	 * @param title the title to display
	 * @param widget the widget to display
	 * @param isCollapsed whether the widget is collapsed after creation
	 * @throw std::length_error if there is no free handle
	 * @return the handle to access the element
	 */
	Handle push_back(const QString& title,QWidget& widget, bool isCollapsed = true)
		{return insert(title,widget, isCollapsed);}

	/**
	 * @brief adds a widget to the front of the Accordion
	 * @param title the title to display
	 * @param widget the widget to display
	 * @param isCollapsed whether the widget is collapsed after creation
	 * @throw std::length_error if there is no free handle
	 * @return the handle to access the element
	 */
	Handle push_front(const QString& title,QWidget& widget, bool isCollapsed = true)
		{return insert(title,widget, isCollapsed, 0);}


	/**
	 * @brief removes the element
	 * @param handle handle of the element
	 * @throw std::out_of_range if there is no element corresponding to handle
	 */
	void remove(Handle handle);

	/**
	 * @brief removes all elements
	 */
	void clear();

	/**
	 * @brief removes an element and returns its title and reference
	 * @param handle handle handle of the element
	 * @return title and reference
	 * @throw std::out_of_range if there is no element corresponding to handle
	 */
	std::pair<QString, QWidget&> pop(Handle handle);

	/**
	 * @brief removes alle elements from the Accordion and returns their titles
	 *	and references
	 * @return a vector containing all titles and references
	 */
	std::vector<std::pair<QString, QWidget&>> popAll();

	/**
	 * @brief returns the number of elements
	 * @return the number of elements
	 */
	std::size_t size();

	/**
	 * @brief returns the maximal number of elements
	 * @return the maximal number of elements
	 */
	std::size_t max_size()
		{return std::numeric_limits<std::size_t>::max();}
private:
	/**
	 * @brief returns the next free handle
	 * @throw std::length_error if there is no free handle
	 * @return next free handle
	 */
	Handle getNextHandle();

	/**
	 * @brief returns whether handle is valid
	 * @param handle handle to check
	 * @return if handle is valid
	 */
	bool isValidHandle(Handle handle)
		{return elements.end() != elements.find(handle);}

	/**
	 * @brief storage for all elements
	 */
	std::map<Handle, std::unique_ptr<Collapsable>> elements;


	/**
	 * @brief for all elements
	 */
	QVBoxLayout layout;

	/**
	 * @brief the last handle used. (0 is there is no handle in use)
	 */
	Handle lastHandle;
}; // Accordion
}} // end namespaces qtutil, cvv
#endif // CVVISUAL_ACCORDION_HPP
