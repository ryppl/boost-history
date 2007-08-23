// MscProperty system property_manager.h file
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROPERTY_MANAGER
#define BOOST_PROPERTY_MANAGER

#include "exceptions.h"
#include <set>
#include <string>

#if defined(BOOST_MSVC)
    #pragma warning(push)
    #pragma warning( disable: 4561 4793 )
#endif
#include <boost/shared_ptr.hpp>
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
    
    class event_source
    {
    public :
        static event_source* instance()
        {
            if ( m_instance == 0 )  // is it the first call?
            {  
                m_instance = new event_source; // create sole instance
            }
            
            return m_instance; // address of sole instance
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
        static event_source *m_instance;
        event_source(){};
        
    };

    class property_manager
    {
    private:
        static void deleter(property_manager* manager)
        {
            delete manager;
        }
        
    public:
        typedef multimap<string, property_group*> category_to_property_group_map;
        typedef map_value_iterator<category_to_property_group_map::iterator> iterator;
        typedef map_value_iterator<category_to_property_group_map::const_iterator> const_iterator;
        
        static property_manager* instance()
        {
            if ( m_instance.get() == NULL )  // is it the first call?
            {  
                m_instance.reset( new property_manager, property_manager::deleter ); // create sole instance
            }
            
            return m_instance.get(); // address of sole instance
        }
        
        static bool exists()
        {
            return m_instance.get() != NULL;
        }
        
        static void delete_instance()
        {
            if( m_instance.get() != NULL )
            {
                m_instance.reset();
            }
        }
        
    protected:
        property_manager(){;}
    
    // TODO: This needs to be protected so no-one will deal with it, but
    // checked_delete can't be made a friend in gcc, so I can't shared_ptr
    // to work.
    public:
        virtual ~property_manager()
        {
            category_to_property_group_map::iterator itr     = m_property_group_collection.begin();
            category_to_property_group_map::iterator itr_end = m_property_group_collection.end();
            for( ; itr != itr_end; ++itr )
            {
                event_source::instance()->raise_on_remove_event((*itr).second);
            }
        }
        
    public:        
        /**
         * Retrieves an iterator pointing to the first property group.
         */
        iterator begin()
        {
            return iterator( m_property_group_collection.lower_bound("All"),
                             m_property_group_collection.upper_bound("All") );
        }
        
        /**
         * Retrieves an iterator pointing to the first property group.
         */
        const_iterator begin() const
        {
            return const_iterator( m_property_group_collection.lower_bound("All"),
                                   m_property_group_collection.upper_bound("All") );
        }
        
        /**
         * Retrieves an iterator pointing to the end of the root property list.
         */
        iterator end()
        {
            return iterator( m_property_group_collection.upper_bound("All") );
        }
        
        /**
         * Retrieves an iterator pointing to the end of the root property list.
         */
        const_iterator end() const
        {
            return const_iterator( m_property_group_collection.upper_bound("All") );
        }
        
        /**
         * Retrieves the number of property groups.
         */
        size_t count() const
        {
            return m_property_group_collection.count("All");
        }
        
        /**
         * Retrieves an iterator pointing to the first property group for a specified category.
         */
        iterator begin(const string& strCategory)
        {
            return iterator( m_property_group_collection.lower_bound(strCategory) );
        }
        
        /**
         * Retrieves an iterator pointing to the first property group for a specified category.
         */
        const_iterator begin(const string& strCategory) const
        {
            return const_iterator( m_property_group_collection.lower_bound(strCategory) );
        }
        
        /**
         * Retrieves an iterator pointing to the end of the root property list for a specified category.
         */
        iterator end(const string& strCategory)
        {
            return iterator( m_property_group_collection.upper_bound(strCategory) );
        }
        
        /**
         * Retrieves an iterator pointing to the end of the root property list for a specified category.
         */
        const_iterator end(const string& strCategory) const
        {
            return  const_iterator( m_property_group_collection.upper_bound(strCategory) );
        }
        
        /**
         * Retrieves the number of property groups for a specified category.
         */
        size_t count(const string& strCategory) const
        {
            return m_property_group_collection.count(strCategory);
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
        virtual void unregister_property_group( property_group *group, category_collection &categories )
        {
                category_collection::const_iterator categoryItr = categories.begin();
                category_collection::const_iterator categoryEnd = categories.end();
                for ( ; categoryItr != categoryEnd; ++categoryItr )
                {
                    remove_category( *categoryItr, group );
                }
                
                event_source::instance()->raise_on_remove_event( group );
        }
        
        /**
         * Adds a new category for the property group.
         */
        virtual void add_category( const string &category_name, property_group *group )
        {
            m_category_collection.insert( category_name );
            m_property_group_collection.insert( make_pair( category_name, group ) );
        }
        
        virtual void remove_category( const string &category_name, property_group *group )
        {
            category_to_property_group_map::iterator pgItr;
            for ( pgItr = m_property_group_collection.find( category_name ); 
                  pgItr != m_property_group_collection.end(); )
            {
                if ( pgItr->second == group )
                {
                    m_property_group_collection.erase( pgItr++ );
                }
                else
                {
                    pgItr++;
                }
            }
        }
        
    protected:
        #if defined(BOOST_MSVC)
            #pragma warning(push)
            #pragma warning( disable: 4251 )
        #endif
            static boost::shared_ptr<property_manager> m_instance;
            category_to_property_group_map m_property_group_collection;
            category_collection m_category_collection;
        #if defined(BOOST_MSVC)
            #pragma warning(pop)
        #endif
            
        friend class property_group;
    };
}}

#endif // include guard