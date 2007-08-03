// MscProperty system property_group.h file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_GROUP
#define BOOST_PROPERTY_GROUP

#include "map_key_value_iterators.h"
#include "EditorTypeFinder.h"
#include "property_info.h"
#include "action_info.h"
#include "property_manager.h"
#include <set>
#include <list>
#include <sstream>

#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4561)
#endif
#include <boost/bind.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/function.hpp>
#include <boost/any.hpp>
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

#define BOOST_SETTER(c) boost::bind(c, this, _1)
#if defined(BOOST_MSVC) && (_MSC_VER > 1310)
    #define BOOST_GETTER(c) boost::bind(boost::mem_fn(c), this)
#else
    #define BOOST_GETTER(c) boost::bind(c, this)
#endif
#define BOOST_ACTION(c) boost::bind(c, this)

namespace boost { namespace pinhole
{
    template<typename T>
    struct property_system_var_setter
    {
        typedef T result_type;
        
        property_system_var_setter(T &t) : var(t){}
            
        void operator()(const T &value)
        {
            var = value;
        }
        
        T &var;
    };

    template<typename T>
    inline boost::function<void (const T&)> property_system_var_setter_builder(T &t)
    {
        return boost::bind<void>(property_system_var_setter<T>(t), _1);
    }

    #define BOOST_SETTER_VAR(c) property_system_var_setter_builder(c)

    template<typename T>
    struct property_system_var_getter
    {
        typedef T result_type;
        
        property_system_var_getter(T &t) : var(t){}
            
            T operator()()
        {
                return var;
        }
        
        T &var;
    };

    template<typename T>
    inline boost::function<T ()> property_system_var_getter_builder(T &t)
    {
        return boost::bind<T>(property_system_var_getter<T>(t));
    }

    #define BOOST_GETTER_VAR(c) property_system_var_getter_builder(c)

    #define BOOST_SETTER_NONE NULL
    #define BOOST_GETTER_NONE NULL

    class property_group;

    typedef std::set<std::string> category_collection; 
    typedef std::list<property_group*> children_collection;
    typedef std::map<std::string, detail::property_info_base*> property_collection;
    typedef std::map<std::string, detail::action_info*> action_collection;

    /**
     * Manages a list of properties for an object. A property is defined by a
     * string that is unique to the object and a set of getter and setter
     * functions that set and retrieve values.
     */
    class property_group
    {
    public:
        /**
         * Constructor.
         * @param name The name of this property group
         * @param parent The parent of the this object or NULL if it is a root group.
         */
        property_group( std::string name, property_group *parent=NULL ) :
            m_name( name ), 
            m_parent( parent )
        {
                setup_parent_and_category_and_manager();
        }
        
        explicit property_group( const property_group& old_property_group )
        {
            m_name    = old_property_group.m_name;
            m_parent = old_property_group.m_parent;
            
            setup_parent_and_category_and_manager();
        }

        /** Destructor. */
        virtual ~property_group()
        {
            // Calling OnParentDeleting will likely modify m_children_collection,
            // so we can't use it directly.
            children_collection temp_child(m_children_collection);
            children_collection::iterator childIter    = temp_child.begin();
            children_collection::iterator childIterEnd = temp_child.end();
            for( ; childIter != childIterEnd; ++childIter )
            {
                (*childIter)->set_parent(NULL);
            }
            temp_child.clear();
            
            if ( NULL != m_parent )
            {
                m_parent->remove_child(this);
            }
            
            if ( property_manager::instance() != NULL )
            {
                // Unregister this group with this property manager...
                property_manager::instance()->unregister_property_group( this, m_category_collection );
            }
            
            // cleanup all the property_manager classes
            property_collection::iterator propItr    = m_properties.begin();
            property_collection::iterator propItrEnd = m_properties.end();
            for( ; propItr != propItrEnd; ++propItr )
            {
                delete (*propItr).second;
                (*propItr).second = NULL;
            }
            m_properties.clear();
            
            // cleanup all the action classes
            action_collection::iterator actionItr    = m_actions.begin();
            action_collection::iterator actionItrEnd = m_actions.end();
            for( ; actionItr != actionItrEnd; ++actionItr )
            {
                delete (*actionItr).second;
                (*actionItr).second = NULL;
            }
            m_actions.clear();
        }
        

        /**
        * Gets the parent of this property group.
        * @return The parent of this property group.
        */
        property_group* get_parent() const
        {
            return( m_parent );
        }

