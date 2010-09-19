/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_ON_NEUTRIC_HPP_JOFA_100915
#define BOOST_ITL_TYPE_TRAITS_ON_NEUTRIC_HPP_JOFA_100915

namespace boost{ namespace itl
{

template<class Type, class Combiner, bool absorbs_neutrons>
struct on_neutric; //JODO rename

template<class Type, class Combiner>
struct on_neutric<Type, Combiner, false>
{
    typedef on_neutric type;
    typedef typename Type::codomain_type codomain_type;

    static bool is_absorbable(const codomain_type&){ return false; }
};

template<class Type, class Combiner>
struct on_neutric<Type, Combiner, true>
{
    typedef on_neutric type;
    typedef typename Type::codomain_type codomain_type;
    typedef typename Type::codomain_combine codomain_combine;

    static bool is_absorbable(const codomain_type& co_value)
    {
        return co_value == Combiner::neutron();
    }
};

}} // namespace boost itl

#endif


