//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_DETAIL_RAW_OBJECT_HPP
#define BOOST_ACT_DETAIL_RAW_OBJECT_HPP

// ToDo: Use aligned storage, specialize for empty types
template< typename StoredType >
class internal_active_object_base
{
public:
  internal_active_object_base()
  {
    new ( raw_storage_m ) DataType;
  }

  // ToDo: Account for no params (in_place_factory is broken)
  template< typename FactoryType >
  internal_active_object_base( FactoryType const& factory )
  {
    factory.template apply< StoredType >( raw_storage_m );
  }
public:
  void destruct() const
  {
    internal_object().~DataType();
  }
protected:
  // ToDo: Make other cv-qualified overloads
  DataType& internal_object()
  {
    return reinterpret_cast< DataType& >( raw_storage_m ); // ToDo: Handle alignment
  }

  DataType const& internal_object() const
  {
    return reinterpret_cast< DataType const& >( raw_storage_m ); // ToDo: Handle alignment
  }
private:
  unsigned char raw_storage_m[ sizeof DataType ];
};

}
}
}

#endif
