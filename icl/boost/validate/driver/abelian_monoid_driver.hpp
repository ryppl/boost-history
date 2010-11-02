/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_DRIVER_ABELIAN_MONOID_DRIVER_HPP_JOFA_100502
#define BOOST_VALIDATE_DRIVER_ABELIAN_MONOID_DRIVER_HPP_JOFA_100502

#include <iostream>
#include <stdio.h>
#include <boost/icl/interval_set.hpp>
#include <boost/icl/split_interval_set.hpp>
#include <boost/validate/validater/abelian_monoid_validater.hpp>
#include <boost/validate/driver/icl_driver.hpp>
#include <boost/validate/utility.hpp>

namespace boost{namespace icl
{
    class abelian_monoid_driver : public icl_driver
    {
    public:
        abelian_monoid_driver() { setProfile(); }

        void setProfile()
        {
            setValid(true);
            _rootChoice.setSize(RootType::Types_size);
            _rootChoice.setMaxWeights(100);
            _rootChoice[RootType::icl_set]               = 33;
            _rootChoice[RootType::interval_set]          = 33;
            _rootChoice[RootType::split_interval_set]    = 34;
            setRootTypeNames();
            _rootChoice.init();

            _domainChoice.setSize(DomainType::DomainTypes_size);
            _domainChoice.setMaxWeights(100);
            _domainChoice[DomainType::Int]               = 50;
            _domainChoice[DomainType::Double]            = 50;
            setDomainTypeNames();
            _domainChoice.init();

            if(!_rootChoice.is_consistent())
            {
                setValid(false);
                std::cout << _rootChoice.inconsitencyMessage("abelian_monoid_driver::setProfile()") << std::endl;
            }

            if(!_domainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _domainChoice.inconsitencyMessage("abelian_monoid_driver::setProfile()") << std::endl;
            }
        }

        concept_validater* chooseValidater()
        {
            int rootChoice   = _rootChoice.some();
            int domainChoice = _domainChoice.some();

            switch(rootChoice)
            {
            case RootType::icl_set: {
                    switch(domainChoice) {
                    case DomainType::Int:    return new abelian_monoid_validater<std::set<int> >; 
                    case DomainType::Double: return new abelian_monoid_validater<std::set<double> >; 
                    default: return choiceError(ICL_LOCATION("\nRootType::icl_set: domainChoice:\n"), 
                                                domainChoice, _domainChoice);
                    }
                }
            case RootType::interval_set: {
                    switch(domainChoice) {
                    case DomainType::Int:    return new abelian_monoid_validater<interval_set<int> >;
                    case DomainType::Double: return new abelian_monoid_validater<interval_set<double> >;
                    default: return choiceError(ICL_LOCATION("\nRootType::interval_set: domainChoice:\n"), 
                                                domainChoice, _domainChoice);
                    }
                }
            case RootType::split_interval_set: {
                    switch(domainChoice) {
                    case DomainType::Int:    return new abelian_monoid_validater<split_interval_set<int> >;
                    case DomainType::Double: return new abelian_monoid_validater<split_interval_set<double> >;
                    default: return choiceError(ICL_LOCATION("\nRootType::split_interval_set: domainChoice:\n"), 
                                                domainChoice, _domainChoice);
                    }
                }
            default: return choiceError(ICL_LOCATION("rootChoice:\n"), rootChoice, _rootChoice);
            } //switch()
        }
    };

}} // namespace icl boost

#endif // BOOST_VALIDATE_DRIVER_ABELIAN_MONOID_DRIVER_HPP_JOFA_100502
