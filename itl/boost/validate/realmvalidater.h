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

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <boost/validate/typevalidater.h>
#include <boost/validate/itl_validater.hpp>
#include <boost/validate/utility.hpp>

namespace boost{namespace itl
{
    
	class RealmValidater : public itl_validater
    {
    public:
        RealmValidater() { setProfile(); }

        void setProfile()
        {
            setValid(true);
            _rootChoice.setSize(RootType::Types_size);
            _rootChoice.setMaxWeights(100);
            _rootChoice[RootType::itl_set]               = 14;
            _rootChoice[RootType::interval_set]          = 14;
            _rootChoice[RootType::separate_interval_set] = 14;
            _rootChoice[RootType::split_interval_set]    = 14;
            _rootChoice[RootType::itl_map]               = 14;
            _rootChoice[RootType::interval_map]          = 15;
            _rootChoice[RootType::split_interval_map]    = 15;
            setRootTypeNames();
            _rootChoice.init();

            _domainChoice.setSize(DomainType::DomainTypes_size);
            _domainChoice.setMaxWeights(100);
            _domainChoice[DomainType::Int]               = 50;
            _domainChoice[DomainType::Double]            = 50;
            setDomainTypeNames();
            _domainChoice.init();

            _codomainChoice.setSize(CodomainType::CodomainTypes_size);
            _codomainChoice.setMaxWeights(100);
            _codomainChoice[CodomainType::Int]           = 50;
            _codomainChoice[CodomainType::Double]        = 0;
            _codomainChoice[CodomainType::set_int]       = 50;
            setCodomainTypeNames();
            _codomainChoice.init();

            _neutronizerChoice.setSize(NeutronHandlerType::NeutronHandlerTypes_size);
            _neutronizerChoice.setMaxWeights(100);
            _neutronizerChoice[NeutronHandlerType::absorber]      = 50;
            _neutronizerChoice[NeutronHandlerType::enricher]      = 50;
            _neutronizerChoice[NeutronHandlerType::emitter]       = 0;
            _neutronizerChoice[NeutronHandlerType::polluter]      = 0;
            setNeutronHandlerTypeNames();
            _neutronizerChoice.init();

            if(!_rootChoice.is_consistent())
            {
                setValid(false);
                std::cout << _rootChoice.inconsitencyMessage("RealmValidater::setProfile()") << std::endl;
            }

            if(!_domainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _domainChoice.inconsitencyMessage("RealmValidater::setProfile()") << std::endl;
            }

            if(!_codomainChoice.is_consistent())
            {
                setValid(false);
                std::cout << _codomainChoice.inconsitencyMessage("RealmValidater::setProfile()") << std::endl;
            }

            if(!_neutronizerChoice.is_consistent())
            {
                setValid(false);
                std::cout << _neutronizerChoice.inconsitencyMessage("RealmValidater::setProfile()") << std::endl;
            }

        }

		/*CL
        void validate()
        {
            //srand(static_cast<unsigned>(time(NULL))); //Different numbers each run
            srand(static_cast<unsigned>(1)); //Same numbers each run (std)
            //srand(static_cast<unsigned>(4711)); //Same numbers each run (varying)

            for(int idx=0; hasValidProfile(); idx++)
            {
				if(_frequencies.size() == 402)
				{
					reportFrequencies("freq402.txt");
					break;
				}
                if(idx>0 && idx % 100 == 0)
                    reportFrequencies();
                validateType();
            }
        }
		*/

