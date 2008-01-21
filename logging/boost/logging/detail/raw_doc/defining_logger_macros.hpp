namespace boost { namespace logging {

/** 
@page defining_logger_macros Defining macros to do logging

Now that you've @ref defining_your_logger_filter "declared and defined your loggers/filters", it's time to log messages in code.

You'll have to define macros to log messages in code. Such a macro needs to do the following:
- first, see if logger is enabled
  - if so, do log the message
  - else, ignore the message completely

To see if the logger is enabled, you'll use a filter. Note that you can reuse the same filter for multiple loggers, in case you want to.

To define a macro to logmessages in code, I've provided these few macros to help you:
-# BOOST_LOG_USE_LOG_IF_FILTER(logger, filter_is_enabled)
-# BOOST_LOG_USE_LOG_IF_LEVEL(logger, filter, level)
-# BOOST_LOG_USE_SIMPLE_LOG_IF_FILTER(logger, filter_is_enabled)

The parameters are:
- logger - a pointer to a logger you've declared
- filter_is_enabled - how to determine if the filter is enabled. 
  - For simple filters this can be a simple <tt>filter()->is_enabled()</tt>
  - More complex filters can be passed additional information (as you'll see below)
  - As an example of filter that needs an extra argument, we have: \n
    <tt>BOOST_LOG_USE_LOG_IF_LEVEL(l, holder, the_level) = BOOST_LOG_USE_LOG_IF_FILTER(l, holder->is_enabled(the_level)) </tt>
- level - in case you have a filter based on level

How you define your macros is up to you. Assuming you have a filter based on levels, you can:
- either define an <tt>L_(level)</tt> macro, which will care about the level or,
- define an <tt>L..._</tt> macro for each level you want (for instance, LDBG_, LAPP_, LERR_).

Here's how you'd do each of the above:

@code
typedef logger_format_write< > logger_type;
typedef level::holder filter_type;

BOOST_DECLARE_LOG_FILTER(g_log_level, filter_type ) 
BOOST_DECLARE_LOG(g_l, logger_type) 

// Example 1 - have one macro depending on one parameter
#define L_(lvl) BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_log_level(), lvl )


// Example 2
#define LDBG_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_log_level(), debug )
#define LERR_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_log_level(), error )
#define LAPP_ BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_log_level(), info )

@endcode

\n
Using it in code:

@code
// Example 1
L_(debug) << "this is a debug message " << i++;
L_(info)  << "this is an info " << i++;
L_(error) << "this is an error " << i++;



// Example 2
LDBG_ << "this is a debug message " << i++;
LAPP_ << "this is an info " << i++;
LERR_ << "this is an error " << i++;
@endcode

\n\n
Another example. Assume you have a @c file_filter class, which filters based on file name (a file can be turned on or off):

@code
struct file_filter {
    bool is_enabled(const char * file_name) const ; 
    // ...
};
@endcode

In this case, the macro used to do logging would look like:

@code
typedef logger_format_write< > logger_type;
typedef file_filter filter_type;

BOOST_DECLARE_LOG_FILTER(g_log_filter, filter_type ) 
BOOST_DECLARE_LOG(g_l, logger_type) 

#define L_ BOOST_LOG_USE_LOG_IF_FILTER(g_l(), g_log_filter()->is_enabled(__FILE__) )
@endcode

\n
Using it in code:

@code
L_ << "this is a message " << i++;
L_ << "hello world";
@endcode


\n\n
Another example. Assume you have a @c module_filter class, which filters based on module name (logging for a module can be turne on or off) :

@code
struct module_filter {
    bool is_enabled(const char * module_name) const ; 
    // ...
};
@endcode


In this case, the macro used to do logging would look like:

@code
typedef logger_format_write< > logger_type;
typedef module_filter filter_type;

BOOST_DECLARE_LOG_FILTER(g_log_filter, filter_type ) 
BOOST_DECLARE_LOG(g_l, logger_type) 

#define L_(mod) BOOST_LOG_USE_LOG_IF_FILTER(g_l(), g_log_filter()->is_enabled(mod) )
@endcode

\n
Using it in code:

@code
L_("chart") << "activex loaded";
L_("connect") << "connecting to server";
@endcode



*/

}}
