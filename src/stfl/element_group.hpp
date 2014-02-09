#ifndef CVVISUAL_ELEMENTGROUP_HPP
#define CVVISUAL_ELEMENTGROUP_HPP

#include <functional>
#include <QList>
#include <QStringList>
#include <QString>
#include <stdint.h>
#include <stdexcept>

namespace cvv {
namespace stfl {

/**
 * @brief A group of elements with a title.
 */
template<class Element>
class ElementGroup
{
public:
	
    ElementGroup(){
        //titles = QStringList();
        //elements = QList<Element>();
    }
	
	/**
	 * Constructs a new ElementGroup
	 * @param _titles title of this group, consisting of several sub titles
	 * @param _elements elements of this group 
	 */
    ElementGroup(const QStringList _titles, const QList<Element> &_elements):
        titles{_titles}, elements{_elements} {}
	/**
	 * Checks whether or not this group contains the element.
	 */
	bool contains(const Element &element)
	{
		return this->elements.contains(element);
	}

	/**
	 * Return the inherited elements.
	 */
	QList<Element> getElements()
	{
		return this->elements;
	}

	/**
	 * Returns the number of elements in this group.
	 */
    size_t size() const
	{
		return this->elements.size();
	}

    /**
     * Returns the title (consisting of sub titles).
     */
    QStringList getTitles() const
    {
        return this->titles;
    }

	/**
	 * Get the element at the given index (in this group).
	 * 
	 * @param index given index
	 * @throws std::invalid_argument if no such element exists
	 */
	Element get(size_t index)
	{
		if (index >= size())
		{
			throw std::invalid_argument{"there is no call with this id"};
		}
		return this->elements[index];
	}

private:
	const QStringList titles;
	const QList<Element> elements;
};


}
}
#endif
