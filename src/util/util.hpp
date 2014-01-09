#ifndef CVVISUAL_UTIL_HPP
#define CVVISUAL_UTIL_HPP

//required for utilities
#include <memory>
#include <utility>

//included for convinience of others:
#include <cstddef> //size_t
#include <cstdint> // [u]intXX_t
#include <algorithm> // since some people like to forget that one

namespace cvv {
namespace util {

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

/**
 * Reference-class to signal that a type is neither owned nor NULL.
 */
template<typename T>
class reference {
	public:
		static_assert(std::is_same<T, typename std::decay<T>::type>(),
				"T must be a fully decayed type");
		
		// there is no reasonable default-value, so:
		reference() = delete;
		
		// these are all just the defaults but it is nice to see them explicitly
		reference(const reference&) = default;
		reference(reference&&) = default;
		reference& operator=(const reference&) = default;
		reference& operator=(reference&&) = default;
		
		//Constructing from pointers is as fine as from references; we can however
		//not allow const references, since the saved value may be modified through
		//this class:
		reference(T* pointee) : ptr{pointee} {assert(ptr);}
		reference(T& pointee) : ptr{&pointee} {}
		
		// there is no point in having a reference to a temporary object:
		reference(T&&) = delete;
		
		T& operator*() {return *ptr;}
		const T& operator*() const {return *ptr;}
		
		T* operator->() {return ptr;}
		const T* operator->() const {return ptr;}
		
		T& get() {return *ptr;}
		const T& get() const {return *ptr;}
		
		T* get_ptr() {return ptr;}
		const T* get_ptr() const {return ptr;}
		
		// we need this one for using this template in std::set and the like:
		bool friend operator<(const reference& l, const reference& r) {
			return l.ptr < r.ptr;
		}
		
		// comparing for equality is always usefull:
		bool friend operator==(const reference& l, const reference& r) {
			return l.ptr == r.ptr;
		}
		bool friend operator!=(const reference& l, const reference& r) {
			return l.ptr != r.ptr;
		}
		
		// The pointer in this class always points to something:
		explicit constexpr operator bool() const {return true;}
		constexpr bool operator!() const {return false;}
		
	private:
		T* ptr;
};

}} // namespaces 

#endif
