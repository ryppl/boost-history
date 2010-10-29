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
#include <boost/icl/type_traits/is_continuous.hpp>
#include <boost/icl/type_traits/has_inverse.hpp>
#include <boost/icl/functors.hpp>
#include <boost/validate/validater/strict_weak_order_validater.hpp>
#include <boost/validate/validater/partial_order_validater.hpp>

namespace boost{namespace icl
{

    template <typename Type>
    class icl_order_validater : public concept_validater
    {
    public:
        typedef strict_weak_order_validater<Type, std::less> LessValidaterT;
        typedef partial_order_validater<Type, std::less_equal> LessEqualValidaterT;
        typedef partial_order_validater<Type, icl::sub_super_set, icl::element_equal> ContainedInValidaterT;

        enum Laws 
        { 
            strictWeakStdOrder,
            partialStdOrder,
            containedInOrder,
            Laws_size 
        };

        icl_order_validater() {setProfile();}

        void setProfile()
        {
            const int sum_of_weights = 100;
            _lawChoice.setSize(sum_of_weights);
            _lawChoice.setMaxWeights(sum_of_weights);
            _lawChoice[strictWeakStdOrder] = 33;
            _lawChoice[partialStdOrder]    = 33;
            _lawChoice[containedInOrder]   = 34;
            _lawChoice.init();
        }


        LawValidaterI* chooseValidater()
        {
            switch(_lawChoice.some())
            {
            case strictWeakStdOrder: return _lessValidater.chooseValidater();
            case partialStdOrder:    return _lessEqualValidater.chooseValidater();
            case containedInOrder:   return _containedInValidater.chooseValidater();
            default: return NULL;
            }
        }

    private:
        ChoiceT               _lawChoice;
        LessValidaterT        _lessValidater;
        LessEqualValidaterT   _lessEqualValidater;
        ContainedInValidaterT _containedInValidater;
    };

}} // namespace icl boost

