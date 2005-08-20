//
//  Copyright (c) 2002, 2003 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ChangeLog (latest first):
//   2005-06-08 Larry Evans
//     WHAT:
//       1) made map types to more specific
//       2) changed map_type to struct instead typedef and added
//          map_type::counts_non_zero. Replaced BOOST_CHECK for
//          non_zero count with check for 
//          BOOST_CHECK(get_map().counts_non_zero()).
//       3) added cout_layout::tagged_valid 
//       *) added several comments.
//     WHY:
//       1) easier to understand how it works.
//       2) provide reusable utility function to make it easier to
//          track down source of bugs.
//       3) shorten code repeated in several places.
//   2005-05-28 Larry Evans
//     WHAT:
//       1) copied from boost/smart_ptr/src/sp_collector.cpp.
//       2) nested hooks within new class:
//            collector<sp_counted_tagged>
//       3) changed enclosure of 
//            #if defined(BOOST_SP_ENABLE_DEBUG_HOOKS)
//            ...
//            #endif
//          from essentially all source to just member variable, id, in
//          count_layout.
//       4) replaced boost::detail with boost::policy_ptr::detail
//     WHY:
//       1) to emulate (after appropriate changes) sp_collector
//          for policy_ptr versions of smart pointers collected
//          by sp_collector.cpp.
//       2) see sp_counted_impl_p_rmptr_sp_counted_tagged.hpp
//          in boost/policy_ptr/detail
//       3) The macro, BOOST_SP_ENABLE_DEBUG_HOOKS, originally had two purposes:
//           a) to signal use of debugging tools.
//           b) to signal tagged garbage collection (via sp_collector.cpp).
//          Purpose b) is now fulfilled by template specializations on 
//          sp_counted_tagged.  Purpose a) is now the macro's sole purpose;
//          and to fulfill that purpose, the count_layout::id declaration needs
//          to be surrounded by the aforementioned #if...#endif.
//       4) to reflect policy_ptr names corresponding to those for shared_ptr.
//          

#include <boost/assert.hpp>
#include "boost/policy_ptr/std_ptr.hpp"
#include "boost/policy_ptr/detail/collector_sp_counted_tagged.hpp"
#include <boost/detail/lightweight_mutex.hpp>
#include <cstdlib>
#include <map>
#include <deque>
#include <iostream>

  struct
map_type
: public
  std::map       //For some type, X:
  < boost::policy_ptr::detail::sp_counted_tagged const* //key
  , std::pair
    < unsigned char * //X*, NOTE: there should be a 1-to-1 between key and this value,
                      //which points to start of object.
    , size_t          //sizeof(X)
    > 
  >
{
      bool
    counts_non_zero(void)const
    {
        bool result=true;
        for
          ( const_iterator cur(begin())
          ; cur != end() && result
          ; ++cur
          )
        {
            result = cur->first->use_count() > 0;
            if(!result)
            {
                std::cout<<"count == 0:pn="<<cur->first
                <<":start="<<(void const*)cur->second.first
                <<"\n";
            }
        }
        return result;
    }
};


static map_type & get_map()
{
    static map_type m;
    return m;
}

typedef boost::detail::lightweight_mutex mutex_type;

static mutex_type & get_mutex()
{
    static mutex_type m;
    return m;
}

static void * init_mutex_before_main = &get_mutex();

namespace
{
    class X;
    
        typedef 
      boost::policy_ptr::detail::sp_counted_tagged 
    sp_counted_type
    ;

    struct count_layout
    {
        sp_counted_type* pi_;
        int id_;
         
        sp_counted_type const* pi(void)const
        {
            return pi_;
        }
        sp_counted_type* pi(void)
        {
            return pi_;
        }
        
        bool tagged_valid(void)const
        /**@brief
         *  Is this both tagged and valid?
         */
        {
            bool const result
            =  pi_ != 0
            && id_ == boost::policy_ptr::detail::shared_count_id
            ;
            return result;
        }
              
    };

