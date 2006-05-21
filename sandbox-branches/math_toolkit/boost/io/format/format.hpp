///////////////////////////////////////////////////////////////////////////////
// Output operators and manipulators for composite objects
//
// Copyright Terje Slettebø 2003.
//
// Contributions by Vladimir Prus, Paul A. Bristow and Jason House.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_IO_FORMAT_FORMAT_HPP
#define BOOST_IO_FORMAT_FORMAT_HPP

#include <ios>     // For ios_base::xalloc
#include <string>
#include <cstddef>

namespace boost {
namespace io {

///////////////////////////////////////////////////////////////////////////////
// format_data
///////////////////////////////////////////////////////////////////////////////

namespace detail
{
  const std::string defaults[5]={"[","]",",","",""};
}

template<class CharType>
class format_data
{
private:
  typedef std::basic_string<CharType> StringType;

public:
  format_data(
    const StringType &s = StringType(detail::defaults[0].begin(),detail::defaults[0].end()),
    const StringType &e = StringType(detail::defaults[1].begin(),detail::defaults[1].end()),
    const StringType &d = StringType(detail::defaults[2].begin(),detail::defaults[2].end()),
    const StringType &se = StringType(detail::defaults[3].begin(),detail::defaults[3].end()),
    const StringType &ee = StringType(detail::defaults[4].begin(),detail::defaults[4].end())) :
    start(s),end(e),delimiter(d),start_element(se),end_element(ee) {}

public:
  StringType start;
  StringType end;
  StringType delimiter;
  StringType start_element;
  StringType end_element;
};

///////////////////////////////////////////////////////////////////////////////
// format_type
///////////////////////////////////////////////////////////////////////////////

template<class CharType,class T>
class format_type
{
private:
  typedef std::basic_string<CharType> StringType;

public:
  format_type(const format_data<CharType> &d) : data(d)
  {
    set_format=true;
  }

  format_type(bool flag)
  {
    set_format=flag;
  }

  template<class CharTraits>
  static format_data<CharType> &format(std::basic_ostream<CharType,CharTraits> &stream)
  {
    void *&void_ptr=stream.pword(index);

    if(!void_ptr)
      void_ptr=new format_data<CharType>(default_data);

    return *static_cast<format_data<CharType> *>(void_ptr);
  }

  template<class CharTraits>
  static std::basic_ostream<CharType,CharTraits> &apply
    (std::basic_ostream<CharType,CharTraits> &stream,const format_type &object)
  {
    if(set_format)
      format(stream)=object.data;

    return stream;
  }

  static bool set()
  {
    return set_format;
  }

  void set_default_format() const
  {
    default_data=data;
  }

  template<class CharTraits>
  friend std::basic_ostream<CharType,CharTraits> &operator<<
    (std::basic_ostream<CharType,CharTraits> &stream,const format_type<CharType,T> &object)
  {
    return format_type<CharType,T>::apply(stream,object);
  }

private:
  static int index;
  static bool set_format;

public:
  format_data<CharType> data;
  static format_data<CharType> default_data;
};

template<class CharType,class T>
int format_type<CharType,T>::index=std::ios_base::xalloc();

template<class CharType,class T>
bool format_type<CharType,T>::set_format=false;

template<class CharType,class T>
format_data<CharType> format_type<CharType,T>::default_data;

///////////////////////////////////////////////////////////////////////////////
// format
///////////////////////////////////////////////////////////////////////////////

template<class T,class CharType,class CharTraits>
format_type<CharType,T> format(
  const std::basic_string<CharType,CharTraits> &start =
    std::basic_string<CharType,CharTraits>(),
  const std::basic_string<CharType,CharTraits> &end =
    std::basic_string<CharType,CharTraits>(),
  const std::basic_string<CharType,CharTraits> &delimiter =
    std::basic_string<CharType,CharTraits>(),
  const std::basic_string<CharType,CharTraits> &start_element =
    std::basic_string<CharType,CharTraits>(),
  const std::basic_string<CharType,CharTraits> &end_element =
    std::basic_string<CharType,CharTraits>())
{
  return format_type<CharType,T>
    (format_data<CharType>(start,end,delimiter,start_element,end_element));
}

template<class T>
format_type<char,T> format(
  const char *start = "",
  const char *end = "",
  const char *delimiter = "",
  const char *start_element = "",
  const char *end_element = "")
{
  return format_type<char,T>
    (format_data<char>(start,end,delimiter,start_element,end_element));
}

template<class T>
format_type<wchar_t,T> format(
  const wchar_t *start = L"",
  const wchar_t *end = L"",
  const wchar_t *delimiter = L"",
  const wchar_t *start_element = L"",
  const wchar_t *end_element = L"")
{
  return format_type<wchar_t,T>
    (format_data<wchar_t>(start,end,delimiter,start_element,end_element));
}

template<class T,class CharType>
format_type<CharType,T> format(bool flag)
{
  return format_type<CharType,T>(flag);
}

///////////////////////////////////////////////////////////////////////////////
// Placeholder type
///////////////////////////////////////////////////////////////////////////////

class _ {};

///////////////////////////////////////////////////////////////////////////////
// Placeholder index
///////////////////////////////////////////////////////////////////////////////

const std::size_t N = 1;

namespace detail
{

  ///////////////////////////////////////////////////////////////////////////////
  // select_template_format(Stream)
  ///////////////////////////////////////////////////////////////////////////////

  template<class SpecificType,class GeneralType,class CharType,class CharTraits>
  format_data<CharType> &select_template_format
    (std::basic_ostream<CharType,CharTraits> &stream)
  {
    return format_type<CharType,SpecificType>::set() ?
             format_type<CharType,SpecificType>::format(stream) :
             format_type<CharType,GeneralType>::set() ?
               format_type<CharType,GeneralType>::format(stream) :
               format_type<CharType,_>::format(stream);
  }

} // namespace detail
} // namespace io
} // namespace boost

#endif // BOOST_IO_FORMAT_FORMAT_HPP
