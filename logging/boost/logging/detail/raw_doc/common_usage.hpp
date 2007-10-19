namespace boost { namespace logging {

/** 
@page common_usage Scenario 1: Common Usage

define the class_ - with @param

- with levels
- write to different thingies
- etc

Explain about samples!



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

*/

}}