        /**
        * Sets the parent of this property group.
        * @param new_parent The new parent of this property group.
        */
        void set_parent(property_group* new_parent)
        {
            if( NULL != property_manager::instance() )
            {
                // Register this group with this property manager...
                property_manager::instance()->unregister_property_group(this, m_category_collection);
            }
            
            if ( NULL != m_parent )
            {
                m_parent->remove_child(this);
            }
            
            m_parent = new_parent;
            
            
            if ( NULL != m_parent )
            {
                m_parent->add_child(this);
            }
            
            if( NULL != property_manager::instance() )
            {
                // Register this group with this property manager...
                property_manager::instance()->register_property_group(this);
            }
        }

        /**
        * Gets the name of this property group.
        * @return The name of this property group.
        */
        const std::string get_name() const
        {
            return( m_name );
        }

        /**
        * Gets the category collection of this property group.
        * @return The category collection of this property group.
        * @todo Remove this accessor
        */
        const category_collection& get_category_collection()
        {
            return( m_category_collection );
        }

        /**
        * Gets the property group children collection of this property group.
        * @return The property group children collection of this property group.
        * @todo Remove this accessor
        */
        children_collection& get_children_collection()
        {
            return( m_children_collection );
        }

        /** @name Properties */
        //@{
            typedef property_collection::size_type prop_size_type;
            typedef map_key_iterator<std::string, property_collection::iterator> prop_iterator;
            typedef map_key_iterator<std::string, property_collection::const_iterator> const_prop_iterator;

            /**
             * Retrieves an iterator pointing to the name of the first property.
             */
            prop_iterator prop_begin()
            {
                return make_map_key_iterator<string>( m_properties.begin() );
            }

            /**
             * Retrieves an iterator pointing to the name of the first property.
             */
            const_prop_iterator prop_begin() const
            {
                return make_map_key_iterator<string>( m_properties.begin() );
            }

            /**
             * Retrieves an iterator pointing to the end of the property list.
             */
            prop_iterator prop_end()
            {
                return make_map_key_iterator<string>( m_properties.end() );
            }

            /**
             * Retrieves an iterator pointing to the end of the property list.
             */
            const_prop_iterator prop_end() const
            {
                return make_map_key_iterator<string>( m_properties.end() );
            }

            /**
             * Retrieves the number of properties.
             */
            prop_size_type prop_count() const
            {
                return m_properties.size();
            }

            /**
             * Gets a property's value by it's type.
             * @param property The name of the property.
             * @return The value of the property.
             * @throw boost::bad_function_call There isn't a get_as_string function associated with this property.
             * @throw std::out_of_range The property requested does not exist.
             * @throw std::bad_cast The requested return_type was not the same as the property's type.
             */
            template<typename Return_Type>
            Return_Type get(const std::string &property) const
            {
                // The system does not allow you to use pointers as property
                // types due to the ambiguity of their use.
                BOOST_STATIC_ASSERT(false == boost::is_pointer<Return_Type>::value);
                
                property_collection::const_iterator itemItr = m_properties.find(property);
                
                if( m_properties.end() != itemItr )
                {
                    detail::property_info_base* propInfo = (*itemItr).second;
                    
                    // throws boost::bad_function_call if there isn't a get_as_string
                    // function associated with this property.
                    if( NULL != propInfo && typeid(Return_Type) == propInfo->m_type )
                    {
                        return static_cast<detail::property_info<Return_Type>*>(propInfo)->getter();
                    }
                    
                    throw std::bad_cast();
                }
                
                stringstream err;
                err << "The requested property \"" << property << "\" does not exist.";
                throw std::out_of_range(err.str().c_str());                
            }
            
            /**
             * Sets a property's value.
             * @param property The name of the property.
             * @param value The value to set on the property.
             * @throw boost::bad_function_call There isn't a set_as_string function associated with this property.
             * @throw std::out_of_range The property requested does not exist.
             * @throw std::bad_cast The Set_Type was not the same as the property's type.
             */
            template<typename Set_Type>
                void set(const std::string &property, Set_Type value) const
            {
                // The system does not allow you to use pointers as property
                // types due to the ambiguity of their use.
                BOOST_STATIC_ASSERT(false == boost::is_pointer<Set_Type>::value);
                
                property_collection::const_iterator itemItr = m_properties.find(property);
                
                if( m_properties.end() != itemItr )
                {
                    detail::property_info_base* propInfo = (*itemItr).second;

                    // throws  boost::bad_function_call if there isn't a get_as_string
                    // function associated with this property.
                    if( NULL != propInfo && typeid(Set_Type) == propInfo->m_type )
                    {
                        return static_cast<detail::property_info<Set_Type>*>(propInfo)->setter(value);
                    }
                    
                    throw std::bad_cast();
                }
                
                stringstream err;
                err << "The requested property \"" << property << "\" does not exist.";
                throw std::out_of_range(err.str().c_str());                
            }

