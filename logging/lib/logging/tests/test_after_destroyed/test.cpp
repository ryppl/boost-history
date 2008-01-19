/* 
    Tests using the log after it's been destroyed
*/

#include <boost/logging/format.hpp>
#include <string>
#include <sstream>

using namespace boost::logging;

typedef logger_format_write< > log_type;



void test_after_destroyed() ;

bool g_called_correctly = false;

struct last_to_be_destroyed {
    ~last_to_be_destroyed () {
        // this is to be called after s_before_log's destructor
        BOOST_ASSERT(g_called_correctly);
    }
} s_last;


// this will be initialized before the our log (g_l)
// thus, it will be destroyed afterwards
struct before_log {
    ~before_log() {
        g_called_correctly = true;
        test_after_destroyed();
    }
} s_before_log;

BOOST_DEFINE_LOG_FILTER(g_log_filter, filter::no_ts ) 
BOOST_DEFINE_LOG(g_l, log_type)


#define L_ BOOST_LOG_USE_LOG_IF_FILTER(g_l(), g_log_filter()->is_enabled() ) 

// whatever we log to the rolling file, we log here too (easy was to find out all the info that was logged)
std::stringstream g_stringstream;


bool g_log_destroyed = false;
void called_after_log_destruction(const std::string&) {
    g_log_destroyed = true;
}

void test_before_destroyed() {
    g_l()->writer().add_formatter( formatter::append_newline() );
    g_l()->writer().add_destination( destination::cout() );
    g_l()->writer().add_destination( destination::stream(g_stringstream) );
    g_l()->turn_cache_off();
    g_l()->set_after_destroyed( called_after_log_destruction);

    L_ << "hello world";
    BOOST_ASSERT(g_stringstream.str() == "hello world\n");
    g_stringstream.str("");
}

void test_after_destroyed() {
    // this should actually call "called_after_log_destruction" function
    L_ << "hello world 2";
    BOOST_ASSERT( g_log_destroyed);
    // nothing should have been written there
    BOOST_ASSERT( g_stringstream.str() == "");
}

int main() {
    test_before_destroyed();
}
