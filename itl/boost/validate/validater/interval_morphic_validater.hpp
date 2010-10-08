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
#include <boost/itl/detail/interval_morphism.hpp>
#include <boost/validate/laws/set_laws.hpp>
#include <boost/validate/laws/order.hpp>
#include <boost/validate/laws/pushouts.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/concept_validater.hpp>

namespace boost{namespace itl
{

    template <typename Type>
    class interval_morphic_validater : public concept_validater
    {
    public:
        enum Laws 
        { 
            atomize_plus,
            atomize_minus,
            atomize_et,
            atomize_caret,
            cluster_plus,
            cluster_minus,
            cluster_et,
            cluster_caret,
            atomize_insert,
            atomize_erase,
            cluster_insert,
            cluster_erase,
            join_plus,
            absorb_plus,
            Laws_size 
        };

        interval_morphic_validater() {setProfile();}

        void setProfile()
        {
            _lawChoice.setSize(Laws_size);
            _lawChoice.setMaxWeights(100);
            _lawChoice[atomize_plus]   = 8;
            _lawChoice[atomize_minus]  = 8;
            _lawChoice[atomize_et]     = 8;
            _lawChoice[atomize_caret]  = 8;
            _lawChoice[cluster_plus]   = 8;
            _lawChoice[cluster_minus]  = 8;
            _lawChoice[cluster_et]     = 8;
            _lawChoice[cluster_caret]  = 8;
            _lawChoice[atomize_insert] = 8;
            _lawChoice[atomize_erase]  = 7;
            _lawChoice[cluster_insert] = 7;
            _lawChoice[cluster_erase]  = 7;
            _lawChoice[join_plus]      = 7;
            _lawChoice[absorb_plus]    = 0;
            _lawChoice.init();
        }


        LawValidaterI* chooseValidater()
        {
            switch(_lawChoice.some())
            {
            case atomize_plus:   return new LawValidater<BinaryPushout<Type, typename Type::atomized_type, segmental::atomizer, inplace_plus>,  RandomGentor>();
            case atomize_minus:  return new LawValidater<BinaryPushout<Type, typename Type::atomized_type, segmental::atomizer, inplace_minus>, RandomGentor>();
            case atomize_et:     return new LawValidater<BinaryPushout<Type, typename Type::atomized_type, segmental::atomizer, inplace_et>,    RandomGentor>();
            case atomize_caret:  return new LawValidater<BinaryPushout<Type, typename Type::atomized_type, segmental::atomizer, inplace_caret>, RandomGentor>();
            case cluster_plus:   return new LawValidater<BinaryPushout<typename Type::atomized_type, Type, segmental::clusterer, inplace_plus>,  RandomGentor>();
            case cluster_minus:  return new LawValidater<BinaryPushout<typename Type::atomized_type, Type, segmental::clusterer, inplace_minus>, RandomGentor>();
            case cluster_et:     return new LawValidater<BinaryPushout<typename Type::atomized_type, Type, segmental::clusterer, inplace_et>,    RandomGentor>();
            case cluster_caret:  return new LawValidater<BinaryPushout<typename Type::atomized_type, Type, segmental::clusterer, inplace_caret>, RandomGentor>();
            case atomize_insert: return new LawValidater<BinaryPushout<Type, typename Type::atomized_type, segmental::atomizer, inplace_insert>,RandomGentor>();
            case atomize_erase:  return new LawValidater<BinaryPushout<Type, typename Type::atomized_type, segmental::atomizer, inplace_erase>, RandomGentor>();
            case cluster_insert: return new LawValidater<BinaryPushout<typename Type::atomized_type, Type, segmental::clusterer, inplace_insert>,RandomGentor>();
            case cluster_erase:  return new LawValidater<BinaryPushout<typename Type::atomized_type, Type, segmental::clusterer, inplace_erase>, RandomGentor>();
            case join_plus:      return new LawValidater<BinaryPushout<Type, typename Type::joint_type,    segmental::joiner,    inplace_plus>,  RandomGentor>();
            //JODO absorb_plus holds for interval_map. For split_interval_map element_equal has to be used as equality-relation.
            //case absorb_plus:    return new LawValidater<BinaryPushout<Type, typename Type::partial_absorber_type, segmental::identity_absorber, inplace_plus>,  RandomGentor>();
            //JODO doc: violated: inverse required: case absorb_minus:    return new LawValidater<BinaryPushout<Type, typename Type::partial_absorber_type, Interval::AbsorbNeutrons, inplace_minus>,  RandomGentor>();
            default: return NULL;
            }
        }

    private:
        ChoiceT        _lawChoice;
    };

}} // namespace itl boost

