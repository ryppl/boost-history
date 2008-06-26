// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef DATAFLOW_RUNTIME_HPP
#define DATAFLOW_RUNTIME_HPP

#include <boost/dataflow/support/tags.hpp>

#include <string>

namespace boost { namespace dataflow {

namespace runtime
{
    template<typename T, typename PropertyType>
    struct property;
} // namespace runtime

class runtime_info
{
public:
    template<typename T>
    runtime_info(T)
        : name_(runtime::property<T,const char *>()())
        , uuid_(runtime::property<T,int>()())
    {}

    runtime_info(const std::string &name, int uuid)
        : name_(name), uuid_(uuid)
    {}

    const char *name() const
    {   return name_.c_str(); }
    int uuid() const
    {   return uuid_; }
    template<typename T>
    bool is() const
    {
        return uuid_ == runtime::property<T,int>()();
    }
private:
    std::string name_;
    int uuid_;
};

} } // namespace boost::dataflow

#define DATAFLOW_RUNTIME_PROPERTY(type,p_type,val)          \
namespace boost { namespace dataflow { namespace runtime {  \
    template<>                                              \
    struct property<type, p_type>                           \
    {                                                       \
        p_type operator()()                                 \
        {                                                   \
            return val;                                     \
        }                                                   \
    };                                                      \
} } }

DATAFLOW_RUNTIME_PROPERTY(default_tag, const char *, "default_tag")
DATAFLOW_RUNTIME_PROPERTY(default_tag, int, 0)
    
DATAFLOW_RUNTIME_PROPERTY(ports::producer, const char *, "producer")
DATAFLOW_RUNTIME_PROPERTY(ports::producer, int, 0)
DATAFLOW_RUNTIME_PROPERTY(ports::consumer, const char *, "consumer")
DATAFLOW_RUNTIME_PROPERTY(ports::consumer, int, 1)

DATAFLOW_RUNTIME_PROPERTY(operations::connect, const char *, "connect")
DATAFLOW_RUNTIME_PROPERTY(operations::connect, int, 0)
DATAFLOW_RUNTIME_PROPERTY(operations::extract, const char *, "extract")
DATAFLOW_RUNTIME_PROPERTY(operations::extract, int, 1)

#endif // DATAFLOW_RUNTIME_HPP
