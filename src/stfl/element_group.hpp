#ifndef CVVISUAL_ELEMENTGROUP_HPP
#define CVVISUAL_ELEMENTGROUP_HPP

#include <functional>
#include <QList>
#include <QStringList>
#include <QString>
#include <stdint.h>
#include <stdexcept>

#include "../dbg/dbg.hpp"

namespace cvv {
namespace stfl {

/**
 * @brief A group of elements with a title.
 */
template<class Element>
class ElementGroup
{
public:
	
	/**
	 * @brief Contructs an empty ElementGroup.
	 */
    ElementGroup(){}
	
	/**
	 * @brief Constructs a new ElementGroup
	 * @param _titles title of this group, consisting of several sub titles
	 * @param _elements elements of this group 
	 */
    ElementGroup(const QStringList _titles, const QList<Element> &_elements):
        titles{_titles}, elements{_elements} {}
	/**
	 * @brief Checks whether or not this group contains the element.
	 * @param element element to be checked
	 * @return does this group contain the given element?
	 */
	bool contains(const Element &element)
	{
		TRACEPOINT;
		return this->elements.contains(element);
	}

	/**
	 * @brief Return the inherited elements.
	 * @return the interited elements
	 */
	QList<Element> getElements()
	{
		TRACEPOINT;
		return this->elements;
	}

	/**
	 * @brief Returns the number of elements in this group.
	 * @return number of elements in this group
	 */
    size_t size() const
	{
		TRACEPOINT;
		return this->elements.size();
	}

    /**
     * @brief Returns the title (consisting of sub titles).
     * @return the group title
     */
    QStringList getTitles() const
    {
		TRACEPOINT;
        return this->titles;
    }

	/**
	 * @brief Get the element at the given index (in this group).
	 * 
	 * @param index given index
	 * @return element at the given index
	 * @throws std::invalid_argument if no such element exists
	 */
	Element get(size_t index)
	{
		TRACEPOINT;
		if (index >= size())
		{
			throw std::invalid_argument{"there is no call with this id"};
		}
		TRACEPOINT;
		return this->elements[index];
	}

private:
	const QStringList titles;
	const QList<Element> elements;
};


}
}
#endif
