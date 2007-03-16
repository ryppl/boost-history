// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_FINALIZER_HPP_INCLUDED
#define BOOST_IDL_DETAIL_FINALIZER_HPP_INCLUDED

namespace boost { namespace interfaces { namespace detail {   

struct finalizer_base { 
    finalizer_base() : error_(false) { }
    void set_error() const { error_ = true; }
    mutable bool error_;
};

template<typename Advice, typename Tuple>
struct finalizer : finalizer_base {
    typedef const char* cstring;
    typedef void (Advice::*after_type)(Tuple&, cstring, int, cstring*) const;
    typedef void (Advice::*finally_type)(Tuple&, cstring, int, cstring*) const;
    finalizer( const Advice* advice, after_type after, finally_type finally,
               Tuple& tuple, cstring function, int arity, cstring* params )
        : advice_(advice), after_(after), finally_(finally), 
          tuple_(tuple), function_(function),
          arity_(arity), params_(params)
        { }
    ~finalizer() 
        { 
            if (!error_)
               (advice_->*after_)(tuple_, function_, arity_, params_); 
            (advice_->*finally_)(tuple_, function_, arity_, params_); 
        }
    const Advice*  advice_;
    after_type     after_;
    finally_type   finally_;
    Tuple&         tuple_;
    cstring        function_;
    int            arity_;
    cstring*       params_;
};

} } } // End namespaces interfaces, boost.
 
#endif // #ifndef BOOST_IDL_DETAIL_FINALIZER_HPP_INCLUDED
