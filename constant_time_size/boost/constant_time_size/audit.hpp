#ifndef BOOST_CONSTANT_TIME_SIZE_AUDIT_H_
#define BOOST_CONSTANT_TIME_SIZE_AUDIT_H_

namespace boost { 
namespace constant_time_size {

#ifdef BOOST_CONSTANT_TIME_SIZE_AUDIT
static const bool audit = true;
#else
static const bool audit = false;
#endif

}
}

#endif // BOOST_CONSTANT_TIME_SIZE_LIST_H_
