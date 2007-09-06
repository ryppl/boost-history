// (C) Copyright The Trustees of Indiana University 2004-2005
// (C) Copyright 2007 Andrew Sutton
//
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Doug Gregor
//           Ronald Garcia
//           Andrew Sutton
//

#ifndef BOOST_PROPERTY_MAP_DYNAMIC_PROPERTY_MAP_HPP
#define BOOST_PROPERTY_MAP_DYNAMIC_PROPERTY_MAP_HPP

#include <typeinfo>
#include <stdexcept>
#include <sstream>
#include <map>

#include <boost/config.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/any.hpp>
#include <boost/function/function3.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type.hpp>

// dynamic_property_map.hpp - Support for runtime-polymorphic property maps.
// This header is factored out of Doug Gregor's routines for reading GraphML
// files for use in reading GraphViz graph files.


namespace boost { namespace property_map {
    namespace detail {
        // A wrapper around lexical_cast, which does not behave as desired
        // for std::string types.
        template<typename Value>
        inline Value read_value(const std::string& value)
        { return boost::lexical_cast<Value>(value); }

        template<>
        inline std::string read_value<std::string>(const std::string& value)
        { return value; }
    }

    //////////////////////////////////////////////////////////////////////
    // Property map exceptions
    //////////////////////////////////////////////////////////////////////

    struct dynamic_property_exception : public std::exception
    {
        virtual ~dynamic_property_exception() throw() {}
        virtual const char* what() const throw() = 0;
    };

    struct property_not_found : public dynamic_property_exception
    {
        std::string property;
        mutable std::string statement;

        property_not_found(const std::string& property)
            : property(property)
        {}

        virtual ~property_not_found() throw()
        {}

        const char* what() const throw()
        {
            if(statement.empty()) {
                statement = std::string("property not found: ") + property + ".";
            }

            return statement.c_str();
        }
    };

    struct dynamic_get_failure : public dynamic_property_exception
    {
        std::string property;
        mutable std::string statement;

        dynamic_get_failure(const std::string& property)
            : property(property)
        {}

        virtual ~dynamic_get_failure() throw()
        {}

        const char* what() const throw()
        {
            if(statement.empty())
                statement =
                        std::string("dynamic property get cannot retrieve value for property: ")
                        + property + ".";

            return statement.c_str();
        }
    };

    struct dynamic_const_put_error  : public dynamic_property_exception
    {
        virtual ~dynamic_const_put_error() throw()
        {}

        const char* what() const throw()
        {
            return "attempt to put a value into a const property map: ";
        }
    };


    // This interface supports polymorphic manipulation of property maps.
    //
    // ANS: Implemented virtual constructor idiom for these types so that we
    // can cleanly implement a copy constructor for dynamic_properties.
    class dynamic_property_map
    {
    public:
        virtual ~dynamic_property_map() { }

        virtual dynamic_property_map* clone() const = 0;

        virtual any get(const any& key) = 0;
        virtual std::string get_string(const any& key) = 0;
        virtual void put(const any& key, const any& value) = 0;
        virtual const std::type_info& key() const = 0;
        virtual const std::type_info& value() const = 0;
    };


    namespace detail {
        // Property-map adaptor to support runtime polymorphism.
        //
        // ANS: This class had some #ifdefs to explicitly quality the put()
        // and get() function for GCC 2.95. I don't think it should be such
        // an issue these days, so I removed them.
        template<typename PropertyMap>
        class dynamic_property_map_adaptor : public dynamic_property_map
        {
            typedef typename property_traits<PropertyMap>::key_type key_type;
            typedef typename property_traits<PropertyMap>::value_type value_type;
            typedef typename property_traits<PropertyMap>::category category;

            // Overloaded dispatches from the put() member function. Attempts to
            // "put" to a property map that does not model WritablePropertyMap
            // result in a runtime exception.
            // in_value must either hold an object of value_type or a string that
            // can be converted to value_type via iostreams.
            void do_put(const any& in_key, const any& in_value, mpl::bool_<true>)
            {
                key_type key = any_cast<key_type>(in_key);
                if(in_value.type() == typeid(value_type)) {
                    boost::property_map::put(property_map, key, any_cast<value_type>(in_value));
                }
                else {
                    // if in_value is an empty string, put a default constructed
                    // value_type.
                    std::string v = any_cast<std::string>(in_value);
                    if(v.empty()) {
                        boost::property_map::put(property_map, key, value_type());
                    }
                    else {
                        boost::property_map::put(property_map, key, detail::read_value<value_type>(v));
                    }
                }
            }

            void do_put(const any&, const any&, mpl::bool_<false>)
            {
                throw dynamic_const_put_error();
            }

        public:
            explicit dynamic_property_map_adaptor(const PropertyMap& property_map)
                : property_map(property_map)
            { }

            dynamic_property_map_adaptor(const dynamic_property_map_adaptor& dp)
                : property_map(dp.property_map)
            { }

            // NOTE: This uses covariant return types that may or may not be
            // supported by other compilers.
            virtual dynamic_property_map_adaptor* clone() const
            {
                return new dynamic_property_map_adaptor(*this);
            }

            virtual boost::any get(const any& key)
            {
                return boost::property_map::get(property_map, any_cast<key_type>(key));
            }

            virtual std::string get_string(const any& key)
            {
                std::ostringstream out;
                out << boost::property_map::get(property_map, any_cast<key_type>(key));
                return out.str();
            }

