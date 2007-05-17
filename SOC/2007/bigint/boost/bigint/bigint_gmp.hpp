/* Boost bigint_gmp.hpp header file
 *
 * Copyright 2007 Arseny Kapoulkine
 *
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or
 * copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_BIGINT_BIGINT_GMP_HPP
#define BOOST_BIGINT_BIGINT_GMP_HPP

#include <boost/scoped_array.hpp>

#include <boost/bigint/bigint_util.hpp>

#include "gmp.h"

namespace boost { namespace detail {
	// GMP-based implementation
	struct bigint_gmp_implementation
	{
		mpz_t data;

		bigint_gmp_implementation()
		{
			mpz_init(data);
		}

		~bigint_gmp_implementation()
		{
			mpz_clear(data);
		}

		bigint_gmp_implementation(const bigint_gmp_implementation& other)
		{
			mpz_init_set(data, other.data);
		}

		bigint_gmp_implementation& operator=(const bigint_gmp_implementation& other)
		{
			if (this != &other)
			{
				mpz_clear(data);
				mpz_init_set(data, other.data);
			}
			return *this;
		}

		void assign(int number)
		{
			mpz_set_si(data, number);
		}

		void assign(unsigned int number)
		{
			mpz_set_ui(data, number);
		}

		void assign(int64_t number)
		{
			mpz_set_si(data, number);
		}

		void assign(uint64_t number)
		{
			mpz_set_ui(data, number);
		}
		
		template <typename Ch> void assign_str(const Ch* str, int base)
		{
			assert(base >= 2 && base <= 36);
			
			// skip whitespace
			while (detail::bigint::isspace(*str)) ++str;

			int sign = 1;

			if (*str == Ch('-'))
			{
				sign = -1;
				++str;
			}

			static const unsigned char digit_value_tab[] =
			{
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
				0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
				0,    1,    2,    3,    4,    5,    6,    7,    8,    9,    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
				0xff, 10,   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
				25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   0xff, 0xff, 0xff, 0xff, 0xff,
				0xff, 10,   11,   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
				25,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   0xff, 0xff, 0xff, 0xff, 0xff,
			};
			
			// skip zeros
			while (*str == Ch('0')) ++str;
			
			// is there anything left?
			if (!str)
			{
				assign(0);
				return;
			}
			
			size_t d_size = detail::bigint::length(str);
			scoped_array<unsigned char> d(new unsigned char[d_size]);
			
			for (size_t i = 0; i < d_size; ++i)
			{
				if (*str < 0 || *str > 127 || digit_value_tab[*str] >= base)
				{
					d_size = i;
					break;
				}
				
				d[i] = digit_value_tab[*str++];
			}
			
			if (d_size == 0)
			{
				assign(0);
				return;
			}
			
			size_t d_bits = detail::bigint::get_bit_count(d_size, base);
			
			mpz_init2(data, d_bits);
			data->_mp_size = sign * mpn_set_str(data->_mp_d, d.get(), d_size, base);
		}

		void assign(const char* str, int base)
		{
			assign_str(str, base);
		}
		
		void assign(const wchar_t* str, int base)
		{
			assign_str(str, base);
		}

		void add(const bigint_gmp_implementation& lhs, const bigint_gmp_implementation& rhs)
		{
			mpz_add(data, lhs.data, rhs.data);
		}

		void sub(const bigint_gmp_implementation& lhs, const bigint_gmp_implementation& rhs)
		{
			mpz_sub(data, lhs.data, rhs.data);
		}

		void mul(const bigint_gmp_implementation& lhs, const bigint_gmp_implementation& rhs)
		{
			mpz_mul(data, lhs.data, rhs.data);
		}

		void div(const bigint_gmp_implementation& lhs, const bigint_gmp_implementation& rhs)
		{
			mpz_div(data, lhs.data, rhs.data);
		}

		void mod(const bigint_gmp_implementation& lhs, const bigint_gmp_implementation& rhs)
		{
			mpz_mod(data, lhs.data, rhs.data);
		}
		
		void or_(const bigint_gmp_implementation& lhs, const bigint_gmp_implementation& rhs)
		{
			mpz_ior(data, lhs.data, rhs.data);
		}

		void and_(const bigint_gmp_implementation& lhs, const bigint_gmp_implementation& rhs)
		{
			mpz_and(data, lhs.data, rhs.data);
		}

		void xor_(const bigint_gmp_implementation& lhs, const bigint_gmp_implementation& rhs)
		{
			mpz_xor(data, lhs.data, rhs.data);
		}

		void not_(const bigint_gmp_implementation& lhs)
		{
			mpz_com(data, lhs.data);
		}

		void negate(const bigint_gmp_implementation& lhs)
		{
			mpz_neg(data, lhs.data);
		}

		void lshift(const bigint_gmp_implementation& lhs, boost::uint64_t rhs)
		{
			mpz_mul_2exp(data, lhs.data, rhs);
		}

		void rshift(const bigint_gmp_implementation& lhs, boost::uint64_t rhs)
		{
			mpz_div_2exp(data, lhs.data, rhs);
		}

		void inc()
		{
			mpz_add_ui(data, data, 1);
		}

		void dec()
		{
			mpz_sub_ui(data, data, 1);
		}
		
		int compare(const bigint_gmp_implementation& rhs) const
		{
			return mpz_cmp(data, rhs.data);
		}

		std::string str(int base) const
		{
			assert(base >= 2 && base <= 36);
			
			size_t s_size = mpz_sizeinbase(data, base) + (mpz_sgn(data) < 0);
			scoped_array<char> s(new char[s_size + 1]);
			mpz_get_str(s.get(), base, data);
			
			std::string result(s.get()); // NRVO
			return result;
		}
		
		std::wstring wstr(int base) const
		{
			assert(base >= 2 && base <= 36);
			
			size_t s_size = mpz_sizeinbase(data, base) + (mpz_sgn(data) < 0);
			scoped_array<char> s(new char[s_size + 1]);
			mpz_get_str(s.get(), base, data);
			
			std::wstring result(s.get(), s.get() + s_size);
			return result;
		}

		template <typename T> bool can_convert_to() const
		{
			return mpz_fits_sint_p(data) != 0;
		}
		
		template <typename T> T to_number() const
		{
			return mpz_get_si(data);
		}

		bool is_zero() const
		{
			return mpz_sgn(data) == 0;
		}
		
		void abs(const bigint_gmp_implementation& rhs)
		{
			mpz_abs(data, rhs.data);
		}
		
		void pow(const bigint_gmp_implementation& lhs, boost::uint64_t rhs)
		{
			mpz_pow_ui(data, lhs.data, rhs);
		}
		
		void ldiv(const bigint_gmp_implementation& lhs, const bigint_gmp_implementation& rhs, bigint_gmp_implementation& remainder)
		{
			mpz_fdiv_qr(data, remainder.data, lhs.data, rhs.data);
		}
		
		void sqrt(const bigint_gmp_implementation& lhs)
		{
			mpz_sqrt(data, lhs.data);
		}
	};
} }  // namespace boost::detail

#endif // BOOST_BIGINT_BIGINT_GMP_HPP
