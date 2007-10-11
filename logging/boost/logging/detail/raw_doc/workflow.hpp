namespace boost { namespace logging {

/** 
@page workflow Logging workflow


- @ref workflow_introduction 
- @ref workflow_filter 
- @ref workflow_processing 
- @ref workflow_2a 
- @ref workflow_2b 




@section workflow_introduction Introduction

So, what happens when a message is written to the log?

First, you have a logger you write to. @b Every logger contains 2 things:
- a filter, which indicates if the log is turned on or off
- a processor, which will process the message, in case the log is turned on.

Whenever you create a logger, you specify the filter and the processor as template parameters:

@code
// filter is optional - there is a default 
logger< the_processor, the_filter> g_l;
@endcode

Example:

@code
using namespace boost::logging;
logger<write_to_cout, filter::no_ts> g_log;
@endcode



@section workflow_filter Step 1: Filtering the message

So, first time the message is filtered. The filter class only needs to provide the @c is_enabled function.
Then, the logger provides 2 helpers:
- operator bool()
- operator !

Thus, in your code, you can easily find out if a logger is enabled or not:

@code
logger<write_to_cout, filter::no_ts> g_log;

// code
if ( g_log) do_something(g_log);

// or
if ( !g_log) ; else do_something...
@endcode

Usually, you won't write code like above - instead, you'll hide all the above with @ref macros "macros".

Anyway, the end result is that logging is efficient - processing the message happens <b>only if</b> the filter is enabled.
Otherwise, processing is @em completely ignored.



@section workflow_processing Step 2: Processing the message

So, once we've established that the logger is enabled, we'll @em process the message.
Processing means whatever your application thinks logging means.

This can be as simple as dumping the message to cout or whatever.

Example:

@code
struct write_to_cout {
    void operator()(const std::string & msg) const {
        std::cout << msg << std::endl ;
    }
};

using namespace boost::logging;
logger<write_to_cout, filter::no_ts> g_single_log;

#define L_(x) if ( g_single_log) g_single_log.process_msg()(x)

// usage
L_("reading word " + word);
@endcode

As you can see, processing the message means having a functor which implements operator(). You can be happy with something similar to the above. Or...

If you think about it, you can actually divide this step, into 2 smaller steps:
- gathering the message
- writing the message



\n\n
@section workflow_2a Step 2A: Gathering the message

The meaning of "gathering the message" dependends on your application. The message can:
- be a simple string,
- it can contain extra info, like: level, category, etc.
- it can be written all at once, or using the cool "<<" operator
- or any combination of the above

Depending on your needs, gathering can be complex or not. However, it's completely decoupled from the other steps.
Gathering goes hand in hand with @ref macros "macros".

The cool thing is that you decide how the <i>Logging syntax</i> is - depending on how you want to gather the message.
All of the below are viable options:

@code
L_("reading " + word);
L_ << "this " << " is " << "cool";
L_(dbg) << "happily debugging";
L_(err,"chart")("Cannot load chart")(chart_path);
@endcode

How you gather your message, depends on how you <tt>\#define L_ ...</tt>.

So, in other words, gathering the message means getting all the message in "one piece", so that it can further be written. 




\n\n
@section workflow_2b Step 2B: Writing the message

Now that you have the message, you're ready to write it. Again, writing is done by calling operator() on the writer object.

What you choose as the writer object is completely up to you. It can be as simple as this:

@code
// dump message to cout

struct write_to_cout {
    void operator()(const std::string & msg) const {
        std::cout << msg << std::endl ;
    }
};

typedef process_msg< gather::ostream_like::return_str<>, write_to_cout> processor;
logger<processor, filter::no_ts> g_single_log;

#define L_ if ( !g_single_log) ; else g_single_log->read_msg().gather().out()

// usage
L_ << idx << " : reading word " << word;
@endcode

You can define your own types of writers. At this time, the %writer classes that come with this library are in <tt>namespace writer</tt>.

At this time, I've defined the concept of writer::format_write - writing using Formatters and Destinations.
Simply put, this means formatting the message, and then writing it to destination(s).

For each log, you decide where how messages are formatted, and to what destinations they are written. Example:

@code
typedef process_msg< gather::ostream_like::return_cache_str<> , 
    format_write< format_base, destination_base, format_and_write::simple<cache_string> > > process;
logger<process, filter::no_ts> g_l;

#define L_ if ( !g_l) ; else g_l->read_msg().gather().out()

// add formatters : [idx] [time] message [enter]
g_l->writer().add_formatter( write_idx() );
g_l->writer().add_formatter( write_time() );
g_l->writer().add_formatter( append_enter() );

// write to cout and file
g_l->writer().add_destination( write_to_cout() );
g_l->writer().add_destination( write_to_file("out.txt") );


// usage
int i = 1;
L_ << "testing " << i++;

// the above message will be formatted like this : "[1] 22:30 testing 1\n", 
// and will be dumped to cout and "out.txt" file.
@endcode






*/

}}
