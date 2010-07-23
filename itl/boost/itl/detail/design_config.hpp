/*-----------------------------------------------------------------------------+
Author: Joachim Faulhaber
Copyright (c) 2009-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------+
Template parameters of major itl class templates can be designed as
template template parameters or
template type parameter
by setting defines in this file.
+-----------------------------------------------------------------------------*/
#ifndef  BOOST_ITL_DESIGN_CONFIG_HPP_JOFA_090214
#define  BOOST_ITL_DESIGN_CONFIG_HPP_JOFA_090214

// For an interim period, it will be possible to swith between the old and new 
// implementation for intervals.
#define ITL_NEW_INTERVAL_IMPL //JODO remove for the final release.

// If this macro is defined, rightopen_interval with static interval borders
// will be used as default for all interval containers. 
// ITL_USE_STATIC_INTERVAL_BORDER_DEFAULTS should be defined in the application
// before other includes from the ITL
//#define ITL_USE_STATIC_INTERVAL_BORDER_DEFAULTS
// If ITL_USE_STATIC_INTERVAL_BORDER_DEFAULTS is NOT defined, ITL uses intervals
// with dynamic borders as default.


//------------------------------------------------------------------------------
// Auxiliary macros for denoting template signatures.
// Purpose:
// (1) Shorten the lenthy and redundant template signatures.
// (2) Name anonymous template types according to theirs meaning ...
// (3) Making easier to refactor by redefinitin of the macros
// (4) Being able to check template template parameter variants against
//     template type parameter variants.

#ifdef ITL_NEW_INTERVAL_IMPL
#   define ITL_USE_COMPARE_TEMPLATE_TEMPLATE
#   define ITL_USE_COMBINE_TEMPLATE_TEMPLATE
#   define ITL_USE_SECTION_TEMPLATE_TEMPLATE
#else
#   define ITL_USE_COMPARE_TEMPLATE_TEMPLATE
#   define ITL_USE_COMBINE_TEMPLATE_TEMPLATE
#   define ITL_USE_SECTION_TEMPLATE_TEMPLATE
#   define ITL_USE_INTERVAL_TEMPLATE_TEMPLATE
#endif

//#define ITL_USE_COMPARE_TEMPLATE_TYPE
//#define ITL_USE_COMBINE_TEMPLATE_TYPE
//#define ITL_USE_SECTION_TEMPLATE_TYPE
//#define ITL_USE_INTERVAL_TEMPLATE_TYPE

//------------------------------------------------------------------------------
// template parameter Compare can not be a template type parameter as long as
// Compare<Interval<DomainT,Compare> >() is called in std::lexicographical_compare
// implementing operator< for interval_base_{set,map}. see NOTE DESIGN TTP
#ifdef ITL_USE_COMPARE_TEMPLATE_TEMPLATE
#   define ITL_COMPARE template<class>class
#   define ITL_COMPARE_DOMAIN(itl_compare, domain_type) itl_compare<domain_type> 
#   define ITL_COMPARE_INSTANCE(compare_instance, domain_type) compare_instance
#   define ITL_EXCLUSIVE_LESS(interval_type) exclusive_less_than
#else//ITL_USE_COMPARE_TEMPLATE_TYPE
#   define ITL_COMPARE class
#   define ITL_COMPARE_DOMAIN(itl_compare, domain_type) itl_compare 
#   define ITL_COMPARE_INSTANCE(compare_instance, domain_type) compare_instance<domain_type> 
#   define ITL_EXCLUSIVE_LESS(interval_type) exclusive_less_than<interval_type>
#endif

//------------------------------------------------------------------------------
// template parameter Combine could be a template type parameter.
#ifdef ITL_USE_COMBINE_TEMPLATE_TEMPLATE
#   define ITL_COMBINE template<class>class
#   define ITL_COMBINE_CODOMAIN(itl_combine, codomain_type) itl_combine<codomain_type> 
#   define ITL_COMBINE_INSTANCE(combine_instance,codomain_type) combine_instance
#else//ITL_USE_COMBINE_TEMPLATE_TYPE
#   define ITL_COMBINE class
#   define ITL_COMBINE_CODOMAIN(itl_combine, codomain_type) itl_combine 
#   define ITL_COMBINE_INSTANCE(combine_instance,codomain_type) combine_instance<codomain_type>
#endif

//------------------------------------------------------------------------------
// template parameter Section could be a template type parameter.
#ifdef ITL_USE_SECTION_TEMPLATE_TEMPLATE
#   define ITL_SECTION template<class>class
#   define ITL_SECTION_CODOMAIN(itl_intersect, codomain_type) itl_intersect<codomain_type> 
#   define ITL_SECTION_INSTANCE(section_instance,codomain_type) section_instance
#else//ITL_USE_SECTION_TEMPLATE_TYPE
#   define ITL_SECTION class
#   define ITL_SECTION_CODOMAIN(itl_intersect, codomain_type) itl_intersect 
#   define ITL_SECTION_INSTANCE(section_instance,codomain_type) section_instance<codomain_type>
#endif


//------------------------------------------------------------------------------
// template parameter Interval could be a template type parameter.
#ifdef ITL_USE_INTERVAL_TEMPLATE_TEMPLATE
#   define ITL_INTERVAL(itl_compare) template<class,itl_compare>class
#   define ITL_INTERVAL2(itl_compare) template<class DomT2,itl_compare>class
#   define ITL_INTERVAL_TYPE(itl_interval, domain_type, itl_compare) itl_interval<domain_type,itl_compare> 
#   define ITL_INTERVAL_INSTANCE(interval_instance,domain_type,itl_compare) interval_instance
#else//ITL_USE_INTERVAL_TEMPLATE_TYPE
#   define ITL_INTERVAL(itl_compare) class
#   define ITL_INTERVAL2(itl_compare) class
#   define ITL_INTERVAL_TYPE(itl_interval, domain_type, itl_compare) itl_interval  
#   define ITL_INTERVAL_INSTANCE(interval_instance,domain_type,itl_compare) typename interval_instance<domain_type,itl_compare>::type
#endif


//------------------------------------------------------------------------------
//CL #define ITL_INTERVAL_DEFAULT itl::rightopen_interval
//CL #define ITL_INTERVAL_DEFAULT itl::continuous_interval

#ifdef ITL_NEW_INTERVAL_IMPL
#   define ITL_INTERVAL_DEFAULT itl::interval_type_of
#else
#   define ITL_INTERVAL_DEFAULT boost::itl::interval
#endif

//------------------------------------------------------------------------------
//JODO find proper solution here
#ifdef ITL_NEW_INTERVAL_IMPL
//#   define ITL_discrt_INTERVAL_DEF itl::discrete_interval
//#   define ITL_contin_INTERVAL_DEF itl::continuous_interval
#   define ITL_discrt_INTERVAL_DEF itl::rightopen_interval
#   define ITL_contin_INTERVAL_DEF itl::rightopen_interval
#else
#   define ITL_discrt_INTERVAL_DEF itl::interval
#   define ITL_contin_INTERVAL_DEF itl::interval
#endif

//------------------------------------------------------------------------------
#define ITL_ALLOC    template<class>class

//------------------------------------------------------------------------------

#endif // BOOST_ITL_DESIGN_CONFIG_HPP_JOFA_090214


