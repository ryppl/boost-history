//char buffers, aligned and not aligned.
#ifndef BOOST_COMPOSITE_STORAGE_BUFFERS_CHAR_BUF_HPP_INCLUDED
#define BOOST_COMPOSITE_STORAGE_BUFFERS_CHAR_BUF_HPP_INCLUDED
//  (C) Copyright Larry Evans 2010.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//====================================================================
#include <boost/aligned_storage.hpp>

namespace boost
{
namespace composite_storage
{
namespace buffers
{

  template
  < std::size_t Size
  , std::size_t Align=1
  >
  struct
char_buf
;
  template
  < std::size_t Align
  >
  struct
char_buf
  < 0
  , Align
  >
{  
    char_buf( void)
    {}
    char_buf( char_buf const&)
    {}
      void
    operator=( char_buf const&)
    {}
      char const*
    address(void)const
    {
        return reinterpret_cast<char const*>(this);
    }
      char*
    address(void)
    {
        return reinterpret_cast<char*>(this);
    }
};
  template
  < std::size_t Size
  >
  struct
char_buf
  < Size
  , 0
  >
{  
    char_buf( void)
    {}
    char_buf( char_buf const&)
    {}
      void
    operator=( char_buf const&)
    {}
      char const*
    address(void)const
    {
        return buffer;
    }
      char*
    address(void)
    {
        return buffer;
    }
 private:    
    char buffer[Size]
    ;
};
  template
  < std::size_t Size
  , std::size_t Align
  >
  struct
char_buf
{
    char_buf( void)
    {}
    char_buf( char_buf const&)
    {}
      void
    operator=( char_buf const&)
    {}
      char const*
    address(void)const
    {
        void const*p=buffer.address();
        return static_cast<char const*>(p);
    }
      char*
    address(void)
    {
        void*p=buffer.address();
        return static_cast<char*>(p);
    }
 private:    
      ::boost::aligned_storage
      < Size
      , Align
      >
    buffer
    ;
};

}//exit buffers namespace

}//exit composite_storage namespace
}//exit boost namespace
#include <boost/composite_storage/buffers/rval_ref_buf.hpp>
#include <boost/composite_storage/type_traits/remove_cv_ref.hpp>
#endif
