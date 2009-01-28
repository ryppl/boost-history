/*----------------------------------------------------------------------------+
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+-----------------------------------------------------------------------------+
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
+----------------------------------------------------------------------------*/
/* ----------------------------------------------------------------------------
class itl::set
a general set class that extends stl-sets
for concepts InplaceAddable and InplaceSubtractable
-----------------------------------------------------------------------------*/
#ifndef __itl_set_h_JOFA_070519__
#define __itl_set_h_JOFA_070519__

#include <string>
#include <set>
#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl/type_traits/is_set.hpp>
#include <boost/itl/type_traits/is_interval_container.hpp>
#include <boost/itl/type_traits/is_interval_splitter.hpp>
#include <boost/itl/type_traits/is_interval_separator.hpp>
#include <boost/itl/type_traits/absorbs_neutrons.hpp>
#include <boost/itl/type_traits/emits_neutrons.hpp>
#include <boost/itl/set_algo.hpp>
#include <boost/itl/predicates.hpp>


namespace boost{namespace itl
{
    /// adds inplace addition, subtraction and intersection to std::set
    /** 
    Class template <b>itl::set</b> extends <b>std::set</b> by inplace operators
    <b>+=, -= and *=</b>. These operations implement the fundamental set 
    functions union, difference and intersection.  

    In contrast to the classic stl style design, template parameters for set 
    ordering <b>Compare</b> and allocation <b>Alloc</b> are passed as template 
    template parameters. This allows for more flexible passing of those 
    parameters to derived types via typedef.

    <b>Note</b> that itl::set provides all functionality of std::set via
    private    inheritence. This can not be seen in the documentation because 
    unfortunately doxygen does not evaluate using statements.

    @author Joachim Faulhaber
    */
    template 
    <
        typename DomainT, 
        ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT),
        ITL_ALLOC   Alloc   = std::allocator 
    >
    class set: private std::set<DomainT, ITL_COMPARE_DOMAIN(Compare,DomainT), Alloc<DomainT> >
    {
    public:
        typedef typename itl::set<DomainT, Compare,       Alloc >       type;
        typedef typename std::set<DomainT, ITL_COMPARE_DOMAIN(Compare,DomainT), Alloc<DomainT> > base_type;

    public:
        typedef DomainT     domain_type;
        typedef DomainT     codomain_type;
        typedef DomainT     element_type;
        typedef DomainT     key_type;
        typedef DomainT     value_type;
        typedef DomainT     data_type;
        typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
        typedef ITL_COMPARE_DOMAIN(Compare,DomainT) key_compare;
        typedef ITL_COMPARE_DOMAIN(Compare,DomainT) value_compare;
        typedef Alloc<DomainT>   allocator_type;

    public:        
        typedef typename base_type::pointer                pointer;
        typedef typename base_type::const_pointer          const_pointer;
        typedef typename base_type::reference              reference;
        typedef typename base_type::const_reference        const_reference;
        typedef typename base_type::iterator               iterator;
        typedef typename base_type::const_iterator         const_iterator;
        typedef typename base_type::size_type              size_type;
        typedef typename base_type::difference_type        difference_type;
        typedef typename base_type::reverse_iterator       reverse_iterator;
        typedef typename base_type::const_reverse_iterator const_reverse_iterator;

    public:
        set(){}
        set(const domain_compare& comp): 
            std::set<DomainT, domain_compare, Alloc<DomainT> >(comp){}

        template <class InputIterator>
        set(InputIterator f, InputIterator l): std::set<InputIterator>(f,l) {}

        template <class InputIterator>
        set(InputIterator f, InputIterator l, const key_compare& comp): std::set<InputIterator>(f,l,comp) {}

        set(const set& src): base_type::set(src){}

        set& operator=(const set& src) { base_type::operator=(src); return *this; } 
        void swap(set& src) { base_type::swap(src); }

        using base_type::begin;
        using base_type::end;
        using base_type::rbegin;
        using base_type::rend;

        using base_type::size;
        using base_type::max_size;
        using base_type::empty;

        using base_type::key_comp;
        using base_type::value_comp;

        using base_type::insert;
        using base_type::erase;
        using base_type::clear;
        using base_type::find;
        using base_type::count;

        using base_type::lower_bound;
        using base_type::upper_bound;
        using base_type::equal_range;

    public:
        // --------------------------------------------------------------------
        // itl specific extensions

        //JODO concept set
        /// Checks if the element \c x is in the set
        bool contains(const DomainT& x)const { return !(find(x) == end()); }

