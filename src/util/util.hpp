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

/**
 * @brief Creates a new Object of type T on the heap, managed by a std::unique_ptr.
 * 
 * @Note This function uses the naming-conventions of the STL instead of cvv because it actually
 * is a backported function from C++14 that is intended to be in harmony with std::make_shared.
 */
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


/**
 * @brief Check whether a value compares equal to any value inside a container of comparable types.
 */
template<typename ValueType, typename Container>
bool isAnyOf(const ValueType& value, const Container& set)
{
	return std::find(set.begin(), set.end(), value) != set.end();
}

/**
 * @brief Overload for intializer-lists to enable usage like this: isAnyOf(3, {1,2,3})
 */
template<typename ValueType, typename Comparable>
bool isAnyOf(const ValueType& value, const std::initializer_list<Comparable>& set)
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
	
	//Constructing only works from references
	Reference(T& pointee) : ptr{&pointee} {}
	
	// there is no point in having a Reference to a temporary object:
	Reference(T&&) = delete;
	
	/**
	 * Get a reference to the referenced object.
	 */
	T& operator*() const {return *ptr;}
	
	T* operator->() const {return ptr;}
	
	/**
	 * Get a reference to the referenced object.
	 */
	T& get() const {return *ptr;}
	
	/**
	 * Get a pointer to the referenced object.
	 */
	T* getPtr() const {return ptr;}
	
	/**
	 * @brief Compare to references for identity of the referenced object.
	 * 
	 * @note identity != equality: two references to two different ints that both happen to have
	 * the value 1, will still compare unequal.
	 */
	bool friend operator==(const Reference& l, const Reference& r)
	{
		return l.ptr == r.ptr;
	}
	
	/**
	 * Dito.
	 */
	bool friend operator!=(const Reference& l, const Reference& r)
	{
		return l.ptr != r.ptr;
	}
	
private:
	T* ptr;
};

/**
 * Create a cvv::util::Reference to an object. This is intended to be used for
 * template-argument-deduction, so explicitly passing the template-argument should be considered
 * undefined behaviour.
 */
template<typename T>
Reference<T> makeRef(T& val)
{
	return Reference<T>{val};
}

}} // namespaces 

#endif