        AlgebraValidater* chooseValidater()
        {
            int rootChoice         = _rootChoice.some();
            int domainChoice       = _domainChoice.some();
            int codomainChoice     = _codomainChoice.some();
            int neutronizerChoice  = _neutronizerChoice.some();

            switch(rootChoice)
            {
            case RootType::itl_set: {
                    switch(domainChoice) {
                    case DomainType::Int:    return new InplaceSetValidater<itl::set<int> >; 
                    case DomainType::Double: return new InplaceSetValidater<itl::set<double> >; 
                    default: return choiceError(ITL_LOCATION("\nRootType::itl_set: domainChoice:\n"), 
                                                domainChoice, _domainChoice);
                    }
                }
            case RootType::interval_set: {
                    switch(domainChoice) {
                    case DomainType::Int:    return new IntervalSetValidater<interval_set<int> >;
                    case DomainType::Double: return new InplaceSetValidater<interval_set<double> >;
                    default: return choiceError(ITL_LOCATION("\nRootType::interval_set: domainChoice:\n"), 
                                                domainChoice, _domainChoice);
                    }
                }
            case RootType::separate_interval_set: {
                    switch(domainChoice) {
                    case DomainType::Int:    return new IntervalSetValidater<split_interval_set<int> >;
                    case DomainType::Double: return new IntervalSetValidater<split_interval_set<double> >;
                    default: return choiceError(ITL_LOCATION("\nRootType::separate_interval_set: domainChoice:\n"), 
                                                domainChoice, _domainChoice);
                    }
                 }
            case RootType::split_interval_set: {
                    switch(domainChoice) {
                    case DomainType::Int:    return new IntervalSetValidater<split_interval_set<int> >;
                    case DomainType::Double: return new IntervalSetValidater<split_interval_set<double> >;
                    default: return choiceError(ITL_LOCATION("\nRootType::split_interval_set: domainChoice:\n"), 
                                                domainChoice, _domainChoice);
                    }
                }
            //-----------------------------------------------------------------
            // Maps
            //-----------------------------------------------------------------
            case RootType::itl_map: {
                switch(domainChoice) {
                case DomainType::Int: 
                    switch(codomainChoice) {
                    case CodomainType::Int:     return new InplaceMapValidater<itl::map<int,int> >;
                    case CodomainType::set_int: return new InplaceMapValidater<itl::map<int,itl::set<int> > >;
                    default: return choiceError(ITL_LOCATION("\nRootType::itl_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }//switch codomain

                case DomainType::Double:
                    switch(codomainChoice) {
                    case CodomainType::Int:     return new InplaceMapValidater<itl::map<double,int,neutron_enricher> >; 
                    case CodomainType::set_int: return new InplaceSetValidater<itl::map<double,itl::set<int>,neutron_enricher > >;
                    default: return choiceError(ITL_LOCATION("\nRootType::itl_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }//switch codomain

                default: return choiceError(ITL_LOCATION("\nRootType::itl_map: domainChoice:\n"),
                                            domainChoice, _domainChoice);
                }//switch domain
            }//case itl_map 
            //-----------------------------------------------------------------
            case RootType::interval_map: {
                switch(domainChoice) {
                case DomainType::Int:
                    switch(codomainChoice) {
                    case CodomainType::Int:     return new IntervalMapValidater<interval_map<int,int,neutron_emitter> >; 
                    case CodomainType::set_int: return new IntervalMapValidater<interval_map<int,itl::set<int> > >; 
                    default: return choiceError(ITL_LOCATION("\nRootType::interval_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }// switch codomain
                case DomainType::Double:
                    switch(codomainChoice) {
                    case CodomainType::Int:     return new IntervalMapValidater<interval_map<double,int,neutron_polluter> >; 
                    case CodomainType::set_int: return new IntervalMapValidater<interval_map<double,itl::set<int>,neutron_enricher > >;
                    default: return choiceError(ITL_LOCATION("\nRootType::interval_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }// switch codomain
                default: return choiceError(ITL_LOCATION("\nRootType::interval_map: domainChoice:\n"),
                                            domainChoice, _domainChoice);
                }//switch domain
            }//case interval_map
            //-----------------------------------------------------------------
            case RootType::split_interval_map: {
                switch(domainChoice) {
                case DomainType::Int:
                    switch(codomainChoice) {
                    case CodomainType::Int:     return new IntervalMapValidater<split_interval_map<int,int,neutron_enricher> >; 
                    case CodomainType::set_int: return new IntervalMapValidater<split_interval_map<int,itl::set<int>,neutron_enricher> >; 
                    default: return choiceError(ITL_LOCATION("\nRootType::split_interval_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }
                case DomainType::Double:
                    switch(codomainChoice) {
                    case CodomainType::Int:     return new IntervalMapValidater<split_interval_map<double,int,neutron_emitter> >; 
                    case CodomainType::set_int: return new IntervalSetValidater<split_interval_map<double,itl::set<int> > >;
                    default: return choiceError(ITL_LOCATION("\nRootType::split_interval_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                }
                default: return choiceError(ITL_LOCATION("\nRootType::split_interval_map: domainChoice:\n"),
                                            domainChoice, _domainChoice);
                }//switch domain
            }//case split_interval_map
            //-----------------------------------------------------------------

            default: return choiceError(ITL_LOCATION("rootChoice:\n"), rootChoice, _rootChoice);
            } //switch()

            return NULL; //just to please the compiler ;)
        }

    };


}} // namespace itl boost

