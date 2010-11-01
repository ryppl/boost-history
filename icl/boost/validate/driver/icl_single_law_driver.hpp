/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_DRIVER_SINGLE_LAW_DRIVER_HPP_JOFA_080405
#define BOOST_VALIDATE_DRIVER_SINGLE_LAW_DRIVER_HPP_JOFA_080405

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <boost/validate/typevalidater.h>
#include <boost/validate/validater/icl_induced_relations.hpp>
#include <boost/validate/driver/icl_driver.hpp>
#include <boost/validate/utility.hpp>

namespace boost{namespace icl
{
    
    template<class Law>
    class icl_single_law_driver : public icl_driver
    {
    public:
        icl_single_law_driver() { setProfile(); }


        void setProfile()
        {
            setValid(true);
            _rootChoice.setSize(RootType::Types_size);
            _rootChoice.setMaxWeights(100);
            _rootChoice[RootType::icl_set]               = 0;
            _rootChoice[RootType::interval_set]          = 10;
            _rootChoice[RootType::separate_interval_set] = 10;
            _rootChoice[RootType::split_interval_set]    = 10;
            _rootChoice[RootType::icl_map]               = 0;
            _rootChoice[RootType::interval_map]          = 35;
            _rootChoice[RootType::split_interval_map]    = 35;
            setRootTypeNames();
            _rootChoice.init();

            _domainChoice.setSize(DomainType::DomainTypes_size);
            _domainChoice.setMaxWeights(100);
            _domainChoice[DomainType::Int]               = 100;
            _domainChoice[DomainType::Double]            = 0;  //NOTE: induced relations only
            setDomainTypeNames();                              // work for integral DomainType, because 
            _domainChoice.init();                              // atomized_type needs to be finite. 

            _codomainChoice.setSize(CodomainType::CodomainTypes_size);
            _codomainChoice.setMaxWeights(100);
            _codomainChoice[CodomainType::Int]           = 100;
            _codomainChoice[CodomainType::Double]        = 0;
            _codomainChoice[CodomainType::set_int]       = 0;
            setCodomainTypeNames();
            _codomainChoice.init();

            _identityHandlerChoice.setSize(IdentityHandlerType::IdentityHandlerTypes_size);
            _identityHandlerChoice.setMaxWeights(100);
            _identityHandlerChoice[IdentityHandlerType::partial_absorber]      = 25;
            _identityHandlerChoice[IdentityHandlerType::partial_enricher]      = 25;
            _identityHandlerChoice[IdentityHandlerType::total_absorber]        = 25;
            _identityHandlerChoice[IdentityHandlerType::total_enricher]        = 25;
            setIdentityHandlerTypeNames();
            _identityHandlerChoice.init();

            if(!_rootChoice.is_consistent())
            {
                setValid(false);
                std::cout << _rootChoice.inconsitencyMessage("icl_single_law_driver::setProfile()") << std::endl;
            }

            if(!_domainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _domainChoice.inconsitencyMessage("icl_single_law_driver::setProfile()") << std::endl;
            }

            if(!_codomainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _codomainChoice.inconsitencyMessage("icl_single_law_driver::setProfile()") << std::endl;
            }

            if(!_identityHandlerChoice.is_consistent())
            {
                setValid(false);
                std::cout << _identityHandlerChoice.inconsitencyMessage("icl_single_law_driver::setProfile()") << std::endl;
            }

        }


        concept_validater* chooseValidater()
        {
            int rootChoice         = _rootChoice.some();
            int domainChoice       = _domainChoice.some();
            int codomainChoice     = _codomainChoice.some();
            int identityHandlerChoice  = _identityHandlerChoice.some();

            switch(rootChoice)
            {
            case RootType::interval_set: {
                    switch(domainChoice) {
                    case DomainType::Int: return new icl_induced_relations_validater<interval_set<int> >;
                    default: return choiceError(ICL_LOCATION("\nRootType::interval_set: domainChoice:\n"), 
                                                domainChoice, _domainChoice);
                    }
                }
            case RootType::separate_interval_set: {
                    switch(domainChoice) {
                    case DomainType::Int: return new icl_induced_relations_validater<separate_interval_set<int> >;
                    default: return choiceError(ICL_LOCATION("\nRootType::separate_interval_set: domainChoice:\n"), 
                                                domainChoice, _domainChoice);
                    }
                 }
            case RootType::split_interval_set: {
                    switch(domainChoice) {
                    case DomainType::Int: return new icl_induced_relations_validater<split_interval_set<int> >;   
                    default: return choiceError(ICL_LOCATION("\nRootType::split_interval_set: domainChoice:\n"), 
                                                domainChoice, _domainChoice);
                    }
                }
            case RootType::split_interval_map: {
                    switch(domainChoice) {
                        case DomainType::Int: 
                            switch(identityHandlerChoice) {
                                case IdentityHandlerType::partial_absorber: 
                                    return new icl_induced_relations_validater<split_interval_map<int,int,partial_absorber> >;
                                case IdentityHandlerType::partial_enricher: 
                                    return new icl_induced_relations_validater<split_interval_map<int,int,partial_enricher> >;
                                case IdentityHandlerType::total_absorber: 
                                    return new icl_induced_relations_validater<split_interval_map<int,int,total_absorber> >;
                                case IdentityHandlerType::total_enricher: 
                                    return new icl_induced_relations_validater<split_interval_map<int,int,total_enricher> >;
                                default: return choiceError(ICL_LOCATION("\nRootType::split_interval_map: identityHandlerChoice:\n"), 
                                                identityHandlerChoice, _identityHandlerChoice);
                            }
                    default: return choiceError(ICL_LOCATION("\nRootType::split_interval_map: domainChoice:\n"), 
                                                domainChoice, _domainChoice);
                    }
                }
            case RootType::interval_map: {
                    switch(domainChoice) {
                        case DomainType::Int: 
                            switch(identityHandlerChoice) {
                                case IdentityHandlerType::partial_absorber: 
                                    return new icl_induced_relations_validater<interval_map<int,int,partial_absorber> >;
                                case IdentityHandlerType::partial_enricher: 
                                    return new icl_induced_relations_validater<interval_map<int,int,partial_enricher> >;
                                case IdentityHandlerType::total_absorber: 
                                    return new icl_induced_relations_validater<interval_map<int,int,total_absorber> >;
                                case IdentityHandlerType::total_enricher: 
                                    return new icl_induced_relations_validater<interval_map<int,int,total_enricher> >;
                                default: return choiceError(ICL_LOCATION("\nRootType::split_interval_map: identityHandlerChoice:\n"), 
                                                identityHandlerChoice, _identityHandlerChoice);
                            }
                    default: return choiceError(ICL_LOCATION("\nRootType::split_interval_map: domainChoice:\n"), 
                                                domainChoice, _domainChoice);
                    }
                }
            default: return choiceError(ICL_LOCATION("rootChoice:\n"), rootChoice, _rootChoice);
            } //switch()

        }

    };


}} // namespace icl boost

#endif // BOOST_VALIDATE_DRIVER_SINGLE_LAW_DRIVER_HPP_JOFA_080405

