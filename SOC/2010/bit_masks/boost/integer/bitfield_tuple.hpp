//  Copyright 2010 Brian Bartman.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_BITFIELD_TUPLE_HPP
#define BOOST_BITFIELD_TUPLE_HPP
#include <boost/integer/details/bft/template_expansion_macros.hpp>
#include <boost/integer/details/bitfield_tuple_impl.hpp>
#include <boost/bitfield/bitfield.hpp>
#include <boost/integer/details/bft/name_lookup.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/size.hpp>
#include <boost/integer/details/bft/reference_builder.hpp>
#include <boost/integer/details/bft/ext/bitfield_tuple_fusion_includes.hpp>
#include <boost/integer/details/fusion_ext_includes.hpp>

namespace boost {

// TODO Move documentation into the correct place and redo with boostbook
// formatting.

/** bitfield_tuple
 *
 *  Description
 *  This is a type which provides access to bitfields stored within integers
 *  or other integral types (see future work, for extensions). This is a pseudo
 *  variadic type. It currently only takes up to 10 parameters but I will be 
 *  extending that to be adjustable via macro similar to the MPL. Each of the 
 *  template parameters must be supplied as either a storage template or a 
 *  member template. For example, if one wanted to declare a bitfield_tuple
 *  that was the size of one byte and contained two boolean bitfields and 
 *  two integers they would do the following,
 *  
 *  #include <boost/integer/bitfield_tuple.hpp>
 *  #include <boost/assert.hpp>
 *  struct bool_one;
 *  struct bool_two;
 *  struct int_one;
 *  struct int_two;
 *
 *  typedef bitfield_tuple<
 *      storage<char>,
 *      member<bool,bool_one,1>,
 *      member<bool,bool_two,1>,
 *      member<int,int_one,2>,
 *      member<int,int_two,2>
 *  >                       example_type;
 *
 *  int main() {
 *      example_type temp;
 *      temp.get<bool_one>() = false; // assigns false to the first bitfield.
 *      temp.get<bool_two>() = true;  // assigns false to the second bitfield.
 *      temp.get<2>()        = -1;    // assigns -1 to the first integer 
 *                                    // bitfield.
 *
 *      BOOST_ASSERT(( temp.get<2>()  == -1 )); // this passes the assert and 
 *                                              // does not exit the program
 *  }
 *
 *  Storage and Members 
 *  The storage type can be specified anywhere with in the template parameters.
 *  Here is an example of how one may wish to specify storage,
 *  
 *  typedef bitfield_tuple<
 *      member<bool,bool_one,1>,
 *      storage<char>,
 *      member<bool,bool_one,1>,
 *      member<int,int_one,2>,
 *      member<int,int_two,2>
 *  >                       example_2_type;
 *  
 *  The example_2_type above creates the same internal structure and
 *  functionality as example_type in the first example. All the storage template
 *  does is specify the type your internal storage takes and may be returned as.
 *
 *  storage template signature
 *  template <typename StorageType> storage;
 *

Note: reference first example to talk about member because this gets confusing.

 *  Members 
 *  Unlike the storage type the member types are stored in the bitfield_tuple
 *  the same way as they are specified in the template parameters. The template
 *  parameters for member are the same as specifying a regular bitfield in C
 *  or C++, so, the first parameter is the type, second is the name of that 
 *  variable in this case its a tag class or something of that 
 *  nature  and third is the width of the bitfield in bits. For example,
 *
 *  member<bool, bool_one,1>
 *
 *  specifies that the type stored within 1 bit is of type bool and is 
 *  accessible from within the bitfield_tuple by using the supplied name, 
 *  bool_one is this case, or by the index which it was specified in the 
 *  template parameters excluding the storage type parameter. This means that 
 *  the members are accessible by either an unsigned integral constant or by
 *  the name provided in side the member template.
 *
 *
 *
 *  Interface overview
 *
 *  bft is a bitfield tuple like those mentioned in previous examples. 
 *
 *  Public Typedefs
 *      bft::members      - This is an mpl::vector which contains 
 *                          bitfield_elements. A bitfield_element is an internal
 *                          type used to specify information about the bitfields
 *                          which make up the bitfield_tuple. The reason that 
 *                          this is public, is for the boost.fusion extension of
 *                          this data structure.
 *
 *      bft::storage_type - The type specified inside the storage template.
 *
 *      bft::bits_used    - This is an integral_constant type that specifies
 *                          the total number of bits used by all bitfields.
 *
 *
 *  bitfield_tuple internal Class templates
 *      bitfield_ref           - Template is a reference type which takes one
 *                          parameter which is expected to be of type 
 *                          bitfield_element and creates a proxy reference type
 *                          to the element specified by the bitfield_element.
 *
 *      bitfield_ref interface
 *          public typedefs
 *              return_type - The type which is to be returned from a specified
 *                            bitfield.
 *              bitfield_type - this is of type boost::integer::bitfield located
 *                              within the boost.vault. The bitfield type is
 *                              used to deal with more complicated issues such
 *                              as endianness within the internal storage type.
 *
 *          constructors
 *              bitfield_ref( bitfield_ref<MaskInfo> const&);
 *              bitfield_ref( storage_type& ref);
 *          private constructors
 *              bitfield_ref();
 *
 *          functions
 *              operator return_type() // implicit conversion operator
 *              bitfield_ref<MaskInfo> const& operator=(return_type const& rhs);
 *
 *
 *      const_bitfield_ref   - Similar to the bitfield_ref class template how ever this
 *                        is a const representation a reference.
 *          The only difference between const_bitfield_ref and bitfield_ref is that
 *          const_bitfield_ref does not define an assignment operator.
 *
 *
 *  bitfield_tuple constructors
 *  
 *      bitfield_tuple() - Default constructor.
 *
 *      bitfield_tuple (storage_type const& x); - Construction over the 
 *          storage_type. This provides a means of constructing a bitfield
 *          tuple over an initial storage type.
 *
 *      bitfield_tuple( bitfield_tuple<T0..T9> const& x); - Copy constructor.
 *
 *  Future work - Constructors - Incomplete and Possible Extensions
 *
 *      bitfield_tuple( ... ); - Member Wise Constructor -
 *          Description: a constructor which will take as many values as there 
 *          are members and put each one of those values into the tuple in 
 *          the same order that the bitfields are specified.
 *
 *
 *      template <typename T> bitfield_tuple(T const& x);
 *          - Bitfield_tuple which have the same members but storage is 
 *          in a different place within the template parameters.
 *          Just haven't gotten around to implementing it, yet.
 *
 *  Bitfield_tuple assignment operators
 *
 *      bitfield_tuple<T0..T9> const& operator=( storage_type const& new_value);
 *          storage type assignment. Allows for the user to assign a perticular
 *          value to the tuples internal storage.
 *
 *      bitfield_tuple<T0..T9> const& operator=(_self const& x);
 *          copy assignment operator.
 *
 *
 *  bitfield_tuple Member Functions.
 *
 *      Rationale for creation of data member function.
 *      This is the only way I was able to make the boost.fusion extension 
 *      work for this data structure. Thats the reason that data returns
 *      a reference instead of a copy of the integer value.
 *
 *      storage_type const& data() const; 
 *          - returns a const reference to the data inside of the 
 *          bitfield_tuple.
 *
 *      storage_type& data(); 
 *          - returns a reference to the data inside of the bitfield_tuple.
 *          
 *  get function
 *      - The get function returns a bitfield_ref or const_bitfield_ref which is 
 *      convertible to the type specified in the first template parameter of the
 *      member type, that corresponds to the bit field being accessed. For
 *      example, using example_type from the first example, if the get function
 *      is called in the following manner, temp.get<bool_one>(), the return type
 *      for get in this case is a bitfield_ref type thats convertible to bool and 
 *      information retrieving data or storing data in the correct bitfield.
 *
 *      bitfield_ref<typename> get<typenam>(); // access by name.
 *      const_bitfield_ref<typename> get<typenam>() const; // access by name.
 *
 *      bitfield_ref<typename> get<size_t>(); // access by index.
 *      const_bitfield_ref<typename> get<size_t>() const; // access by index.
 *
 *
 *  All get functions provide constant time run time access to the bitfield to
 *  which they correspond.
 *
 *  Currently they get function WILL NOT throw if the value provided exceeds the
 *  capacity of the number of bits being used to store a particular value.
 *
 *  If an invalid name or index is provided to a get function it will result in 
 *  look up failure.
 *
 *
 *  Enforced preconditions of the bitfield_tuple template
 *
 *      All of these preconditions listed here result in static assertions if 
 *  the bitfield_tuple template is instantiated. 
 *
 *  1) The storage policy must not be specified more then once within the 
 *      template parameters of bitfield_tuple template.
 *
 *  2) A member can not have the same name type as another member.
 *
 *  3) The width of a member can not be 0.
 *  
 *  4) The width of a member can not exceed the width of its return type.
 *
 *  5) The user must specify a storage type.
 *  
 *
 *  6) The total width of the sum of all bit fields must not exceed the bit 
 *      width of the storage type.
 *
 *
 *  bitfield_tuple boost.fusion extension
 *
 *      The bitfield_tuple use boost.fusion to provide additional runtime 
 *  support. bitfield_tuple acts like an associative array, in terms of 
 *  boost.fusion extension support.
 *
 *  The bitfield_tuple supports the following fusion functions and 
 *  meta-functions.
 *
 *  begin
 *  end 
 *  is_sequence
 *  is_view
 *  category_of
 *  at
 *  size
 *  at_key
 *  tag_of
 *  and all intrinsic functions which are available because of the 
 *  implementation of the above functions.
 *
 *
 *  bitfield_tuple fusion iterator support for functions and meta functions.
 *
 *  deref
 *  value_of
 *  next
 *  prior
 *  distance
 *  key_of
 *  value_at_data
 *  deref_data
 *  advance
 *
 *  Additional intrinsic functions are available for use with this iterator. 
 *
 *  The following is the category used to define the bitfield_tuple_iterators
 *  iterator_facade category.
 *
 *  struct bft_category
 *       : boost::fusion::random_access_traversal_tag,
 *         boost::fusion::associative_tag
 *   { };
 *
 *
 *  future work/plans/thoughts
 *  1) Switch to optionally deduced internal type.
 *  2) Switch the internal storage to an array which will allow the user
 *      to store as many fields as the which to, with out the constraint of
 *      being limited to an integral type. 
 *  3) Complete to_string member function.
 *              I don't think I need this function.
 *  4) the type restrictions on storage may need to be relaxed.
 *  5) The if the storage type is a reference then possible allow for this type
 *      to behave as reference to a bitfield_tuple instead of a bitfield type.
 *      This would mean disabling the default constructor.
 *
 *  6) For second draft I would like to restructure the way the parsed 
 *      arguments are stored. Change from bitfield_elements to a pair
 *      the first item being the key/name the second item being a bitfield.
 *      Instead of returning a bitfield_ref class return a bitfield class. 
 *      this may be slightly more difficult then needed, because I need to store 
 *      both a const and non const version of a bitfield. But even I don't do 
 *      that I would like to change the way the reference class works.
 */
template <  BOOST_MAKE_BFT_TEMPLATE_PARAMS() >
struct bitfield_tuple
    : protected details::bitfield_tuple_base<
        BOOST_BFT_PARAMETER_LIST()
    >
{
private:
    typedef details::bitfield_tuple_base<BOOST_BFT_PARAMETER_LIST()> _base;
    typedef bitfield_tuple<BOOST_BFT_PARAMETER_LIST()>  _self;

public:
    typedef typename _base::field_vector            members;
    typedef typename _base::storage_type            storage_type;
    typedef typename _base::offset                  bits_used;


    /** Second revision of the proxy reference type this one deal better 
     *  with const qualifiers and reduce the number of reference types from
     *  two to one.
     */
    template <typename BitfieldElement>
    struct bitfield_reference {
    private:
        typedef bitfield_reference<BitfieldElement>     _self;
    public:
        typedef typename BitfieldElement::return_type   return_type;
        typedef typename mpl::if_<
            is_const< BitfieldElement >,
            const storage_type,
            storage_type
        >::type                                         storage_t;
            
        // typedef typename Bitfield 
        typedef integer::bitfield<
            storage_t,
            BitfieldElement::offset::value,
            BitfieldElement::offset::value
              +
            BitfieldElement::field_width::value - 1,
            return_type
        >                                               field_type;

        /** Reference constructor. */
        explicit bitfield_reference(storage_t& field)
            :field_( field )
        { }


        /** copy constructor.
         *  This is because references are copy constructible.
         */
        bitfield_reference( _self const& x)
           :field_( x.field_ )
        { }
        
        /** Implicit conversion operator 
         *  Returns the value retrieved from the mask.
         */
        inline operator return_type() const {
            return field_.get();
        }

        /** Assignment Of return_type into reference.
         *  This allows values to be assigned to the get function, as part of 
         *  the tuple like interface.
         */
        _self const& operator=(return_type const& rhs) {
            field_.set( rhs );
            return *this;
        }

    private:
        field_type field_;
    };

    /** Value constructor.
     *  This sets the initial value of the internal data to x.
     *  Also functions as the default constructor.
     */
    explicit bitfield_tuple(storage_type x = 0)
        :_data(x)
    { }

    /** Copy constructor. */
    bitfield_tuple(_self const& x)
        :_data( x.data() )
    { }

    /** Assignment from an integer
     *  Allows for the user to assign a type which they wish for this type to
     *  access.
     */
    _self const& operator=( storage_type const& new_value) {
        _data = new_value;
    }

    /** Copy Assignment Operator.
     *  Provides assignment from this type to another.
     */
    _self const& operator=(_self const& x) {
        _data = x._data;
    }



    /** Internal storage accessors
     *  Returns a reference of the internally stored type. This provides the
     *  ability for the fusion sequence extension to work correctly.
     */
    //@{
    storage_type data( ) const {
         return _data;
    }
    //@}

    /** Get function interfaces.
     *  These provide access into the tuple via "reference".
     *  If an invalid index or name is provided then then the user will cause
     *  look up failure.
     */
    //@{
    template <typename Name>
    inline typename details::disable_if_reference_type_by_name<_self,Name>::type
    get() {
        typedef typename details::disable_if_reference_type_by_name<
            _self,
            Name
        >::type reference_info;
        return reference_info( _data );
    }


    template <typename Name>
    inline typename details::disable_if_reference_type_by_name<
        const _self,
        Name
    >::type
    get() const {
        typedef typename details::disable_if_reference_type_by_name<
            const _self,
            Name
        >::type         reference_info;
        return reference_info( _data );
    }

    template <std::size_t Index>
    inline typename details::enable_if_reference_type_by_index<
        _self,
        Index
    >::type
    get() {
        typedef typename details::enable_if_reference_type_by_index<
            _self,
            Index
        >::type             reference_info;
        return reference_info(_data);
    }

    template <std::size_t Index>
    inline typename details::enable_if_reference_type_by_index<
        const _self,
        Index
    >::type
    get() const {
        typedef typename details::enable_if_reference_type_by_index<
            const _self,
            Index
        >::type                  reference_info;
        return reference_info( _data );
    }
    //@}

private:
    storage_type _data;
};

} // end boost



#endif
