namespace boost { namespace logging {

/** 
@page headers_to_include Headers to #include

- @ref headers_to_include_declare 
- @ref headers_to_include_define 
- @ref headers_to_include_example 

\n
@section headers_to_include_declare When declaring the logs

- when using Formatters and Destinations, or the Named Writer (an easy interface to Formatters and Destinations)

@code
#include <boost/logging/format_fwd.hpp>
@endcode

- when using Logging, without Formatters/Destinations

@code
#include <boost/logging/logging.hpp>
@endcode



\n\n
@section headers_to_include_define When defining the logs


- when using the Named Writer (an easy interface to Formatters and Destinations)

@code
#include <boost/logging/format/named_writer.hpp>
@endcode


- when using Formatters and Destinations

@code
// when defining logs and you don't log messages on a dedicated thread 
#include <boost/logging/format.hpp>

// when defining logs and you log messages on a dedicated thread (see writer::on_dedicated_thread class)
#include <boost/logging/format_ts.hpp>
@endcode


- when using tags

@code
#include <boost/logging/formatter/tags.hpp>
@endcode


- when using Logging, without Formatters/Destinations

@code
#include <boost/logging/logging.hpp>
@endcode




\n\n
@section headers_to_include_example An Example - the starter project

Note that usually you'll have a header file of your own, where you declare the logs. And a source file where you define and initialize your logs.
You can take a look at the @ref starter_project "starter project".



*/

}}
