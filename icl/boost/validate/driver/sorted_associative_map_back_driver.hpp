/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_DRIVER_SORTED_ASSOCIATIVE_MAP_BACK_DRIVER_HPP_JOFA_091202
#define BOOST_VALIDATE_DRIVER_SORTED_ASSOCIATIVE_MAP_BACK_DRIVER_HPP_JOFA_091202

#include <iostream>
#include <stdio.h>
#include <boost/icl/map.hpp>
#include <boost/icl/interval_map.hpp>
#include <boost/icl/split_interval_map.hpp>
#include <boost/validate/validater/sorted_associative_back_validater.hpp>
#include <boost/validate/driver/icl_driver.hpp>
#include <boost/validate/utility.hpp>

namespace boost{namespace icl
{
    
    class sorted_associative_map_back_driver : public icl_driver
    {
    public:
        sorted_associative_map_back_driver() { setProfile(); }

        void setProfile()
        {
            setValid(true);
            _rootChoice.setSize(RootType::Types_size);
            _rootChoice.setMaxWeights(100);
            _rootChoice[RootType::icl_set]               = 0;
            _rootChoice[RootType::interval_set]          = 0;
            _rootChoice[RootType::separate_interval_set] = 0;
            _rootChoice[RootType::split_interval_set]    = 0;
            _rootChoice[RootType::icl_map]               = 0;
            _rootChoice[RootType::interval_map]          = 50;
            _rootChoice[RootType::split_interval_map]    = 50;
            setRootTypeNames();
            _rootChoice.init();

            _domainChoice.setSize(DomainType::DomainTypes_size);
            _domainChoice.setMaxWeights(100);
            _domainChoice[DomainType::Int]               = 100;
            _domainChoice[DomainType::Double]            = 0;
            setDomainTypeNames();
            _domainChoice.init();

            _codomainChoice.setSize(CodomainType::CodomainTypes_size);
            _codomainChoice.setMaxWeights(100);
            _codomainChoice[CodomainType::Nat]           = 100;
            _codomainChoice[CodomainType::Int]           = 0;
            _codomainChoice[CodomainType::Double]        = 0;
            _codomainChoice[CodomainType::set_int]       = 0;
            setCodomainTypeNames();
            _codomainChoice.init();

            _identityHandlerChoice.setSize(IdentityHandlerType::IdentityHandlerTypes_size);
            _identityHandlerChoice.setMaxWeights(100);
            _identityHandlerChoice[IdentityHandlerType::partial_absorber]    = 25;
            _identityHandlerChoice[IdentityHandlerType::partial_enricher]    = 25;
            _identityHandlerChoice[IdentityHandlerType::total_absorber]      = 25;
            _identityHandlerChoice[IdentityHandlerType::total_enricher]      = 25;
            setIdentityHandlerTypeNames();
            _identityHandlerChoice.init();

            if(!_rootChoice.is_consistent())
            {
                setValid(false);
                std::cout << _rootChoice.inconsitencyMessage("sorted_associative_map_back_driver::setProfile()") << std::endl;
            }

            if(!_domainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _domainChoice.inconsitencyMessage("sorted_associative_map_back_driver::setProfile()") << std::endl;
            }

            if(!_codomainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _codomainChoice.inconsitencyMessage("sorted_associative_map_back_driver::setProfile()") << std::endl;
            }

            if(!_identityHandlerChoice.is_consistent())
            {
                setValid(false);
                std::cout << _identityHandlerChoice.inconsitencyMessage("sorted_associative_map_back_driver::setProfile()") << std::endl;
            }

        }


        concept_validater* chooseValidater()
        {
            int rootChoice         = _rootChoice.some();
            //int domainChoice       = _domainChoice.some();
            //int codomainChoice     = _codomainChoice.some();
            int identityHandlerChoice  = _identityHandlerChoice.some();

            switch(rootChoice)
            {
            //-----------------------------------------------------------------
            case RootType::interval_map: {
                switch(identityHandlerChoice) {
                case IdentityHandlerType::partial_absorber: return new sorted_associative_back_validater<interval_map<int,int,partial_absorber>, icl::list<std::pair<int,int> > >;
                case IdentityHandlerType::partial_enricher: return new sorted_associative_back_validater<interval_map<int,int,partial_enricher>, icl::list<std::pair<int,int> > >;
                case IdentityHandlerType::total_absorber:   return new sorted_associative_back_validater<interval_map<int,int,total_absorber  >, icl::list<std::pair<int,int> > >;
                case IdentityHandlerType::total_enricher:   return new sorted_associative_back_validater<interval_map<int,int,total_enricher  >, icl::list<std::pair<int,int> > >;
                default: return choiceError(ICL_LOCATION("\nRootType::interval_map: identityHandlerChoice:\n"), identityHandlerChoice, _identityHandlerChoice);
                }//switch identityHandlerChoice
            }//case interval_map 
            //-----------------------------------------------------------------
            case RootType::split_interval_map: {
                switch(identityHandlerChoice) {
                case IdentityHandlerType::partial_absorber: return new sorted_associative_back_validater<split_interval_map<int,int,partial_absorber>, icl::list<std::pair<int,int> > >;
                case IdentityHandlerType::partial_enricher: return new sorted_associative_back_validater<split_interval_map<int,int,partial_enricher>, icl::list<std::pair<int,int> > >;
                case IdentityHandlerType::total_absorber:   return new sorted_associative_back_validater<split_interval_map<int,int,total_absorber  >, icl::list<std::pair<int,int> > >;
                case IdentityHandlerType::total_enricher:   return new sorted_associative_back_validater<split_interval_map<int,int,total_enricher  >, icl::list<std::pair<int,int> > >;
                default: return choiceError(ICL_LOCATION("\nRootType::split_interval_map: identityHandlerChoice:\n"), identityHandlerChoice, _identityHandlerChoice);
                }//switch identityHandlerChoice
            }//case split_interval_map 
            //-----------------------------------------------------------------

            default: return choiceError(ICL_LOCATION("rootChoice:\n"), rootChoice, _rootChoice);
            } //switch()

        }
    };


}} // namespace icl boost

#endif // BOOST_VALIDATE_DRIVER_SORTED_ASSOCIATIVE_MAP_BACK_DRIVER_HPP_JOFA_091202
