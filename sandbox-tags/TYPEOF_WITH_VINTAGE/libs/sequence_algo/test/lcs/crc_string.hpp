// (C) Copyright Craig Henderson 2002
//               cdm.henderson@virgin.net
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appears in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  The author makes no representations
// about the suitability of this software for any purpose.  It is
// provided "as is" without express or implied warranty.

#ifndef __CRC_STRING_H
#define __CRC_STRING_H

// vc6 warning pragmas
#pragma warning(disable: 4663)  // to explicitly specialize class template 'xx' use the following syntax
#pragma warning(disable: 4244)  // conversion from 'xxx' to 'yyy', possible loss of data
#include "boost/crc.hpp"

namespace cdmh {

// (limited) string class optimised for comparisons
// use this class if the text does not change frequently and the it is
// used in a lot of comparisons. the class is optimised by calculating
// a CRC value for the text and a comparison of two instances of this
// class therefore takes the time of a single 32 bit compare, rather
// than a full string compare
template<typename T>
class crc_string
{
  public:
    typedef std::basic_string<T> string_type;

  private:
    typedef boost::crc_optimal<32, 0x8005, 0, 0, true, true> crc_t;

    crc_t::value_type   crc_;
    string_type         string_;

  protected:
    void assign(const T *str)
    {
        assign(str, strlen(str));
    }

    void assign(const T *str, typename string_type::size_type len)
    {
        string_.erase();
        string_.append(str,len);
        crc_t crc;
        crc.process_block(string_.data(), string_.data()+len);
        crc_ = crc();
    }

  public:
    crc_string()
    {
    }

    crc_string(const T *str)
    {
        assign(str);
    }

    crc_string(const std::basic_string<T> &str)
    {
        assign(str.c_str());
    }

    crc_string(const std::basic_string<T> &str, typename string_type::size_type len)
    {
        assign(str.c_str(), len);
    }

    crc_string(const T *str, typename string_type::size_type len)
    {
        assign(str, len);
    }
    
    crc_string &operator=(const T *str)
    {
        assign(str);
        return *this;
    }

    bool operator==(const crc_string<T> &other) const
    {
        if (crc_ != other.crc_)
            return false;

        if (string_.length() != other.string_.length())
            return false;

        return (string_ == other.string_);
    }

    bool operator==(const T *str) const
    {
        crc_string<T> crcstring(str);
        return *this == crcstring;
    }

    bool operator!=(const crc_string<T> &other) const
    {
        return (!other == *this);
    }

    bool operator!=(const T *str) const
    {
        return !(*this == str);
    }

    std::size_t crc(void) const
    {
        return crc_;
    }

    typename string_type::size_type length(void) const
    {
        return string_.length();
    }

    const string_type &str(void) const
    {
        return string_;
    }

    const typename string_type::value_type *c_str(void) const
    {
        return string_.c_str();
    }
    
    friend std::ostream &operator<<(std::ostream &ostr, const crc_string<T> &str)
    {
        ostr << str.string_;
        return ostr;
    }
};

}   // namespace cdmh

#pragma warning(default: 4663)  // to explicitly specialize class template 'xx' use the following syntax
#pragma warning(default: 4244)  // onversion from 'xxx' to 'yyy', possible loss of data

#endif  // __CRC_STRING_H
