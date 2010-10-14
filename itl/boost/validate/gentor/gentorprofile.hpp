/*-----------------------------------------------------------------------------+    
Interval Template Library
Author: Joachim Faulhaber
Copyright (c) 2007-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#pragma once

#include <math.h>
#include <boost/validate/type/nat.hpp>
#include <boost/itl/type_traits/interval_type_default.hpp>
#include <boost/itl/interval.hpp>

namespace boost{namespace itl
{

    class GentorProfile
    {
    public:
        GentorProfile();

        void set_defaults();
        void set_debug_defaults();
        void set_release_defaults();

        void set_std_profile(int unit, int factor = 1);
        void set_polygon_profile(int max_polygon_set_size, int max_polygon_size, int min_coord, int max_coord);

        void set_range_int(int lwb, int upb) 
        { _range_int = _interval<int>::right_open(lwb, upb); }
        void set_range_nat(cnat lwb, cnat upb) 
        { _range_nat = _interval<cnat>::right_open(lwb, upb); }
        void set_range_double(double lwb, double upb) 
        { _range_double = _interval<double>::right_open(lwb, upb); }
        void set_range_ContainerSize(int lwb, int upb) 
        { _range_ContainerSize = _interval<int>::right_open(lwb, upb); }
        void set_range_interval_int(int lwb, int upb) 
        { _range_interval_int = _interval<int>::right_open(lwb, upb); }
        void set_range_interval_double(double lwb, double upb) 
        { _range_interval_double = _interval<double>::right_open(lwb, upb); }
        void set_maxIntervalLength(int val) 
        { _maxIntervalLength = val; }
        void set_range_codomain_ContainerSize(int lwb, int upb) 
        { _range_codomain_ContainerSize = _interval<int>::right_open(lwb, upb); }
        void set_repeat_count(int repeat) { _repeat_count = repeat; }
        void set_trials_count(int trials) { _trials_count = trials; }
        void set_trials_count_release(int trials) { _trials_count_release = trials; }
        void set_laws_per_cycle(int count){ _laws_per_cycle = count; }
        void set_debug_slowdown(double factor){ _debug_slowdown = factor; }

        _interval<int>::type       range_int()             { return _range_int; }
        _interval<cnat>::type      range_nat()             { return _range_nat; }
        _interval<double>::type    range_double()          { return _range_double; }
        _interval<int>::type       range_ContainerSize()   { return _range_ContainerSize; }
        _interval<int>::type       range_interval_int()    { return _range_interval_int; }
        _interval<double>::type    range_interval_double() { return _range_interval_double; }
        int                 maxIntervalLength()     { return _maxIntervalLength; }
        _interval<int>::type       range_codomain_ContainerSize()
                                                    { return _range_codomain_ContainerSize; }
        int                 repeat_count()          { return _repeat_count; }
        int                 trials_count()          { return _trials_count; }
        int                 trials_count_release()  { return _trials_count_release; }
        int                 laws_per_cycle()        { return _laws_per_cycle; }

        int                 unit()                  { return _unit; }
        int                 scaling()               { return _scaling; }

        double              debug_slowdown()const   { return _debug_slowdown; }

        int adjusted_trials_count()const;

        void report_profile();

    private:
        _interval<int>::type       _range_int;
        _interval<cnat>::type       _range_nat;
        _interval<double>::type    _range_double;
        _interval<int>::type       _range_ContainerSize;

        _interval<int>::type       _range_interval_int;
        _interval<double>::type    _range_interval_double;
        int                 _maxIntervalLength;

        _interval<int>::type       _range_codomain_ContainerSize;
        int                 _repeat_count;
        int                 _trials_count;
        int                 _trials_count_release;
        int                 _laws_per_cycle;

        double              _debug_slowdown;

        int                 _unit;
        int                 _scaling;
    };


    class GentorProfileSgl // SINGLETON PATTERN
    {
    // Singleton pattern part -----------------------------------------------------
    public:
        static GentorProfileSgl* it(); // function to call the unique instance

    protected:
        GentorProfileSgl(); // default constructor is not callable from outside
                            // preventing illegal instances

    // specific interface ---------------------------------------------------------
    public:
        void set_range_int(int lwb, int upb)           { m_profile.set_range_int(lwb, upb); }
        void set_range_nat(cnat lwb, cnat upb)         { m_profile.set_range_nat(lwb, upb); }
        void set_range_double(double lwb, double upb)  { m_profile.set_range_double(lwb, upb); }
        void set_range_ContainerSize(int lwb, int upb) { m_profile.set_range_ContainerSize(lwb, upb); }
        void set_range_interval_int(int lwb, int upb)  { m_profile.set_range_interval_int(lwb, upb); }
        void set_range_interval_double(double lwb, double upb){ m_profile.set_range_interval_double(lwb, upb); }
        void set_maxIntervalLength(int val)            { m_profile.set_maxIntervalLength(val); }
        void set_range_codomain_ContainerSize(int lwb, int upb)   
                                                       { m_profile.set_range_codomain_ContainerSize(lwb, upb); }
        void set_repeat_count(int repeat)              { m_profile.set_repeat_count(repeat); }
        void set_trials_count(int trials)              { m_profile.set_trials_count(trials); }
        void set_laws_per_cycle(int count)              { m_profile.set_laws_per_cycle(count); }

        _interval<int>::type       range_int()                { return m_profile.range_int();           }
        _interval<cnat>::type      range_nat()                { return m_profile.range_nat();           }
        _interval<double>::type    range_double()             { return m_profile.range_double();        }
        _interval<int>::type       range_ContainerSize(){ return m_profile.range_ContainerSize(); }
        _interval<int>::type       range_interval_int()       { return m_profile.range_interval_int();  }
        _interval<double>::type    range_interval_double()    { return m_profile.range_interval_double();}
        int                 maxIntervalLength()        { return m_profile.maxIntervalLength();   }
        _interval<int>::type       range_codomain_ContainerSize(){ return m_profile.range_codomain_ContainerSize(); }
        int                 repeat_count()             { return m_profile.repeat_count(); }
        int                 trials_count()             { return m_profile.trials_count(); }
        int                 laws_per_cycle()           { return m_profile.laws_per_cycle(); }

        void                report_profile()           { return m_profile.report_profile(); }

        void set_std_profile(int unit, int factor=1)   { return m_profile.set_std_profile(unit, factor); }
        void set_polygon_profile(int max_polygon_set_size, int max_polygon_size, int min_coord, int max_coord)
        { return m_profile.set_polygon_profile(max_polygon_set_size, max_polygon_size, min_coord, max_coord); }


    private:
        // Singleton pattern part -------------------------------------------------
        static GentorProfileSgl*    s_instance; // pointer to the unique instance

        // specific members -------------------------------------------------------

        // TODO add specifc members as needed
        GentorProfile m_profile;
    };

    template<typename NumberT>
    struct GentorProfileSgl_numeric_range
    {
        static typename _interval<NumberT>::type get();
    };

    template<>
    struct GentorProfileSgl_numeric_range<int>
    {
        static _interval<int>::type get() 
        { return GentorProfileSgl::it()->range_int(); }
    };

    template<>
    struct GentorProfileSgl_numeric_range<unsigned int>
    {
        static _interval<unsigned int>::type get() 
        {
            _interval<cnat>::type inter_val = GentorProfileSgl::it()->range_nat();
            return _interval<unsigned int>::right_open(inter_val.lower(), inter_val.upper());
        }
    };

    template<>
    struct GentorProfileSgl_numeric_range<cnat>
    {
        static _interval<cnat>::type get() 
        { return GentorProfileSgl::it()->range_nat(); }
    };

    template<>
    struct GentorProfileSgl_numeric_range<double>
    {
        static _interval<double>::type get() 
        { return GentorProfileSgl::it()->range_double(); }
    };

}} // namespace itl boost
