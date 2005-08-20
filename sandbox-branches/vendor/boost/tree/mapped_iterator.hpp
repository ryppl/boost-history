//  Boost-sandbox general library ntree.hpp header file  ----------------------

//  (C) Copyright Marc Cromme 2002, marc@cromme.dk. Permission to
//  copy, use, modify, sell and distribute this software is granted
//  provided this copyright notice appears in all copies. This
//  software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_MAPPEDITERATOR_HPP
#define BOOST_MAPPEDITERATOR_HPP

#include <iterator>


namespace boost
{
   

   template <class PairAssocC>
   class mapped_iterator 
      : public std::iterator<typename PairAssocC::iterator::iterator_category,
			     typename PairAssocC::mapped_type,
			     typename PairAssocC::difference_type,
			     typename PairAssocC::mapped_type*,
			     typename PairAssocC::mapped_type&>
   {
      public:
	 /// The default constructor results in undefined state.
	 mapped_iterator() : iter_() 
	 { 
	 }

	 /// This iterator is constructed from any pair associative iterator.
	 explicit
	 mapped_iterator(const typename PairAssocC::iterator & iter) :
	    iter_(iter)
	 {
	 }

	 /// The copy constructor is normal.
	 mapped_iterator(const mapped_iterator& iter) 
	    : iter_(iter.iter_) 
	 { 
	 }

	 /// Assignment is normal.
	 mapped_iterator& operator=(const mapped_iterator& rhs)
	 { 
	    if (*this == rhs)
	       return *this;
	 
	    iter_ = rhs.iter_;
	    return *this;
	 }

	 /// Dereferencing the mapped part of the underlying representation.
	 typename PairAssocC::mapped_type& operator*() const 
	 {
	    return iter_->second;
	 }

	 /// Dereferencing the key part of the underlying representation.
	 const typename PairAssocC::key_type& key() const 
	 {
	    return iter_->first;
	 }

	 /// Dereferencing the mapped part of the underlying representation.
	 typename PairAssocC::mapped_type* operator->() const 
	 { 
	    return &(operator*()); 
	 }

	 /// usual increment operation
	 mapped_iterator& 
	 operator++() 
	 {
	    ++iter_;
	    return *this;
	 }

	 /// usual increment operation
	 mapped_iterator 
	 operator++(int) 
	 {
	    mapped_iterator tmp = *this;
	    ++iter_;
	    return tmp;
	 }

	 /// usual decrement operation
	 mapped_iterator& 
	 operator--() 
	 {
	    --iter_;
	    return *this;
	 }

	 /// usual decrement operation
	 mapped_iterator operator--(int) 
	 {
	    mapped_iterator tmp = *this;
	    --iter_;
	    return tmp;
	 }

	 /// usual random access operation
	 mapped_iterator 
	 operator+(typename PairAssocC::difference_type n) const 
	 { 
	    return mapped_iterator(iter_ + n); 
	 }

	 /// usual random access operation
	 mapped_iterator& 
	 operator+=(typename PairAssocC::difference_type n) 
	 {
	    iter_ += n;
	    return *this;
	 }

	 /// usual random access operation
	 mapped_iterator 
	 operator-(typename PairAssocC::difference_type n) const 
	 { 
	    return mapped_iterator(iter_ - n); 
	 }

	 /// usual random access operation
	 mapped_iterator& 
	 operator-=(typename PairAssocC::difference_type n) 
	 {
	    iter_ -= n;
	    return *this;
	 }

	 /// usual random access operation
	 typename PairAssocC::reference 
	 operator[](typename PairAssocC::difference_type n) const 
	 { 
	    return *(*this + n); 
	 }  

	 typename PairAssocC::iterator base() const 
	 { 
	    return iter_; 
	 }

      private: 
	 //typename mapped_iterator<PairAssocC> iterator;
	 //typename PairAssocC::iterator base_iterator;
 
	 typename PairAssocC::iterator iter_ ;
   };

