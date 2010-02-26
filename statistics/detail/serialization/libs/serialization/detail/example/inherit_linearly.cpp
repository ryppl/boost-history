//////////////////////////////////////////////////////////////////////////////
// serialization::detail::example::inherit_linearly.cpp                     //
//                                                                          //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <fstream>
#include <string>
#include <boost/serialization/detail/inherit/inherit_linearly.hpp>
#include <boost/serialization/detail/inherit/detail/test_wrapper.hpp>
#include <boost/serialization/string.hpp>
#include <boost/mpl/vector.hpp>
#include <libs/serialization/detail/example/inherit_linearly.h>

struct foo{
	
    foo():value(0){}
    foo(int v):value(v){}

	int value;

	bool operator==(const foo& other)const{
    	return value == other.value;
    }

	private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & value;
    }                  

};

    template<typename A,typename B>
	std::basic_ostream<A,B>&
    operator<<(std::basic_ostream<A,B>& os,const foo& that){
		return os << that.value;
    }


void example_inherit_linearly(std::ostream& os){

	os << "->example_inherit_linearly" << std::endl;

	namespace ser = boost::serialization::detail;
    const char* path = "./test_wrappers";
    typedef std::ofstream ofs_;
    typedef std::ifstream ifs_;
    typedef boost::archive::text_oarchive oa_;
    typedef boost::archive::text_iarchive ia_;

	// BUG : std::string causes runtime error.

	typedef int data1_;
    typedef bool data2_;
    typedef double data3_; 
    // typedef std::string data3_;  // BUG
    typedef foo data4_; 
	typedef ser::test_wrapper<data1_> wrap1_;
	typedef ser::test_wrapper<data2_> wrap2_;
	typedef ser::test_wrapper<data3_> wrap3_;
	typedef ser::test_wrapper<data4_> wrap4_;
	typedef boost::mpl::vector<wrap1_,wrap2_,wrap3_,wrap4_> types_;
	typedef ser::inherit_linearly<types_> inlin_;

	data1_ v1 = -1;
    data2_ v2 = true;
    data3_ v3 = -0.999;
    //data3_ v3 = "minus one"; // BUG
    data4_ v4(-1);
	inlin_ inlin;
    
    static_cast<wrap1_&>(inlin).value = v1;
    static_cast<wrap2_&>(inlin).value = v2;
    static_cast<wrap3_&>(inlin).value = v3;
    static_cast<wrap4_&>(inlin).value = v4;

	os << "description : ";
	{
    	// prints (-1,1,-0.999,-1)
    	os << inlin << std::endl;
    }
	os << "save" << std::endl;
	{
    	ofs_ ofs(path);
        oa_ oa(ofs);
        oa << inlin;
    }
	os << "load" << std::endl;
	{
    	inlin_ inlin;
    	ifs_ ifs(path);
        ia_ ia(ifs);
        ia >> inlin;
        BOOST_ASSERT(static_cast<const wrap1_&>(inlin).value == v1);
        BOOST_ASSERT(static_cast<const wrap2_&>(inlin).value == v2);
        BOOST_ASSERT(static_cast<const wrap3_&>(inlin).value == v3);
        BOOST_ASSERT(static_cast<const wrap4_&>(inlin).value == v4);
    }
	os << "<-" << std::endl; 

}
