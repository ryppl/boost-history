#ifndef TEMPLATE_PROFILER_HPP_INLUCDED
#define TEMPLATE_PROFILER_HPP_INLUCDED

namespace template_profiler {
    struct incomplete;
}

#ifdef PROFILE_TEMPLATES

#if defined(_MSC_VER)
    #define PROFILE_TRACER() enum template_profiler_test { template_profiler_value = sizeof(delete ((::template_profiler::incomplete*)0),0) };
#elif defined(__GNUC__)
    #define PROFILE_TRACER() struct template_profiler_test { int f() { int template_profiler_value = 1; return template_profiler_value/0; } };
#else
    #error Unknown compiler
#endif

#else

    #define PROFILE_TRACER()

#endif

#endif
