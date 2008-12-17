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
#include <boost/validate/utility.hpp>

namespace boost{namespace itl
{
    namespace RootType 
    {
        enum RootTypes 
        { 
            itl_set, interval_set, separate_interval_set, split_interval_set, 
            itl_map, interval_map, split_interval_map, 
            Types_size 
        };
    }

    namespace DomainType 
    {
        enum DomainTypes { Int, Double, DomainTypes_size };
    }

    namespace CodomainType 
    {
        enum CodomainTypes { Int, Double, set_int, CodomainTypes_size };
    }

    namespace NeutronHandlerType 
    {
        enum NeutronHandlerTypes { absorber, enricher, emitter, NeutronHandlerTypes_size };
    }

    
    class RealmValidater
    {
    public:
        RealmValidater() { setProfile(); }

    private:
        void setRootTypeNames()
        {
            std::vector<std::string> type_names(RootType::Types_size);
            type_names[RootType::itl_set]               = "itl_set"; 
            type_names[RootType::interval_set]          = "interval_set"; 
            type_names[RootType::separate_interval_set] = "separate_interval_set"; 
            type_names[RootType::split_interval_set]    = "split_interval_set"; 
            type_names[RootType::itl_map]               = "itl_map"; 
            type_names[RootType::interval_map]          = "interval_map"; 
            type_names[RootType::split_interval_map]    = "split_interval_map"; 
            _rootChoice.setTypeNames(type_names);
        }
        void setDomainTypeNames()
        {
            std::vector<std::string> type_names(CodomainType::CodomainTypes_size);
            type_names[CodomainType::Int]               = "Int"; 
            type_names[CodomainType::Double]            = "Double"; 
            _domainChoice.setTypeNames(type_names);
        }
        void setCodomainTypeNames()
        {
            std::vector<std::string> type_names(CodomainType::CodomainTypes_size);
            type_names[CodomainType::Int]               = "Int"; 
            type_names[CodomainType::Double]            = "Double"; 
            type_names[CodomainType::set_int]           = "set_int"; 
            _codomainChoice.setTypeNames(type_names);
        }

        void setInvalid() { _isValid = false; }

        AlgebraValidater* choiceError(const std::string& location, int value, const ChoiceT& choice)
        {
            reportTypeChoiceError(location, value, choice); 
            setInvalid();
            return NULL; 
        }

    public:
        bool hasValidProfile()const { return _isValid; }

        void setProfile()
        {
            _isValid = true;
            _rootChoice.setSize(RootType::Types_size);
            _rootChoice.setMaxWeights(100);
            _rootChoice[RootType::itl_set]               = 0;
            _rootChoice[RootType::interval_set]          = 0;
            _rootChoice[RootType::separate_interval_set] = 0;
            _rootChoice[RootType::split_interval_set]    = 0;
            _rootChoice[RootType::itl_map]               = 33;
            _rootChoice[RootType::interval_map]          = 33;
            _rootChoice[RootType::split_interval_map]    = 34;
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

            if(!_rootChoice.is_consistent())
            {
                setInvalid();
                std::cout << _rootChoice.inconsitencyMessage("RealmValidater::setProfile()") << std::endl;
            }

            if(!_domainChoice.is_consistent())
            {
                setInvalid();
                std::cout << _domainChoice.inconsitencyMessage("RealmValidater::setProfile()") << std::endl;
            }

            if(!_codomainChoice.is_consistent())
            {
                setInvalid();
                std::cout << _codomainChoice.inconsitencyMessage("RealmValidater::setProfile()") << std::endl;
            }

        }

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

