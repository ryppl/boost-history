//  
//  Copyright (c) 2000-2001
//  Joerg Walter, Mathias Koch
//  
//  Permission to use, copy, modify, distribute and sell this software
//  and its documentation for any purpose is hereby granted without fee,
//  provided that the above copyright notice appear in all copies and
//  that both that copyright notice and this permission notice appear
//  in supporting documentation.  The authors make no representations
//  about the suitability of this software for any purpose.  
//  It is provided "as is" without express or implied warranty.
//  
//  The authors gratefully acknowledge the support of 
//	GeNeSys mbH & Co. KG in producing this work.
//

#ifndef NUMERICS_EXCEPTION_H
#define NUMERICS_EXCEPTION_H

#include <exception>

#include "config.h"

namespace numerics {

    struct divide_by_zero:
        public std::exception {
        virtual const char *what () const throw () {
            return "exception: divide by zero";
        }
    };
    struct internal_logic:
        public std::exception {
        virtual const char *what () const throw () {
            return "exception: internal logic";
        }
    };
    struct external_logic:
        public std::exception {
        virtual const char *what () const throw () {
            return "exception: external logic";
        }
    };
    struct bad_size:
        public std::exception {
        virtual const char *what () const throw () {
            return "exception: bad size";
        }
    };
    struct bad_index:
        public std::exception {
        virtual const char *what () const throw () {
            return "exception: bad index";
        }
    };
    struct singular:
        public std::exception {
        virtual const char *what () const throw () {
            return "exception: singular";
        }
    };
    struct non_triangular:
        public std::exception {
        virtual const char *what () const throw () {
            return "exception: non triangular";
        }
    };
    struct non_band:
        public std::exception {
        virtual const char *what () const throw () {
            return "exception: non band";
        }
    };

#ifdef NUMERICS_BOUNDS_CHECK
    template<class E>
    NUMERICS_INLINE
    void check (bool expression, const E &e) {
        if (! expression)
            throw e;
    }
#else
//    template<class E>
//    NUMERICS_INLINE
//    void check (bool expression, const E &e) {}
// FIXME: this one is too agressive for MSVC?!
#define check(expression, e)
#endif
    
}

#endif 


