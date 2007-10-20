/*
FIXME change the workflow & logger's definition - it doesn't need a filter, but it can have one!




scenarios:
have 3 scenarios
- several levels, same log
- several levels, different logs
- with custom router FIXME (that is, use cache_string_several_str())
- one logger; several levels - use a sink (that is, see how we find "is_enabled")
  - we can simply use the level.is_enabled(xxx) question, and then write to the log
- no levels
- etc
- fastest , no <<
- fastest , using <<
- using your own formatters and destinations integrated


FIXME after showing scenarios, show customizing?







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