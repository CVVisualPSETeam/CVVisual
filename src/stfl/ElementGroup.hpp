#ifndef CVVISUAL_ELEMENTGROUP_HPP
#define CVVISUAL_ELEMENTGROUP_HPP

#include <functional>
#include <QList>
#include <QStringList>
#include <QString>
#include <stdint.h>

template<class Element>
class ElementGroup
{
public:
    ElementGroup(QStringList _titles, const QList<Element> _elements){
        this->elements = _elements;
        this->titles = _titles;
    }

    bool contains(Element element){
        return this->elements.contains(element);
    }

    const std::vector<Element> getElements(){
        return this->elements;
    }

    size_t size(){
        return this->elements.size();
    }

    const QStringList getTitles(){
        return this->titles;
    }

    Element get(size_t index){
        return this->elements[index];
    }

private:
    QStringList titles;
    QList<Element> elements;
};

#endif // ELEMENTGROUP_H
