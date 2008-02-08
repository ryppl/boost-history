#include <boost/logging/format.hpp>
#include <boost/logging/format/formatter/tags.hpp>
#include <boost/logging/format/formatter/syslog.hpp>

namespace bl = boost::logging;
typedef bl::tag::holder<bl::default_, bl::tag::level> tag_holder;
BOOST_LOG_FORMAT_MSG(tag_holder)
BOOST_LOG_DESTINATION_MSG(tag_holder)

typedef bl::logger_format_write< > logger_type;

#define L_(lvl) BOOST_LOG_USE_LOG_IF_LEVEL(g_l(), g_log_filter(), lvl ) .set_tag( BOOST_LOG_TAG_LEVEL(lvl) )

BOOST_DEFINE_LOG_FILTER(g_log_filter, bl::level::holder ) 
BOOST_DEFINE_LOG(g_l, logger_type)

int main() {
    g_l()->writer().add_formatter( bl::formatter::idx(), "[%] "  );
    g_l()->writer().add_formatter( bl::formatter::append_newline() );
    // write to cout and syslog
    g_l()->writer().add_destination( bl::destination::cout() );
    g_l()->writer().add_destination( bl::destination::syslog() );
    g_l()->mark_as_initialized();

    int i = 1;
    L_(debug) << "this is so cool " << i++;
    L_(error) << "first error " << i++;
}



