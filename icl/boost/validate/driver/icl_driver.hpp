/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_VALIDATE_DRIVER_ICL_DRIVER_HPP_JOFA_090303
#define BOOST_ICL_VALIDATE_DRIVER_ICL_DRIVER_HPP_JOFA_090303

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <boost/thread.hpp>
#include <boost/validate/validater/concept_validater.hpp>
#include <boost/validate/utility.hpp>


#ifdef BOOST_MSVC 
#pragma warning(push)
#pragma warning(disable:4996) // 'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#endif                        

namespace boost{namespace icl
{
    namespace RootType 
    {
        enum RootTypes 
        { 
            icl_set, interval_set, separate_interval_set, split_interval_set, 
            icl_map, interval_map, split_interval_map, 
            Types_size 
        };
    }

    namespace DomainType 
    {
        enum DomainTypes { Int, Double, DomainTypes_size };
    }

    namespace CodomainType 
    {
        enum CodomainTypes { Nat, Int, Double, set_int, raw_bitset, CodomainTypes_size };
    }

    namespace IdentityHandlerType 
    {
        enum IdentityHandlerTypes { partial_absorber, partial_enricher, total_absorber, total_enricher, IdentityHandlerTypes_size };
    }

    namespace FreeChoice 
    {
        enum FreeChoice { _1, _2, _3, _4, _5, FreeChoice_size };
    }


    namespace inform
    {
        enum informs { never=0, rarely, frequently };
    }


    class icl_driver
    {
    public:
        icl_driver()
            : _required_law_validation_count(0)
            , _required_law_count(0)
            , _info_level(inform::frequently)
        {
            _laws_per_cycle = GentorProfileSgl::it()->laws_per_cycle();
        }

        bool hasValidProfile()const { return _isValid; }

        virtual void setProfile() = 0;
        virtual concept_validater* chooseValidater() = 0;

        void require_validation_count(int count){ _required_law_validation_count = count; }
        int  required_validation_count()const   { return _required_law_validation_count;  }

        void require_law_count(int count){ _required_law_count = count; }
        int  required_law_count()const   { return _required_law_count;  }

        void terminate_at_law_count(int law_count, int instance_count = 1)
        { 
            require_law_count(law_count); 
            require_validation_count(instance_count); 
        }

        void set_information_level(int inform){ _info_level = inform; }

