//  Boost.SqlCli library ----------------------------------------------------//

//  Copyright Nicola Musatti 2006 - 2007. 
//  Use, modification, and distribution are subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/sql_cli for library home page. ------------//

#if ! defined(BOOST_SQL_CLI_ODBC_PARAM_HPP)
#define BOOST_SQL_CLI_ODBC_PARAM_HPP

#include <boost/optional.hpp>
#include <boost/none.hpp>

#include <boost/sql_cli/null_value.hpp>
#include <boost/sql_cli/odbc/odbc_config.hpp>
#include <boost/sql_cli/odbc/traits.hpp>

namespace boost
{
namespace sql_cli
{
namespace odbc
{

class param
{
public:
    param(unsigned short n, SQLINTEGER s) : num(n), soi(s) {}
    virtual ~param() {}

    unsigned short number() const { return num; }

    virtual SQLPOINTER parameter_value_ptr() const = 0;

    virtual SQLSMALLINT value_type_id() const = 0;
    virtual SQLSMALLINT param_type_id() const = 0;

    SQLINTEGER * strlen_or_indptr() { return &soi; }

    virtual bool update() = 0;

protected:
    void strlen_or_indptr(SQLINTEGER s) { soi = s; }

private:
    unsigned short num;
    SQLINTEGER soi;
};

template <typename ValueT> 
class param_value
{
public:
    typedef ValueT value_type;

    param_value(value_type & v) : val(v) {}

    SQLPOINTER ptr() const { return (SQLPOINTER)(&val); }

    bool update() { return false; }

    SQLINTEGER size() const { return traits<value_type>::strlen_or_indptr; }

private:
    value_type & val;
};

template <> 
class param_value<std::string>
{
public:
    typedef std::string value_type;

    param_value(value_type & v) : val(v), buf(v.begin(), v.end()) 
    {
        buf.push_back('\0');
    }

    SQLPOINTER ptr() const { return (SQLPOINTER)(&buf[0]); }

    bool update() 
    { 
        buf.assign(val.begin(), val.end()); 
        buf.push_back('\0');
        return true; 
    }

    SQLINTEGER size() const { return traits<value_type>::strlen_or_indptr; }

private:
    value_type & val;
    std::vector<char> buf;
};

template <> 
class param_value<std::wstring>
{
public:
    typedef std::wstring value_type;

    param_value(value_type & v) : val(v), buf(v.begin(), v.end()) 
    {
        buf.push_back(L'\0');
    }

    SQLPOINTER ptr() const { return (SQLPOINTER)(&buf[0]); }

    bool update() 
    { 
        buf.assign(val.begin(), val.end()); 
        buf.push_back(L'\0');
        return true; 
    }

    SQLINTEGER size() const { return traits<value_type>::strlen_or_indptr; }

private:
    value_type & val;
    std::vector<wchar_t> buf;
};

template <typename ValueT> 
class param_value<sql_cli::null_value<ValueT> >
{
public:
    typedef sql_cli::null_value<ValueT> value_type;

    param_value(value_type &) {}

    SQLPOINTER ptr() const { return (SQLPOINTER)0; }

    bool update() { return true; }

    SQLINTEGER size() const { return SQL_NULL_DATA; }
};

template <typename ValueT> 
struct param_value<boost::optional<ValueT> >
{
public:
    typedef boost::optional<ValueT> value_type;

    param_value(value_type & v) : val(v) {}

//    SQLPOINTER ptr() const { return (SQLPOINTER)(val ? val.get_ptr() : 0); }
    SQLPOINTER ptr() const { return (SQLPOINTER)(val.get_ptr()); }

    bool update() { return true; }

    SQLINTEGER size() const 
    { 
        return val ? traits<value_type>::strlen_or_indptr : SQL_NULL_DATA; 
    }

private:
    value_type & val;
};

template <> 
struct param_value<boost::optional<std::string> >
{
public:
    typedef boost::optional<std::string> value_type;

    param_value(value_type & v) : val(v) 
    {
        if ( val )
        {
            buf.assign(val->begin(), val->end());
            buf.push_back('\0');
        }
    }

    SQLPOINTER ptr() const 
    {
        return (SQLPOINTER)(val ? &buf[0] : 0);
    }

    bool update()
    { 
        if ( val )
        {
            buf.assign(val->begin(), val->end());
            buf.push_back('\0');
        }
        return true;
    }

    SQLINTEGER size() const 
    { 
        return val ? traits<value_type>::strlen_or_indptr : SQL_NULL_DATA; 
    }

private:
    value_type & val;
    std::vector<char> buf;
};

template <> 
struct param_value<boost::optional<std::wstring> >
{
public:
    typedef boost::optional<std::wstring> value_type;

    param_value(value_type & v) : val(v) 
    {
        if ( val )
        {
            buf.assign(val->begin(), val->end());
            buf.push_back(L'\0');
        }
    }

    SQLPOINTER ptr() const 
    {
        return (SQLPOINTER)(val ? &buf[0] : 0);
    }

    bool update()
    { 
        if ( val )
        {
            buf.assign(val->begin(), val->end());
            buf.push_back(L'\0');
        }
        return true;
    }

    SQLINTEGER size() const 
    { 
        return val ? traits<value_type>::strlen_or_indptr : SQL_NULL_DATA; 
    }

private:
    value_type & val;
    std::vector<wchar_t> buf;
};

template <typename ValueT> 
class typed_param : public param
{
public:
    typedef ValueT value_type;
    typedef traits<value_type> traits_type;

    typed_param(ValueT & v, unsigned short n) : val(v),
            param(n, traits_type::strlen_or_indptr)
    {
    }

    SQLSMALLINT value_type_id() const { return traits_type::value_type_id; }
    SQLSMALLINT param_type_id() const { return traits_type::param_type_id; }

    SQLPOINTER parameter_value_ptr() const { return val.ptr(); }
    
    bool update()
    { 
        bool ret = val.update(); 
        if ( ret )
            strlen_or_indptr(val.size());
        return ret;
    }

protected:
    param_value<ValueT> val;

private:
    typed_param & operator=(typed_param const &);
};

} // namespace odbc
} // namespace sql_cli
} // namespace boost

#endif // ! defined(BOOST_SQL_CLI_ODBC_PARAM_HPP)