   template<typename PairAssocC>
   inline bool 
   operator==(const mapped_iterator<PairAssocC>& x, 
	      const mapped_iterator<PairAssocC>& y) 
   { 
      return x.base() == y.base(); 
   }

   template<typename PairAssocC>
   inline bool 
   operator<(const mapped_iterator<PairAssocC>& x, 
	     const mapped_iterator<PairAssocC>& y) 
   { 
      return y.base() < x.base(); 
   }

   template<typename PairAssocC>
   inline bool 
   operator!=(const mapped_iterator<PairAssocC>& x, 
	      const mapped_iterator<PairAssocC>& y) 
   { 
      return !(x == y); 
   }

   template<typename PairAssocC>
   inline bool 
   operator>(const mapped_iterator<PairAssocC>& x, 
	     const mapped_iterator<PairAssocC>& y) 
   { 
      return y < x; 
   }

   template<typename PairAssocC>
   inline bool 
   operator<=(const mapped_iterator<PairAssocC>& x, 
	      const mapped_iterator<PairAssocC>& y) 
   { 
      return !(y < x); 
   }

   template<typename PairAssocC>
   inline bool 
   operator>=(const mapped_iterator<PairAssocC>& x, 
	      const mapped_iterator<PairAssocC>& y) 
   { 
      return !(x < y); 
   }

   template<typename PairAssocC>
   inline typename mapped_iterator<PairAssocC>::difference_type
   operator-(const mapped_iterator<PairAssocC>& x, 
	     const mapped_iterator<PairAssocC>& y) 
   { 
      return y.base() - x.base(); 
   }

   template<typename PairAssocC>
   inline mapped_iterator<PairAssocC> 
   operator+(typename mapped_iterator<PairAssocC>::difference_type n,
	     const mapped_iterator<PairAssocC>& x) 
   { 
      return mapped_iterator<PairAssocC>(x.base() + n); 
   }


   // const iterator

   template <class PairAssocC>
   class const_mapped_iterator 
      : public std::iterator<typename PairAssocC::iterator::iterator_category,
			     typename PairAssocC::mapped_type,
			     typename PairAssocC::difference_type,
			     const typename PairAssocC::mapped_type*,
			     const typename PairAssocC::mapped_type&>
   {
      public:
	 /// The default constructor results in undefined state.
	 const_mapped_iterator() : iter_() 
	 { 
	 }

	 /// This iterator is constructed from any pair associative iterator.
	 explicit
	 const_mapped_iterator(const typename PairAssocC::const_iterator& iter)
	    : iter_(iter)
	 {
	 }

	 /// The copy constructor is normal.
	 const_mapped_iterator(const const_mapped_iterator& iter) 
	    : iter_(iter.iter_) 
	 { 
	 }

// 	 /// Implicit mapping from mapped_iterator to const_mapped_iterator
// 	 const_mapped_iterator(const mapped_iterator& iter) 
// 	    : iter_(iter.base()) 
// 	 { 
// 	 }

	 /// Assignment is normal.
	 const_mapped_iterator& operator=(const const_mapped_iterator& rhs)
	 { 
	    if (*this == rhs)
	       return *this;
	 
	    iter_ = rhs.iter_;
	    return *this;
	 }

	 /// Dereferencing the mapped part of the underlying representation.
	 const typename PairAssocC::mapped_type& operator*() const 
	 {
	    return iter_->second;
	 }

	 /// Dereferencing the key part of the underlying representation.
	 const typename PairAssocC::key_type& key() const 
	 {
	    return iter_->first;
	 }

	 /// Dereferencing the mapped part of the underlying representation.
	 const typename PairAssocC::mapped_type* operator->() const 
	 { 
	    return &(operator*()); 
	 }

	 /// usual increment operation
	 const_mapped_iterator& 
	 operator++() 
	 {
	    ++iter_;
	    return *this;
	 }

	 /// usual increment operation
	 const_mapped_iterator 
	 operator++(int) 
	 {
	    const_mapped_iterator tmp = *this;
	    ++iter_;
	    return tmp;
	 }

	 /// usual decrement operation
	 const_mapped_iterator& 
	 operator--() 
	 {
	    --iter_;
	    return *this;
	 }

	 /// usual decrement operation
	 const_mapped_iterator operator--(int) 
	 {
	    const_mapped_iterator tmp = *this;
	    --iter_;
	    return tmp;
	 }

	 /// usual random access operation
	 const_mapped_iterator 
	 operator+(typename PairAssocC::difference_type n) const 
	 { 
	    return const_mapped_iterator(iter_ + n); 
	 }

	 /// usual random access operation
	 const_mapped_iterator& 
	 operator+=(typename PairAssocC::difference_type n) 
	 {
	    iter_ += n;
	    return *this;
	 }

	 /// usual random access operation
	 const_mapped_iterator 
	 operator-(typename PairAssocC::difference_type n) const 
	 { 
	    return const_mapped_iterator(iter_ - n); 
	 }

	 /// usual random access operation
	 const_mapped_iterator& 
	 operator-=(typename PairAssocC::difference_type n) 
	 {
	    iter_ -= n;
	    return *this;
	 }

	 /// usual random access operation
	 typename PairAssocC::reference 
	 operator[](typename PairAssocC::difference_type n) const 
	 { 
	    return *(*this + n); 
	 }  

	 typename PairAssocC::const_iterator base() const 
	 { 
	    return iter_; 
	 }

      private: 
	 //typename const_mapped_iterator<PairAssocC> iterator;
	 //typename PairAssocC::iterator base_iterator;
 
 	 typename PairAssocC::const_iterator iter_ ;
   };

