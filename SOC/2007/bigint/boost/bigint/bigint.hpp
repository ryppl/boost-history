/* Boost bigint.hpp header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_BIGINT_BIGINT_HPP
#define BOOST_BIGINT_BIGINT_HPP

#include <string>

#include <boost/cstdint.hpp>

#include <boost/bigint/bigint_config.hpp>
#include <boost/bigint/bigint_util.hpp>

namespace boost {
template <typename I> class bigint_base
{
	I impl;

public:
	bigint_base()
	{
	}
	
	bigint_base(int number)
	{
		impl.assign(number);
	}
	
	bigint_base(unsigned int number)
	{
		impl.assign(number);
	}
	
	bigint_base(int64_t number)
	{
		impl.assign(number);
	}
	
	bigint_base(uint64_t number)
	{
		impl.assign(number);
	}

	explicit bigint_base(const char* str, int base = 10)
	{
		impl.assign(str, base);
	}
	
	explicit bigint_base(const wchar_t* str, int base = 10)
	{
		impl.assign(str, base);
	}
	 
	explicit bigint_base(const std::string& str, int base = 10)
	{
		impl.assign(str.c_str(), base);
	}
	
	explicit bigint_base(const std::wstring& str, int base = 10)
	{
		impl.assign(str.c_str(), base);
	}

	// - basic arithmetic operations (addition, subtraction, multiplication, division)
	const bigint_base& operator+=(const bigint_base& other)
	{
		impl.add(impl, other.impl);
		return *this;
	}
	
	const bigint_base& operator-=(const bigint_base& other)
	{
		impl.sub(impl, other.impl);
		return *this;
	}

	const bigint_base& operator*=(const bigint_base& other)
	{
		impl.mul(impl, other.impl);
		return *this;
	}

	const bigint_base& operator/=(const bigint_base& other)
	{
		impl.div(impl, other.impl);
		return *this;
	}

	// - modulo
	const bigint_base& operator%=(const bigint_base& other)
	{
		impl.mod(impl, other.impl);
		return *this;
	}

	// - bit operations (bit logic (or, and, xor), bit shifts (left/right))
	const bigint_base& operator|=(const bigint_base& other)
	{
		impl.or_(impl, other.impl);
		return *this;
	}
	
	const bigint_base& operator&=(const bigint_base& other)
	{
		impl.and_(impl, other.impl);
		return *this;
	}
	
	const bigint_base& operator^=(const bigint_base& other)
	{
		impl.xor_(impl, other.impl);
		return *this;
	}
	
	const bigint_base& operator<<=(uint64_t other)
	{
		impl.lshift(impl, other);
		return *this;
	}
	
	const bigint_base& operator>>=(boost::uint64_t other)
	{
		impl.rshift(impl, other);
		return *this;
	}

	const bigint_base& operator++()
	{
		impl.inc();
		return *this;
	}
	
	bigint_base operator++(int)
	{
		bigint_base old = *this;
		impl.inc();
		return old;
	}

	const bigint_base& operator--()
	{
		impl.dec();
		return *this;
	}
	
	bigint_base operator--(int)
	{
		bigint_base old = *this;
		impl.dec();
		return old;
	}
	
	// unary operators
	bigint_base operator+() const
	{
		return *this;
	}
	
	bigint_base operator-() const
	{
		bigint_base<I> result;
		result.impl.negate(impl);
		return result;
	}
	
	bigint_base operator~() const
	{
		bigint_base<I> result;
		result.impl.not_(impl);
		return result;
	}
	
    // implicit conversion to "bool"

#if defined(__SUNPRO_CC) && BOOST_WORKAROUND(__SUNPRO_CC, <= 0x580)

    operator bool () const
    {
        return impl;
    }

#elif defined( _MANAGED )

private:
    static void unspecified_bool( bigint_base*** )
    {
    }

    typedef void (*unspecified_bool_type)( bigint_base*** );

public:
    operator unspecified_bool_type() const // never throws
    {
        return impl.is_zero() ? 0 : unspecified_bool;
    }

#elif \
    ( defined(__MWERKS__) && BOOST_WORKAROUND(__MWERKS__, < 0x3200) ) || \
    ( defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ < 304) )

private:
    typedef std::string (bigint_base::*unspecified_bool_type)(int) const;

public: 
    operator unspecified_bool_type() const // never throws
    {
        return impl.is_zero() ? 0 : &bigint_base::str;
    }

#else 

private:
    typedef I* bigint_base::*unspecified_bool_type;

public:
    operator unspecified_bool_type() const // never throws
    {
        return impl.is_zero() ? 0 : &bigint_base::impl;
    }

#endif

    // operator! is redundant, but some compilers need it

    bool operator! () const // never throws
    {
        return impl.is_zero();
    }

	std::string str(int base = 10) const
	{
		return impl.str(base);
	}
	
	std::wstring wstr(int base = 10) const
	{
		return impl.wstr(base);
	}

	// conversion to numeric types (including 64 bit)
	template <typename T> bool can_convert_to() const
	{
		return impl.can_convert_to<T>();
	}
	
	template <typename T> T to_number() const
	{
		return impl.to_number<T>();
	}

	// - basic arithmetic operations (addition, subtraction, multiplication, division)
	friend bigint_base operator+(const bigint_base& lhs, const bigint_base& rhs)
	{
		bigint_base<I> result;
		result.impl.add(lhs.impl, rhs.impl);
		return result;
	}

	friend bigint_base operator-(const bigint_base& lhs, const bigint_base& rhs)
	{
		bigint_base<I> result;
		result.impl.sub(lhs.impl, rhs.impl);
		return result;
	}

	friend bigint_base operator*(const bigint_base& lhs, const bigint_base& rhs)
	{
		bigint_base<I> result;
		result.impl.mul(lhs.impl, rhs.impl);
		return result;
	}

	friend bigint_base operator/(const bigint_base& lhs, const bigint_base& rhs)
	{
		bigint_base<I> result;
		result.impl.div(lhs.impl, rhs.impl);
		return result;
	}
	
	// - modulo
	friend bigint_base operator%(const bigint_base& lhs, const bigint_base& rhs)
	{
		bigint_base<I> result;
		result.impl.mod(lhs.impl, rhs.impl);
		return result;
	}

	// - bit operations (bit logic (or, and, xor), bit shifts (left/right))
	friend bigint_base operator|(const bigint_base& lhs, const bigint_base& rhs)
	{
		bigint_base<I> result;
		result.impl.or_(lhs.impl, rhs.impl);
		return result;
	}
	
	friend bigint_base operator&(const bigint_base& lhs, const bigint_base& rhs)
	{
		bigint_base<I> result;
		result.impl.and_(lhs.impl, rhs.impl);
		return result;
	}
	
	friend bigint_base operator^(const bigint_base& lhs, const bigint_base& rhs)
	{
		bigint_base<I> result;
		result.impl.xor_(lhs.impl, rhs.impl);
		return result;
	}
	
	// do we need << and >> for bigints?
	friend bigint_base operator<<(const bigint_base& lhs, boost::uint64_t rhs)
	{
		bigint_base<I> result;
		result.impl.lshift(lhs.impl, rhs);
		return result;
	}

	friend bigint_base operator>>(const bigint_base& lhs, boost::uint64_t rhs)
	{
		bigint_base<I> result;
		result.impl.rshift(lhs.impl, rhs);
		return result;
	}
	
	friend bool operator<(const bigint_base& lhs, const bigint_base& rhs)
	{
		return lhs.impl.compare(rhs.impl) < 0;
	}

	friend bool operator<=(const bigint_base& lhs, const bigint_base& rhs)
	{
		return lhs.impl.compare(rhs.impl) <= 0;
	}

	friend bool operator>(const bigint_base& lhs, const bigint_base& rhs)
	{
		return lhs.impl.compare(rhs.impl) > 0;
	}
	
	friend bool operator>=(const bigint_base& lhs, const bigint_base& rhs)
	{
		return lhs.impl.compare(rhs.impl) >= 0;
	}
	
	friend bool operator==(const bigint_base& lhs, const bigint_base& rhs)
	{
		return lhs.impl.compare(rhs.impl) == 0;
	}
	
	friend bool operator!=(const bigint_base& lhs, const bigint_base& rhs)
	{
		return lhs.impl.compare(rhs.impl) != 0;
	}
	
	friend bigint_base abs(const bigint_base& value)
	{
		bigint_base result;
		result.impl.abs(value);
		return result;
	}

	friend bigint_base pow(const bigint_base<I>& lhs, boost::uint64_t rhs)
	{
		bigint_base result;
		result.impl.pow(lhs.impl, rhs);
		return result;
	}

	// non-standard. Do we need to change it (introduce some equivalent to ldiv_t type) or is it ok?
	friend bigint_base ldiv(const bigint_base& lhs, const bigint_base& rhs, bigint_base& remainder)
	{
		bigint_base result;
		result.impl.ldiv(lhs.impl, rhs, remainder.impl);
		return result;
	}

	friend bigint_base sqrt(const bigint_base& lhs)
	{
		bigint_base result;
		result.impl.sqrt(lhs.impl);
		return result;
	}

	template <typename T, typename Tr> friend std::basic_ostream<T, Tr>& operator<<(std::basic_ostream<T, Tr>& lhs, const bigint_base& rhs)
	{
		int base = (lhs.flags() & std::ios_base::hex) ? 16 : (lhs.flags() & std::ios_base::oct) ? 8 : 10;
		return lhs << detail::bigint::to_string(rhs, base, T());
	}

	template <typename T, typename Tr> friend std::basic_istream<T, Tr>& operator>>(std::basic_istream<T, Tr>& lhs, bigint_base& rhs)
	{
		std::basic_string<T> result;
		lhs >> result;
		
		int base = (lhs.flags() & std::ios_base::hex) ? 16 : (lhs.flags() & std::ios_base::oct) ? 8 : 10;
		rhs = bigint_base(result, base);
		
		return lhs;
	}
};

} // namespace boost

// Do we have GMP?
#ifdef BOOST_BIGINT_HAS_GMP_SUPPORT

#include <boost/bigint/bigint_gmp.hpp>

namespace boost {

typedef bigint_base<detail::bigint_gmp_implementation> bigint;

} // namespace boost

#else

// The default implementation should be here, but there's none yet
#error "No default implementation for now"

#endif

#endif // BOOST_BIGINT_BIGINT_HPP
