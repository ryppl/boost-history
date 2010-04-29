/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_VALIDATE_VALIDATER_UNSIGNED_QUANTIFIER_VALIDATER_HPP_JOFA_090314
#define BOOST_ITL_VALIDATE_VALIDATER_UNSIGNED_QUANTIFIER_VALIDATER_HPP_JOFA_090314

#include <boost/itl/functors.hpp>
#include <boost/itl/type_traits/absorbs_neutrons.hpp>
#include <boost/itl/type_traits/is_total.hpp>
#include <boost/validate/laws/monoid.hpp>
#include <boost/validate/laws/map_laws.hpp>
#include <boost/validate/laws/inversion_laws.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/concept_validater.hpp>

#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif                        

namespace boost{namespace itl
{

typedef WeightedNumberGentor<int> ChoiceT;

template <typename Type>
class unsigned_quantifier_validater : public concept_validater
{
public:

    enum Laws 
    { 
        inplacePlusAssociativity,
        inplacePlusNeutrality,
        inplacePlusCommutativity,
        inplaceEtAssociativity,
        inplaceEtCommutativity,
        inplaceNaturalInversion,
        sectionAbsorbtion,
        Laws_size 
    };

    unsigned_quantifier_validater() {setProfile();}

    void setProfile()
    {
        const int sum_of_shares = 100;

        _lawChoice.setSize(Laws_size);
        _lawChoice.setMaxWeights(sum_of_shares);

        int rest_shares = sum_of_shares, item_index = 0;
        _lawChoice[inplacePlusAssociativity] = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusNeutrality]    = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusCommutativity] = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtAssociativity]   = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtCommutativity]   = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceNaturalInversion]  = share(Laws_size, item_index, rest_shares);
        _lawChoice[sectionAbsorbtion]        = share(Laws_size, item_index, rest_shares);
        _lawChoice.init();
    }


    LawValidaterI* chooseValidater()
    {
        switch(_lawChoice.some())
        {
        case inplacePlusAssociativity:  return new LawValidater<InplaceAssociativity<Type, inplace_plus> >;
        case inplacePlusNeutrality:     return new LawValidater<InplaceNeutrality   <Type> >;
        case inplacePlusCommutativity:  return new LawValidater<InplaceCommutativity<Type> >;
        case inplaceEtAssociativity:    return new LawValidater<InplaceAssociativity<Type, inplace_et> >;
        case inplaceEtCommutativity:    return new LawValidater<InplaceCommutativity<Type, inplace_et> >;
        case inplaceNaturalInversion:
            if(absorbs_neutrons<Type>::value)
                                        return new LawValidater<InplaceNaturalInversion<Type, inplace_plus, std_equal> >;
            else                        return new LawValidater<InplaceNaturalInversion<Type, inplace_plus, protonic_equal> >;
        case sectionAbsorbtion:            
            if(absorbs_neutrons<Type>::value)
                                        return new LawValidater<SectionAbsorbtion<Type,std_equal> >;
            else                        return new LawValidater<SectionAbsorbtion<Type,protonic_equal> >;
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

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // BOOST_ITL_VALIDATE_VALIDATER_UNSIGNED_QUANTIFIER_VALIDATER_HPP_JOFA_090314

