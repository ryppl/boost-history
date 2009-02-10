/*----------------------------------------------------------------------------+
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#pragma once

#include <boost/mpl/bool.hpp> 
#include <boost/mpl/if.hpp> 
#include <boost/itl/type_traits/is_continuous.hpp>
#include <boost/itl/type_traits/has_inverse.hpp>
#include <boost/itl/functors.hpp>
#include <boost/itl/interval_morphism.hpp>
#include <boost/validate/laws/set_laws.hpp>
#include <boost/validate/laws/order.h>
#include <boost/validate/laws/pushouts.h>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/algebra_validater.hpp>

namespace boost{namespace itl
{
    typedef WeightedNumberGentor<int> ChoiceT;

    template <typename Type>
    class interval_morphic_validater : public algebra_validater
    {
    public:
        enum Laws 
        { 
            atomize_plus,
            atomize_minus,
            atomize_star,
            cluster_plus,
            cluster_minus,
            cluster_star,
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
            _lawChoice[atomize_plus]   = 10;
            _lawChoice[atomize_minus]  = 5;
            _lawChoice[atomize_star]   = 5;
            _lawChoice[cluster_plus]   = 10;
            _lawChoice[cluster_minus]  = 5;
            _lawChoice[cluster_star]   = 5;
            _lawChoice[atomize_insert] = 10;
            _lawChoice[atomize_erase]  = 10;
            _lawChoice[cluster_insert] = 10;
            _lawChoice[cluster_erase]  = 20;
            _lawChoice[join_plus]      = 10;
            _lawChoice[absorb_plus]    = 0;
            _lawChoice.init();
        }


        LawValidaterI* chooseValidater()
        {
            switch(_lawChoice.some())
            {
            case atomize_plus:   return new LawValidater<BinaryPushout<Type, typename Type::atomized_type, Interval::Atomize, inplace_plus>,  RandomGentor>();
            case atomize_minus:  return new LawValidater<BinaryPushout<Type, typename Type::atomized_type, Interval::Atomize, inplace_minus>, RandomGentor>();
            case atomize_star:   return new LawValidater<BinaryPushout<Type, typename Type::atomized_type, Interval::Atomize, inplace_et>,    RandomGentor>();
            case cluster_plus:   return new LawValidater<BinaryPushout<typename Type::atomized_type, Type, Interval::Cluster, inplace_plus>,  RandomGentor>();
            case cluster_minus:  return new LawValidater<BinaryPushout<typename Type::atomized_type, Type, Interval::Cluster, inplace_minus>, RandomGentor>();
            case cluster_star:   return new LawValidater<BinaryPushout<typename Type::atomized_type, Type, Interval::Cluster, inplace_et>,    RandomGentor>();
            case atomize_insert: return new LawValidater<BinaryPushout<Type, typename Type::atomized_type, Interval::Atomize, inserter>,      RandomGentor>();
            case atomize_erase:  return new LawValidater<BinaryPushout<Type, typename Type::atomized_type, Interval::Atomize, eraser>,        RandomGentor>();
            case cluster_insert: return new LawValidater<BinaryPushout<typename Type::atomized_type, Type, Interval::Cluster, inserter>,      RandomGentor>();
            case cluster_erase:  return new LawValidater<BinaryPushout<typename Type::atomized_type, Type, Interval::Cluster, eraser>,        RandomGentor>();
            case join_plus:      return new LawValidater<BinaryPushout<Type, typename Type::joint_type,    Interval::Join,    inplace_plus>,  RandomGentor>();
            //JODO absorb_plus holds for interval_map. For split_interval_map element_equal has to be used as equality-relation.
            //case absorb_plus:    return new LawValidater<BinaryPushout<Type, typename Type::partial_absorber_type, Interval::AbsorbNeutrons, inplace_plus>,  RandomGentor>();
            //JODO doc: violated: inverse required: case absorb_minus:    return new LawValidater<BinaryPushout<Type, typename Type::partial_absorber_type, Interval::AbsorbNeutrons, inplace_minus>,  RandomGentor>();
            default: return NULL;
            }
        }

        void validate()
        {
            _validater = chooseValidater();
            if(_validater)
            {
                _validater->run();
                _validater->addFrequencies(_frequencies);
                _validater->addViolations(_violationsCount, _violations);
                delete _validater;
            }
        }

        void addFrequencies(ValidationCounterT& summary) { summary += _frequencies; }
        void addViolations(ViolationCounterT& summary, ViolationMapT& collector)
        { 
            summary += _violationsCount; 
            collector += _violations;  
        }

    private:
        ChoiceT        _lawChoice;
        LawValidaterI* _validater;
        ValidationCounterT _frequencies;
        ViolationCounterT  _violationsCount;
        ViolationMapT      _violations;
    };

}} // namespace itl boost

