/*-----------------------------------------------------------------------------+
Copyright (c) 2009-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_XT_BITS_GENTOR_HPP_JOFA_091009
#define BOOST_ITL_XT_BITS_GENTOR_HPP_JOFA_091009

#include <boost/itl/type_traits/to_string.hpp>
#include <boost/itl_xt/gentorit.hpp>
#include <boost/itl_xt/numbergentor.hpp>
#include <boost/itl_xt/bits.hpp>

namespace boost{namespace icl
{

template <class NaturalT> 
class bits_gentor: public RandomGentorAT<icl::bits<NaturalT> >
{
public:
    typedef typename icl::interval<NaturalT>::type  range_type;
    typedef icl::bits<NaturalT> bits_type;

    void some(bits_type& value)
    {
        value = bits_type(_natural_gentor(_value_range));
    };

    void set_range(const range_type& range)
    { _value_range = range; }

private:
    NumberGentorT<NaturalT> _natural_gentor;
    range_type              _value_range;
};

}} // namespace icl boost

#endif


