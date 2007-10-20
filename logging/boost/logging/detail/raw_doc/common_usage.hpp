namespace boost { namespace logging {

/** 
@page common_usage Scenario 1: Common Usage

Scenario 1 should be the most common:
- You have multiple levels (in this example: debug < info < error)
- You want to format the message before it's written 
  (in this example: prefix it by index, by time, and append an enter to it)
- You have <b>one log</b>, which writes to several log destinations
  (in this example: the console, the output window, and a file

In our example, the output will be written to the console, debug window, and "out.txt" file.
It will look similar to this one:

@code
12:59.27 [1] this is so cool 1
12:59.27 [2] hello, world
12:59.27 [3] good to be back ;) 2
@endcode

To take a look at the code for example, see:

@htmlonly
<a href="http://svn.boost.org/svn/boost/sandbox/logging/lib/logging/samples/scenarios/mul_levels_one_logger.cpp">Scenario 1 Code</a>
@endhtmlonly




*/

}}
