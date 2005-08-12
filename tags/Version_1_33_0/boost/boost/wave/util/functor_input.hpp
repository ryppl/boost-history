/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library

    http://www.boost.org/

    Copyright (c) 2001-2005 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(FUNCTOR_INPUT_HPP_ED3A4C21_8F8A_453F_B438_08214FAC106A_INCLUDED)
#define FUNCTOR_INPUT_HPP_ED3A4C21_8F8A_453F_B438_08214FAC106A_INCLUDED

#include <boost/assert.hpp>
#include <boost/spirit/iterator/multi_pass.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {
namespace wave {
namespace util {

///////////////////////////////////////////////////////////////////////////////
// 
//  class functor_input
// 
//      Implementation of the InputPolicy used by multi_pass 
//      functor_input gets tokens from a functor
//      Note: the functor must have a typedef for result_type
//            It also must have a static variable of type result_type defined 
//            to represent eof that is called eof.
//
//      This functor input policy template is essentially the same as the 
//      predefined multi_pass functor_input policy. The difference is, 
//      that the first token is not read at initialization time, but only 
//      just before returning the first token. 
//
///////////////////////////////////////////////////////////////////////////////
struct functor_input {

    template <typename FunctorT>
    class inner {
    private:
        typedef typename FunctorT::result_type result_type;

    public:
        typedef result_type value_type;

    private:
        struct Data {
            Data(FunctorT const &ftor_) 
            :   ftor(ftor_), was_initialized(false)
            {}
            
            FunctorT ftor;
            value_type curtok;
            bool was_initialized;
        };
        
    public:
        typedef std::ptrdiff_t difference_type;
        typedef result_type *pointer;
        typedef result_type &reference;

    protected:
        inner()
        :   data(0)
        {}

        inner(FunctorT const &x)
        :   data(new Data(x))
        {}

        inner(inner const &x)
        :   data(x.data)
        {}

        void destroy()
        {
            delete data;
            data = 0;
        }

        bool same_input(inner const &x) const
        {
            return data == x.data;
        }

        void swap(inner &x)
        {
            boost::spirit::impl::mp_swap(data, x.data);
        }
        
        void ensure_initialized() const
        {
            if (data && !data->was_initialized) {
                data->curtok = (data->ftor)();    // get the first token
                data->was_initialized = true;
            }
        }
        
    public:
        reference get_input() const
        {
            ensure_initialized();
            return data->curtok;
        }

        void advance_input()
        {
            BOOST_ASSERT(0 != data);
            data->curtok = (data->ftor)();
            data->was_initialized = true;
        }

        bool input_at_eof() const
        {
            ensure_initialized();
            return !data || data->curtok == data->ftor.eof;
        }

        FunctorT& get_functor() const
        {
            BOOST_ASSERT(0 != data);
            return data->ftor;
        }

    private:
        mutable Data *data;
    };
};

///////////////////////////////////////////////////////////////////////////////
}   // namespace util
}   // namespace wave
}   // namespace boost 

#endif // !defined(FUNCTOR_INPUT_HPP_ED3A4C21_8F8A_453F_B438_08214FAC106A_INCLUDED)
