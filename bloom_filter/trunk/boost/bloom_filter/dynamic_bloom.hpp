//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Alejandro Cabrera 2011.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bloom_filter for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_BLOOM_FILTER_DYNAMIC_BLOOM_HPP
#define BOOST_BLOOM_FILTER_DYNAMIC_BLOOM_HPP 1
/**
 * \author Alejandro Cabrera
 * \brief A generic Bloom filter providing compile-time unrolling
 *        of hash function application. 
 */
#include <cmath>
#include <cassert>

#include <boost/config.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/dynamic_bitset.hpp>

#include <boost/bloom_filter/detail/apply_hash.hpp>
#include <boost/bloom_filter/hash/default.hpp>

namespace boost {
  namespace bloom_filter {
    template <typename T,
	      class HashFunctions = mpl::vector<boost_hash<T, 3> >,
	      class Block = size_t,
	      class Allocator = std::allocator<Block> >
    class dynamic_bloom_filter {
    public:
      typedef T value_type;
      typedef T key_type;
      typedef HashFunctions hash_function_type;
      typedef Block block_type;
      typedef Allocator allocator_type;

    public:
      
      // constructors
      dynamic_bloom_filter() {}
      
      explicit dynamic_bloom_filter(const size_t bit_capacity) : bits(bit_capacity) {}
      
      template <typename InputIterator>
      dynamic_bloom_filter(const size_t bit_capacity,
			   const InputIterator start, 
			   const InputIterator end) 
	: bits(bit_capacity)
      {
	for (InputIterator i = start; i != end; ++i)
	  this->insert(*i);
      }

      // query functions
      static BOOST_CONSTEXPR size_t num_hash_functions() {
        return mpl::size<HashFunctions>::value;
      };

      double false_positive_rate() const {
        const double n = static_cast<double>(this->bits.count());
        static const double k = static_cast<double>(num_hash_functions());
        static const double m = static_cast<double>(this->bits.size());
        static const double e =
	  2.718281828459045235360287471352662497757247093699959574966;
        return std::pow(1 - std::pow(e, -k * n / m), k);
      };

      size_t count() const {
        return this->bits.count();
      };

      size_t bit_capacity() const {
	return this->bits.size();
      }

      bool empty() const {
	return this->count() == 0;
      }

      // core operations
      void insert(const T& t) {
        static const unsigned N = mpl::size<HashFunctions>::value - 1;
        detail::dynamic_apply_hash<N, T, HashFunctions, Block, Allocator>::
	  insert(t, bits, bits.size());
      }

      template <typename InputIterator>
      void insert(const InputIterator start, const InputIterator end) {
	for (InputIterator i = start; i != end; ++i) {
	  this->insert(*i);
	}
      }

      bool probably_contains(const T& t) const {
        static const unsigned N = mpl::size<HashFunctions>::value - 1;
        return detail::
	  dynamic_apply_hash<N, T, HashFunctions, Block, Allocator>::
	  contains(t, bits, bits.size());
      }

      // auxilliary operations
      void clear() {
        this->bits.reset();
      }

      void swap(dynamic_bloom_filter& other) {
	dynamic_bloom_filter tmp = other;
	other = *this;
	*this = tmp;
      }

      void resize(const size_t new_capacity) {
	bits.clear();
	bits.resize(new_capacity);
      }

      template <typename _T, typename _HashFunctions, 
		typename _Block, typename _Allocator>
      friend bool operator==(const dynamic_bloom_filter<_T, _HashFunctions, _Block, _Allocator>&, 
			     const dynamic_bloom_filter<_T, _HashFunctions, _Block, _Allocator>&);

      template <typename _T, typename _HashFunctions, 
		typename _Block, typename _Allocator>
      friend bool operator!=(const dynamic_bloom_filter<_T, 
							_HashFunctions, 
							_Block, 
							_Allocator>&, 
			     const dynamic_bloom_filter<_T, 
							_HashFunctions, 
							_Block, 
							_Allocator>&);

      dynamic_bloom_filter& operator|=(const dynamic_bloom_filter& rhs) {
	assert(this->bit_capacity() == rhs.bit_capacity());
        this->bits |= rhs.bits;
        return *this;
      }

      dynamic_bloom_filter& operator&=(const dynamic_bloom_filter& rhs) {
	assert(this->bit_capacity() == rhs.bit_capacity());
        this->bits &= rhs.bits;
        return *this;
      }

    private:
      dynamic_bitset<block_type, allocator_type> bits;
    };

    template<class T, class HashFunctions,
	     class Block, class Allocator>
    dynamic_bloom_filter<T, HashFunctions, Block, Allocator>
    operator|(const dynamic_bloom_filter<T, 
					 HashFunctions, 
					 Block, Allocator>& lhs,
	      const dynamic_bloom_filter<T, 
					 HashFunctions, 
					 Block, Allocator>& rhs)
    {
      assert(lhs.bit_capacity() == rhs.bit_capacity());
      dynamic_bloom_filter<T, HashFunctions, Block, Allocator> ret(lhs);
      ret |= rhs;
      return ret;
    }

    template<class T, class HashFunctions,
	     class Block, class Allocator>
    dynamic_bloom_filter<T, HashFunctions, Block, Allocator>
    operator&(const dynamic_bloom_filter<T, 
					 HashFunctions, 
					 Block, Allocator>& lhs,
	      const dynamic_bloom_filter<T, 
					 HashFunctions, 
					 Block, Allocator>& rhs)
    {
      assert(lhs.bit_capacity() == rhs.bit_capacity());
      dynamic_bloom_filter<T, HashFunctions, Block, Allocator> ret(lhs);
      ret &= rhs;
      return ret;
    }


    template<class T, class HashFunctions,
	     class Block, class Allocator>
    bool
    operator==(const dynamic_bloom_filter<T, 
					  HashFunctions, 
					  Block, Allocator>& lhs,
	       const dynamic_bloom_filter<T, 
					  HashFunctions, 
					  Block, Allocator>& rhs)
    {
      return lhs.bits == rhs.bits;
    }

    template<class T, class HashFunctions,
	     class Block, class Allocator>
    bool
    operator!=(const dynamic_bloom_filter<T, 
					  HashFunctions, 
					  Block, Allocator>& lhs,
	       const dynamic_bloom_filter<T, 
					  HashFunctions, 
					  Block, Allocator>& rhs)
    {
      return !(lhs == rhs);
    }

    template<class T, class HashFunctions,
	     class Block, class Allocator>
    void
    swap(dynamic_bloom_filter<T, 
			      HashFunctions, 
			      Block, Allocator>& lhs,
	 dynamic_bloom_filter<T, 
			      HashFunctions, 
			      Block, Allocator>& rhs)
    {
      lhs.swap(rhs);
    }
  } // namespace bloom_filter
} // namespace boost
#endif