        /** Is <tt>*this</tt> contained in <tt>super</tt>? */
        bool contained_in(const set& super)const { return Set::contained_in(*this, super); }

        /** Does <tt>*this</tt> contain <tt>sub</tt>? */
        bool contains(const set& sub)const { return Set::contained_in(sub, *this); }

        /** <tt>*this</tt> and <tt>x2</tt> are disjoint, if their intersection is empty */
        bool disjoint(const set& x2)const { return disjoint(*this, x2); }

		/** Add an element \c value to the set. */
        set& add(const value_type& value) { insert(value); return *this; } 

		/** Subtract an element \c value from the set. */
        set& subtract(const value_type& value);

        /** \c key_value allows for a uniform access to \c key_values which is
            is used for common algorithms on sets and maps. */
        template<typename IteratorT>
        static const key_type& key_value(IteratorT& value_){ return (*value_); }

        /** \c data_value allows for a uniform access to \c data_values which is
            is used for common algorithms on sets and maps. */
        template<typename IteratorT>
        static const data_type& data_value(IteratorT& value_){ return (*value_); }

        /** \c key_less allows for a uniform notation of key comparison which
            is used for common algorithms on sets and maps. */
        template<typename LeftIterT, typename RightIterT>
        static bool key_less(LeftIterT& lhs_, RightIterT& rhs_) 
        { return key_compare()(*lhs_, *rhs_); }

        /** \c make_element allows for a uniform notation of key comparison which
            is used for common algorithms on sets and maps. */
        static value_type make_element(const key_type& key_val, const data_type& data_val)
        { return key_val; }

        /** \c iterative_size() yields the number of elements that is visited
            throu complete iteration. For interval sets \c iterative_size() is
            different from \c size(). */
        size_t iterative_size()const { return size(); }

		size_t cardinality()const { return size(); }

        /** Erase the elements in *this set to which property \c hasProperty applies. 
        Keep all the rest. */
        template<class Predicate>
        set& erase_if(const Predicate&);

        /** Copy the elements in set \c src to which property \c hasProperty applies 
            into \c *this set. */
        template<class Predicate>
        set& assign_if(const set& src, const Predicate&);

        /** Represent this set as a string */
        std::string as_string(const char* sep = " ")const;
    };



    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    set<DomainT,Compare,Alloc>&
        set<DomainT,Compare,Alloc>::subtract(const value_type& val)
    {
        iterator it_ = find(val);
        if(it_ != end())
            erase(it_);

        return *this;
    }


    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    std::string set<DomainT,Compare,Alloc>::as_string(const char* sep)const
    { 
        const_iterator it_ = begin();
        
        if(it_ == end()) return std::string();
        else
        {
            std::string y = to_string<DomainT>::apply(*it_++);
            while(it_ != end()) { y += sep; y += to_string<DomainT>::apply(*it_++); }
            return y;
        }
    }


    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
        template <class Predicate>
    set<DomainT,Compare,Alloc>& set<DomainT,Compare,Alloc>
        ::erase_if(const Predicate& pred)
    {
        iterator it = begin();
        while(it != end())
            if(pred(*it))
                erase(it++); 
            else ++it;
        return *this;

    }

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
        template <class Predicate>
    set<DomainT,Compare,Alloc>& set<DomainT,Compare,Alloc>
        ::assign_if(const set<DomainT,Compare,Alloc>& src, const Predicate& pred)
    {
        clear();
        const_iterator it = src.begin();
        while(it != src.end()) {
            if(pred(*it)) 
                add(*it++);
        }
        return *this;
    }

	//-----------------------------------------------------------------------------
	// non member functions
	//-----------------------------------------------------------------------------
    /** Standard equality, which is lexicographical equality of the sets
        as sequences, that are given by their Compare order. */
    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline bool operator == (const itl::set<DomainT,Compare,Alloc>& lhs,
                             const itl::set<DomainT,Compare,Alloc>& rhs)
    {
        typedef std::set<DomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;
        return operator==((const base_type&)lhs, (const base_type&)rhs);
    }

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline bool operator != (const itl::set<DomainT,Compare,Alloc>& lhs,
                             const itl::set<DomainT,Compare,Alloc>& rhs)
	{ return !(lhs == rhs); }

    /** Element equality. Two sets are equal if they contain the same elements */
    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline bool is_element_equal(const itl::set<DomainT,Compare,Alloc>& lhs,
                                 const itl::set<DomainT,Compare,Alloc>& rhs)
    {
        typedef std::set<DomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;
        return operator==((const base_type&)lhs, (const base_type&)rhs);
    }

