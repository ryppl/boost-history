/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_is_combinable_JOFA_090115_H__
#define __itl_is_combinable_JOFA_090115_H__

#include <boost/mpl/bool.hpp> 
#include <boost/mpl/if.hpp> 
#include <boost/mpl/and.hpp> 
#include <boost/mpl/or.hpp> 
#include <boost/mpl/not.hpp> 
#include <boost/type_traits/is_same.hpp>

namespace boost{namespace itl
{

template<class Type>
struct is_overloadable
{
    typedef is_overloadable<Type> type;
    static const bool value = 
        is_same<Type, typename Type::overloadable_type>::value;
};


template<class Type>
struct is_interval_map
{
    typedef is_interval_map<Type> type; 
    static const bool value =
        is_interval_container<Type>::value && is_map<Type>::value; 
};

template<class Type>
struct is_interval_set
{ 
    typedef is_interval_set<Type> type; 
    static const bool value =
        is_interval_container<Type>::value && !is_interval_map<Type>::value; 
};


//------------------------------------------------------------------------------
// is_interval_set_derivative
//------------------------------------------------------------------------------
template<class Type, class AssociateT>
struct is_interval_set_derivative;

template<class Type>
struct is_interval_set_derivative<Type, typename Type::domain_type>
{ 
    static const bool value = is_interval_container<Type>::value; 
};

template<class Type>
struct is_interval_set_derivative<Type, typename Type::interval_type>
{ enum{ value = is_interval_container<Type>::value }; };

template<class Type, class AssociateT>
struct is_interval_set_derivative
{
    enum{ value = false };
};

//------------------------------------------------------------------------------
// is_interval_map_derivative
//------------------------------------------------------------------------------
template<class Type, class AssociateT>
struct is_interval_map_derivative;

template<class Type>
struct is_interval_map_derivative<Type, typename Type::domain_mapping_type>
{
    typedef is_interval_map_derivative<Type, typename Type::domain_mapping_type> type;
    static const bool value = is_interval_container<Type>::value;
};

template<class Type>
struct is_interval_map_derivative<Type, typename Type::interval_mapping_type>
{
    typedef is_interval_map_derivative<Type, typename Type::interval_mapping_type> type;
    static const bool value = is_interval_container<Type>::value;
};

template<class Type>
struct is_interval_map_derivative<Type, typename Type::value_type>
{
    typedef is_interval_map_derivative<Type, typename Type::value_type> type;
    static const bool value = is_interval_container<Type>::value;
};

template<class Type, class AssociateT>
struct is_interval_map_derivative
{
    typedef is_interval_map_derivative<Type, AssociateT> type;
    static const bool value = false;
};

//------------------------------------------------------------------------------
// is_intra_derivative
//------------------------------------------------------------------------------
template<class Type, class AssociateT>
struct is_intra_derivative
{
    typedef is_intra_derivative<Type, AssociateT> type;
    static const bool value = 
            (   is_interval_set<Type>::value 
            &&  is_interval_set_derivative<Type, AssociateT>::value)
        ||  (   is_interval_map<Type>::value
            &&  is_interval_map_derivative<Type, AssociateT>::value);
};

template<class Type, class AssociateT>
struct is_cross_derivative
{
    typedef is_cross_derivative<Type, AssociateT> type;
    static const bool value = 
            (   is_interval_map<Type>::value 
            &&  is_interval_set_derivative<Type, AssociateT>::value);
};


//------------------------------------------------------------------------------
//- segmentational_fineness
//------------------------------------------------------------------------------
template<class Type> struct unknown_fineness
{
    typedef unknown_fineness<Type> type;
    static const int value = 0;
};

template<class Type> struct known_fineness
{
    typedef known_fineness<Type> type;
    static const int value = Type::fineness;
};

template<class Type>struct segmentational_fineness
{
    typedef segmentational_fineness<Type> type;
    static const int value = 
        mpl::if_<is_interval_container<Type>, 
                  known_fineness<Type>,
                unknown_fineness<Type>
            >::type::value;
};



//------------------------------------------------------------------------------
// is_interval_set_companion
//------------------------------------------------------------------------------
template<class GuideT, class CompanionT> struct is_interval_set_companion;

template
<
    class Dom, ITL_COMPARE Cmp, template<class,ITL_COMPARE>class Itv, ITL_ALLOC Alc,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet1,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet2
>
struct is_interval_set_companion<IntervalSet1<Dom,Cmp,Itv,Alc>, 
                                 IntervalSet2<Dom,Cmp,Itv,Alc> >
{
    typedef IntervalSet1<Dom,Cmp,Itv,Alc> GuideT;
    typedef IntervalSet2<Dom,Cmp,Itv,Alc> CompanionT;
    typedef is_interval_set_companion<GuideT,CompanionT> type;

    static const bool value = true;
};

// Every IntervalSet can be a companion of every IntervalMap for 
// operations intersection and erase.
template
<
    class Dom, class Cod, class Trt, ITL_COMPARE Cmp, ITL_COMBINE Cmb, ITL_SECTION Sec, 
    template<class,ITL_COMPARE>class Itv, ITL_ALLOC Alc,
    template<class, class, class, ITL_COMPARE, ITL_COMBINE, ITL_SECTION,
             template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalMap,
    template<class, ITL_COMPARE, template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalSet
>
struct is_interval_set_companion<IntervalMap<Dom,Cod,Trt,Cmp,Cmb,Sec,Itv,Alc>, 
                                 IntervalSet<Dom,        Cmp,        Itv,Alc> >
{ 
    typedef IntervalMap<Dom,Cod,Trt,Cmp,Cmb,Sec,Itv,Alc> GuideT;
    typedef IntervalSet<Dom,        Cmp,        Itv,Alc> CompanionT;
    typedef is_interval_set_companion<GuideT,CompanionT> type;

    static const bool value = true;
};

template<class GuideT, class CompanionT> struct is_interval_set_companion
{ 
    typedef is_interval_set_companion<GuideT,CompanionT> type;
    enum{ value = is_interval_set_derivative<GuideT,CompanionT>::value }; 
};


//------------------------------------------------------------------------------
// is_interval_map_companion
//------------------------------------------------------------------------------
template<class GuideT, class CompanionT> struct is_interval_map_companion;

template
<
    class Dom, class Cod, class Trt, ITL_COMPARE Cmp, ITL_COMBINE Cmb, ITL_SECTION Sec, 
    template<class,ITL_COMPARE>class Itv, ITL_ALLOC Alc,
    template<class, class, class, ITL_COMPARE, ITL_COMBINE, ITL_SECTION,
             template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalMap1,
    template<class, class, class, ITL_COMPARE, ITL_COMBINE, ITL_SECTION,
             template<class,ITL_COMPARE>class, ITL_ALLOC>class IntervalMap2
>
struct is_interval_map_companion<IntervalMap1<Dom,Cod,Trt,Cmp,Cmb,Sec,Itv,Alc>, 
                                 IntervalMap2<Dom,Cod,Trt,Cmp,Cmb,Sec,Itv,Alc> >
{
    typedef IntervalMap1<Dom,Cod,Trt,Cmp,Cmb,Sec,Itv,Alc> GuideT;
    typedef IntervalMap2<Dom,Cod,Trt,Cmp,Cmb,Sec,Itv,Alc> CompanionT;
    typedef is_interval_map_companion<GuideT,CompanionT> type;

    static const bool value = true;
};

template<class GuideT, class CompanionT> struct is_interval_map_companion
{ 
    typedef is_interval_map_companion<GuideT,CompanionT> type;
    enum{ value = is_interval_map_derivative<GuideT,CompanionT>::value }; 
};




//------------------------------------------------------------------------------
//- is_coarser_interval_{set,map}_companion
//------------------------------------------------------------------------------
template<class GuideT, class CompanionT> 
struct is_coarser_interval_set_companion
{
    typedef is_coarser_interval_set_companion<GuideT, CompanionT> type;
    static const bool value =
           is_interval_set_companion<GuideT, CompanionT>::value
        && (  segmentational_fineness<GuideT>::value 
            > segmentational_fineness<CompanionT>::value);
};

template<class GuideT, class CompanionT> 
struct is_coarser_interval_map_companion
{
    typedef is_coarser_interval_map_companion<GuideT, CompanionT> type;
    static const bool value =
           is_interval_map_companion<GuideT, CompanionT>::value
        && (  segmentational_fineness<GuideT>::value 
            > segmentational_fineness<CompanionT>::value);
};



//------------------------------------------------------------------------------
// is_right_interval_{set,map}_combinable
//------------------------------------------------------------------------------
template<class GuideT, class CompanionT>
struct is_right_interval_set_combinable
{ 
    typedef is_right_interval_set_combinable<GuideT,CompanionT> type;
    enum{ value = mpl::and_<  is_interval_set<GuideT>
                            , is_interval_set_companion<GuideT, CompanionT> 
                           >::value
        }; 
};

template<class GuideT, class CompanionT>
struct is_right_interval_map_combinable
{ 
    typedef is_right_interval_map_combinable<GuideT,CompanionT> type;
    enum{ value = mpl::and_<  is_interval_map<GuideT>
                            , is_interval_map_companion<GuideT, CompanionT> 
                           >::value
        }; 
};

template<class GuideT, class CompanionT>
struct is_right_intra_combinable
{ 
    typedef is_right_intra_combinable<GuideT,CompanionT> type;
    static const bool value =
        mpl::or_<is_right_interval_set_combinable<GuideT, CompanionT>,
                 is_right_interval_map_combinable<GuideT, CompanionT> 
                >::value;
};

template<class GuideT, class CompanionT>
struct is_right_cross_combinable
{ 
    typedef is_right_cross_combinable<GuideT,CompanionT> type;
    static const bool value =
        mpl::and_
        <     is_interval_map<GuideT>
            , mpl::or_<  is_interval_map_companion<GuideT, CompanionT>
                       , is_interval_set_companion<GuideT, CompanionT> > 
        >::value;
};

template<class GuideT, class CompanionT>
struct is_right_inter_combinable
{ 
    typedef is_right_inter_combinable<GuideT,CompanionT> type;
    static const bool value =
        mpl::or_
        <     
            mpl::and_<is_interval_map<GuideT>, 
                      is_right_cross_combinable<GuideT, CompanionT> >
          , mpl::and_<is_interval_set<GuideT>, 
                      is_right_intra_combinable<GuideT, CompanionT> >
        >::value;
};

//------------------------------------------------------------------------------
// is_binary_interval_{set,map}_combinable
//------------------------------------------------------------------------------
template<class GuideT, class CompanionT>
struct is_binary_interval_set_combinable
{ 
    typedef is_binary_interval_set_combinable<GuideT,CompanionT> type;
    enum{ value = mpl::and_<  is_interval_set<GuideT>
                            , is_coarser_interval_set_companion<GuideT, CompanionT> 
                           >::value
        }; 
};

template<class GuideT, class CompanionT>
struct is_binary_interval_map_combinable
{ 
    typedef is_binary_interval_map_combinable<GuideT,CompanionT> type;
    enum{ value = mpl::and_<  is_interval_map<GuideT>
                            , is_coarser_interval_map_companion<GuideT, CompanionT> 
                           >::value
        }; 
};

template<class GuideT, class CompanionT>
struct is_binary_intra_combinable
{ 
    typedef is_binary_intra_combinable<GuideT,CompanionT> type;
    static const bool value =
        mpl::or_<is_binary_interval_set_combinable<GuideT, CompanionT>,
                 is_binary_interval_map_combinable<GuideT, CompanionT> 
                >::value;
};

template<class GuideT, class CompanionT>
struct is_binary_cross_combinable
{ 
    typedef is_binary_cross_combinable<GuideT,CompanionT> type;
    static const bool value =
        mpl::and_
        <     is_interval_map<GuideT>
            , mpl::or_<  is_coarser_interval_map_companion<GuideT, CompanionT>
                       ,         is_interval_set_companion<GuideT, CompanionT> > 
        >::value;
};

template<class GuideT, class CompanionT>
struct is_binary_inter_combinable
{ 
    typedef is_binary_inter_combinable<GuideT,CompanionT> type;
    static const bool value =
        mpl::or_
        <     
            mpl::and_<is_interval_map<GuideT>, 
                      is_binary_cross_combinable<GuideT, CompanionT> >
          , mpl::and_<is_interval_set<GuideT>, 
                      is_binary_intra_combinable<GuideT, CompanionT> >
        >::value;
};


}} // namespace itl boost

#endif


