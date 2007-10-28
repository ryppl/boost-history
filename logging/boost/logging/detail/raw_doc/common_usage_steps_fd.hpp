namespace boost { namespace logging {

/** 
@page common_usage_steps_fd Common steps when using Formatters and destinations

The usual steps when using the Boost Logging Lib are:
- Step 1: (optional) Specify your format and/or message class. By default, it's @c std::(w)string.
  You'll use this when you want an @ref "optimize string class".
- Step 2: (optional) Specify your @ref boost::logging::manipulator "formatter & destination base classes"
- Step 3: Specify your logger class(es)
- Step 4: Declare the loggers and the filters you'll use (in a header file)
- Step 5: Define the macros through which you'll log
- Step 6: Define the loggers and the filters you'll use (in a source file). We need this separation
  (into declaring and defining the logs/filters), in order to make compilation times fast.
- Step 7: Add @ref boost::logging::manipulator "formatters and destinations". That is, how the message is to be formatted...
- Step 8: Use it
- Step 9: Enjoy the results!

*/

}}