    /** Strict weak less ordering which is given by the Compare order */
    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline bool operator < (const itl::set<DomainT,Compare,Alloc>& lhs,
                            const itl::set<DomainT,Compare,Alloc>& rhs)
    {
        typedef std::set<DomainT,ITL_COMPARE_DOMAIN(Compare,DomainT),Alloc<DomainT> > base_type;
        return operator<((const base_type&)lhs, (const base_type&)rhs);
    }

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline bool operator > (const itl::set<DomainT,Compare,Alloc>& lhs,
                            const itl::set<DomainT,Compare,Alloc>& rhs)
	{ return rhs < lhs; }

    /** Partial ordering which is induced by Compare */
    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline bool operator <= (const itl::set<DomainT,Compare,Alloc>& lhs,
                             const itl::set<DomainT,Compare,Alloc>& rhs)
	{ return !(lhs > rhs); }

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline bool operator >= (const itl::set<DomainT,Compare,Alloc>& lhs,
                             const itl::set<DomainT,Compare,Alloc>& rhs)
	{ return !(lhs < rhs); }

	//--------------------------------------------------------------------------

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline itl::set<DomainT,Compare,Alloc>& 
	operator += (      itl::set<DomainT,Compare,Alloc>& object,
	    const typename itl::set<DomainT,Compare,Alloc>::value_type& operand)
    { return object.add(operand); } 

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    itl::set<DomainT,Compare,Alloc> 
	operator +  (const itl::set<DomainT,Compare,Alloc>& object,
	    const typename itl::set<DomainT,Compare,Alloc>::value_type& operand)
    { return itl::set<DomainT,Compare,Alloc>(object) += operand; }

	/// Add a set \c operand to this set \object.
    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline itl::set<DomainT,Compare,Alloc>& 
	operator += (      itl::set<DomainT,Compare,Alloc>& object,
	             const itl::set<DomainT,Compare,Alloc>& operand)
    { Set::add(object, operand); return object; }

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    itl::set<DomainT,Compare,Alloc> 
	operator +  (const itl::set<DomainT,Compare,Alloc>& object,
	             const itl::set<DomainT,Compare,Alloc>& operand)
    { return itl::set<DomainT,Compare,Alloc>(object) += operand; }

	//--------------------------------------------------------------------------

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline itl::set<DomainT,Compare,Alloc>& 
	operator |= (      itl::set<DomainT,Compare,Alloc>& object,
	    const typename itl::set<DomainT,Compare,Alloc>::value_type& operand)
    { return object.add(operand); } 

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    itl::set<DomainT,Compare,Alloc> 
	operator |  (const itl::set<DomainT,Compare,Alloc>& object,
	    const typename itl::set<DomainT,Compare,Alloc>::value_type& operand)
    { return itl::set<DomainT,Compare,Alloc>(object) |= operand; }

	/// Add a set \c operand to this set \object.
    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline itl::set<DomainT,Compare,Alloc>& 
	operator |= (      itl::set<DomainT,Compare,Alloc>& object,
	             const itl::set<DomainT,Compare,Alloc>& operand)
    { Set::add(object, operand); return object; }

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    itl::set<DomainT,Compare,Alloc> 
	operator |  (const itl::set<DomainT,Compare,Alloc>& object,
	             const itl::set<DomainT,Compare,Alloc>& operand)
    { return itl::set<DomainT,Compare,Alloc>(object) |= operand; }

	//--------------------------------------------------------------------------

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline itl::set<DomainT,Compare,Alloc>& 
	operator -= (      itl::set<DomainT,Compare,Alloc>& object,
	    const typename itl::set<DomainT,Compare,Alloc>::value_type& operand)
    { return object.subtract(operand); } 

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    itl::set<DomainT,Compare,Alloc> 
	operator -  (const itl::set<DomainT,Compare,Alloc>& object,
	    const typename itl::set<DomainT,Compare,Alloc>::value_type& operand)
    { return itl::set<DomainT,Compare,Alloc>(object) -= operand; }


    /// Subtract a set \c x2 from this set.
    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline itl::set<DomainT,Compare,Alloc>& 
	operator -= (      itl::set<DomainT,Compare,Alloc>& object,
	             const itl::set<DomainT,Compare,Alloc>& operand)
    { Set::subtract(object, operand); return object; }

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    itl::set<DomainT,Compare,Alloc> 
	operator -  (const itl::set<DomainT,Compare,Alloc>& object,
	             const itl::set<DomainT,Compare,Alloc>& operand)
    { return itl::set<DomainT,Compare,Alloc>(object) -= operand; }
	
