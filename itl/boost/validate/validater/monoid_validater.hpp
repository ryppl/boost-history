/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_VALIDATE_VALIDATER_MONOID_VALIDATER_HPP_JOFA_100501
#define BOOST_ITL_VALIDATE_VALIDATER_MONOID_VALIDATER_HPP_JOFA_100501

#include <boost/itl/functors.hpp>
#include <boost/validate/validater/concept_validater.hpp>
#include <boost/validate/laws/monoid.hpp>

namespace boost{namespace icl
{

template <typename Type>
class monoid_validater : public concept_validater
{
public:

    enum Laws {associativity, neutrality, Laws_size};

    monoid_validater(){ setProfile(); }

    void setProfile()
    {
        _lawChoice.setSize(Laws_size);
        _lawChoice.setMaxWeights(100);
        _lawChoice[associativity] = 50;
        _lawChoice[neutrality]    = 50;
        _lawChoice.init();
    }

    LawValidaterI* chooseValidater()
    {
        switch(_lawChoice.some())
        {
        case associativity: return new LawValidater<InplaceAssociativity<Type> >;
        case neutrality:    return new LawValidater<InplaceNeutrality   <Type> >;
        default: return NULL;
        }
    }

private:
    ChoiceT _lawChoice;
};


}} // namespace icl boost

#endif // BOOST_ITL_VALIDATE_VALIDATER_MONOID_VALIDATER_HPP_JOFA_100501