    struct shared_ptr_layout
    /**@brief
     *  reflect the layout of member variables:
     *    stored_type pointee_;
     *    counter_type count_;
     *  in shared_storage_ in boost/policy_ptr/policy/std.hpp
     */
    {
        X * px;
        
        count_layout pn;
        
        void release(void)
        {
          #if 0
            std::cout<<"shared_ptr_layout::release:px="<<px
            <<":pi="<<pn.pi_
            <<":use_count="<<pn.pi_->use_count()
            <<"\n";
          #endif
            px=0;
            pn.pi_->release();
            pn.pi_=0;
        }
    };
}

// assume 4 byte alignment for pointers when scanning
size_t const pointer_align = 4;

    typedef 
  std::map
  < boost::policy_ptr::detail::sp_counted_tagged const*
  , long //number of heap smart_ptr's whose referent is that of key.
  > 
map2_type
;

static void scan_and_count
  ( unsigned char const * area  //start of a heap allocated object
  , size_t size //size of area
  , map_type const & m
  , map2_type & m2
  )
/**@brief
 *  For each shared_ptr_layout, q, in area, increments m2[q.pn.pi()],
 */
{
    unsigned char const * p = area;

    for
      ( size_t n = 0
      ; n + sizeof(shared_ptr_layout) <= size
      ; p += pointer_align, n += pointer_align
      )
    {
        shared_ptr_layout const * q = reinterpret_cast<shared_ptr_layout const *>(p);

        if
          (  q->pn.tagged_valid()     //counter valid?
          && m.count(q->pn.pi()) != 0 //counter inserted by sp_scalar_constructor_hook? 
          )
        {
            ++m2[q->pn.pi()]
              //increment number of smart_ptrs(in heap) to counter's referent.
              ;
        }
    }
}

    typedef 
  std::deque
  < boost::policy_ptr::detail::sp_counted_tagged const *
  > 
open_type
;

static void scan_and_mark
  ( unsigned char const * area
  , size_t size
  , map2_type & m2
  , open_type & open
  )
{
    unsigned char const * p = area;

    for
      ( size_t n = 0
      ; n + sizeof(shared_ptr_layout) <= size
      ; p += pointer_align, n += pointer_align
      )
    {
        shared_ptr_layout const * q = reinterpret_cast<shared_ptr_layout const *>(p);

        if
          (  q->pn.tagged_valid() 
          && m2.count(q->pn.pi()) != 0
          )
        {
            open.push_back(q->pn.pi());
            m2.erase(q->pn.pi());
        }
    }
}

static void find_unreachable_objects_impl(map_type const & m, map2_type & m2)
{
    // scan objects for shared_ptr members, compute internal counts

    {
        std::cout << "... " << m.size() << " objects in m.\n";
        
        BOOST_ASSERT(m.counts_non_zero()); //should be no counts == 0 in m.

        for(map_type::const_iterator i = m.begin(); i != m.end(); ++i)
        {
            scan_and_count(i->second.first, i->second.second, m, m2);
        }

        std::cout << "... " << m2.size() << " objects in m2.\n";
    }

    // mark reachable objects

    {
        open_type open;

        for(map2_type::iterator i = m2.begin(); i != m2.end(); ++i)
        {
            boost::policy_ptr::detail::sp_counted_tagged const * p = i->first;
            if(p->use_count() != i->second) //any smart_ptr's to p's referent NOT in heap?
              open.push_back(p);
        }

        std::cout << "... " << m2.size() << " objects in open.\n";
        
        //At this point, objects in open have smart_ptr's NOT in the heap which
        //are pointing to them.

        { //erase those in open from m2:
            for(open_type::iterator j = open.begin(); j != open.end(); ++j)
            {
                m2.erase(*j);
            }
        }

        { //Perform breadth-first mark of objects in open.
          //"Mark" means remove from m2.
            while(!open.empty())
            {
                boost::policy_ptr::detail::sp_counted_tagged const* p = open.front();
                open.pop_front();
    
                map_type::const_iterator i = m.find(p);
                BOOST_ASSERT(i != m.end());
    
                scan_and_mark(i->second.first, i->second.second, m2, open);
            }
            
        } //end breadth-first mark.
    }

    // m2 now contains the unreachable objects
}

    typedef 
  boost::std_ptr<X>::shared_tagged
