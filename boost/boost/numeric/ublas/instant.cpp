#include "config.h"
#include "vector.h"
#include "matrix.h"

#ifdef NUMERICS_USE_INSTANT 

namespace numerics {

    NUMERICS_INLINE
    unsigned common (size_type size1, size_type size2) {
#ifndef NUMERICS_FAST_COMMON
        check<bad_size>::precondition (size1 == size2);
        return std::min (size1, size2);
#else // NUMERICS_FAST_COMMON
        return size1;
#endif // NUMERICS_FAST_COMMON
    } 

    namespace detail {

        NUMERICS_INLINE
        float conj (const float &t) {
            return t;
        }
        NUMERICS_INLINE
        double conj (const double &t) {
            return t;
        }

        NUMERICS_INLINE
        type_traits<float>::norm_type abs (const float &t) {
#ifdef USE_MSVC
            return ::fabsf (t);
#else // USE_MSVC
            return std::fabs (t);
#endif // USE_MSVC
        }
        NUMERICS_INLINE
        type_traits<double>::norm_type abs (const double &t) {
#ifdef USE_MSVC
            return ::fabs (t);
#else // USE_MSVC
            return std::fabs (t);
#endif // USE_MSVC
        }

        NUMERICS_INLINE
        type_traits<float>::norm_type norm_1 (const float &t) {
            return abs (t);
        }
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_1 (const double &t) {
            return abs (t);
        }

        NUMERICS_INLINE
        type_traits<float>::norm_type norm_2 (const float &t) {
            return abs (t);
        }
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_2 (const double &t) {
            return abs (t);
        }

        NUMERICS_INLINE
        type_traits<float>::norm_type norm_inf (const float &t) {
            return abs (t);
        }
        NUMERICS_INLINE
        type_traits<double>::norm_type norm_inf (const double &t) {
            return abs (t);
        }

    }

}

#endif // NUMERICS_USE_INSTANT
