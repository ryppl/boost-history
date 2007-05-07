#pragma once

#include <string>

#include <boost/cstdint.hpp>

#include "bigint_gmp.hpp"

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

	explicit bigint_base(const char* str)
	{
		impl.assign(str);
	}
	 // do we need const wchar_t* support?
	 
	explicit bigint_base(const std::string& str)
	{
		impl.assign(str.c_str());
	}
	 // should we provide additional ctor for std::wstring? or should it be actually templated ctor from basic_string?

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
	
	bool operator!() const
	{
		return *this == 0;
	}
	
	// some safe bool conversion operator here - I'm not writing one now because
	// none is portable :) need to steal it from boost::shared_ptr

	std::string str() const
	{
		return impl.str();
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
		return lhs << rhs.str();
	}

	template <typename T, typename Tr> friend std::basic_istream<T, Tr>& operator>>(std::basic_istream<T, Tr>& lhs, bigint_base& rhs)
	{
		std::string data;
		lhs >> data;
		
		rhs = bigint_base(data);
		
		return lhs;
	}
	
	static bigint_base from_number(boost::int64_t value)
	{
		return bigint_base((int)value);
	}
	
	static bigint_base from_number(boost::uint64_t value)
	{
		return bigint_base((int)value);
	}
};

typedef bigint_base<detail::bigint_gmp_implementation> bigint;

} // namespace boost