            /**
             * Sets a property's value.
             * @param property The name of the property.
             * @param value A String representation of the value to set on the property.
             * @throw boost::bad_function_call There isn't a set_as_string function associated with this property.
             * @throw std::out_of_range The property requested does not exist.
             * @throw std::invalid_argument The value string could not be converted to the
             * type expected by the internal setter function.
             */
            void set_as_string(const std::string &property, const std::string &value)
            {
                property_collection::iterator itemItr = m_properties.find(property);
                
                if( m_properties.end() != itemItr )
                {
                    // throws  boost::bad_function_call if there isn't a set_as_string
                    // function associated with this property.
                    (*itemItr).second->set_as_string(value);
                }
                else
                {
                    stringstream err;
                    err << "The requested property \"" << property << "\" does not exist.";
                    throw std::out_of_range(err.str().c_str());
                }
            }

            /**
             * Gets a property's value.
             * @param property The name of the property.
             * @return A String representation of the value of the property.
             * @throw boost::bad_function_call There isn't a get_as_string function associated with this property.
             * @throw std::out_of_range The property requested does not exist.
             */
            std::string get_as_string(const std::string &property) const
            {
                property_collection::const_iterator itemItr = m_properties.find(property);
                
                if( m_properties.end() != itemItr )
                {
                    // throws  boost::bad_function_call if there isn't a get_as_string
                    // function associated with this property.
                    return (*itemItr).second->get_as_string();
                }
                
                stringstream err;
                err << "The requested property \"" << property << "\" does not exist.";
                throw std::out_of_range(err.str().c_str());    
            }

            /**
            * Gets a property's Editor object.
            * @param property The name of the property.
            * @return The Editor of the property.
            * @throw no_metadata_defined_error There isn't a property editor associated with this property.
            * @throw std::out_of_range The property requested does not exist.
            */
            const Editor* get_metadata(const std::string &property) const
            {
                property_collection::const_iterator itemItr = m_properties.find(property);
                
                if( itemItr != m_properties.end() )
                {
                    // throws  boost::bad_function_call if there isn't a get
                    // function associated with this property.
                    if ( (*itemItr).second->m_editor == NULL )
                    {
                        throw( no_metadata_defined_error() );
                    }
                    else
                    {
                        return( (*itemItr).second->m_editor );
                    }
                }
                else
                {
                    stringstream err;
                    err << "The requested property \"" << property << "\" does not exist.";
                    throw std::out_of_range(err.str().c_str());
                }
            }

            /**
            * Gets a property's type_info structure.
            * @param property The name of the property.
            * @return The type_info structure of the property.
            * @throw std::out_of_range The property requested does not exist.
            */
            const type_info& get_type_info(const std::string &property) const
            {
                property_collection::const_iterator itemItr = m_properties.find(property);
                
                if( itemItr != m_properties.end() )
                {
                    return (*itemItr).second->m_type;
                }
                else
                {
                    stringstream err;
                    err << "The requested property \"" << property << "\" does not exist.";
                    throw std::out_of_range(err.str().c_str());
                }
            }

            /**
            * Retrieves whether the property is read only or not.
            * @param property The name of the property.
            * @retval true The property is read only.
            * @retval false The property is writeable.
            * @throw std::out_of_range The property requested does not exist.
            */
            bool is_read_only(const std::string &property) const
            {
                property_collection::const_iterator itemItr = m_properties.find(property);
            
                if( itemItr != m_properties.end() )
                {
                    return (*itemItr).second->is_read_only();
                }
                else
                {
                    stringstream err;
                    err << "The requested property \"" << property << "\" does not exist.";
                    throw std::out_of_range(err.str().c_str());
                }
            }
        //@}

        /** @name Actions */
        //@{
            typedef action_collection::size_type action_size_type;
            typedef map_key_iterator<std::string, action_collection::iterator> action_iterator;
            typedef map_key_iterator<std::string, action_collection::const_iterator> const_action_iterator;

            /**
             * Retrieves an iterator pointing to the name of the first property.
             */
            action_iterator action_begin()
            {
                return make_map_key_iterator<string>( m_actions.begin() );
            }

            /**
             * Retrieves an iterator pointing to the name of the first property.
             */
            const_action_iterator action_begin() const
            {
                return make_map_key_iterator<string>( m_actions.begin() );
            }

            /**
             * Retrieves an iterator pointing to the end of the property list.
             */
            action_iterator action_end()
            {
                return make_map_key_iterator<string>( m_actions.end() );
            }