   template<typename PairAssocC>
   inline bool 
   operator==(const const_mapped_iterator<PairAssocC>& x, 
	      const const_mapped_iterator<PairAssocC>& y) 
   { 
      return x.base() == y.base(); 
   }

   template<typename PairAssocC>
   inline bool 
   operator<(const const_mapped_iterator<PairAssocC>& x, 
	     const const_mapped_iterator<PairAssocC>& y) 
   { 
      return y.base() < x.base(); 
   }

   template<typename PairAssocC>
   inline bool 
   operator!=(const const_mapped_iterator<PairAssocC>& x, 
	      const const_mapped_iterator<PairAssocC>& y) 
   { 
      return !(x == y); 
   }

   template<typename PairAssocC>
   inline bool 
   operator>(const const_mapped_iterator<PairAssocC>& x, 
	     const const_mapped_iterator<PairAssocC>& y) 
   { 
      return y < x; 
   }

   template<typename PairAssocC>
   inline bool 
   operator<=(const const_mapped_iterator<PairAssocC>& x, 
	      const const_mapped_iterator<PairAssocC>& y) 
   { 
      return !(y < x); 
   }

   template<typename PairAssocC>
   inline bool 
   operator>=(const const_mapped_iterator<PairAssocC>& x, 
	      const const_mapped_iterator<PairAssocC>& y) 
   { 
      return !(x < y); 
   }

   template<typename PairAssocC>
   inline typename const_mapped_iterator<PairAssocC>::difference_type
   operator-(const const_mapped_iterator<PairAssocC>& x, 
	     const const_mapped_iterator<PairAssocC>& y) 
   { 
      return y.base() - x.base(); 
   }

   template<typename PairAssocC>
   inline const_mapped_iterator<PairAssocC> 
   operator+(typename const_mapped_iterator<PairAssocC>::difference_type n,
	     const const_mapped_iterator<PairAssocC>& x) 
   { 
      return const_mapped_iterator<PairAssocC>(x.base() + n); 
   }



 
} // namespace boost

#endif  // BOOST_MAPPEDITERATOR_HPP

