/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_VALIDATER_SORTED_ASSOCIATIVE_VALIDATER_HPP_JOFA_091125
#define BOOST_VALIDATE_VALIDATER_SORTED_ASSOCIATIVE_VALIDATER_HPP_JOFA_091125

#include <boost/validate/laws/atomic_equivalence.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/algebra_validater.hpp>

namespace boost{namespace itl
{

typedef WeightedNumberGentor<int> ChoiceT;

template <typename Type, typename TargetT = itl::list<typename Type::value_type> >
class sorted_associative_validater : public algebra_validater
{
public:

    enum Laws 
    { 
        copy_fore          ,
        copy_back          ,
        reverse_copy_fore  ,
        reverse_copy_back  ,
        includes_fore      ,
        includes_back      ,
        set_union_fore     ,
        set_union_back     ,
        set_difference_fore,
        set_difference_back,
        set_intersection_fore,
        set_intersection_back,
        set_symmetric_difference_fore,
        set_symmetric_difference_back,
        Laws_size 
    };

    sorted_associative_validater() {setProfile();}

    void setProfile()
    {
        const int sum_of_shares = 100;

        _lawChoice.setSize(Laws_size);
        _lawChoice.setMaxWeights(sum_of_shares);

        int rest_shares = sum_of_shares, item_index = 0;
		_lawChoice[copy_fore                    ] = share(Laws_size, item_index, rest_shares);
		_lawChoice[copy_back                    ] = share(Laws_size, item_index, rest_shares);
		_lawChoice[reverse_copy_fore            ] = share(Laws_size, item_index, rest_shares);
		_lawChoice[reverse_copy_back            ] = share(Laws_size, item_index, rest_shares);
		_lawChoice[includes_fore                ] = share(Laws_size, item_index, rest_shares);
		_lawChoice[includes_back                ] = share(Laws_size, item_index, rest_shares);
		_lawChoice[set_union_fore               ] = share(Laws_size, item_index, rest_shares);
		_lawChoice[set_union_back               ] = share(Laws_size, item_index, rest_shares);
		_lawChoice[set_difference_fore          ] = share(Laws_size, item_index, rest_shares);
		_lawChoice[set_difference_back          ] = share(Laws_size, item_index, rest_shares);
		_lawChoice[set_intersection_fore        ] = share(Laws_size, item_index, rest_shares);
		_lawChoice[set_intersection_back        ] = share(Laws_size, item_index, rest_shares);
		_lawChoice[set_symmetric_difference_fore] = share(Laws_size, item_index, rest_shares);
		_lawChoice[set_symmetric_difference_back] = share(Laws_size, item_index, rest_shares);

        _lawChoice.init();
    }


    LawValidaterI* chooseValidater()
    {
        switch(_lawChoice.some())
        {
		case copy_fore          : return new LawValidater< UnaryAtomicEquivalence<Type, TargetT, std_copy_forward           >, RandomGentor>;
		case copy_back          : return new LawValidater< UnaryAtomicEquivalence<Type, TargetT, std_copy_backward          >, RandomGentor>;
		case reverse_copy_fore  : return new LawValidater< UnaryAtomicEquivalence<Type, TargetT, std_reverse_copy_forward   >, RandomGentor>;
		case reverse_copy_back  : return new LawValidater< UnaryAtomicEquivalence<Type, TargetT, std_reverse_copy_backward  >, RandomGentor>;
		case includes_fore      : return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_includes_forward      >, RandomGentor>;
		case includes_back      : return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_includes_backward     >, RandomGentor>;
		case set_union_fore     : return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_set_union_forward      >, RandomGentor>;
		case set_union_back     : return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_set_union_backward     >, RandomGentor>;
		case set_difference_fore: return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_set_difference_forward >, RandomGentor>;
		case set_difference_back: return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_set_difference_backward>, RandomGentor>;
		case set_intersection_fore: return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_set_intersection_forward >, RandomGentor>;
		case set_intersection_back: return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_set_intersection_backward>, RandomGentor>;
		case set_symmetric_difference_fore: return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_set_symmetric_difference_forward >, RandomGentor>;
		case set_symmetric_difference_back: return new LawValidater<BinaryAtomicEquivalence<Type, TargetT, std_set_symmetric_difference_backward>, RandomGentor>;
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

#endif // BOOST_VALIDATE_VALIDATER_SORTED_ASSOCIATIVE_VALIDATER_HPP_JOFA_091125