            /**
             * Retrieves an iterator pointing to the end of the property list.
             */
            const_action_iterator action_end() const
            {
                return make_map_key_iterator<string>( m_actions.end() );
            }

            /**
             * Retrieves the number of properties.
             */
            action_size_type action_count() const
            {
                return m_actions.size();
            }

            /**
             * Triggers an action.
             * @param action The name of the action.
             * @throw std::out_of_range The property requested does not exist.
             */
            void trigger(const std::string &action) const
            {
                action_collection::const_iterator itemItr = m_actions.find(action);
                
                if( m_actions.end() != itemItr )
                {
                    // throws  boost::bad_function_call if there isn't a set
                    // function associated with this property.
                    (*itemItr).second->trigger();
                }
                else
                {
                    stringstream err;
                    err << "The requested action \"" << action << "\" does not exist.";
                    throw std::out_of_range(err.str().c_str());
                }
            }

        //@}

    protected:

        /**
         * Adds a property to the property list.
         * @param The name of the property.
         * @param description A brief description of the property for the user interface.
         * @param setter The function used to set the property.
         * @param getter The function used to get the property.
         */
        template<typename Value_Type>
        void add_property( std::string name, 
                           std::string description,
                           boost::function<void (const Value_Type&)> setter, 
                           boost::function<Value_Type ()> getter )
        {
            typedef typename detail::EditorTypeFinder<Value_Type>::type editor_type;

            // You are using a Value_Type that does not have a default editor defined. Use once
            // of the add_property functions where you explicitly define the editor or editor type.
            BOOST_STATIC_ASSERT((false == boost::is_same<editor_type, boost::mpl::void_>::value));

            add_property<Value_Type>( name, description, setter, getter, new editor_type() );
        }

        /**
         * Adds a property to the property list.
         * @param The name of the property.
         * @param description A brief description of the property for the user interface.
         * @param setter The function used to set the property.
         * @param getter The function used to get the property.
         * @param pEditor A pointer to the editor to be used with this property, or null
         *                if there isn't one.
         */
        template<typename Value_Type>
        void add_property( std::string name, 
                           std::string description,
                           boost::function<void (const Value_Type&)> setter, 
                           boost::function<Value_Type ()> getter,
                           Editor *pEditor )
        {
            // If you get an error here, then the type you are using for the property likely doesn't have a proper operator<< for it
            detail::property_info<Value_Type> *prop = new detail::property_info<Value_Type>();

            prop->m_name        = name;
            prop->m_description = description;
            if( BOOST_SETTER_NONE != setter )
            {
                prop->setter  = setter;
            }
            if( BOOST_GETTER_NONE != getter )
            {
                prop->getter  = getter;
            }
            prop->m_editor  = pEditor;

            m_properties.insert( std::make_pair(name, prop) );
        }

        /**
         * Adds an action to the action list.
         * @param The name of the action.
         * @param description A brief description of the action for the user interface.
         * @param action The function used to trigger the action.
         */
        void add_action( std::string name, 
                         std::string description,
                         boost::function<void ()> action )
        {
            detail::action_info *action_info = new detail::action_info();
            
            action_info->m_name        = name;
            action_info->m_description = description;
            action_info->m_action      = action;
            
            m_actions.insert( std::make_pair(name, action_info) );
        }

        /**
        * Adds this property to the specified category.
        * @param category_name The name of the category to add.
        */
        void add_category( const std::string &category_name )
        {
            m_category_collection.insert( category_name );
            
            // notify the Property Manager of this new category
            if ( property_manager::instance() != NULL )
            {
                property_manager::instance()->add_category( category_name, this );
            }
        }

        #if defined(BOOST_MSVC)
            #pragma warning(push)
            #pragma warning( disable: 4251 )
        #endif
            category_collection m_category_collection;
            children_collection m_children_collection;
            property_collection m_properties;
            action_collection m_actions;
        #if defined(BOOST_MSVC)
            #pragma warning(pop)
        #endif

    private:
        property_group();

        void add_child(property_group* pChild)
        {
            if( NULL != pChild )
            {
                m_children_collection.push_back( pChild );
            }
        }
        
        void remove_child(property_group* pChild)
        {
            if( NULL != pChild )
            {
                m_children_collection.remove(pChild);
            }
        }
            
        void setup_parent_and_category_and_manager()
        {
            if ( NULL != m_parent )
            {
                m_parent->add_child(this);
            }
            
            add_category( "All" );
            
            if ( property_manager::instance() != NULL )
            {
                // Register this group with this property manager...
                property_manager::instance()->register_property_group( this );
            }
        }        

        std::string m_name;
        property_group *m_parent;
    };

}}

#endif // include guard