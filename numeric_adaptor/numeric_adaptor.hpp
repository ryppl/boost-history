// Numeric Adaptor Library
//
// Copyright Barend Gehrels 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef NUMERIC_ADAPTOR_NUMERIC_ADAPTOR_HPP_
#define NUMERIC_ADAPTOR_NUMERIC_ADAPTOR_HPP_


template <typename POLICY>
struct numeric_adaptor
{
	inline numeric_adaptor()
	{
		POLICY::init(value);
	}

	// Copy constructor
	inline numeric_adaptor(const numeric_adaptor<POLICY>& v)
	{
		POLICY::init(value);
		POLICY::copy(v.value, value);
	}

	// Constructor with a normal IEEE type
	template <typename T>
	inline numeric_adaptor(const T& v)
	{
		POLICY::init(value);
		POLICY::template set<T>(value, v);
	}

	virtual ~numeric_adaptor()
	{
		POLICY::destruct(value);
	}

	// Assignment from other value
	inline numeric_adaptor<POLICY> operator=(const numeric_adaptor<POLICY>& v)
	{
		POLICY::copy(v.value, this->value);
		return *this;
	}

	// Assignment from normal IEEE type
	template <typename T>
	inline numeric_adaptor<POLICY> operator=(const T& v)
	{
		POLICY::template set<T>(this->value, v);
		return *this;
	}

	// Cast to normal IEEE type
	template <typename T>
	inline operator T() const
	{
		return POLICY::template big_numeric_cast<T>(value);
	}


	// Comparisons
	inline bool operator<(const numeric_adaptor<POLICY>& other) const
	{
		return POLICY::compare(value, other.value) < 0;
	}

	inline bool operator>(const numeric_adaptor<POLICY>& other) const
	{
		return POLICY::compare(value, other.value) > 0;
	}

	inline bool operator==(const numeric_adaptor<POLICY>& other) const
	{
		return POLICY::compare(value, other.value) == 0;
	}

	// Operators
	friend inline numeric_adaptor<POLICY> operator+(
			const numeric_adaptor<POLICY>& a,
			const numeric_adaptor<POLICY>& b)
	{
		typename POLICY::type r;
		POLICY::init(r);
		POLICY::add(r, a.value, b.value);
		return numeric_adaptor<POLICY>(r, true);
	}

	friend inline numeric_adaptor<POLICY> operator*(
			const numeric_adaptor<POLICY>& a,
			const numeric_adaptor<POLICY>& b)
	{
		typename POLICY::type r;
		POLICY::init(r);
		POLICY::multiply(r, a.value, b.value);
		return numeric_adaptor<POLICY>(r, true);
	}

	friend inline numeric_adaptor<POLICY> operator-(
			const numeric_adaptor<POLICY>& a,
			const numeric_adaptor<POLICY>& b)
	{
		typename POLICY::type r;
		POLICY::init(r);
		POLICY::subtract(r, a.value, b.value);
		return numeric_adaptor<POLICY>(r, true);
	}

	friend inline numeric_adaptor<POLICY> operator/(
			const numeric_adaptor<POLICY>& a,
			const numeric_adaptor<POLICY>& b)
	{
		typename POLICY::type r;
		POLICY::init(r);
		POLICY::divide(r, a.value, b.value);
		return numeric_adaptor<POLICY>(r, true);
	}

	// Functions
	static inline numeric_adaptor<POLICY> sqrt(const numeric_adaptor<POLICY>& v)
	{
		typename POLICY::type r;
		POLICY::init(r);
		POLICY::sqrt(r, v.value);
		return numeric_adaptor<POLICY>(r, true);
	}

	private :
		typename POLICY::type value;

		// Constructor with a type. Bool (or any other signature changing parameter)
		// is necessary for cases where type == CT
		inline numeric_adaptor<POLICY>(const typename POLICY::type& v, bool)
		{
			POLICY::init(value);
			POLICY::copy(v, value);
		}

};


#endif
