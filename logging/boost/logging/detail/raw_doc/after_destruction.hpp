namespace boost { namespace logging {

/** 
@page after_destruction Using the logger(s)/filter(s) after they've been destroyed

- @ref after_destruction_can_happen 
- @ref after_destruction_avoid 
- @ref after_destruction_logger 
    - @ref after_destruction_ignore 
    - @ref after_destruction_write 
    - @ref after_destruction_leak 
- @ref after_destruction_filter 



\n\n
@section after_destruction_can_happen Can this happen?

The short answer : yes. How? The order of inialization between translation units is not defined, thus the same applies
for destruction. The following can happen: 
- a global object is constructed @em before a logger
- thus, it will be destroyed @em after the logger
- if in its destructor it tries to use the logger, there we go - logger is used after it's been destroyed.

\n\n
@section after_destruction_avoid Avoiding the issue: making sure it never happens

The easiest way to make sure you don't run into this problem, is this:
- if you know you have a global object of class A and
- you'll use a logger in your A's destructor (directly or indirectly), 

Then:
- make sure you use it in the A's constructor as well
- this way, @em always, the logger will be instantiated before your global object, 
  thus destroyed after your global object


\n\n
@section after_destruction_logger Using the logger(s) after they've been destroyed

In this case, you have 3 options:
-# ignore this
-# have a function to write to, after the logger is destroyed (default)
-# leak the logger object


\n\n
@subsection after_destruction_ignore Ignoring this case

To ignore this issue altogether, define BOOST_LOG_AFTER_BEING_DESTROYED_IGNORE directive globally.


\n\n
@subsection after_destruction_write Write to function (default)

You can have a raw (non-member) function to write to, in case the logger is used after being destroyed. This is the default. 
Also, you can force it by defining BOOST_LOG_AFTER_BEING_DESTROYED_WRITE_TO_FUNCTION directive globally.

In this case, your loggers need to be global objects. We rely on the fact that after the object is destroyed,
the memory occupied by it won't be overwritten by the system. So basically you can have a @em bool member "is_alive"
which we set to false in the logger's destructor.

By default, this raw function does nothing. You can overwrite it:

@code
void call_after_log_destruction(const std::string& str) {
  std::cout << str;
}
...

g_l()->set_after_destroyed( call_after_log_destruction);
@endcode

The raw function has one argument : the string you use to write to your destination. Unless you overwrite it (@ref BOOST_LOG_DESTINATION_MSG), it's std::string.



\n\n
@subsection after_destruction_leak Leaking the logger(s)

To avoid the problem, you can also choose to leak the logger - thus, it will never be destroyed, thus we won't run into this problem.
To choose this, define the BOOST_LOG_AFTER_BEING_DESTROYED_LEAK_LOGGER directive globally.




\n\n
@section after_destruction_filter Using the filter(s) after they've been destroyed

Filters, unlike loggers, are much simpler classes. Thus, you don't need to do anything for a filter - once it's destroyed, 
its memory will not be overwritten by the system, so that if the filter was on, will still remain on or if it was off, will still be off.

For this to work:
-# all filters need to be global objects
-# the <em>data members</em> of the filter classes need to be trivial

For the latter case, what I mean is that they should not point to dynamically allocated data (which, when destructed, would be released).
For example:
- @c int is a trivial type
- @c std::string is @em not a trivial type (points to memory, which it releases on destructor)
- @c std::vector is @em not a trivial type
- <tt>char [256]</tt> is a trivial type

Or, you can simply leak the data members.

The filter classes that come with this library are safe to use.



*/

}}