            virtual void put(const any& in_key, const any& in_value)
            {
                do_put(in_key,
                    in_value,
                    mpl::bool_<is_convertible<category*, writable_property_map_tag*>::value>());
            }

            virtual const std::type_info& key() const
            { return typeid(key_type); }

            virtual const std::type_info& value() const
            { return typeid(value_type); }

            PropertyMap& base()
            { return property_map; }

            const PropertyMap& base() const
            { return property_map; }

        private:
            PropertyMap property_map;
        };
    }

    // A container type for dynamic property maps
    struct dynamic_properties
    {
        typedef std::multimap<std::string, dynamic_property_map*> property_maps_type;

        // This function type is called to generate maps when a put() is
        // invoked on a named property map that isn't in the collection.
        typedef boost::function3<
                std::auto_ptr<dynamic_property_map>,
                const std::string&,
                const boost::any&,
                const boost::any&
            > generate_fn_type;
    public:

        typedef property_maps_type::iterator iterator;
        typedef property_maps_type::const_iterator const_iterator;

        dynamic_properties()
            : generate_fn()
        { }

        dynamic_properties(const generate_fn_type& g)
            : generate_fn(g)
        { }

        dynamic_properties(const dynamic_properties& dp)
            : generate_fn(dp.generate_fn)
        {
            property_maps_type::const_iterator i, end = dp.property_maps.end();
            for(i = dp.property_maps.begin(); i != end; ++i) {
                property_maps.insert(make_pair(i->first, i->second->clone()));
            }
        }

        ~dynamic_properties()
        {
            // Delete any owned property maps
            property_maps_type::iterator i, end = property_maps.end();
            for(i = property_maps.begin(); i != end; ++i) {
                delete i->second;
            }
        }

        template<typename PropertyMap>
        dynamic_properties&
        property(const std::string& name, PropertyMap property_map)
        {
            // TBD: exception safety
            typedef typename detail::dynamic_property_map_adaptor<PropertyMap> map;
            std::auto_ptr<dynamic_property_map> pm(new map(property_map));

            property_maps_type::iterator i =
                property_maps.insert(property_maps_type::value_type(name, 0));
            i->second = pm.release();

            return *this;
        }

        iterator begin()
        { return property_maps.begin(); }

        const_iterator begin() const
        { return property_maps.begin(); }

        iterator end()
        { return property_maps.end(); }

        const_iterator end() const
        { return property_maps.end(); }

        iterator lower_bound(const std::string& name)
        { return property_maps.lower_bound(name); }

        const_iterator lower_bound(const std::string& name) const
        { return property_maps.lower_bound(name); }

        void
        insert(const std::string& name, std::auto_ptr<dynamic_property_map> pm)
        {
            property_maps.insert(property_maps_type::value_type(name, pm.release()));
        }

        template<typename Key, typename Value>
        std::auto_ptr<dynamic_property_map>
        generate(const std::string& name, const Key& key, const Value& value)
        {
            if(!generate_fn) {
                throw property_not_found(name);
            } else {
                return generate_fn(name,key,value);
            }
        }

    private:
        property_maps_type property_maps;
        generate_fn_type generate_fn;
    };

    template<typename Key, typename Value>
    bool
    put(const std::string& name, dynamic_properties& dp, const Key& key, const Value& value)
    {
        dynamic_properties::iterator i, end = dp.end();
        for(i = dp.lower_bound(name); i != end && i->first == name; ++i) {
            if(i->second->key() == typeid(key)) {
                i->second->put(key, value);
                return true;
            }
        }

        std::auto_ptr<dynamic_property_map> new_map = dp.generate(name, key, value);
        if (new_map.get()) {
            new_map->put(key, value);
            dp.insert(name, new_map);
            return true;
        }
        else {
            return false;
        }
    }

#ifndef BOOST_NO_EXPLICIT_FUNCTION_TEMPLATE_ARGUMENTS
    template<typename Value, typename Key>
    Value
    get(const std::string& name, const dynamic_properties& dp, const Key& key)
    {
        dynamic_properties::const_iterator i, end = dp.end();
        for(i = dp.lower_bound(name); i != end && i->first == name; ++i) {
            if(i->second->key() == typeid(key)) {
                return any_cast<Value>(i->second->get(key));
            }
        }

        throw dynamic_get_failure(name);
    }
#endif

    template<typename Value, typename Key>
    Value
    get(const std::string& name, const dynamic_properties& dp, const Key& key, type<Value>)
    {
        dynamic_properties::const_iterator i, end = dp.end();
        for(i = dp.lower_bound(name); i != end && i->first == name; ++i) {
            if(i->second->key() == typeid(key)) {
                return any_cast<Value>(i->second->get(key));
            }
        }

        throw dynamic_get_failure(name);
    }

    template<typename Key>
    std::string
    get(const std::string& name, const dynamic_properties& dp, const Key& key)
    {
        dynamic_properties::const_iterator i, end = dp.end();
        for(i = dp.lower_bound(name); i != end && i->first == name; ++i) {
            if(i->second->key() == typeid(key)) {
                return i->second->get_string(key);
            }
        }

        throw dynamic_get_failure(name);
    }

    // The easy way to ignore properties.
    inline std::auto_ptr<dynamic_property_map>
    ignore_other_properties(const std::string&, const boost::any&, const boost::any&)
    {
        return std::auto_ptr<dynamic_property_map>(0);
    }
} }

#endif
