namespace boost { namespace logging {

/** 
@page caching Caching messages before logs are initialized

- @ref caching_why 
- @ref caching_BOOST_LOG_BEFORE_INIT_CACHE_FILTER 
- @ref caching_BOOST_LOG_BEFORE_INIT_LOG_ALL 
- @ref caching_BOOST_LOG_BEFORE_INIT_IGNORE_BEFORE_INIT 
- @ref caching_ts 

@section caching_why Caching - why is it needed?

Logging is all fine and dandy, but what if you do some %logging before the actual logs are initialized?
It's quite easy to end up doing this:
- usually you initialize logs somewhere within your @c main()
- as applications grow, you'll have global/static variables
- you'll do %logging from the body of the global/static variable's constructors 
    - direcly (from a constructor)
    - indirectly (from some constructor you call a function, which calls a function... which in turn does some logging)

You could even run into more complicated scenarios, where you create other threads, 
which, until you initialize your logs, do some logging. It's good practice to log a thread's begin and end, for instance.

Even though you think this'll never happen to you, usage of singletons and other static variables is quite common,
so better to guard against it.

One solution would be for the library to rely on an external function, like <tt>void boost::logging::init_logs()</tt>,
and have your application have to define it, and it its body, initialize the logs. The library would then make sure
the @c init_logs() is called before any log is used.

There are several problems with this solution:
- logs could be used too soon before you have all the needed data to initialize them (for instance, some could depend
  on command line arguments)
- before any log is used, I would need to make sure @c init_logs() has been called - thus, for each log usage,
  I would need to check if init_logs has been called or not - not very efficient
- within init_logs() I might end up using logs, thus ending in an infinite loop (log needs init_logs(), which uses log)
- you would not have any control over when @c init_logs() is called - what if they need some context information -
  they wouldn't have any context do rely on
- depending on your application, some logs could only be initialized later than others
- if your application has several modules, assume each module has its own log. Thus, each module should be able to 
  initialize its own log when the module is initialized

Thus, I came up with a caching mechanism. You can choose to:
- Cache messages that are written before logs are initialized. For each logged message, you will also cache its corresponding filter 
  (so that if, when initializing the logs, a certain filter is turned off, that message won't be logged)
- Cache messages that are written before logs are initialized. When logs are initialized, all these cached messages are logged
- Ignore messages that are written before the logs are initialized

<b>By default, for each log, cache is turned on. To turn cache off (mark the log as initialized), just call @c turn_cache_off() on it.
You'll see that I'm doing this on all examples that come with the library.</b>




@section caching_BOOST_LOG_BEFORE_INIT_CACHE_FILTER Cache messages before logs are initialized/ cache their filter as well (BOOST_LOG_BEFORE_INIT_CACHE_FILTER)

It's a bit inefficient (after invoking the filter, it will always ask if cache is on or off). Also,
it increases the application's size a bit - for each log statement, I will generate a callback that I can call later to see if the filter
is still turned on or off.

@code
...
#define L_ BOOST_LOG_USE_LOG_IF_FILTER(g_l(), g_log_filter()->is_enabled() ) 
...

L_ << "this message will not be logged";
g_log_filter()->set_enabled(false);
g_l()->mark_as_initialized();
@endcode


@section caching_BOOST_LOG_BEFORE_INIT_LOG_ALL Cache messages before logs are initialized regardless of their filter (BOOST_LOG_BEFORE_INIT_LOG_ALL)

This case is the @b default. In the second case, when cache is on, all messages are cached, regardless of their filter (it's like all filters are on).
Then, when cache is turned off, all messages are logged.

If you do want to use this setting, make sure you define the @c BOOST_LOG_BEFORE_INIT_LOG_ALL globally.

@code
...
#define L_ BOOST_LOG_USE_LOG_IF_FILTER(g_l(), g_log_filter()->is_enabled() ) 
...

L_ << "this message will be logged, even if filter will be turned off";
g_log_filter()->set_enabled(false);
g_l()->mark_as_initialized();
@endcode



@section caching_BOOST_LOG_BEFORE_INIT_IGNORE_BEFORE_INIT Ignore all messages before turn_cache_off (BOOST_LOG_BEFORE_INIT_IGNORE_BEFORE_INIT)

In the last case, all messages before @c turn_cache_off() are ignored.

If you do want to use this setting, make sure you define the @c BOOST_LOG_BEFORE_INIT_IGNORE_BEFORE_INIT globally.

@code
...
#define L_ BOOST_LOG_USE_LOG_IF_FILTER(g_l(), g_log_filter()->is_enabled() ) 
...

L_ << "this message will NOT be logged";
g_l()->mark_as_initialized();
@endcode



@section caching_ts Setting thread-safety of "is_cache_turned_on"

In case you chose to use cache (either @c BOOST_LOG_BEFORE_INIT_CACHE_FILTER or @c BOOST_LOG_BEFORE_INIT_LOG_ALL), you can also set 
thread-safety of "is_cache_turned_on".

Each time a message is logged, we need to ask : @c is_cache_turned_on. However, we also want to be as efficient as possible - we don't want
this question to take too much CPU time. Thus, I came up with a simple algorithm:
- first, query an @c is_enabled bool-like variable, which at the beginning is false (very efficient, we can use TSS here)
  - if this is true, it's clear that caching has been turned off
  - if this is false, we don't know for sure, thus, continue to ask
- second, use a thread-safe "bool"-like variable (surrounded by a mutex, a bit slow, but that's life)
  - if this is true, we're still using cache 
  - if this is false, caching has been turned off
    - set @c is_enabled to true

Now, for the @c is_enabled variable, we can use several TSS tricks - like:
- use @c ts_resource_single_thread (the default) - is_enabled is a simple bool variable. This is very efficient,
  and is perfect if for your logs, you call turn_cache_off() while there's a single thread running (thus, no concurrency issues).
- use @c tss_resource_once_init - this resource, once initialized, it propagates through all threads. 
- use @c tss_resource_with_cache - each thread updates its value once at a certain period

There are several such policies, find them in the lock_resource_finder namespace.

If you want to override the above, make sure you define the @c BOOST_LOG_BEFORE_INIT_LOCK_RESOURCE_CLASS macro before including
any Boost Log headers. Example:

@code
#define BOOST_LOG_BEFORE_INIT_LOCK_RESOURCE_CLASS ::boost::logging::lock_resource_finder::tss_once_init<>
@endcode

*/

}}
