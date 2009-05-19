// Pinhole property_manager.hpp file
//
// Copyright Jared McIntyre 2007-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_MANAGER
#define BOOST_PROPERTY_MANAGER

#include "exceptions.hpp"
#include "map_key_value_iterators.hpp"
#include "category_iterator.hpp"
#include <set>
#include <string>

#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4561 4793 )
#endif
#include <boost/tr1/memory.hpp>
#include <boost/signals.hpp>
#include <boost/algorithm/string.hpp>
#if defined(BOOST_MSVC)
    #pragma warning(pop)
#endif

namespace boost { namespace pinhole
{
    class property_group;
    
    typedef std::set<std::string> category_collection;
    typedef std::list<property_group*> children_collection;
    
    class BOOST_PINHOLE_DECL event_source
    {
    public :
        static event_source* instance()
        {
            static boost::shared_ptr<boost::pinhole::event_source> instance;

            if ( !instance )  // is it the first call?
            {  
                instance.reset( new event_source ); // create sole instance
            }
            
            return instance.get(); // address of sole instance
        }
        
        #if defined(BOOST_MSVC)
            #pragma warning(push)
            #pragma warning( disable: 4251 )
        #endif
            boost::signal<void(property_group*)> add_event;
            boost::signal<void(property_group*)> remove_event;
        #if defined(BOOST_MSVC)
            #pragma warning(pop)
        #endif
        void raise_on_add_event( property_group *group )
        {
            add_event( group );
        }
        
        void raise_on_remove_event( property_group *group )
        {
            remove_event( group );
        }
        
    private :
        
        event_source(){};
        
    };

    class BOOST_PINHOLE_DECL property_manager
    {
    public:
        typedef std::tr1::shared_ptr<property_manager> instance_type;

    private:
        static void deleter(property_manager* manager)
        {
            delete manager;
        }
        
    public:
        typedef boost::pinhole::children_collection::iterator iterator;
        typedef boost::pinhole::children_collection::const_iterator const_iterator;
        typedef boost::pinhole::category_iterator<boost::pinhole::children_collection::iterator> filtered_iterator;
        typedef boost::pinhole::category_iterator<boost::pinhole::children_collection::const_iterator> const_filtered_iterator;
        
        static instance_type instance()
        {
            if ( !exists() )  // is it the first call?
            {  
                internal_instance().reset( new property_manager, property_manager::deleter ); // create sole instance
            }
            
            return internal_instance(); // address of sole instance
        }
        
        static bool exists()
        {
            return (internal_instance() != NULL);
        }
        
        static void delete_instance()
        {
            internal_instance().reset();
        }
        
    protected:
        property_manager(){;}
    
    // TODO: This needs to be protected so no-one will deal with it, but
    // checked_delete can't be made a friend in gcc, so I can't shared_ptr
    // to work.
    public:
        virtual ~property_manager()
        {
            children_collection::iterator itr     = m_property_group_collection.begin();
            children_collection::iterator itr_end = m_property_group_collection.end();
            for( ; itr != itr_end; ++itr )
            {
                event_source::instance()->raise_on_remove_event((*itr));
            }
        }
        
    public:        
        /**
         * Retrieves an iterator pointing to the first property group.
         */
        iterator begin()
        {
            return m_property_group_collection.begin();
        }
        
        /**
         * Retrieves an iterator pointing to the first property group.
         */
        const_iterator begin() const
        {
            return m_property_group_collection.begin();
        }
        
        /**
         * Retrieves an iterator pointing to the end of the root property list.
         */
        iterator end()
        {
            return m_property_group_collection.end();
        }
        
        /**
         * Retrieves an iterator pointing to the end of the root property list.
         */
        const_iterator end() const
        {
            return m_property_group_collection.end();
        }
        
        /**
         * Retrieves the number of property groups.
         */
        size_t count() const
        {
            return m_property_group_collection.size();
        }
        
        /**
         * Retrieves an iterator pointing to the first property group for a specified category.
         */
        filtered_iterator begin(const std::string& strCategory)
        {
            return make_category_iterator( strCategory, m_property_group_collection.begin(), m_property_group_collection.end() );
        }
        
        /**
         * Retrieves an iterator pointing to the first property group for a specified category.
         */
        const_filtered_iterator begin(const std::string& strCategory) const
        {
            return make_category_iterator( strCategory, m_property_group_collection.begin(), m_property_group_collection.end() );
        }
        
        /**
         * Retrieves an iterator pointing to the end of the root property list for a specified category.
         */
        filtered_iterator end(const std::string& strCategory)
        {
            return make_category_iterator( strCategory, m_property_group_collection.end(), m_property_group_collection.end() );
        }
        
        /**
         * Retrieves an iterator pointing to the end of the root property list for a specified category.
         */
        const_filtered_iterator end(const std::string& strCategory) const
        {
            return make_category_iterator( strCategory, m_property_group_collection.end(), m_property_group_collection.end() );
        }
        
        /**
         * Retrieves the number of property groups for a specified category.
         */
        size_t count(const std::string& strCategory) const
        {
            return std::distance(begin(strCategory), end(strCategory));
        }
        
        /**
         * Gets the master category list.
         * @return master category list.
         */
        const category_collection* get_category_collection()
        {
            return( &m_category_collection );
        }

    protected:

        /** Provides direct access to the shared_ptr that owns the property_manager singleton. */
        static boost::shared_ptr<boost::pinhole::property_manager>& internal_instance()
        {
            static boost::shared_ptr<boost::pinhole::property_manager>
                instance(new boost::pinhole::property_manager);

            return instance;
        }

        /**
         * Register's group with the property_manager.
         */
        virtual void register_property_group( property_group *group )
        {
            event_source::instance()->raise_on_add_event( group );
        }
        
        /**
         * Unregister's group from the property_manager.
         */
        virtual void unregister_property_group( property_group *group )
        {
            event_source::instance()->raise_on_remove_event( group );
        }
        
        /**
         * Adds a root property group.
         */
        virtual void add_property_group( property_group *group )
        {
            m_property_group_collection.push_back( group );
        }

        /**
        * Removes a root property group.
        */
        virtual void remove_property_group( property_group *group )
        {
            m_property_group_collection.remove( group );
        }
        
        /**
         * Adds a new category for the property group.
         */
        virtual void add_category( const std::string &category_name )
        {
            m_category_collection.insert( category_name );
        }
        
    protected:
        #if defined(BOOST_MSVC)
            #pragma warning(push)
            #pragma warning( disable: 4251 )
        #endif
            boost::pinhole::children_collection m_property_group_collection;
            category_collection m_category_collection;
        #if defined(BOOST_MSVC)
            #pragma warning(pop)
        #endif
            
        friend class property_group;
    };
}}

#endif // include guard