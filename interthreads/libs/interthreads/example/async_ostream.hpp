#ifndef BOOST_INTERTHREADS_ASYNC_OSTREAM_HPP
#define BOOST_INTERTHREADS_ASYNC_OSTREAM_HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
//  copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include "./detail/async_ostream_sink.hpp"

#include <boost/iostreams/categories.hpp>   // iostreams::sink_tag
#include <boost/iostreams/stream.hpp>       // iostreams::stream

#include <iosfwd>                           // std::streamsize
#include <iostream>                         // std::ostream


namespace boost {
namespace interthreads {
    class async_ostream : public iostreams::stream<detail::async_ostream_sink> {
    public:
        typedef iostreams::stream<detail::async_ostream_sink> base_type;
        typedef char            char_type;
        typedef iostreams::sink_tag    category;

        async_ostream(std::ostream& os);
        void flush();

        static void thread_specific_setup();
    };
    
    extern async_ostream cout_;
}
}

#endif

