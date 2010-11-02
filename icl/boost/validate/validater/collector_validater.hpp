/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_VALIDATE_VALIDATER_ICL_COLLECTOR_VALIDATER_HPP_JOFA_090315
#define BOOST_ICL_VALIDATE_VALIDATER_ICL_COLLECTOR_VALIDATER_HPP_JOFA_090315


#include <boost/icl/functors.hpp>
#include <boost/icl/predicates/element_equal.hpp>
#include <boost/validate/laws/monoid.hpp>
#include <boost/validate/laws/inversion_laws.hpp>
#include <boost/validate/laws/symmetric_difference.hpp>
#include <boost/validate/laws/set_laws.hpp>
#include <boost/validate/validater/law_validater.hpp>
#include <boost/validate/validater/concept_validater.hpp>

#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif                        

namespace boost{namespace icl
{

template <typename Type>
class collector_validater : public concept_validater
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
        inplaceSymmetricDifference,
        inplaceFlip,
        inplacePlusDistributivity,
        inplaceEtDistributivity,
        inplacePlusDashRightDistrib,
        inplaceEtDashRightDistrib,
        inplacePlusDeMorgan,
        inplaceEtDeMorgan,
        Laws_size 
    };

    collector_validater() {setProfile();}

    void setProfile()
    {
        const int sum_of_shares = 100;
        _lawChoice.setSize(Laws_size);
        _lawChoice.setMaxWeights(sum_of_shares);

        int rest_shares = sum_of_shares, item_index = 0;
        _lawChoice[inplacePlusAssociativity]   = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusNeutrality]      = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusCommutativity]   = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtAssociativity]     = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtCommutativity]     = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceNaturalInversion]    = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceSymmetricDifference] = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceFlip]                = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusDistributivity]  = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtDistributivity]    = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusDashRightDistrib]= share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtDashRightDistrib]  = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplacePlusDeMorgan]        = share(Laws_size, item_index, rest_shares);
        _lawChoice[inplaceEtDeMorgan]          = share(Laws_size, item_index, rest_shares);
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
        case inplaceSymmetricDifference:return new LawValidater<InplaceSymmetricDifference<Type> >;
        case inplaceFlip:               return new LawValidater<InplaceFlip<Type> >;
        case inplaceEtDistributivity:  
            if(icl::is_interval_splitter<Type>::value && absorbs_identities<Type>::value && !is_total<Type>::value)
                                        return new LawValidater<InplaceDistributivity<Type, inplace_et, inplace_plus, element_equal> >;
            else                        return new LawValidater<InplaceDistributivity<Type, inplace_et, inplace_plus, std_equal> >;
        case inplacePlusDashRightDistrib:
            if(icl::is_interval_splitter<Type>::value && absorbs_identities<Type>::value && !is_total<Type>::value)
                                        return new LawValidater<InplaceRightDistributivity<Type, inplace_plus, inplace_minus, element_equal> >;
            else                        return new LawValidater<InplaceRightDistributivity<Type, inplace_plus, inplace_minus, std_equal> >;
        case inplaceEtDashRightDistrib: return new LawValidater<InplaceRightDistributivity<Type, inplace_et, inplace_minus> >;
        case inplacePlusDeMorgan:       return new LawValidater<InplaceDeMorgan<Type, inplace_plus, inplace_et, icl::std_equal> >;
        case inplaceEtDeMorgan:        
            if(icl::is_interval_splitter<Type>::value || icl::is_interval_separator<Type>::value)
                                        return new LawValidater<InplaceDeMorgan<Type, inplace_et, inplace_plus, icl::element_equal> >;
            else                        return new LawValidater<InplaceDeMorgan<Type, inplace_et, inplace_plus, icl::std_equal> >;

        default: return NULL;
        }
    }

private:
    ChoiceT        _lawChoice;
};


}} // namespace icl boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // BOOST_ICL_VALIDATE_VALIDATER_ICL_COLLECTOR_VALIDATER_HPP_JOFA_090315

