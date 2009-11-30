/*-----------------------------------------------------------------------------+    
A Law Based Test Automaton 'LaBatea'
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_VALIDATE_DRIVER_SORTED_ASSOCIATIVE_BITSET_DRIVER_HPP_JOFA_091126
#define BOOST_VALIDATE_DRIVER_SORTED_ASSOCIATIVE_BITSET_DRIVER_HPP_JOFA_091126

#include <iostream>
#include <stdio.h>
#include <boost/validate/validater/sorted_associative_validater.hpp>
#include <boost/validate/driver/itl_driver.hpp>
#include <boost/validate/utility.hpp>

namespace boost{namespace itl
{
    
    class sorted_associative_bitset_driver : public itl_driver
    {
    public:
        sorted_associative_bitset_driver() { setProfile(); }

        void setProfile()
        {
            setValid(true);
            _freeChoice.setSize(FreeChoice::FreeChoice_size);
            _freeChoice.setMaxWeights(100);
            _freeChoice[FreeChoice::_1] = 20;
            _freeChoice[FreeChoice::_2] = 20;
            _freeChoice[FreeChoice::_3] = 20;
            _freeChoice[FreeChoice::_4] = 20;
            _freeChoice[FreeChoice::_5] = 20;
            setRootTypeNames();
            _freeChoice.init();

            if(!_freeChoice.is_consistent())
            {
                setValid(false);
                std::cout << _freeChoice.inconsitencyMessage("interval_bitset_driver::setProfile()") << std::endl;
            }
        }

        algebra_validater* chooseValidater()
        {
            int freeChoice         = _freeChoice.some();

            switch(freeChoice)
            {
            case FreeChoice::_1:
                return new sorted_associative_validater< interval_bitset<int, bits<unsigned char> >,  itl::list<int> >; 
            case FreeChoice::_2:
                return new sorted_associative_validater< interval_bitset<int, bits<unsigned short> >, itl::list<int> >; 
            case FreeChoice::_3:
                return new sorted_associative_validater< interval_bitset<int, bits<unsigned int> >,   itl::list<int> >; 
            case FreeChoice::_4:
                return new sorted_associative_validater< interval_bitset<int, bits<unsigned long> >,  itl::list<int> >; 
            case FreeChoice::_5:
                return new sorted_associative_validater< interval_bitset<int, bits<unsigned long long> >, itl::list<int> >; 
            default: return choiceError(ITL_LOCATION("freeChoice:\n"), freeChoice, _freeChoice);
            } //switch()

            return NULL; //just to please the compiler ;)
        }
    };

}} // namespace itl boost

#endif // BOOST_VALIDATE_DRIVER_SORTED_ASSOCIATIVE_BITSET_DRIVER_HPP_JOFA_091126
