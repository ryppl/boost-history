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

    struct internal_logic:
        public std::exception {
        virtual const char *what () const {
            return "internal logic";
        }
    };
    struct external_logic:
        public std::exception {
        virtual const char *what () const {
            return "external logic";
        }
    };
    struct bad_size:
        public std::exception {
        virtual const char *what () const {
            return "bad size";
        }
    };
    struct bad_index:
        public std::exception {
        virtual const char *what () const {
            return "bad index";
        }
    };
    struct singular:
        public std::exception {
        virtual const char *what () const {
            return "singular";
        }
    };
    struct non_triangular:
        public std::exception {
        virtual const char *what () const {
            return "non triangular";
        }
    };
    struct non_band:
        public std::exception {
        virtual const char *what () const {
            return "non band";
        }
    };

    template<class E>
    struct check {
        static 
        NUMERICS_INLINE
        bool enabled () {
#ifdef NUMERICS_BOUNDS_CHECK
            return true;
#else // NUMERICS_BOUNDS_CHECK
            return false;
#endif // NUMERICS_BOUNDS_CHECK
        }
        static 
        NUMERICS_INLINE
        void precondition (bool expr) {
#ifdef NUMERICS_BOUNDS_CHECK
            if (! expr)
                throw E ();
#endif // NUMERICS_BOUNDS_CHECK
        }
        static 
        NUMERICS_INLINE
        void postcondition (bool expr) {
#ifdef NUMERICS_BOUNDS_CHECK
            if (! expr)
                throw E ();
#endif // NUMERICS_BOUNDS_CHECK
        }
        static 
        NUMERICS_INLINE
        void invariant (bool expr) {
#ifdef NUMERICS_BOUNDS_CHECK
            if (! expr)
                throw E ();
#endif // NUMERICS_BOUNDS_CHECK
        }
    };
}

#endif // NUMERICS_EXCEPTION_H