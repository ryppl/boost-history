/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_xml_oarchive.ipp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <algorithm>
#include <cstring>
#if defined(BOOST_NO_STDC_NAMESPACE) && ! defined(__LIBCOMO__)
namespace std{
    using ::strlen;
} // namespace std
#endif

#include <boost/archive/basic_xml_archive.hpp>
#include <boost/archive/basic_xml_oarchive.hpp>
#include <boost/detail/no_exceptions_support.hpp>

namespace boost {
namespace archive {

namespace detail {
template<class CharType>
struct XML_name {
    void operator()(CharType t) const{
        const unsigned char lookup_table[] = {
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0, // -.
            1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0, // 0-9
            0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // A-
            1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1, // -Z _
            0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // a-
            1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0, // -z
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        };
        if((unsigned)t > 127)
            return;
        if(0 == lookup_table[(unsigned)t])
            boost::throw_exception(
                xml_archive_exception(
                    xml_archive_exception::xml_archive_tag_name_error
                )
            );
    }
};

} // namespace detail

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// implemenations of functions common to both types of xml output

template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::write_attribute(
    const char *attribute_name,
    int t,
    const char *conjunction
){
    this->This()->put(' ');
    this->This()->put(attribute_name);
    this->This()->put(conjunction);
    this->This()->save(t);
    this->This()->put('"');
}

template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::write_attribute(
    const char *attribute_name,
    const char *key
){
    this->This()->put(' ');
    this->This()->put(attribute_name);
    this->This()->put("=\"");
    this->This()->put(key);
    this->This()->put('"');
}

template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::indent(){
    int i;
    for(i = depth; i-- > 0;)
        this->This()->put('\t');
}

template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::save_start(const char *name)
{
    if(NULL == name)
        return;

    // be sure name has no invalid characters
    std::for_each(name, name + std::strlen(name), detail::XML_name<const char>());

    end_preamble();
    if(depth > 0){
        this->This()->put('\n');
        indent();
    }
    ++depth;
    this->This()->put('<');
    this->This()->save(name);
    pending_preamble = true;
    indent_next = false;
}

template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::save_end(const char *name)
{
    if(NULL == name)
        return;

    // be sure name has no invalid characters
    std::for_each(name, name + std::strlen(name), detail::XML_name<const char>());

    end_preamble();
    --depth;
    if(indent_next){
        this->This()->put('\n');
        indent();
    }
    indent_next = true;
    this->This()->put("</");
    this->This()->save(name);
    this->This()->put('>');
    if(0 == depth)
        this->This()->put('\n');
}

template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::end_preamble(){
    if(pending_preamble){
        this->This()->put('>');
        pending_preamble = false;
    }
}

template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::save_override(const object_id_type & t, int)
{
    int i = t.t; // extra .t is for borland
    write_attribute(OBJECT_ID(), i, "=\"_");
}
template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::save_override(
    const object_reference_type & t,
    int
){
    int i = t.t; // extra .t is for borland
    write_attribute(OBJECT_REFERENCE(), i, "=\"_");
}
template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::save_override(const version_type & t, int)
{
    int i = t.t; // extra .t is for borland
    write_attribute(VERSION(), i);
}
template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::save_override(const class_id_type & t, int)
{
    write_attribute(CLASS_ID(), t);
}
template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::save_override(
    const class_id_reference_type & t,
    int
){
    write_attribute(CLASS_ID_REFERENCE(), t);
}
template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::save_override(
    const class_id_optional_type & t,
    int
){
    write_attribute(CLASS_ID(), t);
}
template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::save_override(const class_name_type & t, int)
{
    const char * key = t;
    if(NULL == key)
        return;
    write_attribute(CLASS_NAME(), key);
}
template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::save_override(const tracking_type & t, int)
{
    write_attribute(TRACKING(), t.t); // extra .t is for borland
}

template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(void)
basic_xml_oarchive<Archive>::init(){
    // xml header
    this->This()->put("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n");
    this->This()->put("<!DOCTYPE boost_serialization>\n");
    // xml document wrapper - outer root
    this->This()->put("<boost_serialization");
    write_attribute("signature", ARCHIVE_SIGNATURE());
    write_attribute("version", ARCHIVE_VERSION());
    this->This()->put(">\n");
}

template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(BOOST_PP_EMPTY())
basic_xml_oarchive<Archive>::basic_xml_oarchive(unsigned int flags) :
    detail::common_oarchive<Archive>(flags),
    depth(0),
    indent_next(false),
    pending_preamble(false)
{
}

template<class Archive>
BOOST_ARCHIVE_OR_WARCHIVE_DECL(BOOST_PP_EMPTY())
basic_xml_oarchive<Archive>::~basic_xml_oarchive(){
    if(0 == (this->get_flags() & no_header)){
        BOOST_TRY{
                this->This()->put("</boost_serialization>");
        }
        BOOST_CATCH(...){}
        BOOST_CATCH_END
    }
}

} // namespace archive
} // namespace boost
