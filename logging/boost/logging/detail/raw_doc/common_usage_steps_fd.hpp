namespace boost { namespace logging {

/** 
@page common_usage_steps_fd Common steps when using Formatters and destinations

The usual steps when using the Boost Logging Lib are:
- Step 1: Specify your @ref boost::logging::manipulator "formatter & destination base classes"
- Step 2: Define your logger class(es)
- Step 3: Define your filters (for instance, if you use levels, use a level::holder)
- Step 4: Declare which loggers you'll use (in a header file)
- Step 5: Define which loggers you'll use (in a source file). We need this separation
  (into Step 4 and 5, in order to make compilation times fast)
- Step 6: Define the macros through which you'll log
- Step 7: Add @ref boost::logging::manipulator "formatters and destinations". That is, how the message is to be formatted...
- Step 8: Use it
- Step 9: Enjoy the results!

*/

}}