	//--------------------------------------------------------------------------
	// itl::set::intersection operators &=, &
	//--------------------------------------------------------------------------
    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline itl::set<DomainT,Compare,Alloc>& 
	operator &= (      itl::set<DomainT,Compare,Alloc>& object,
	    const typename itl::set<DomainT,Compare,Alloc>::value_type& operand)
    {
		typedef itl::set<DomainT,Compare,Alloc> ObjectT;
		typename ObjectT::const_iterator element_ = object.find(operand);
		ObjectT intersection = element_ == object.end() ? ObjectT() 
			                                            : ObjectT(*element_);
		object.swap(intersection);
		return object; 
	} 

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    itl::set<DomainT,Compare,Alloc> 
	operator &  (const itl::set<DomainT,Compare,Alloc>& object,
	    const typename itl::set<DomainT,Compare,Alloc>::value_type& operand)
    { return itl::set<DomainT,Compare,Alloc>(object) &= operand; }



    /** Intersect set \c object with \c operand. 
	    So \c object becomes the intersection of \c object and \c operand */
    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline itl::set<DomainT,Compare,Alloc>& 
	operator &= (      itl::set<DomainT,Compare,Alloc>& object,
	             const itl::set<DomainT,Compare,Alloc>& operand)
    { Set::intersect(object, operand); return object; }

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    itl::set<DomainT,Compare,Alloc> 
	operator &  (const itl::set<DomainT,Compare,Alloc>& object,
	             const itl::set<DomainT,Compare,Alloc>& operand)
    { return itl::set<DomainT,Compare,Alloc>(object) &= operand; }

	//--------------------------------------------------------------------------
	// itl::set::symmetric_difference operators ^=, ^
	//--------------------------------------------------------------------------
    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline itl::set<DomainT,Compare,Alloc>& 
	operator ^= (      itl::set<DomainT,Compare,Alloc>& object,
	    const typename itl::set<DomainT,Compare,Alloc>::value_type& operand)
    {
		typedef itl::set<DomainT,Compare,Alloc> ObjectT;
		if(object.contains(operand))
			return erase(operand);
		else
			return add(operand);
	} 

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    itl::set<DomainT,Compare,Alloc> 
	operator ^  (const itl::set<DomainT,Compare,Alloc>& object,
	    const typename itl::set<DomainT,Compare,Alloc>::value_type& operand)
    { return itl::set<DomainT,Compare,Alloc>(object) &= operand; }



    /** Intersect set \c object with \c operand. 
	    So \c object becomes the intersection of \c object and \c operand */
    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    inline itl::set<DomainT,Compare,Alloc>& 
	operator ^= (      itl::set<DomainT,Compare,Alloc>& object,
	             const itl::set<DomainT,Compare,Alloc>& operand)
    { Set::flip(object, operand); return object; }

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    itl::set<DomainT,Compare,Alloc> 
	operator ^  (const itl::set<DomainT,Compare,Alloc>& object,
	             const itl::set<DomainT,Compare,Alloc>& operand)
    { return itl::set<DomainT,Compare,Alloc>(object) &= operand; }



    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    set<DomainT,Compare,Alloc>& 
        insert(      set<DomainT,Compare,Alloc>& object, 
               const set<DomainT,Compare,Alloc>& operand) 
    { return object += operand; }

    template <typename DomainT, ITL_COMPARE Compare, ITL_ALLOC Alloc>
    set<DomainT,Compare,Alloc>& 
        erase(      set<DomainT,Compare,Alloc>& object, 
              const set<DomainT,Compare,Alloc>& operand) 
    { return object -= operand; }



    //-------------------------------------------------------------------------
    template <class Type>
    struct is_set<itl::set<Type> >
    { 
		typedef is_set<itl::set<Type> > type;
		static const bool value = true; 
	};

    template <class Type>
    struct is_interval_container<itl::set<Type> >
    { 
		typedef is_interval_container<itl::set<Type> > type;
		enum{value = true}; 
	};

    template <class Type>
    struct is_interval_splitter<itl::set<Type> >
    { enum{value = false}; };

    template <class Type>
    struct absorbs_neutrons<itl::set<Type> >
    { enum{value = false}; };

    template <class Type>
    struct emits_neutrons<itl::set<Type> >
    { enum{value = false}; };

    template <class Type>
    struct type_to_string<itl::set<Type> >
    {
        static std::string apply()
        { return "set<"+ type_to_string<Type>::apply() +">"; }
    };


}} // namespace itl boost

#endif // __itl_set_h_JOFA_070519__

