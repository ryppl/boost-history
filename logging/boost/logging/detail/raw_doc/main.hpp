namespace boost { namespace logging {

/** 
@page main_intro Boost Logging Library v2 : Introduction

- @ref main_motivation
- @ref main_common_usage
- @ref page_changelog 

@section main_motivation Motivation

Applications today are becoming increasingly complex. Part of making them easier to develop/maintain is to do logging. 
Logging allows you to later see what happened in your application. It can be a great help when debugging and/or testing it. 
The great thing about logging is that it can (and usually does) also happen at the customer site - if an error occurs, 
by examining the log, you can get a picture of where the problem is.

Used properly, logging is a very powerful tool. Besides aiding debugging/ testing, it can also show you 
how your application is used (which modules, etc.), how time-consuming certain parts of your program are, 
how much bandwidth you application consumes, etc. - it's up to you how much information you log, and where.

The Boost Log Library has these features: 
- A simple and clear separation of concepts 
    - concepts are also easily separated into namespaces
- A very flexible interface
- You don't pay for what you don't use.
- Fits a lot of scenarios: from very simple (dumping all to one log) 
  to very complex (multiple logs, some enabled/some not, levels, etc).
- Allows you to choose how you use logs in your code (by defining your own LOG_ macros, suiting your application)
- Allows you to use Log levels (debug, error, fatal, etc). However this is an orthogonal concept - the library
  will work whether you use levels, categories or whatever , or not.
- Efficient filtering of log messages - that is, if a log is turned off, the message is not processed at all
- Thread-safe - the library allows you several degrees of thread-safety, as you'll see
- Allows for formatters and destinations 
    - formatters format the message (like, prepending extra information - an index, the time, thread id, etc)
    - destinations specify where the message is to be written
    - Formatters and Destinations are orthogonal to the rest of the library - if you want you can use them, otherwise
      you can define your own writing mechanism
- Easy manipulation of the logs (turning on/off, setting formatters, destinations, etc)


\n\n
@section main_common_usage Common Usage

To get you started, here's the <b>most common usage</b>:
\n

@copydoc mul_levels_one_logger 

@ref scenarios_code_1 "Click to see the code"
\n\n\n

To see more examples, check out @ref common_scenarios.


\n\n\n
@section main_changelog Changelog

@ref page_changelog "See the changelog".

*/

}}
