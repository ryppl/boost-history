#pragma once

#include "gmp.h"

namespace boost {
namespace detail {
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

		void assign(const char* str)
		{
			mpz_set_str(data, str, 10);
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

		std::string str() const
		{
			// Yeah, string does not have to be contiguous. This is proof of concept after all
			std::string result;
			result.resize(mpz_sizeinbase(data, 10) + 1);
			const char* str = mpz_get_str(const_cast<char*>(result.data()), 10, data);
			result.resize(strlen(str));

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
}}