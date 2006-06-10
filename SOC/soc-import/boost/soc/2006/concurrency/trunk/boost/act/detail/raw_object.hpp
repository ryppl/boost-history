//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

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
