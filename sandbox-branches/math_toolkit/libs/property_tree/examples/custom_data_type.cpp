// ----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/any.hpp>
#include <list>
#include <string>
#include <iostream>

// Custom extractor - converts data from boost::any to Type
template<class Type>
struct my_extractor
{
    inline bool operator()(const boost::any &data, 
                           Type &extracted,
                           const std::locale &loc) const
    {
        extracted = boost::any_cast<Type>(data);
        return true;    // Success
    }
};

// Custom inserter - converts data from Type to boost::any
template<class Type>
struct my_inserter
{
    inline bool operator()(boost::any &data, 
                           const Type &to_insert,
                           const std::locale &loc) const
    {
        data = to_insert;
        return true;    // Success
    }
};

// Custom property_tree traits - using custom extractor and inserter
template<class Ch>
struct my_traits
{

    // Character type to be used by ptree keys
    typedef Ch char_type;

    // Key type to be used by ptree
    typedef std::basic_string<Ch> key_type;
    
    // Data type to be used by ptree
    typedef boost::any data_type;
    
    // Extractor to be used by ptree
    template<class Type>
    struct extractor: public my_extractor<Type> { }; 

    // Inserter to be used by ptree
    template<class Type>
    struct inserter: public my_inserter<Type> { };

    // Key comparison function
    inline bool operator()(const key_type &key1, 
                           const key_type &key2) const
    {
        return key1 < key2;
    }

};

int main()
{
    
    // Property_tree with boost::any as data type
    typedef boost::property_tree::basic_ptree<my_traits<char> > my_ptree;
    my_ptree pt;

    // Put/get int value
    pt.put("int value", 3);
    int int_value = pt.get<int>("int value");
    std::cout << "Int value: " << int_value << "\n";

    // Put/get string value
    pt.put<std::string>("string value", "foo bar");
    std::string string_value = pt.get<std::string>("string value");
    std::cout << "String value: " << string_value << "\n";

    // Put/get list<int> value
    int list_data[] = { 1, 2, 3, 4, 5 };
    pt.put<std::list<int> >("list value", std::list<int>(list_data, list_data + sizeof(list_data) / sizeof(*list_data)));
    std::list<int> list_value = pt.get<std::list<int> >("list value");
    std::cout << "List value: ";
    for (std::list<int>::iterator it = list_value.begin(); it != list_value.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    // Note: parsers will work with my_ptree type, but only if 
    // type contained in boost::any is string. Otherwise,
    // they will throw bad_any_cast, as will be the case here.
    // The reason is that obviously parsers only work with string data. 
    // You will need to provide extractor/inserter that can always 
    // succesfully convert from/to basic_string
    
    // This section tests if parsers compile and throw only expected exceptions
    std::istringstream istr;
    std::ostringstream ostr;
    try { boost::property_tree::write_info(ostr, pt); } catch (boost::bad_any_cast &) { }
    try { boost::property_tree::read_info(istr, pt); } catch (...) { }
    try { boost::property_tree::write_ini(ostr, pt); } catch (boost::bad_any_cast &) { }
    try { boost::property_tree::read_ini(istr, pt); } catch (...) { }
    try { boost::property_tree::write_json(ostr, pt); } catch (boost::bad_any_cast &) { }
    try { boost::property_tree::read_json(istr, pt); } catch (...) { }
    try { boost::property_tree::write_xml(ostr, pt); } catch (boost::bad_any_cast &) { }
    try { boost::property_tree::read_xml(istr, pt); } catch (...) { }

}
