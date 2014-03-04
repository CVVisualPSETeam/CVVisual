#ifndef CVVISUAL_UTILITY_HPP
#define CVVISUAL_UTILITY_HPP

#include <stdexcept>
#include <type_traits>
#include <utility>

namespace cvv
{
namespace util
{

template <typename T> class Optional
{
      public:
	Optional() : isSet_{ false }
	{
	}
	Optional(T value) : isSet_(true)
	{
		new (&storage) T{ std::move(value) };
	}

	Optional(const Optional &other) : isSet_{ other.isSet_ }
	{
		if (isSet())
		{
			new (getPtr()) T{ *other };
		}
	}

	Optional(Optional &&other) : isSet_{ other.isSet_ }
	{
		if (isSet())
		{
			new (getPtr()) T{ std::move(*other) };
		}
	}

	Optional &operator=(const Optional &other)
	{
		destruct();
		if (other.isSet())
		{
			new (getPtr()) T{ *other };
			isSet_ = true;
		}
	}

	Optional &operator=(Optional &&other)
	{
		if (isSet() && other.isSet())
		{
			std::swap(get(), other.get());
		}
		else if (isSet()) // && !other.isSet()
		{
			destruct();
		}
		else if (other.isSet()) // && !isSet()
		{
			new (getPtr()) T{ std::move(*other) };
			isSet_ = true;
		}
	}

	~Optional()
	{
		destruct();
	}

	T &get()
	{
		if (isSet_)
		{
			return *getPtr();
		}
		else
		{
			throw std::logic_error{
				"Optional does not contain a value"
			};
		}
	}

	const T &get() const
	{
		if (isSet_)
		{
			return *getPtr();
		}
		else
		{
			throw std::logic_error{
				"Optional does not contain a value"
			};
		}
	}

	T &operator*()
	{
		return get();
	}

	const T &operator*() const
	{
		return get();
	}

	bool isSet() const
	{
		return isSet_;
	}

	explicit operator bool() const
	{
		return isSet();
	}

      private:
	void destruct()
	{
		if (isSet_)
		{
			// call the destructor explicitly:
			reinterpret_cast<T *>(&storage)->~T();
			isSet_ = false;
		}
	}

	T *getPtr()
	{
		return reinterpret_cast<T *>(&storage);
	}

	const T *getPtr() const
	{
		return reinterpret_cast<const T *>(&storage);
	}

	using storage_t =
	    typename std::aligned_storage<sizeof(T), alignof(T)>::type;

	storage_t storage;
	bool isSet_;
};
}
} // namespaces

#endif
