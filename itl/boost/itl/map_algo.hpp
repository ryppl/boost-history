/*-----------------------------------------------------------------------------+
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_MAPALGO_H_JOFA_080225__
#define __itl_MAPALGO_H_JOFA_080225__

#include <boost/itl/detail/notate.hpp>
#include <boost/itl/set_algo.hpp>

namespace boost{namespace itl
{
    namespace Map 
    {
        template<class MapType>
        bool contained_in(const MapType& sub, const MapType& super)
        {
            if(&super == &sub)                   return true;
            if(sub.empty())                      return true;
            if(super.empty())                    return false;
            if(super.size()    < sub.size()    ) return false;
            if(*sub.begin()    < *super.begin()) return false;
            if(*super.rbegin() < *sub.rbegin() ) return false;

            typename MapType::const_iterator common_lwb_;
            typename MapType::const_iterator common_upb_;
            if(!Set::common_range(common_lwb_, common_upb_, sub, super))
                return false;

            typename MapType::const_iterator sub_ = sub.begin(), super_;
            while(sub_ != sub.end())
            {
                super_ = super.find((*sub_).KEY_VALUE);
                if(super_ == super.end()) 
                    return false;
                else if(!(sub_->CONT_VALUE == super_->CONT_VALUE))
                    return false;
                sub_++;
            }
            return true;
        }

        template<class MapType>
        void intersect(MapType& result, const MapType& x1, const MapType& x2)
        {
            typename MapType::const_iterator common_lwb_;
            typename MapType::const_iterator common_upb_;

            result.clear();
            if(!Set::common_range(common_lwb_, common_upb_, x1, x2))
                return;

            typename MapType::const_iterator x1_ = common_lwb_, x2_;

            while(x1_ != common_upb_)
            {
                x2_ = x2.find((*x1_).KEY_VALUE);
                if(x2_ != x2.end())
                {
                    result.insert(*x1_);
                    if(is_set<typename MapType::codomain_type>::value)
						result.template add<MapType::codomain_intersect>(*x2_); //MEMO template cast for gcc
                    else
                        result.template add<MapType::codomain_combine>(*x2_);
                }
                x1_++;
            }
        }

        template<class MapType>
        void intersect(MapType& result, const MapType& x2)
        {
            // result = result * x2;
            MapType tmp;
            intersect(tmp, result, x2);
            tmp.swap(result);
        }

        template<class MapType, class SetType>
        void intersect(MapType& result, const MapType& x1, const SetType& x2)
        {
            typename MapType::const_iterator common_lwb_;
            typename MapType::const_iterator common_upb_;

            result.clear();
            if(!Set::common_range(common_lwb_, common_upb_, x1, x2))
                return;

            typename MapType::const_iterator x1_ = common_lwb_;
            typename SetType::const_iterator common_;

            while(x1_ != common_upb_)
            {
                common_ = x2.find((*x1_).KEY_VALUE);
                if(common_ != x2.end())
                    result.insert(*x1_);

                x1_++;
            }
        }

        template<class MapType, class SetType>
        void intersect(MapType& result, const SetType& x2)
        {
            // result = result * x2;
            MapType tmp;
            intersect(tmp, result, x2);
            tmp.swap(result);
        }

        //----------------------------------------------------------------------
		// flip
        //----------------------------------------------------------------------
        template<class MapType>
        void flip(MapType& result, const MapType& x2)
        {
			if(is_total<MapType>::value && absorbs_neutrons<MapType>::value)
			{
				result.clear();
				return;
			}

            typename MapType::const_iterator x2_ = x2.begin(), cur_x2_, x1_;
            while(x2_ != x2.end()) 
			{
				cur_x2_ = x2_;
				std::pair<typename MapType::iterator,bool> insertion 
					= result.insert(*x2_++);
				if(!insertion.WAS_SUCCESSFUL)
				{
					//result.erase(insertion.ITERATOR);
					if(is_set<typename MapType::codomain_type>::value)
					{
						typename MapType::iterator res_ = insertion.ITERATOR;
						typename MapType::codomain_type common_value = res_->CONT_VALUE;
						MapType::inverse_codomain_intersect()(common_value, cur_x2_->CONT_VALUE);
						result.subtract(*res_);
						result.add(MapType::value_type(res_->KEY_VALUE, common_value));
					}
                    else
                        result.subtract(*insertion.ITERATOR);
				}
			}

			if(is_total<MapType>::value && !absorbs_neutrons<MapType>::value)
				FORALL(typename MapType, it_, result)
					it_->CONT_VALUE = neutron<typename MapType::codomain_type>::value();
        }



		template<class MapType>
		typename MapType::const_iterator next_proton(typename MapType::const_iterator& iter_, const MapType& object)
		{
			while(   iter_ != object.end() 
				  && iter_->CONT_VALUE == neutron<typename MapType::codomain_type>::value())
				++iter_;

			return iter_;
		}

		/** Function template <tt>lexicographical_equal</tt> implements 
        lexicographical equality except for neutronic content values. */
        template<class MapType>
		bool lexicographical_protonic_equal(const MapType& left, const MapType& right)
        {
            if(&left == &right)        
				return true;

            typename MapType::const_iterator left_  = left.begin();
            typename MapType::const_iterator right_ = right.begin();

			left_  = next_proton(left_,  left);
			right_ = next_proton(right_, right);

            while(left_ != left.end() && right_ != right.end())
            {
                if(!(left_->KEY_VALUE == right_->KEY_VALUE && left_->CONT_VALUE == right_->CONT_VALUE))
                    return false;

				++left_;
				++right_;
				left_  = next_proton(left_,  left);
				right_ = next_proton(right_, right);
            }

			return left_ == left.end() && right_ == right.end();
        }


    } // namespace Map
}} // namespace boost itl

#endif

