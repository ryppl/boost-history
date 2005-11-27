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

    // Data type to be used by ptree
    typedef boost::any data_type;
    
    // Extractor to be used by ptree
    template<class Type>
    struct extractor: public my_extractor<Type> { }; 

    // Inserter to be used by ptree
    template<class Type>
    struct inserter: public my_inserter<Type> { };

    // Key comparison function
    inline bool operator()(const std::basic_string<Ch> &key1, 
                           const std::basic_string<Ch> &key2) const
    {
        return key1 < key2;
    }

};

int main()
{
    
    // Property_tree with boost::any as data type
    typedef boost::property_tree::basic_ptree<char, my_traits<char> > my_ptree;
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

    // Note: parsers will not work with my_ptree type: 
    // they require that data type of ptree is string
    /*
    boost::property_tree::write_info("test.info", pt);      // Error!
    */

}