        bool validate()
        {
            srand(static_cast<unsigned>(time(NULL))); //seed time: different numbers each run 
            //srand(static_cast<unsigned>(1)); //seed 1: same numbers each run (std)
            //srand(static_cast<unsigned>(4711)); //seed var: same numbers every run (varying)

            for(int idx=0; !terminates(); idx++)
            {
                if(idx>0 && idx % _laws_per_cycle == 0)
                    if(_info_level == inform::frequently)
                        reportFrequencies();
                validateType();
            }

            if(_info_level >= inform::rarely)
                reportFrequencies();

            return icl::is_empty(_violationsCount);
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

        int least_law_validation_count()const
        {
            // The least count of validation cycles performed on a single law instance.
            int min_test_count = 9999;
            
            ICL_const_FORALL(ValidationCounterT, it, _frequencies)
                min_test_count = min_test_count < it->second.count() ?
                                 min_test_count : it->second.count() ;

            return min_test_count;
        }

        void reportFrequencies()
        {
			extern boost::mutex g_Mutex; //JODO URG REV ...
			g_Mutex.lock();
            std::cout << "------------------------------------------------------------------------------" << std::endl;
            std::cout << "--- Successfully tested law instantiation -------------------------runs---time" << std::endl;
            int valid_count = 1;
            double avg_evaluation_time = 0.0;
            long   instance_count      = 0;
            ICL_FORALL(ValidationCounterT, it, _frequencies)
            {
                long law_validation_count = it->second.count();
                double avg_law_evaluation_time = 
                    it->second.time()/(law_validation_count);
                printf("%3d %-58s%9ld%7.0lf\n", 
                    valid_count, it->first.c_str(), law_validation_count, avg_law_evaluation_time);

                avg_evaluation_time += avg_law_evaluation_time;
                instance_count      += law_validation_count;
                valid_count++;
            }

            std::cout << "------------------------------------------------------------------------------" << std::endl;
            // Summary for the current cycle
            double avg_evaluation_time_per_law = avg_evaluation_time/icl::size(_frequencies);
            printf( " %10.3lf%-53s%7ld%7.0lf\n", 
                    avg_evaluation_time_per_law, " total avg of atomic evaluation (micro sec)", instance_count, avg_evaluation_time_per_law);

            int violation_count = 1;
            if(!icl::is_empty(_violations))
            {
                std::cout << "------------------------------------------------------------------------------" << std::endl;
                std::cout << "--- Law violations -----------------------------------------------count-------" << std::endl;
            }
            ICL_FORALL(ViolationMapT, it, _violations)
            {
                printf("%3d %-59s%8d\n", violation_count, it->first.c_str(), it->second.getViolationsCount());
                violation_count++;
            }
            if(!icl::is_empty(_violations))
                std::cout << "------------------------------------------------------------------------------" << std::endl;
            ICL_FORALL(ViolationMapT, it, _violations)
            {
                PolyLawViolations violas = it->second;
                violas.reportFirst();
            }
            if(!icl::is_empty(_violations))
                std::cout << "------------------------------------------------------------------------------" << std::endl;
			g_Mutex.unlock();
        }

        void reportFrequencies(const std::string& filename)
        {
            FILE* fp = fopen(filename.c_str(), "w");
            int valid_count = 1;
            ICL_FORALL(ValidationCounterT, it, _frequencies)
            {
                fprintf(fp, "%3d %-66s\n", valid_count, it->first.c_str());
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


    protected:
        void setValid(bool truth) 
        { 
            _isValid = truth;
        }

        void setRootTypeNames()
        {
            std::vector<std::string> type_names(RootType::Types_size);
            type_names[RootType::icl_set]               = "icl_set"; 
            type_names[RootType::interval_set]          = "interval_set"; 
            type_names[RootType::separate_interval_set] = "separate_interval_set"; 
            type_names[RootType::split_interval_set]    = "split_interval_set"; 
            type_names[RootType::icl_map]               = "icl_map"; 
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
            type_names[CodomainType::raw_bitset]        = "raw_bitset"; 
            _codomainChoice.setTypeNames(type_names);
        }
        void setIdentityHandlerTypeNames()
        {
            std::vector<std::string> type_names(IdentityHandlerType::IdentityHandlerTypes_size);
            type_names[IdentityHandlerType::partial_absorber]        = "partial_absorber"; 
            type_names[IdentityHandlerType::partial_enricher]        = "partial_enricher"; 
            type_names[IdentityHandlerType::total_absorber]          = "total_absorber"; 
            type_names[IdentityHandlerType::total_enricher]          = "total_enricher"; 
            _identityHandlerChoice.setTypeNames(type_names);
        }
        void setFreeChoiceNames()
        {
            std::vector<std::string> type_names(FreeChoice::FreeChoice_size);
            type_names[FreeChoice::_1]        = "_1"; 
            type_names[FreeChoice::_2]        = "_2"; 
            type_names[FreeChoice::_3]        = "_3"; 
            type_names[FreeChoice::_4]        = "_4"; 
            _codomainChoice.setTypeNames(type_names);
        }

        concept_validater* choiceError(const std::string& location, int value, const ChoiceT& choice)
        {
            reportTypeChoiceError(location, value, choice); 
            setValid(false);
            return NULL; 
        }

    private:
        bool terminates()const
        {
            if(!hasValidProfile())
                return true;
            else if(_required_law_count == 0 || _required_law_validation_count == 0)
                return false; // If counts are not limited: Run for ever.
            else if(icl::size(_frequencies) < static_cast<size_t>(_required_law_count))
                return false; // Not yet reached all laws
            else
                // All laws reached. Enough validation cycles for every law?
                return _required_law_validation_count <= least_law_validation_count();
        }

    protected:
        ChoiceT            _rootChoice;
        ChoiceT            _domainChoice;
        ChoiceT            _codomainChoice;
        ChoiceT            _identityHandlerChoice;
        ChoiceT            _freeChoice;

    private:
        concept_validater* _validater;
        ValidationCounterT _frequencies;
        ViolationCounterT  _violationsCount;
        ViolationMapT      _violations;
        bool               _isValid;

        int _laws_per_cycle; // After _laws_per_cycle times a cycle is
                             // done and times and frequencies of law 
                             // validations are reported for all instances.

        int _required_law_validation_count;
        int _required_law_count;
        int _info_level;
    };


}} // namespace icl boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // BOOST_ICL_VALIDATE_DRIVER_ICL_DRIVER_HPP_JOFA_090303


