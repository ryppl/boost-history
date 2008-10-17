#ifndef TEMPLATE_PROFILER_HPP_INLUCDED
#define TEMPLATE_PROFILER_HPP_INLUCDED

namespace template_profiler {
    struct incomplete;
    template<int N>
    struct int_ {
        enum { value = N };
        typedef int type;
    };
    template<class T>
    struct make_zero {
        enum { value = 0 };
    };
    extern int value;
}

char template_profiler_size_one(...);

#ifdef PROFILE_TEMPLATES

#if defined(_MSC_VER)
    #define PROFILE_TRACER() enum template_profiler_test { template_profiler_value = sizeof(delete ((::template_profiler::incomplete*)0),0) };
#elif defined(__GNUC__)
    #if (__GNUC__ < 4) || (__GNUC_MINOR__ < 3)
        #define PROFILE_TRACER()\
        struct template_profiler_test {};\
        enum {template_profiler_size = sizeof(template_profiler_size_one(static_cast<template_profiler_test*>(0))) };\
        typedef ::template_profiler::int_<\
            sizeof(\
                ::template_profiler::value /\
                ::template_profiler::make_zero<\
                    ::template_profiler::int_<\
                        template_profiler_size\
                    >\
                >::value\
            )\
        > template_profiler_test_result;
    #else
        namespace template_profiler {
            __attribute__((deprecated)) int f(int);
            int f(double);
        }
        #define PROFILE_TRACER() enum { template_profiler_size = sizeof(::template_profiler::f(1)) };
    #endif
#else
    #error Unknown compiler
#endif

#else

    #define PROFILE_TRACER()

#endif

#endif
