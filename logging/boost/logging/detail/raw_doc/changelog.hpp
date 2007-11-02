/** 
@page page_changelog Changelog

v0.10.1, 2 nov 2007
- added TSS - however, not tested (TSS is off, for now)
- compiles on VS2005, gcc 3.4.2 , gcc 4.1 and gcc 4.3

v0.9.12.3, 1 nov 2007
- move ts/* files to detail directory
- doc - standalone page with changelog
- moved changelog.hpp to detail/raw_doc dir

v0.9.11, 31 oct 2007
- no more stdafx.* files

v0.9.10, 31 oct 2007
- destination::file - on linux don't add ios_base::in when not original overwrite. Many thanks to Martin Baeker!

v0.9.9, 31 oct 2007
- fixed fastest*.cpp scenarios so that they compile. Many thanks to Martin Baeker!

v0.9.8, 31 oct 2007
- added small things to make it compile with gcc 4.1 and 4.3
  (note: I just solved some errors that were pointed out by Jens Seidel, not sure if this fixed everything)

v0.9.7, 30 oct 2007
- added small things to make it compile with gcc 4.1 and 4.3
  (note: I just solved some errors that were pointed out by Jens Seidel, not sure if this fixed everything)

v0.9.6, 30 oct 2007
- removed garbage from stdafx.h files - so that it can be compiled under Linux

v0.9.5, 30 oct 2007
- compiles with gcc 4.1 and 4.3 
  (note: I just solved some errors that were pointed out by Jens Seidel, not sure if this fixed everything)

v0.9.4, 30 oct 2007
- compiles with gcc 3.4.2

v0.9.3, 30 oct 2007
- updated documentation 
    - updated workflow/most of the pages in the table of contents
    - removed most of references to @c process_msg class

v0.9.2, 30 oct 2007
- Breaking chage: 
  - @c process_msg class has been removed. Now @c logger takes its place
  - Instead of logger<use_format_write<>>, use logger_format_write<>

v0.9.1, 30 oct 2007
- implemented BOOST_DEFINE_LOG/BOOST_DECLARE_LOG for when not compiling fast
- BEFORE a breaking change that will happen :
  - logger as is now will dissapear
  - the new logger class will be the old process_msg class
  

v0.9.0, 28 oct 2007
- use_format_write has 4 params now, I've added 2 new params: thread_safety and gather 
  - if you want to leave something unchanged, use "default_" as argument
- added more complex example: Line Counter application
- Breaking change: 
  - filters are declared with BOOST_DECLARE_LOG_FILTER, and defined with BOOST_DEFINE_LOG_FILTER
  - filters are now used with operator->, instead of "."
  - Example: 
		BOOST_DEFINE_LOG_LEVEL(g_log_level, level::holder ) 
		... 
		g_log_level->set_enabled(level::error);
  - rationale: filters, same as levels, could be used before main


v0.8.3, 22 oct 2007
- now have 8 scenarios
- updated documentation


v0.8.2, 22 oct 2007
- added one more common scenario (with_route)


v0.8.1, 22 oct 2007
- added one more common scenario
- finally figured out how I can add examples using doxygen :)


v0.8, 20 oct 2007
- added use_format_write class
- removed the filter from the logger class (need to update documentation)
- added Common scenario


v0.7.3, 19 oct 2007
- added the manipulator::class_ concept
- added some documentation


v0.7.2, 18 oct 2007
- format_write cares if formatter/destination is generic or not
- solved bug when deleting manipulators (same_type needed to have a virtual destructor)

v0.4, 9 oct 2007
- ts_write and on_dedicated_thread work

v0.5, 11 oct 2007
- compiles
- added documentation

v0.5.2, 11 oct 2007
- changed license
- added to boost

v0.6, 13 oct 2007
- added formatters from v1. of the library
- added convert_format cool function :P

v0.7, 15 oct 2007
- compiles with gcc 3.4.2

*/
