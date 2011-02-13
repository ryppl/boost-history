// packet_example.cpp  -------------------------------------------------------//

//  (C) Copyright VicenteJ Botet Escriba 2010

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See library home page at http://www.boost.org/libs/integer/endian

//----------------------------------------------------------------------------//

//~ #define _CRT_SECURE_NO_DEPRECATE  // quiet VC++ 8.0 foolishness
#define BOOST_NO_STATIC_ASSERT
//~ #include <iostream>
//~ #include <cassert>
	  
#include <boost/integer/endian/endian_pack.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/length.hpp>
#include <boost/units/systems/si/plane_angle.hpp>

using namespace boost::integer;
using namespace boost::chrono;
using namespace boost::integer::endianness;
using namespace boost::units;

namespace bitfield {
template <typename T, typename Tag, std::size_t Width>
struct member {
    static const std::size_t width=Width;
};

template <std::size_t Width>
struct filler {
    static const std::size_t width=Width;
};

template <typename T>
struct width {
    static const std::size_t value=T::width;
};


template<typename Storage, 
    typename T1, 
    typename T2=filler<0>, 
    typename T3=filler<0>, 
    typename T4=filler<0>, 
    typename T5=filler<0> 
>
struct tuple {
  char placeholder[
    (
        width<T1>::value + 
        width<T2>::value + 
        width<T3>::value + 
        width<T4>::value + 
        width<T5>::value + 
        7
    )/8];
}; // bitfield

}

namespace internet {


struct IpHeader {
    struct version {};
    struct header_length {};
    struct precedence {};
    struct low_delay {};
    struct high_thruput {};
    struct high_reliability {};
    struct minimize_cost {};
    struct dont_frag {};
    struct more_frag {};
    struct frag_offset {};
        
    /////////////////////////////
    bitfield::tuple<ubig8_pt, 
        bitfield::member<int, version, 4>, 
        bitfield::member<unsigned int, header_length, 4>
    > version_headerLength;
        
    bitfield::tuple<ubig8_pt, 
        bitfield::member<unsigned int, precedence, 3>, 
        bitfield::member<bool, low_delay, 1>, 
        bitfield::member<bool, high_thruput, 1>, 
        bitfield::member<bool, high_reliability, 1>, 
        bitfield::member<bool, minimize_cost, 1>
    > differentiated_services;
        
    ubig16_pt total_length;
    /////////////////////////////
    ubig16_pt identification;
    bitfield::tuple<ubig16_pt, 
        bitfield::filler<1>, 
        bitfield::member<bool, dont_frag, 1>, 
        bitfield::member<bool, more_frag, 1>, 
        bitfield::member<int, frag_offset, 13> 
    > flags_frag;
    /////////////////////////////
    ubig8_pt time_to_live;
    ubig8_pt protocol;
    ubig16_pt header_checksum;
    /////////////////////////////
    ubig32_pt source_address;
    /////////////////////////////
    ubig32_pt destination_address;
}; // IpHeader

struct UdpHeader {
  ubig16_pt source_port;
  ubig16_pt destination_port;
  ubig16_pt length;
  ubig16_pt checksum;
}; // UdpHeader


} // internet


struct UserMessage {
  endian_pack<little, system_clock::time_point > timestamp;
  ulittle32_pt aircraft_id;
  struct Position {
     endian_pack<little, quantity<si::length, boost::int_least32_t> > x;
     endian_pack<little, quantity<si::length, boost::int_least32_t> > y;
     endian_pack<little, quantity<si::length, boost::int_least32_t> > z;
  } position;
  struct Attitude {
      endian_pack<little, quantity<si::plane_angle, boost::int_least8_t> > heading;
      endian_pack<little, quantity<si::plane_angle, boost::int_least8_t> > pitch;
      endian_pack<little, quantity<si::plane_angle, boost::int_least8_t> > roll;
  } attitude;
}; // UserMessage

struct Packet {
  internet::IpHeader ipHeader;
  internet::UdpHeader udpHeader;
  UserMessage userMessage;
}; // Packet


int main() {
  Packet packet;
  packet.ipHeader.source_address = 0x12345678;
  packet.udpHeader.destination_port = 1234;
  packet.userMessage.timestamp = system_clock::now();
  packet.userMessage.position.y = 17 * si::meter;
} // main 

