#ifndef BOOST_ARCHIVE_DETAIL_POLYMORPHIC_IARCHIVE_IMPL_HPP
#define BOOST_ARCHIVE_DETAIL_POLYMORPHIC_IARCHIVE_IMPL_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// polymorphic_iarchive_impl.hpp

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <cstddef>
#include <string>
#include <ostream>
#include <boost/noncopyable.hpp>
#include <boost/cstdint.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/archive/fast_array_serialization.hpp>

#include <boost/config.hpp>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std{ 
    using ::size_t; 
} // namespace std
#endif

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

namespace boost { 
namespace archive {
namespace detail{

class BOOST_ARCHIVE_DECL(BOOST_PP_EMPTY()) basic_iserializer;
class BOOST_ARCHIVE_DECL(BOOST_PP_EMPTY()) basic_pointer_iserializer;

template<class ArchiveImplementation>
class polymorphic_iarchive_impl : 
    public polymorphic_iarchive,
    // note: gcc dynamic cross cast fails if the the derivation below is 
    // not public.  I think this is a mistake.
    public /*protected*/ ArchiveImplementation,
    private boost::noncopyable
{
private:
    // these are used by the serialization library.
    virtual void load_object(
        void *t, 
        const basic_iserializer & bis
    ){
        ArchiveImplementation::load_object(t, bis);
    }
    virtual const basic_pointer_iserializer * load_pointer(
        void * & t, 
        const basic_pointer_iserializer * bpis_ptr,
        const basic_pointer_iserializer * (*finder)(
            const boost::serialization::extended_type_info & type
        )
    ){
        return ArchiveImplementation::load_pointer(t, bpis_ptr, finder);
    }
    virtual void set_library_version(unsigned int archive_library_version){
        ArchiveImplementation::set_library_version(archive_library_version);
    }
    virtual unsigned int get_library_version() const{
        return ArchiveImplementation::get_library_version();
    }
    virtual unsigned int get_flags() const {
        return ArchiveImplementation::get_flags();
    }
    virtual void reset_object_address(const void * new_address, const void * old_address){
        ArchiveImplementation::reset_object_address(new_address, old_address);
    }
    virtual void delete_created_pointers(){
        ArchiveImplementation::delete_created_pointers();
    }
    virtual void load_binary(void * t, std::size_t size){
        ArchiveImplementation::load_binary(t, size);
    }

	template <class T>
	void load_array_impl
	       (
	           T * p, std::size_t len
			 , typename boost::enable_if<fast_array_serialization<ArchiveImplementation,T> >::type * =0
		   )
	{
	  ArchiveImplementation::load_array(p,len);
	}

	template <class T>
	void load_array_impl
	       (
	         T * p, std::size_t len, 
			 typename boost::disable_if<fast_array_serialization<ArchiveImplementation,T> >::type * =0
		   )
	{
	  polymorphic_iarchive::load_array(p,len);
	}

    // primitive types the only ones permitted by polymorphic archives

#define BOOST_ARCHIVE_IMPLEMENT_POLYMPORPHIC_FUNCTION(T)  \
    virtual void load(T & t)                              \
	{                                                     \
        ArchiveImplementation::load(t);                   \
    }                                                     \
                                                          \
    virtual void save_array(T * p, std::size_t len)       \
	{                                                     \
	    load_array_impl(p,len);                           \
	}

#include <boost/archive/detail/implement_polymorphic_function.hpp>
	
#undef BOOST_ARCHIVE_IMPLEMENT_POLYMPORPHIC_FUNCTION

    virtual void load(std::string & t){
        ArchiveImplementation::load(t);
    }
    #ifndef BOOST_NO_STD_WSTRING
    virtual void load(std::wstring & t){
        ArchiveImplementation::load(t);
    }
    #endif
    // used for xml and other tagged formats default does nothing
    virtual void load_start(const char * name){
        ArchiveImplementation::load_start(name);
    }
    virtual void load_end(const char * name){
        ArchiveImplementation::load_end(name);
    }

    virtual void register_basic_serializer(const basic_iserializer & bis){
        ArchiveImplementation::register_basic_serializer(bis);
    }
public:
    // the >> operator 
    template<class T>
    polymorphic_iarchive & operator>>(T & t){
        return polymorphic_iarchive::operator>>(t);
    }

    // the & operator 
    template<class T>
    polymorphic_iarchive & operator&(T & t){
        return polymorphic_iarchive::operator&(t);
    }

    // all current archives take a stream as constructor argument
    template <class _Elem, class _Tr>
    polymorphic_iarchive_impl(
        std::basic_istream<_Elem, _Tr> & is, 
        unsigned int flags = 0
    ) :
        ArchiveImplementation(is, flags)
    {}
};

} // namespace detail
} // namespace archive
} // namespace boost

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // BOOST_ARCHIVE_DETAIL_POLYMORPHIC_IARCHIVE_IMPL_HPP
