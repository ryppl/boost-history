// Pinhole property_manager.hpp file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_INFO
#define BOOST_PROPERTY_INFO

#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4561 4793 )
#endif
#include <boost/type_traits.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/format.hpp>
#include <boost/any.hpp>
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

namespace boost { namespace pinhole { namespace detail
{
    #define BOOL_TRUE "True"
    #define BOOL_FALSE "False"
    #define W_BOOL_TRUE L"True"
    #define W_BOOL_FALSE L"False"

    ///////////////////////////////////////////////////
    //               set_as_string Override Functors
    ///////////////////////////////////////////////////

    /**
     * The default setter processor. This uses lexical cast
     * to convert the passed in set string to any type that
     * has proper stream operators.
     */
    template<typename String_Type, typename Value_Type>
    struct internal_string_set
    {
        template<typename Set_Type>
        inline void operator()( Set_Type setter, String_Type value )
        {
            try
            {
                setter( boost::lexical_cast<Value_Type>(value) );
            }
            catch(boost::bad_lexical_cast &)
            {
                throw std::invalid_argument( "An invalid property value was used to set." );
            }
        }
    };

    /**
     * Setter for handling string types. Since a string was passed in, and
     * the type in the setter function is a string, we don't need to do any
     * conversion.
     */
    template<>
    struct internal_string_set<std::string, std::string>
    {
        template<typename Set_Type>
        inline void operator()( Set_Type setter, std::string value )
        {
            setter( value );
        }
    };

    /**
     * Setter for handling string types. Since a string was passed in, and
     * the type in the setter function is a string, we don't need to do any
     * conversion.
     */
    template<>
    struct internal_string_set<std::wstring, std::wstring>
    {
        template<typename Set_Type>
        inline void operator()( Set_Type setter, std::wstring value )
        {
            setter( value );
        }
    };

    /**
     * Setter for incompatible string types.
     */
    template<>
    struct internal_string_set<std::wstring, std::string>
    {
        template<typename Set_Type>
        inline void operator()( Set_Type setter, std::wstring value )
        {
            throw std::invalid_argument( "A wstring cannot be used to set a string." );
        }
    };

    /**
     * Setter for incompatible string types.
     */
    template<>
    struct internal_string_set<std::string, std::wstring>
    {
        template<typename Set_Type>
        inline void operator()( Set_Type setter, std::string value )
        {
            throw std::invalid_argument( "A string cannot be used to set a wstring." );
        }
    };

    /**
    * Setter for handling bool types. Since a bool was passed in, 
    * we need to convert 
    */
    template<>
    struct internal_string_set<std::string, bool>
    {
        template<typename Set_Type>
        inline void operator()( Set_Type setter, std::string value )
        {
            setter( boost::iequals(value, BOOL_TRUE) || value == "1" );
        }
    };


    /**
    * Setter for handling bool types. Since a bool was passed in, 
    * we need to convert 
    */
    template<>
    struct internal_string_set<std::wstring, bool>
    {
        template<typename Set_Type>
        inline void operator()( Set_Type setter, std::wstring value )
        {
            setter( boost::iequals(value, W_BOOL_TRUE) || value == L"1" );
        }
    };

    ///////////////////////////////////////////////////
    //               get_as_string Override Functors
    ///////////////////////////////////////////////////

    /**
     * The default getter processor. This uses lexical cast
     * to convert the returned value from the propertied
     * getter function and convert to a string using
     * lexical_cast. Any type that has proper stream operators
     * should work.
     */

    template<class String_Type, typename Value_Type>
    struct internal_string_get
    {
        template<typename Get_Type>
        inline String_Type operator()( Get_Type getter ) const
        {
            return boost::lexical_cast<String_Type>( getter() );
        }
    };

    /**
     * Getter for handling string types. Since a string returned,
     * and getter returns a string type, we don't need to do any
     * conversion.
     */
    template<>
    struct internal_string_get<std::string, std::string>
    {
        template<typename Get_Type>
        inline std::string operator()( Get_Type getter ) const
        {
            return getter();
        }
    };

    /**
    * Getter for handling string types. Since a string returned,
    * and getter returns a string type, we don't need to do any
    * conversion.
    */
    template<>
    struct internal_string_get<std::wstring, std::wstring>
    {
        template<typename Get_Type>
        inline std::wstring operator()( Get_Type getter ) const
        {
            return getter();
        }
    };

    /**
    * Getter for handling incompatible string types.
    */
    template<>
    struct internal_string_get<std::string, std::wstring>
    {
        template<typename Get_Type>
        inline std::string operator()( Get_Type getter ) const
        {
            throw std::invalid_argument( "A wstring property cannot be returned as a string." ); 
        }
    };

    /**
    * Getter for handling incompatible string types.
    */
    template<>
    struct internal_string_get<std::wstring, std::string>
    {
        template<typename Get_Type>
        inline std::wstring operator()( Get_Type getter ) const
        {
            throw std::invalid_argument( "A wstring property cannot be returned as a string." ); 
        }
    };

    /**
    * Getter for handling bool types. Since a string is returned,
    * we need to convert the bool to a string.
    */
    template<>
    struct internal_string_get<std::string, bool>
    {
        template<typename Get_Type>
        inline std::string operator()( Get_Type getter ) const
        {
            return( getter() ? BOOL_TRUE : BOOL_FALSE );
        }
    };

