/*



FIXME change the workflow & logger's definition - it doesn't need a filter, but it can have one!




scenarios:
remaining scenarios
- one logger; several levels - use a sink (that is, see how we find "is_enabled")
  - we can simply use the level.is_enabled(xxx) question, and then write to the log
- no levels
- fastest , no <<
- fastest , using <<
- using your own formatters and destinations integrated

--> doxygen_web -> copy from doxygen, once all is done, and examples finally work

------> FIXME after showing scenarios, show customizing?







about process_msg
about macros - macros.hpp -> have specific page
about non_const_context

about formatting
about thread-safety


writer namespace , etc.
compilers


convert- also explain that you can convert from str x to y; for instance write_time can actually append the time (instead of prepending it - default)!

formatters - most of them don't need thread-safety, destinations usually need . Explain why/how/when




*/