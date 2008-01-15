namespace boost { namespace logging {

/** 
@page page_todo TODOs

Types of priorities:
- must_have - this is a must have
- high - high priority
- normal - normal priority
- low - nice to have


- @ref todo_implementation
- @ref todo_docs
- @ref todo_formatters 
- @ref todo_destinations 

Lots of things on my plate ;) \n
If you want to make sure a feature is implemented sooner, drop me a note: http://torjo.com/contact.html



@section todo_implementation Implementation

- @c normal         on_dedicated_thead - remove dependency on boost::thread

- @c high           What should I do if someone is trying to use the log after it's been destroyed?

- @c high           Provide a Jamfile to build the examples/tests

- @c high           test TSS on vs2003 and gcc/pthreads \n
  (note: tested on pthreads; however - about internal implementation : 2 TSS objects are leaked on pthreads, need to see why)

- @c must_have      if *used* before main, it'll cache the messages, and then write them ASAP; \n
    will have a func - like, do_write() - which will write all that was cached, and then turn cache off \n
    ----- i have to see if reasonable to later ask if a log is enabled or not, or just dump all messages written before initializing the log

- @c normal         allow to see how much time logging actually takes \n
  This should work even if logging happens on a dedicated thread. Give a name to your tracker - so you can track logging time when messages are logged,
  and time logging actually happens (on dedicated thread).
  I should feature this on the documentation - I don't think any other lib has this! I should be able to add this for filter as well.

- @c low            configuration :  should allow the library to be configured at runtime, from a file, much like log4j
  - Votes : 1

- @c low      hierarchical logs : just like log4j - be able to have some logs inherit from parent logs
  I can implement this in multiple ways - one would be to use function names (BOOST_CURRENT_FUNCTION)
  and see the namespace/class name from there. Or, I could implement it using __FILE__.
  - Votes: 1

- @c high          TSS ostringstream:  I can have a tss(std::ostringstream), when gathering data if creation of std::ostringstream takes time.
  this does not need to apply to ostringstream only - it should apply to any gather::ostream_like class
  - Votes: 1

- @c low            have a "class"/"function" filter \n - so basically you can have is_enabled(this), which will call
  is_enabled<some_type>, which you can later on, or at runtime, enable or disable.
  --> turn on/off based on class name (typeid(name).string() - could be problematic in case the names are cryptic.
  What i can do is register some classes - with their name, so that you have a one-to-one corresp.
  you can even enforce a class to register its name TOTHINK \n
  note: this can be viewed as a hierarchical filter. In the same way, I could create a hierarchical logger (that is, find the logger, 
  based on __FILE__ or __FUNCTION__)
  \n Implementation details: \n
  have a filter based on file name (the __FILE__) - should use TSS. comparing is very simple and efficient.
  std::map<const char*,bool, char_cmp>; \n
  std::map<std::string,bool> enabled; \n
  -- the idea is that a __FILE__ is a const char* and will always have the same (pointer) value. So once I know if it's enabled or not,
  it'll always be like that. once specifically turned on/off - we update the 2 maps

- @c low            composite_filter \n I want to allow an and_filter = filter1 && filter2, and an or_filter = filter1 || filter2.
  to see how I can find out if such a filter is enabled or not - for instance, receive 2 args = pass 1 to filter1 and 2 to filter2;
  or if get only one arg, you need to specify to whom to pass to

- @c low            Design: logged assert. I believe log library needs to provide support for custom assert. 
  Such assert would log into log file and invoke system assert depends in assert level. It could be an addon to core functionality. \n
  Note: SMART_ASSERT should be used for this.

- @c normal           Allow using log from DLL and EXE (on Windows). Same for Linux. This should normally work - just need to explain how.


@section todo_formatters Formatters


- @c low    new formatter: limit the text to gives maximal length. It may happend the dump is unexpectedly large and would
  fill disk/overwrite cache/take too much time/be unreadable. Limiting one log size would help here a lot and would relieve end user of doing it manually.

- @c normal           quite often I am not interested not in absolute time (2005/12/11 16:33) but in time offset from some event. 
  I suggest to add API: \n
  some_formatter.set_time_start_point( bool either_in_all_threads_or_just_in_current_thread); \n
  and escape sequences: \n
  $Xms   - e.g. "12 ms", "78321 ms" (only milliseconds are used) \n
  $Xsec  - e.g. "2 s, 23 ms" (only seconds and millis) \n
  $Xmin  - e.g. "3 m, 21 s, 10 ms" \n
  $Xhour - e.g. "1 h, 33 m, 20 s, 54 ms" \n
  $Xday  - e.g. "2 d, 1 h, 33 m, 20 s, 54 ms" \n

- @c normal           new formatter: thread ID (both Win32 and POSIX) is of low use. I suggest to add API to specify string name as alternative to thread ID 
   formatter.set_current_thread_name("gui thread");

- @c normal           Time string cashing time and time_strf calls are not cashed. It's a lot of work for nothing. Note: should use TSS

- @c normal         Design: hexdump. I believe log library needs to provide support for dumping memory in hex format on log file. 


@section todo_destinations Destinations

- @c normal         new destination : to Event log

- @c normal         shared_memory is to use interprocess lib.



@section todo_docs Documentation

- @c low            add "for documentation, see http://www.boost.org/lib/logging/" to each file

- @c low            have a document with differences from v1 and v2
  - Votes: 1

- @c normal         explain about cachestring clases

- @c normal         about is_generic formatters/destinations (note that I might have already written about this)

- @c low            Nice to have docs wish: a page discussing stranghts of this logger vs other common ones (log4cpp etc).

- @c normal         Documentation about performance of the library is missing - absolute times, # of dynamic allocations per typical log, etc.

- @c high           For formatters/destinations - make it easier to see - not necessary from examples.
                    By looking at the namespace itself, it contains other stuff as well. See if I can use @ ingroup or something

- @c normal         Explain about config files - you can use named_spacer,named.

- @c normal         explain about gather class - how to define (BOOST_LOG_GATHER_CLASS)
*/

}}
