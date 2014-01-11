#ifndef CVVISUAL_AKKORDEON_H
#define CVVISUAL_AKKORDEON_H
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
 * @brief The Akkordeon class
 *
 * Contains multiple widgets an their title. These get stored in collapsables.
 * The collapsables are stored in a collumn.
 */
class Akkordeon : public QWidget
{
	Q_OBJECT
public:
	/**
	 * @brief handle_t the handle type to access elements
	 */
	typedef std::size_t handle_t;

	/**
	 * @brief Akkordeon Constructs an empty akkordeon.
	 * @param parent the paren widget
	 */
	explicit Akkordeon(QWidget *parent = 0);

	/**
	 * @throw std::out_of_range if there is no element corresponding to handle
	 * @return the element corrsponding to handle
	 */
	Collapsable& element(handle_t handle)
		{return *elements.at(handle);}

	/**
	 * @brief collapseElement collapses an element
	 * @param handle the element to collapse
	 * @param b
	 * @parblock
	 * 		true: collapses the widget
	 * 		false: expands the widget
	 * @endparblock
	 * @throw std::out_of_range if there is no element corresponding to handle
	 */
	void collapseElement(handle_t handle, bool b = true)
		{element(handle).collapse(b);}

	/**
	 * @brief expandElement expands an element
	 * @param handle element to expand
	 * @param b
	 * @parblock
	 * 		true: expands the widget
	 * 		false: collapses the widget
	 * @endparblock
	 * @throw std::out_of_range if there is no element corresponding to handle
	 */
	void expandElement(handle_t handle , bool b = true)
		{collapseElement(handle, !b);}

	/**
	 * @brief collapseAll collapses all elements
	 * @param b
	 * @parblock
	 * 		true: collapses all elements
	 * 		false: expands all elements
	 * @endparblock
	 */
	void collapseAll(bool b = true);

	/**
	 * @brief expandAll expands all elements
	 * @param b
	 * @parblock
	 * 		true: expands all elements
	 * 		false: collapses all elements
	 * @endparblock
	 */
	void expandAll(bool b = true)
		{collapseAll(!b);}

	/**
	 * @brief hide makes the element invisible
	 * @param handle the element
	 * @param b
	 * @parblock
	 * 		true: makes the element invisible
	 * 		false: makes the element visible
	 * @endparblock
	 * @throw std::out_of_range if there is no element corresponding to handle
	 */
	void hide(handle_t handle, bool b = true)
		{element(handle).setVisible(!b);}

	/**
	 * @brief show makes the element visible
	 * @param handle the element
	 * @param b
	 * @parblock
	 * 		true: makes the element visible
	 * 		false: makes the element invisible
	 * @endparblock
	 * @throw std::out_of_range if there is no element corresponding to handle
	 */
	void show(handle_t handle, bool b = true)
		{hide(handle,!b);}

	/**
	 * @brief hideAll sets all elements visibility to !b
	 * @param b
	 * @parblock
	 * 		true: makes all elements invisible
	 * 		false: makes all elements visible
	 * @endparblock
	 */
	void hideAll(bool b = true);

	/**
	 * @brief showAll sets all elements visibility to b
	 * @param b
	 * @parblock
	 * 		true: makes all elements visible
	 * 		false: makes all elements invisible
	 * @endparblock
	 */
	void showAll(bool b = true)
		{hideAll(!b);}

	/**
	 * @brief insert inserts a widget at the given position
	 * @param title the title to display
	 * @param widget the widget to display
	 * @param isCollapsed whether the widget is collapsed after creation
	 * @param position the position. if it is greater than the number of elements the widget
	 *	will be added to the end
	 * @throw std::length_error if there is no free handle
	 * @return the handle to access the element
	 */
	handle_t insert(const QString& title,QWidget& widget, bool isCollapsed = true,
			std::size_t position = std::numeric_limits<std::size_t>::max());


	/**
	 * @brief push_back adds a widget to the end of the akkordeon
	 * @param title the title to display
	 * @param widget the widget to display
	 * @param isCollapsed whether the widget is collapsed after creation
	 * @throw std::length_error if there is no free handle
	 * @return the handle to access the element
	 */
	handle_t push_back(const QString& title,QWidget& widget, bool isCollapsed = true)
		{return insert(title,widget, isCollapsed);}

	/**
	 * @brief push_front adds a widget to the front of the akkordeon
	 * @param title the title to display
	 * @param widget the widget to display
	 * @param isCollapsed whether the widget is collapsed after creation
	 * @throw std::length_error if there is no free handle
	 * @return the handle to access the element
	 */
	handle_t push_front(const QString& title,QWidget& widget, bool isCollapsed = true)
		{return insert(title,widget, isCollapsed, 0);}


	/**
	 * @brief remove removes the element
	 * @param handle handle of the element
	 * @throw std::out_of_range if there is no element corresponding to handle
	 */
	void remove(handle_t handle);

	/**
	 * @brief clear removes all elements
	 */
	void clear();

	/**
	 * @brief pop removes an element and returns its title and reference
	 * @param handle handle handle of the element
	 * @return title and reference
	 * @throw std::out_of_range if there is no element corresponding to handle
	 */
	std::pair<QString, QWidget&> pop(handle_t handle);

	/**
	 * @brief popAll removes alle elements from the akkordeon and returns their titles
	 *	and references
	 * @return a vector containing all titles and references
	 */
	std::vector<std::pair<QString, QWidget&>> popAll();

	/**
	 * @brief size returns the number of elements
	 * @return the number of elements
	 */
	std::size_t size();

	/**
	 * @brief max_size returns the maximal number of elements
	 * @return the maximal number of elements
	 */
	std::size_t max_size()
		{return std::numeric_limits<std::size_t>::max();}
private:
	/**
	 * @brief getNextHandle returns the next free handle
	 * @throw std::length_error if there is no free handle
	 * @return next free handle
	 */
	handle_t getNextHandle();

	/**
	 * @brief isValidHandle returns whether handle is valid
	 * @param handle handle to check
	 * @return if handle is valid
	 */
	bool isValidHandle(handle_t handle)
		{return elements.end() != elements.find(handle);}

	/**
	 * @brief elements storage for all elements
	 */
	std::map<handle_t, std::unique_ptr<Collapsable>> elements;


	/**
	 * @brief layout layout for all elements
	 */
	QVBoxLayout layout;

	/**
	 * @brief lastHandle the last handle used. (0 is there is no handle in use)
	 */
	handle_t lastHandle;
}; // Akkordeon
}} // end namespaces qtutil, cvv
#endif // CVVISUAL_AKKORDEON_H
