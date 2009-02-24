/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
Copyright (c) 1999-2006: Cortex Software GmbH, Kantstrasse 57, Berlin
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_set_algo_H_JOFA_990225__
#define __itl_set_algo_H_JOFA_990225__

#include <boost/itl/detail/notate.hpp>
#include <boost/itl/predicates.hpp>
#include <boost/itl/functors.hpp>

/*
<b>SetAlgo </b>
SetAlgo is a collection of algorithms that work with sets but may also
used with other set-implementations like e.g. hash_set.
*/

namespace boost{namespace itl
{

    namespace Set
    {

        template<class ObjectT, class ConstObjectT, class IteratorT>
        bool common_range(IteratorT& lwb, IteratorT& upb, ObjectT& x1, const ConstObjectT& x2)
        {
            // lwb and upb are iterator of x1 marking the lower and upper bound of
            // the common range of x1 and x2.
            typedef typename ConstObjectT::const_iterator ConstObject_iterator;

            lwb = x1.end();
            upb = x1.end();

            if(x1.empty() || x2.empty()) 
                return false;

            IteratorT x1_fst_ = x1.begin();
            IteratorT x1_lst_ = x1.end(); x1_lst_--;

            ConstObject_iterator x2_fst_ = x2.begin();
            ConstObject_iterator x2_lst_ = x2.end(); x2_lst_--;

            typename ObjectT::key_compare key_less;

            if(key_less(ObjectT::key_value(x1_lst_), ConstObjectT::key_value(x2_fst_))) // {x1}   {x2}
                return false;
            if(key_less(ConstObjectT::key_value(x2_lst_), ObjectT::key_value(x1_fst_))) // {x2}   {x1} 
                return false;

            // We do have a common range
            lwb = x1.lower_bound(ConstObjectT::key_value(x2_fst_));
            upb = x1.upper_bound(ConstObjectT::key_value(x2_lst_));

            return true;
        }

        template<class ObjectT>
        ObjectT& add(ObjectT& result, const ObjectT& x2)
        {
            if(&result == &x2)
                return result;

            typedef typename ObjectT::const_iterator Object_const_iterator;
            for(Object_const_iterator x2_ = x2.begin(); x2_ != x2.end(); x2_++)
                result += *x2_;

            return result;
        }

        template<class ObjectT>
        ObjectT& operator += (ObjectT& result, const ObjectT& x2)
        { return Set::add(result, x2); }

        template<class ObjectT, class CoObjectT>
        ObjectT& subtract(ObjectT& result, const CoObjectT& x2)
        {
            typename CoObjectT::const_iterator common_lwb_;
            typename CoObjectT::const_iterator common_upb_;
            if(!common_range(common_lwb_, common_upb_, x2, result))
                return result;

            typename CoObjectT::const_iterator x2_ = common_lwb_;
            typename ObjectT::iterator common_;

            while(x2_ != common_upb_)
                result.subtract(*x2_++);

            return result;
        }

        template<class ObjectT, class CoObjectT>
        ObjectT& erase(ObjectT& result, const CoObjectT& x2)
        {
            typename CoObjectT::const_iterator common_lwb_;
            typename CoObjectT::const_iterator common_upb_;
            if(!common_range(common_lwb_, common_upb_, x2, result))
                return result;

            typename CoObjectT::const_iterator x2_ = common_lwb_;
            typename ObjectT::iterator common_;

            while(x2_ != common_upb_)
                result.erase(*x2_++);

            return result;
        }


        /** Function template <tt>contained_in</tt> implements the subset relation. 
        <tt>contained_in(sub, super)</tt> is true if <tt>sub</tt> is contained in <tt>super</tt> */
        template<class SetType>
        bool contained_in(const SetType& sub, const SetType& super)
        {
            if(&super == &sub)                   return true;
            if(sub.empty())                      return true;
            if(super.empty())                    return false;
            if(*sub.begin()    < *super.begin()) return false;
            if(*super.rbegin() < *sub.rbegin() ) return false;

            typename SetType::const_iterator common_lwb_;
            typename SetType::const_iterator common_upb_;
            if(!common_range(common_lwb_, common_upb_, sub, super))
                return false;

            typename SetType::const_iterator sub_ = common_lwb_, super_;
            while(sub_ != common_upb_)
            {
                super_ = super.find(*sub_++);
                if(super_ == super.end()) 
                    return false;
            }
            return true;
        }

        /** Function template <tt>lexicographical_equal</tt> implements 
        lexicographical equality. */
        template<class SetType>
        bool lexicographical_equal(const SetType& left, const SetType& right)
        {
            if(&left == &right)        return true;
            if(left.iterative_size() != right.iterative_size()) 
                return false;

            // so we have two sorted containers with equal element counts
            typename SetType::const_iterator left_  = left.begin();
            typename SetType::const_iterator right_ = right.begin();

            while(left_ != left.end())
            {
                if(!(*left_==*right_))
                    return false;
            
                ++left_; ++right_;
            }

            return true;
        }


        /** */
        template<class SetType>
        void intersect(SetType& result, const SetType& x1, const SetType& x2)
        {
            typename SetType::const_iterator common_lwb_;
            typename SetType::const_iterator common_upb_;

            result.clear();
            if(!common_range(common_lwb_, common_upb_, x1, x2))
                return;

            typename SetType::const_iterator x1_ = common_lwb_, x2_;

            while(x1_ != common_upb_)
            {
                x2_ = x2.find(*x1_++);
                if(x2_ != x2.end())
                    result.insert(*x2_);
            }
        }

        template<class SetType>
        SetType& intersect(SetType& result, const SetType& x2)
        {
            SetType aux;
            intersect(aux, result, x2);
            aux.swap(result);
            return result;
        }


		template<class SetType>
        void flip(SetType& result, const SetType& x2)
        {
            typename SetType::const_iterator x2_ = x2.begin(), x1_;
            while(x2_ != x2.end()) 
			{
				std::pair<typename SetType::iterator,bool> insertion = result.insert(*x2_++);
				if(!insertion.WAS_SUCCESSFUL)
					result.erase(insertion.ITERATOR);
			}
        }



        template<class SetType>
        bool disjoint(const SetType& x1, const SetType& x2)
        {
            typename SetType::const_iterator common_lwb_;
            typename SetType::const_iterator common_upb_;

            if(!common_range(common_lwb_, common_upb_, x1, x2))
                return true;

            typename SetType::const_iterator x1_ = common_lwb_, x2_;

            while(x1_ != common_upb_)
            {
                x2_ = x2.find(*x1_++);
                if(x2_ != x2.end()) return false; // found a common element
            }
            // found no common element
            return true;    
        }

        template<class SetType>
        void subtract(SetType& result, const SetType& x1, const SetType& x2)
        {
            SetType temp;
            typename SetType::const_iterator x1_ = x1.begin(), x2_;

            if(&x1 != &x2)
                while(x1_ != x1.end())
                {
                    x2_ = x2.find(*x1_);
                    if(x2_ == x2.end())
                        temp.insert(*x1_);
                    ++x1_;
                }
                temp.swap(result);
        }

    } // namespace Set

}} // namespace itl boost

#endif