shared_tagged_type
;
    typedef 
  std::deque< shared_tagged_type > 
free_list_type
/**@brief
 *  Temporary holding area for referents to be freed.
 *  All referents, after they're shared_tagged_types are
 *  inserted, should have refcount==1; hence, when
 *  the ~free_list_type is called, all referents are freed.
 */
;

static void scan_and_free
  ( unsigned char * area
  , size_t size
  , map2_type const & m2
  , free_list_type & free
  )
{
    unsigned char * p = area;

    for
      ( size_t n = 0
      ; n + sizeof(shared_ptr_layout) <= size
      ; p += pointer_align, n += pointer_align
      )
    {
        shared_ptr_layout * q = reinterpret_cast<shared_ptr_layout *>(p);

        if(q->pn.tagged_valid() && m2.count(q->pn.pi()) != 0 && q->px != 0)
        {
            shared_tagged_type * ppx = reinterpret_cast< shared_tagged_type * >(p);
            free.push_back(*ppx);
            //following is equivalent to ppx->boost::shared_ptr::reset()
            q->release();
            BOOST_ASSERT(get_map().counts_non_zero());
        }
    }
}

namespace boost
{
namespace policy_ptr
{
namespace detail
{

  std::size_t 
collector<sp_counted_tagged>::
find_unreachable_objects(bool report)
{
    map2_type m2;

#ifdef BOOST_HAS_THREADS

    // This will work without the #ifdef, but some compilers warn
    // that lock is not referenced

    mutex_type::scoped_lock lock(get_mutex());

#endif

    map_type const & m = get_map();

    find_unreachable_objects_impl(m, m2);

    if(report)
    {
        for(map2_type::iterator j = m2.begin(); j != m2.end(); ++j)
        {
            map_type::const_iterator i = m.find(j->first);
            BOOST_ASSERT(i != m.end());
            std::cout << "Unreachable object at " 
            << i->second.first << ", " 
            << i->second.second << " bytes long.\n";
        }
    }

    return m2.size();
}

  void 
collector<sp_counted_tagged>::
free_unreachable_objects()
{

    {
    
        free_list_type free;
    
        {
            map2_type m2;
    
    #ifdef BOOST_HAS_THREADS
    
            mutex_type::scoped_lock lock(get_mutex());
    
    #endif
    
            map_type const & m = get_map();
    
            find_unreachable_objects_impl(m, m2);
    
            for(map2_type::iterator j = m2.begin(); j != m2.end(); ++j)
            {
                map_type::const_iterator i = m.find(j->first);
                BOOST_ASSERT(i != m.end());
                scan_and_free(i->second.first, i->second.second, m2, free);
            }
        }
    
        std::cout << "... about to free " << free.size() << " objects.\n";
    }
    
    BOOST_ASSERT(get_map().counts_non_zero());
}

  void 
collector<sp_counted_tagged>::
sp_scalar_constructor_hook
  ( void * referent_ptr
  , std::size_t referent_size
  , boost::policy_ptr::detail::sp_counted_tagged const * pn
  )
{
#ifdef BOOST_HAS_THREADS

    mutex_type::scoped_lock lock(get_mutex());

#endif
#if 0
    std::cout<<"sp_scalar_constructor_hook"
    <<":referent_ptr="<<referent_ptr
    <<":pn="<<pn<<"\n";
#endif
    get_map()[pn] = 
      std::make_pair(static_cast<unsigned char*>(referent_ptr), referent_size);
}

  void 
collector<sp_counted_tagged>::
sp_scalar_destructor_hook
  ( sp_counted_tagged * pn
  )
{
#ifdef BOOST_HAS_THREADS

    mutex_type::scoped_lock lock(get_mutex());

#endif
#if 0
    std::cout<<"sp_scalar_destructor_hook"
    <<":pn="<<pn<<"\n";
#endif
    get_map().erase(pn);
}

}//exit detail namespace

}//exit policy_ptr namespace

void sp_array_constructor_hook(void *)
{
}

void sp_array_destructor_hook(void *)
{
}

} // namespace boost