    /**
    * Getter for handling bool types. Since a string is returned,
    * we need to convert the bool to a string.
    */
    template<>
    struct internal_string_get<std::wstring, bool>
    {
        template<typename Get_Type>
        inline std::wstring operator()( Get_Type getter ) const
        {
            return( getter() ? W_BOOL_TRUE : W_BOOL_FALSE );
        }
    };

    struct property_info_base
    {
    public:
        property_info_base(const type_info &type) : 
          m_type(type)
        {;}

        std::string      m_name;
        std::string      m_description;
        boost::any       m_metadata;
        const type_info &m_type;

        virtual void set_as_string(std::string value) = 0;
        virtual void set_as_wstring(std::wstring value) = 0;
        virtual std::string get_as_string() const = 0;
        virtual std::wstring get_as_wstring() const = 0;
        virtual bool is_read_only() const = 0;
    };

    template<typename T>
    struct property_info : property_info_base
    {
    private:

    public:
        typedef typename boost::remove_reference<T>::type Value_Type;
        typedef boost::function<void (const Value_Type&)> setter_type;
        typedef boost::function<Value_Type ()> getter_type;

        // The system does not allow you to use pointers as property
        // types due to the ambiguity of their use.
        BOOST_STATIC_ASSERT(false == boost::is_pointer<Value_Type>::value);

        setter_type setter;
        getter_type getter;

        property_info() : property_info_base(typeid(T)) {;}

        /**
         * Calls the appropriate getter function for this parameter and converts
         * the value to a string for return.
         *
         * @return A String representation of the value of the property.
         * @throw boost::bad_function_call There isn't a get_as_string function associated with this property.
         */
        virtual std::string get_as_string() const
        {
            // If you get an error here, and it complains about:
            // error C2679: binary '<<' : no operator found which takes a right-hand operand of type
            // then the type you are using for the property doesn't have a proper operator<< for it
            //
            // throws  boost::bad_function_call if there isn't a get_as_string function associated with this property.
            return internal_string_get<std::string, Value_Type>()(getter);
        }

        /**
        * Calls the appropriate getter function for this parameter and converts
        * the value to a wide-character string for return.
        *
        * @return A String representation of the value of the property.
        * @throw boost::bad_function_call There isn't a get_as_string function associated with this property.
        */
        virtual std::wstring get_as_wstring() const
        {
            // If you get an error here, and it complains about:
            // error C2679: binary '<<' : no operator found which takes a right-hand operand of type
            // then the type you are using for the property doesn't have a proper operator<< for it
            //
            // throws  boost::bad_function_call if there isn't a get_as_string function associated with this property.
            return internal_string_get<std::wstring, Value_Type>()(getter);
        }

        /**
         * Calls the appropriate setter function for this parameter and converts
         * the passed in string to whatever type the setter is expect. For example,
         * if the properties setter is expecting an int, the string will be
         * converted to an int before being passed to the function.
         * <br><br>
         * We call an internal setter in a struct so that we can  override
         * what the set does based on the type of Value_Type. For example,
         * we wanted to write a special handler for the string type for
         * performance, so we overrode the string handler in "struct Internal<string>"
         * to just pass the string along instead of convert it. The beauty of doing it
         * this way is that there is no runtime cost. All functions are inlined,
         * and the additional function calls and type processing are optimized out
         * at compile time.
         *
         * @param value A String representation of the value to set on the property.
         * @throw boost::bad_function_call There isn't a set_as_string function associated with this property.
         * @throw std::invalid_argument The value string could not be converted to the
         * type expected by the internal setter function.
         */
        virtual void set_as_string(std::string value)
        {
            // see the function documentation for more information about this.
            //
            // throws  boost::bad_function_call if there isn't a set_as_string function associated with this property.
            internal_string_set<std::string, Value_Type>()(setter, value);
        }

        /**
        * Calls the appropriate setter function for this parameter and converts
        * the passed in wide-character string to whatever type the setter is expect. For example,
        * if the properties setter is expecting an int, the string will be
        * converted to an int before being passed to the function.
        * <br><br>
        * We call an internal setter in a struct so that we can  override
        * what the set does based on the type of Value_Type. For example,
        * we wanted to write a special handler for the string type for
        * performance, so we overrode the string handler in "struct Internal<string>"
        * to just pass the string along instead of convert it. The beauty of doing it
        * this way is that there is no runtime cost. All functions are inlined,
        * and the additional function calls and type processing are optimized out
        * at compile time.
        *
        * @param value A String representation of the value to set on the property.
        * @throw boost::bad_function_call There isn't a set_as_string function associated with this property.
        * @throw std::invalid_argument The value string could not be converted to the
        * type expected by the internal setter function.
        */
        virtual void set_as_wstring(std::wstring value)
        {
            // see the function documentation for more information about this.
            //
            // throws  boost::bad_function_call if there isn't a set_as_string function associated with this property.
            internal_string_set<std::wstring, Value_Type>()(setter, value);
        }

        /**
        * Checks if this is a read-only property (ie, cant set)
        * @return true if the property is read-only.
        */
        virtual bool is_read_only() const
        {
            return( !setter );
        }
    };
}}}

#endif // include guard