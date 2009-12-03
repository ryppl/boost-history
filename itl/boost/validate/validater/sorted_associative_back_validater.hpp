/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_VALIDATER_SORTED_ASSOCIATIVE_BACK_VALIDATER_HPP_JOFA_091202
#define BOOST_VALIDATE_VALIDATER_SORTED_ASSOCIATIVE_BACK_VALIDATER_HPP_JOFA_091202

#include <boost/validate/laws/atomic_equivalence.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/algebra_validater.hpp>

namespace boost{namespace itl
{

typedef WeightedNumberGentor<int> ChoiceT;

template <typename Type, typename TargetT = itl::list<typename Type::value_type> >
class sorted_associative_back_validater : public algebra_validater
{
public:

    enum Laws 
    { 
        copy          ,
        reverse_copy  ,
        equals        ,
        lexicographical_compare,
        includes      ,
        set_union     ,
        set_difference,
        set_intersection,
        set_symmetric_difference,
        Laws_size 
    };

    sorted_associative_back_validater() {setProfile();}

    void setProfile()
    {
        const int sum_of_shares = 100;

        _lawChoice.setSize(Laws_size);
        _lawChoice.setMaxWeights(sum_of_shares);

        int rest_shares = sum_of_shares, item_index = 0;
        _lawChoice[copy                    ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[reverse_copy            ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[equals                  ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[lexicographical_compare ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[includes                ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[set_union               ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[set_difference          ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[set_intersection        ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[set_symmetric_difference] = share(Laws_size, item_index, rest_shares);

        _lawChoice.init();
    }


    LawValidaterI* chooseValidater()
    {
        switch(_lawChoice.some())
        {
        case copy                    : return new LawValidater< UnaryAtomicEquivalence<Type, TargetT, std_copy_back           >, RandomGentor>;
        case reverse_copy            : return new LawValidater< UnaryAtomicEquivalence<Type, TargetT, std_reverse_copy_back   >, RandomGentor>;
        case equals                  : return new LawValidater<BinaryAtomicEquivalence<Type, bool,    std_equals_back         >, RandomGentor>;
        case lexicographical_compare : return new LawValidater<BinaryAtomicEquivalence<Type, bool,    std_lexicographical_compare_back>, RandomGentor>;
        case includes                : return new LawValidater<BinaryAtomicEquivalence<Type, bool,    std_includes_back       >, RandomGentor>;
        case set_union               : return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_set_union_back      >, RandomGentor>;
        case set_difference          : return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_set_difference_back >, RandomGentor>;
        case set_intersection        : return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_set_intersection_back >, RandomGentor>;
        case set_symmetric_difference: return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_set_symmetric_difference_back >, RandomGentor>;
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
    ChoiceT            _lawChoice;
    LawValidaterI*     _validater;
    ValidationCounterT _frequencies;
    ViolationCounterT  _violationsCount;
    ViolationMapT      _violations;
};


}} // namespace itl boost

#endif // BOOST_VALIDATE_VALIDATER_SORTED_ASSOCIATIVE_BACK_VALIDATER_HPP_JOFA_091202
