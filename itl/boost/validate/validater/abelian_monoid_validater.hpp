/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_VALIDATE_VALIDATER_ABELIAN_MONOID_VALIDATER_HPP_JOFA_100501
#define BOOST_ITL_VALIDATE_VALIDATER_ABELIAN_MONOID_VALIDATER_HPP_JOFA_100501

#include <boost/itl/functors.hpp>
//#include <boost/validate/laws/monoid.hpp>
#include <boost/validate/validater/concept_validater.hpp>
#include <boost/validate/validater/monoid_validater.hpp>

namespace boost{namespace itl
{

typedef WeightedNumberGentor<int> ChoiceT;

template <typename Type>
class abelian_monoid_validater : public concept_validater
{
public:

    enum Laws {monoid_laws, commutativity, Laws_size};

    abelian_monoid_validater(){ setProfile(); }

    void setProfile()
    {
        _lawChoice.setSize(Laws_size);
        _lawChoice.setMaxWeights(100);
        _lawChoice[monoid_laws]   = 66;
        _lawChoice[commutativity] = 34;
        _lawChoice.init();
    }

    LawValidaterI* chooseValidater()
    {
        switch(_lawChoice.some())
        {
        case monoid_laws:   return _monoid_validater.chooseValidater();
        case commutativity: return new LawValidater<InplaceCommutativity<Type> >;
        default: return NULL;
        }
    }

private:
    ChoiceT                _lawChoice;
    monoid_validater<Type> _monoid_validater;
};


}} // namespace itl boost

#endif // BOOST_ITL_VALIDATE_VALIDATER_ABELIAN_MONOID_VALIDATER_HPP_JOFA_100501

