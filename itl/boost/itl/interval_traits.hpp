/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_INTERVAL_TRAITS_HPP_JOFA_100926
#define BOOST_ITL_INTERVAL_TRAITS_HPP_JOFA_100926


namespace boost{ namespace itl
{

//------------------------------------------------------------------------------
//- Adapter class
//------------------------------------------------------------------------------
template<class Type>
struct interval_traits
{
    typedef typename Type::domain_type    domain_type;
    typedef typename Type::domain_compare domain_compare;

    static Type construct(const domain_type& lo, const domain_type& up);

    static domain_type upper(const Type& inter_val);
    static domain_type lower(const Type& inter_val);
};


}} // namespace boost itl

#endif


