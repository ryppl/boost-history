/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#pragma once

#include <boost/mpl/bool.hpp> 
#include <boost/mpl/if.hpp> 
#include <boost/itl/type_traits/is_continuous.hpp>
#include <boost/itl/type_traits/has_inverse.hpp>
#include <boost/itl/functors.hpp>
#include <boost/validate/laws/order.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/concept_validater.hpp>

namespace boost{namespace itl
{

    template <typename Type, template<class>class Relation, template<class>class Equality = itl::std_equal>
    class partial_order_validater : public concept_validater
    {
    public:
        enum Laws 
        { 
            transitivity,
            antisymmetry,
            reflexivity,
            Laws_size 
        };

        partial_order_validater() {setProfile();}

        void setProfile()
        {
            _lawChoice.setSize(Laws_size);
            _lawChoice.setMaxWeights(100);
            _lawChoice[transitivity]  = 33;
            _lawChoice[antisymmetry]  = 33;
            _lawChoice[reflexivity]   = 34;
            _lawChoice.init();
        }


        LawValidaterI* chooseValidater()
        {
            switch(_lawChoice.some())
            {
            case transitivity: return new LawValidater<Transitivity<Type, Relation> >;
            case antisymmetry: return new LawValidater<Antisymmetry<Type, Relation, Equality> >;
            case reflexivity:  return new LawValidater<Reflexivity <Type, Relation> >;
            default: return NULL;
            }
        }


    private:
        ChoiceT _lawChoice;
    };

}} // namespace itl boost

