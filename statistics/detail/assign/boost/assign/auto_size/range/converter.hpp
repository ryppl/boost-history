//////////////////////////////////////////////////////////////////////////////
// assign::detail::range::converter.hpp                                     //
//                                                                          //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_RANGE_CONVERTER_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_RANGE_CONVERTER_ER_2010_HPP
#include <boost/assign/list_of.hpp> // contains parts of impl
#include <boost/typeof/typeof.hpp>
#include <boost/range.hpp>

// This brings the code within assign_detail::converter<> into a macro to be 
// expanded in what would otherwise be a derived class. This is done to solve 
// some compiler complaints in the presence of multiple inheritance.
// 
// Warning: some compiler workarounds needed to be removed  
//
// Requirements : 
// Expression        Description
// this_             type of the class within which this macro is expanded

#define BOOST_ASSIGN_AS_CONVERTER                                              \
                                                                               \
    public:                                                                    \
                                                                               \
        template< class Container >                                            \
        operator Container() const                                             \
        {                                                                      \
            return                                                             \
            	this->convert_to_container<Container>();                       \
        }                                                                      \
                                                                               \
        template< class Container >                                            \
        Container convert_to_container() const                                 \
        {                                                                      \
            static Container* c = 0;                                           \
            BOOST_STATIC_CONSTANT( bool, is_array_flag = sizeof( assign_detail::assign_is_array( c ) ) \
                                   == sizeof( type_traits::yes_type ) );       \
                                                                               \
            typedef BOOST_DEDUCED_TYPENAME mpl::if_c< is_array_flag,           \
                                                      assign_detail::array_type_tag,          \
                                             assign_detail::default_type_tag >::type tag_type;\
                                                                               \
            return convert<Container>( c, tag_type() );                        \
        }                                                                      \
                                                                               \
    protected:                                                                 \
                                                                               \
        template< class Container >                                            \
        Container convert( const Container*, assign_detail::default_type_tag ) const \
        {                                                                      \
                                                                               \
            return Container( this->begin(), this->end() );                    \
        }                                                                      \
                                                                               \
        template< class Array >                                                \
        Array convert( const Array*, assign_detail::array_type_tag ) const     \
        {                                                                      \
            typedef BOOST_DEDUCED_TYPENAME Array::value_type value_type;       \
                                                                               \
            Array ar;                                                          \
            const std::size_t sz = ar.size();                                  \
            if( sz < boost::size( *this ) )                                    \
                throw assign::assignment_exception( "array initialized with too many elements" );\
            std::size_t n = 0;                                                 \
            BOOST_AUTO( i ,  this->begin() );                                  \
            BOOST_AUTO( e ,  this->end() );                                    \
            for( ; i != e; ++i, ++n )                                          \
                ar[n] = *i;                                                    \
            for( ; n < sz; ++n )                                               \
                ar[n] = value_type();                                          \
            return ar;                                                         \
        }                                                                      \
                                                                               \
        template< class Adapter >                                              \
        Adapter convert_to_adapter( const Adapter* = 0 ) const                 \
        {                                                                      \
            Adapter a;                                                         \
            BOOST_AUTO( i , this->begin() );                                   \
            BOOST_AUTO( e , this->end() );                                     \
            for( ; i != e; ++i )                                               \
                a.push( *i );                                                  \
            return a;                                                          \
        }                                                                      \
    protected:                                                                 \
        struct adapter_converter;                                              \
        friend struct adapter_converter;                                       \
        struct adapter_converter                                               \
        {                                                                      \
            const this_& gl;                                                   \
            adapter_converter( const this_& t ) : gl( t )                      \
            {}                                                                 \
                                                                               \
            adapter_converter( const adapter_converter& r )                    \
            : gl( r.gl )                                                       \
            { }                                                                \
                                                                               \
            template< class Adapter >                                          \
            operator Adapter() const                                           \
            {                                                                  \
                return gl.convert_to_adapter<Adapter>();                       \
            }                                                                  \
        };                                                                     \
    public:                                                                    \
        template< class Container >                                            \
        Container to_container( Container& c ) const                           \
        {                                                                      \
            return convert( &c, assign_detail::default_type_tag() );           \
        }                                                                      \
                                                                               \
        adapter_converter to_adapter() const                                   \
        {                                                                      \
            return adapter_converter( *this );                                 \
        }                                                                      \
                                                                               \
        template< class Adapter >                                              \
        Adapter to_adapter( Adapter& a ) const                                 \
        {                                                                      \
            return this->convert_to_adapter( &a );                             \
        }                                                                      \
                                                                               \
        template< class Array >                                                \
        Array to_array( Array& a ) const                                       \
        {                                                                      \
            return convert( &a, assign_detail::array_type_tag() );             \
        }                                                                      \
/**/

#endif
