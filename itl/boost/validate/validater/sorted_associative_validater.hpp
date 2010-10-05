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

#include <boost/mpl/if.hpp>
#include <boost/validate/laws/atomic_equivalence.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/concept_validater.hpp>

#include <boost/validate/std/algorithm/copy.hpp>
#include <boost/validate/std/algorithm/set_algo.hpp>
#include <boost/validate/std/algorithm/relation.hpp>
#include <boost/validate/std/algorithm/find.hpp>


namespace boost{namespace itl
{

template <typename Type, typename TargetT = itl::list<typename Type::value_type> >
class sorted_associative_validater : public concept_validater
{
public:
    typedef typename Type::atomized_type     atomic_type;
    typedef typename atomic_type::value_type value_type;
    //CL typedef typename Type::domain_type       domain_type;
    typedef typename domain_type_of<Type>::type domain_type;
    typedef typename Type::codomain_type     codomain_type;

    typedef typename mpl::if_< is_interval_map<Type>
                             , std::pair<domain_type, codomain_type>
                             , domain_type>::type element_type;

public:

    enum Laws 
    { 
          copy         
        , copy_backward         
        , reverse_copy 
        , equal        
        , lexicographical_compare
        , includes      
        , set_union     
        , set_difference
        , set_intersection
        , set_symmetric_difference
        , find 
        , lower_bound 
        , upper_bound 
        , Laws_size 
    };

    sorted_associative_validater() {setProfile();}

    void setProfile()
    {
        const int sum_of_shares = 100;

        _lawChoice.setSize(Laws_size);
        _lawChoice.setMaxWeights(sum_of_shares);

        int rest_shares = sum_of_shares, item_index = 0;
        _lawChoice[copy                    ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[copy_backward           ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[reverse_copy            ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[equal                   ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[lexicographical_compare ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[includes                ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[set_union               ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[set_difference          ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[set_intersection        ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[set_symmetric_difference] = share(Laws_size, item_index, rest_shares);
        _lawChoice[find                    ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[lower_bound             ] = share(Laws_size, item_index, rest_shares);
        _lawChoice[upper_bound             ] = share(Laws_size, item_index, rest_shares);

        _lawChoice.init();
    }


    LawValidaterI* chooseValidater()
    {
        switch(_lawChoice.some())
        {
        case copy                    : return new LawValidater< UnaryAtomicEquivalence <Type, TargetT,     std_copy                     > >;
        case copy_backward           : return new LawValidater< UnaryAtomicEquivalence <Type, TargetT,     std_copy_backward            > >;
        case reverse_copy            : return new LawValidater< UnaryAtomicEquivalence <Type, TargetT,     std_reverse_copy             > >;
        case equal                   : return new LawValidater<BinaryAtomicEquivalence <Type, bool,        std_equals                   > >;
        case lexicographical_compare : return new LawValidater<BinaryAtomicEquivalence <Type, bool,        std_lexicographical_compare  > >;
        case includes                : return new LawValidater<BinaryAtomicEquivalence <Type, bool,        std_includes                 > >;
        case set_union               : return new LawValidater<BinaryAtomicEquivalence <Type, TargetT,     std_set_union                > >;
        case set_difference          : return new LawValidater<BinaryAtomicEquivalence <Type, TargetT,     std_set_difference           > >;
        case set_intersection        : return new LawValidater<BinaryAtomicEquivalence <Type, TargetT,     std_set_intersection         > >;
        case set_symmetric_difference: return new LawValidater<BinaryAtomicEquivalence <Type, TargetT,     std_set_symmetric_difference > >;
        case find                    : return new LawValidater< UnaryAtomicEquivalence2<Type, element_type, std_find                    > >;
        case lower_bound             : return new LawValidater< UnaryAtomicEquivalence2<Type, element_type, std_lower_bound             > >;
        case upper_bound             : return new LawValidater< UnaryAtomicEquivalence2<Type, element_type, std_upper_bound             > >;
        default: return NULL;
        }
    }

private:
    ChoiceT _lawChoice;
};


}} // namespace itl boost

#endif // BOOST_VALIDATE_VALIDATER_SORTED_ASSOCIATIVE_VALIDATER_HPP_JOFA_091125
