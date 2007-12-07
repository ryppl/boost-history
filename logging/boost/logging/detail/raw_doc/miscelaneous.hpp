namespace boost { namespace logging {

/** 
@page miscelaneous Miscelaneous things

@section misc_use_defaults Template parameters - Using defaults

This parameter is optional. This means you don't need to set it, unless you want to.
Just leave it as @c default_, and move on to the paramers you're interested in.

Example:

@code
typedef logger_format_write< default_, default_, writer::threading::on_dedicated_thread > log_type;
@endcode

@section misc_unicode Internationalization - Using Unicode charaters

In case you want to log unicode characters, it's very easy:

- just <tt>\#define BOOST_LOG_USE_WCHAR_T</tt> before including any Boost.Logging files
- For Windows, in case the @c UNICODE or @c _UNICODE is defined, the @c BOOST_LOG_USE_WCHAR_T is defined automatically for you



*/

}}
