#ifndef BOOST_TRANSACT_DETAIL_FILE_HPP
#define BOOST_TRANSACT_DETAIL_FILE_HPP

#include <boost/transact/exception.hpp>

namespace boost{
namespace transact{
namespace detail{

struct eof_exception : io_failure{};

}
}
}


#endif
