namespace boost { namespace logging {

/** 
@page old_examples Old examples - to be removed




@section main_examples Examples

To see all its power, here are a few examples:



\n\n
Example 1: you choose to use the <i>L_(msg)</i> sytax:

@code
#include <boost/logging/logging.hpp>

using namespace boost::logging;
struct write_to_cout_and_file { ... };

// defining the logs
logger<write_to_cout_and_file, filter::no_ts> g_single_log("out.txt");
#define L_(x) if ( g_single_log) g_single_log.process_msg()(x)

// code
L_("reading word " + word);
@endcode



\n\n
Example 2: you choose to use the <i>L_ << "some " << " cool " << "log message"</i> syntax

@code
#include <boost/logging/logging.hpp>
#include <boost/logging/process_msg.hpp>

struct write_to_file { ... };
struct write_to_cout { ... };
struct write_to_dbg { ... };

using namespace boost::logging;

// defining the logs
typedef process_msg< gather::ostream_like::return_str<>, write_to_cout> processor_cout;
typedef process_msg< gather::ostream_like::return_str<>, write_to_file> processor_file;
typedef process_msg< gather::ostream_like::return_str<>, write_to_dbg> processor_dbg;

logger<processor_cout, filter::no_ts> g_log_app;
logger<processor_file, filter::no_ts> g_log_err("err.txt");
logger<processor_dbg,  filter::no_ts> g_log_dbg;

#define LAPP_ if ( !g_log_app) ; else g_log_app->read_msg().gather().out()
#define LERR_ if ( !g_log_err) ; else g_log_err->read_msg().gather().out()
#define LDBG_ if ( !g_log_dbg) ; else g_log_dbg->read_msg().gather().out()

// code
LAPP_ << idx << " : reading word " << word;
LERR_ << "error at " << idx << ", while reading " << word;
LDBG_ << "debug info: " << idx << ", reading " << word;
@endcode



\n\n
Example 3: you choose to use the same syntax as Example 2, but also use formatters and destinations.

@code
#include <boost/logging/logging.hpp>
#include <boost/logging/format/optimize.hpp>
#include <boost/logging/process_msg/ostream_like.hpp>
#include <boost/logging/format.hpp>

using namespace boost::logging;

...

typedef optimize::cache_string_one_str<> cache_string;
typedef process_msg< 
    gather::ostream_like::return_cache_str<> , 
    format_write< 
        format_base, 
        destination_base, format_and_write::simple<cache_string> > > process;
logger<process, filter::no_ts> g_l;
    
#define L_ if ( !g_l) ; else g_l->read_msg().gather().out()

int main() {
    // add formatters : [idx] [time] message [enter]
    g_l->writer().add_formatter( write_idx() );
    g_l->writer().add_formatter( write_time() );
    g_l->writer().add_formatter( append_enter() );

    // write to cout and file
    g_l->writer().add_destination( write_to_cout() );
    g_l->writer().add_destination( write_to_file("out.txt") );

    int i = 1;
    L_ << "will be prefixed by index and time , enter is appended as well " << i++;
}

@endcode



\n\n
Example 4: you choose to use levels

@code
using namespace boost::logging;

struct write_to_file { ... };

typedef process_msg< gather::ostream_like::return_str<>, write_to_file> processor;

level::holder_no_ts level_holder;

typedef logger<processor, filter_level<level::holder_no_ts, level::debug> > debug_logger;
typedef logger<processor, filter_level<level::holder_no_ts, level::error> > error_logger;
typedef logger<processor, filter_level<level::holder_no_ts, level::info> > info_logger;

// debug info goes into "dbg.txt" file
// error info goes into "err.txt" file
// app   info goes into "out.txt" file
debug_logger g_log_dbg( init_both, "dbg.txt", &level_holder );
error_logger g_log_err( init_both, "err.txt", &level_holder );
info_logger g_log_app( init_both, "out.txt", &level_holder );


#define LAPP_ if ( !g_log_app) ; else g_log_app->read_msg().gather().out()
#define LERR_ if ( !g_log_err) ; else g_log_err->read_msg().gather().out()
#define LDBG_ if ( !g_log_dbg) ; else g_log_dbg->read_msg().gather().out()

int main() {
    // set level at this time, print everything
    level_holder.set_enabled(level::info);    
    ...
    LAPP_ << "info at : " << idx << " : reading word " << word;
    ...
    // only errors and above
    level_holder.set_enabled(level::error);
    LDBG_ << "this will not be printed";
}

@endcode


*/

}}
