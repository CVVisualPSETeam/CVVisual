#ifndef CVVISUAL_UTIL_HPP
#define CVVISUAL_UTIL_HPP

//required for utilities
#include <initializer_list>
#include <memory>
#include <utility>

//included for convinience of others:
#include <cstddef> //size_t
#include <cstdint> // [u]intXX_t
#include <algorithm> // since some people like to forget that one

namespace cvv { namespace util {

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


template<typename T>
bool isAnyOf(const T& value, const std::initializer_list<T>& set)
{
	return std::find(set.begin(), set.end(), value) != set.end();
}

/**
 * Reference-class to signal that a type is neither owned nor NULL.
 * 
 * Note that const Reference<Foo> does not mean that the pointed to Foo is const. If that is what
 * you want use Reference<const Foo>
 */
template<typename T>
class Reference
{
public:
	// there is no reasonable default-value, so:
	Reference() = delete;
	
	// these are all just the defaults but it is nice to see them explicitly
	Reference(const Reference&) = default;
	Reference(Reference&&) = default;
	Reference& operator=(const Reference&) = default;
	Reference& operator=(Reference&&) = default;
	
	//Constructing from pointers is as fine as from References; we can however
	//not allow const References, since the saved value may be modified through
	//this class:
	explicit Reference(T* pointee) : ptr{pointee} {assert(ptr);}
	explicit Reference(T& pointee) : ptr{&pointee} {}
	
	// there is no point in having a Reference to a temporary object:
	Reference(T&&) = delete;
	
	T& operator*() const {return *ptr;}
	
	T* operator->() const {return ptr;}
	
	T& get() const {return *ptr;}
	
	T* getPtr() const {return ptr;}
	
	// comparing for equality is always usefull:
	bool friend operator==(const Reference& l, const Reference& r)
	{
		return l.ptr == r.ptr;
	}
	bool friend operator!=(const Reference& l, const Reference& r)
	{
		return l.ptr != r.ptr;
	}
	
private:
	T* ptr;
};

template<typename T>
Reference<T> makeRef(T& val)
{
	return Reference<T>{val};
}

}} // namespaces 

#endif