        AlgebraValidater* chooseValidater()
        {
            int rootChoice = _rootChoice.some();
            int domainChoice    = _domainChoice.some();
            int codomainChoice  = _codomainChoice.some();

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
            case RootType::itl_map: {
                switch(domainChoice) {
                case DomainType::Int: 
                    switch(codomainChoice) {
                    // than enricher maps have.
                    case CodomainType::Int:     return new InplaceMapValidater<itl::map<int,int,neutron_enricher> >;
                    //case CodomainType::Double:  return new InplaceMapValidater<itl::map<int,double,neutron_enricher> >;
                    case CodomainType::set_int: return new InplaceMapValidater<itl::map<int,itl::set<int> > >;
                    default: return choiceError(ITL_LOCATION("\nRootType::itl_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }//switch codomain

                case DomainType::Double:
                    switch(codomainChoice) {
                    case CodomainType::Int:     return new InplaceMapValidater<itl::map<double,int,neutron_enricher> >; 
                    //case CodomainType::Double:  return new InplaceMapValidater<itl::map<double,double,neutron_enricher> >;
                    case CodomainType::set_int: return new InplaceMapValidater<itl::map<double,itl::set<int> > >;
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
                    //JODO SectionAbsorbtion has to be tested for all absorber maps
                    case CodomainType::Int:     return new IntervalMapValidater<interval_map<int,int,neutron_enricher> >; 
                    //case CodomainType::Double:  return new IntervalMapValidater<interval_map<int,double,neutron_enricher> >;
                    case CodomainType::set_int: return new IntervalMapValidater<interval_map<int,itl::set<int> > >; 
                    default: return choiceError(ITL_LOCATION("\nRootType::interval_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }// switch codomain
                case DomainType::Double:
                    switch(codomainChoice) {
                    case CodomainType::Int:     return new IntervalMapValidater<interval_map<double,int,neutron_enricher> >; 
                    //case CodomainType::Double:  return new IntervalMapValidater<interval_map<double,double,neutron_enricher> >;
                    case CodomainType::set_int: return new IntervalMapValidater<interval_map<double,itl::set<int> > >;
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
                    //case CodomainType::Double:  return new IntervalMapValidater<split_interval_map<int,double,neutron_enricher> >;
                    case CodomainType::set_int: return new IntervalMapValidater<split_interval_map<int,itl::set<int> > >; 
                    default: return choiceError(ITL_LOCATION("\nRootType::split_interval_map: codomainChoice:\n"),
                                                codomainChoice, _codomainChoice);
                    }
                case DomainType::Double:
                    switch(codomainChoice) {
                    case CodomainType::Int:     return new IntervalMapValidater<split_interval_map<double,int,neutron_enricher> >; 
                    //case CodomainType::Double:  return new IntervalMapValidater<split_interval_map<double,double,neutron_enricher> >;
                    case CodomainType::set_int: return new IntervalMapValidater<split_interval_map<double,itl::set<int> > >;
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


        void validateType()
        {
            _validater = chooseValidater();
            if(_validater)
            {
                _validater->validate();
                _validater->addFrequencies(_frequencies);
                _validater->addViolations(_violationsCount, _violations);
                delete _validater;
            }
        }

        void reportFrequencies()
        {
            std::cout << "------------------------------------------------------------------------------" << std::endl;
            int valid_count = 1;
            FORALL(ValidationCounterT, it, _frequencies)
            {
                printf("%3d %-66s%8d\n", valid_count, it->KEY_VALUE.c_str(), it->CONT_VALUE);
                valid_count++;
            }
            std::cout << "------------------------------------------------------------------------------" << std::endl;
            int violation_count = 1;
            FORALL(ViolationMapT, it, _violations)
            {
                printf("%3d %-66s%8d\n", violation_count, it->KEY_VALUE.c_str(), it->CONT_VALUE.getViolationsCount());
                violation_count++;
            }
            if(!_violations.empty())
                std::cout << "------------------------------------------------------------------------------" << std::endl;
            FORALL(ViolationMapT, it, _violations)
            {
                PolyLawViolations violas = it->CONT_VALUE;
                violas.reportFirst();
            }
            if(!_violations.empty())
                std::cout << "------------------------------------------------------------------------------" << std::endl;
        }

		void reportFrequencies(const std::string& filename)
        {
			FILE* fp = fopen(filename.c_str(), "w");
            int valid_count = 1;
            FORALL(ValidationCounterT, it, _frequencies)
            {
                fprintf(fp, "%3d %-66s\n", valid_count, it->KEY_VALUE.c_str());
                valid_count++;
            }
        }

        void reportTypeChoiceError(const std::string& location, int rootChoice, const ChoiceT& chooser)const
        {
            std::cout << location
                << "Type choice: " << rootChoice << " is out of range or unselectable in switch clause.\n"
                << "Expected types and their weights are:\n"
                << chooser.asString();
        }

    private:
        ChoiceT            _rootChoice;
        ChoiceT            _domainChoice;
        ChoiceT            _codomainChoice;
        AlgebraValidater*  _validater;
        ValidationCounterT _frequencies;
        ViolationCounterT  _violationsCount;
        ViolationMapT      _violations;
        bool               _isValid;
    };


}} // namespace itl boost

