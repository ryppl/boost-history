/* compile in-place support for Boost.Serialization

Copyright 2008 Ulrich Eckhardt

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
// $Id$

// This file must not be included more than once. Note that this 'once' is
// not per translation unit but per binary! Never include this in a header!
#ifdef BOOST_SERIALIZATION_COMPILE_IN_PLACE_CPP_INCLUDED
#  error "this file should only be included once per binary"
#endif

#define BOOST_SERIALIZATION_COMPILE_IN_PLACE_CPP_INCLUDED
#include "../../libs/serialization/src/basic_archive.cpp"
#include "../../libs/serialization/src/basic_iarchive.cpp"
#include "../../libs/serialization/src/basic_iserializer.cpp"
#include "../../libs/serialization/src/basic_oarchive.cpp"
#include "../../libs/serialization/src/basic_oserializer.cpp"
#include "../../libs/serialization/src/basic_pointer_iserializer.cpp"
#include "../../libs/serialization/src/basic_pointer_oserializer.cpp"
#include "../../libs/serialization/src/basic_serializer_map.cpp"
#include "../../libs/serialization/src/basic_text_iprimitive.cpp"
#include "../../libs/serialization/src/basic_text_oprimitive.cpp"
#include "../../libs/serialization/src/basic_text_wiprimitive.cpp"
#include "../../libs/serialization/src/basic_text_woprimitive.cpp"
#include "../../libs/serialization/src/basic_xml_archive.cpp"
#include "../../libs/serialization/src/basic_xml_grammar.ipp"
#include "../../libs/serialization/src/binary_iarchive.cpp"
#include "../../libs/serialization/src/binary_oarchive.cpp"
#include "../../libs/serialization/src/binary_wiarchive.cpp"
#include "../../libs/serialization/src/binary_woarchive.cpp"
#include "../../libs/serialization/src/codecvt_null.cpp"
#include "../../libs/serialization/src/extended_type_info.cpp"
#include "../../libs/serialization/src/extended_type_info_no_rtti.cpp"
#include "../../libs/serialization/src/extended_type_info_typeid.cpp"
#include "../../libs/serialization/src/polymorphic_iarchive.cpp"
#include "../../libs/serialization/src/polymorphic_oarchive.cpp"
#include "../../libs/serialization/src/stl_port.cpp"
#include "../../libs/serialization/src/text_iarchive.cpp"
#include "../../libs/serialization/src/text_oarchive.cpp"
#include "../../libs/serialization/src/text_wiarchive.cpp"
#include "../../libs/serialization/src/text_woarchive.cpp"
#include "../../libs/serialization/src/utf8_codecvt_facet.cpp"
#include "../../libs/serialization/src/void_cast.cpp"
#include "../../libs/serialization/src/xml_grammar.cpp"
#include "../../libs/serialization/src/xml_iarchive.cpp"
#include "../../libs/serialization/src/xml_oarchive.cpp"
#include "../../libs/serialization/src/xml_wgrammar.cpp"
#include "../../libs/serialization/src/xml_wiarchive.cpp"
#include "../../libs/serialization/src/xml_woarchive.cpp"
