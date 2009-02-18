//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_PROCESS_LOCK__HPP
#define BOOST_SYNCHRO_PROCESS_LOCKS__HPP

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/upgradable_lock.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>

namespace boost { namespace synchro {


    template <typename T>
    struct lockable_type<boost::interprocess::scoped_lock<T> > {
        typedef T type;
    };
    template <typename T>
    struct lockable_type<boost::interprocess::sharable_lock<T> > {
        typedef T type;
    };
    template <typename T>
    struct lockable_type<boost::interprocess::upgradable_lock<T> > {
        typedef T type;
    };

    template<typename Mutex>
    class unique_locker<Mutex,multi_process_tag>:  public unique_lock_type<Mutex>::type {
        //typename scope_tag<Mutex>::type == multi_process_tag
    private:
        typedef Mutex lockable_type;
        typedef multi_process_tag scope_tag_type;
        typedef typename unique_lock_type<Mutex>::type base_type;

        BOOST_NON_CONST_COPY_CONSTRUCTOR_DELETE(unique_locker) /*< disable copy construction >*/
        BOOST_NON_CONST_COPY_CONSTRUCTOR_DELETE(unique_locker) /*< disable copy asignement >*/
        
        unique_locker(): base_type() {}

        explicit unique_locker(Mutex& m_): base_type(m_)
        {}
        unique_locker(Mutex& m_,adopt_lock_t): base_type(m_, boost::interprocess::accept_ownership)
        {}
        unique_locker(Mutex& m_,defer_lock_t): base_type(m_, boost::interprocess::defer_lock)
        {}
        unique_locker(Mutex& m_,try_to_lock_t): base_type(m_, boost::interprocess::try_to_lock)
        {}
        template<typename TimeDuration>
        unique_locker(Mutex& m_,TimeDuration const& target_time): base_type(m_, target_time)
        {}
        unique_locker(Mutex& m_,system_time const& target_time): base_type(m_, target_time)
        {}
        template<typename TimeDuration>
        unique_locker(Mutex& m_,TimeDuration const& target_time, throw_timeout_t)
            : base_type(m_, defer_lock)
        {
            lock_for(target_time);
        }
        unique_locker(Mutex& m_,system_time const& target_time, throw_timeout_t)
            : base_type(m_, defer_lock)
        {
            lock_until(target_time);
        }
        template<typename TimeDuration>
        unique_locker(TimeDuration const& target_time, Mutex& m_)
            : base_type(m_, defer_lock)
        {
            lock_for(target_time);
        }
        unique_locker(system_time const& target_time, Mutex& m_)
            : base_type(m_, defer_lock)
        {
            lock_until(target_time);
        }
#ifdef BOOST_HAS_RVALUE_REFS
        unique_locker(unique_locker&& other): base_type(other)
        {}
        explicit unique_locker(upgrade_locker<Mutex,scope_tag_type>&& other) : base_type(other)
        {}

        unique_locker<Mutex, scope_tag_type>&& move()
        {
            return static_cast<unique_locker<Mutex, scope_tag_type>&&>(*this);
        }


        unique_locker& operator=(unique_locker<Mutex, scope_tag_type>&& other)
        {
            unique_locker temp(other);
            swap(temp);
            return *this;
        }

        unique_locker& operator=(upgrade_locker<Mutex, multi_process_tag>&& other)
        {
            unique_locker temp(other);
            swap(temp);
            return *this;
        }
        void swap(unique_locker&& other)
        {
            this->base_type.swap(other);
        }
#else
        unique_locker(interprocess::detail::moved_object<unique_locker<Mutex, scope_tag_type> > other)
            : base_type(interprocess::detail::moved_object<base_type>(other.get()))
        {}
        unique_locker(interprocess::detail::moved_object<upgrade_locker<Mutex,scope_tag_type> > other)
            : base_type(interprocess::detail::moved_object<upgrade_locker<Mutex,scope_tag_type> >(other.get()))
        {}
        // interprocess specific
        unique_locker(interprocess::detail::moved_object<upgrade_locker<Mutex,scope_tag_type> > other
                    , try_to_lock_t)
            : base_type(interprocess::detail::moved_object<upgrade_locker<Mutex,scope_tag_type> >(other.get())
                        , boost::interprocess::try_to_lock)
        {}

        // interprocess specific
        unique_locker(interprocess::detail::moved_object<upgrade_locker<Mutex,scope_tag_type> > other
                    , system_time const& target_time)
            : base_type(interprocess::detail::moved_object<upgrade_locker<Mutex,scope_tag_type> >(other.get())
                        , target_time)
        {}

            operator interprocess::detail::moved_object<unique_locker<Mutex, scope_tag_type> >()
        {
            return move();
        }

        interprocess::detail::moved_object<unique_locker<Mutex, scope_tag_type> > move()
        {
            return interprocess::detail::moved_object<unique_locker<Mutex, scope_tag_type> >(*this);
        }

        unique_locker& operator=(interprocess::detail::moved_object<unique_locker<Mutex, scope_tag_type> > other)
        {
            unique_locker temp(other);
            swap(temp);
            return *this;
        }

        unique_locker& operator=(interprocess::detail::moved_object<upgrade_locker<Mutex, scope_tag_type> > other)
        {
            unique_locker temp(other);
            swap(temp);
            return *this;
        }
        void swap(unique_locker& other)
        {
            this->base_type.swap(other);
        }
        void swap(interprocess::detail::moved_object<unique_locker<Mutex, scope_tag_type> > other)
        {
            this->base_type.swap(interprocess::detail::moved_object<base_type>(other.get()));
        }
#endif

        ~unique_locker() {}

        Mutex* mutex() const
        {
            return static_cast<Mutex*>(this->base_type.mutex());
        }

        bool is_locking(lockable_type* l) const {
            return l==mutex();
        } /*< strict lockers specific function >*/

        bool owns_lock() const {
            return this->owns();
        }

        template<typename TimeDuration>
        bool try_lock_for(TimeDuration const& relative_time)
        {
            return this->timed_lock(relative_time);
        }

        bool try_lock_until(::boost::system_time const& absolute_time)
        {
            return this->timed_lock(absolute_time);
        }

        template<typename TimeDuration>
        void lock_for(TimeDuration const& relative_time)
        {
            if(!try_lock_for(relative_time)) throw timeout_exception();
        }

        void lock_until(::boost::system_time const& absolute_time)
        {
            if(!try_lock_until(absolute_time)) throw timeout_exception();
        }

        friend class shared_locker<Mutex,scope_tag_type>;
        friend class upgrade_locker<Mutex,scope_tag_type>;
    };

    
    template<typename Mutex>
    class try_unique_locker<Mutex,multi_process_tag>:  public unique_locker<Mutex,multi_process_tag> {
        //typename scope_tag<Mutex>::type == multi_process_tag
    public:
        typedef Mutex lockable_type;
        typedef multi_process_tag scope_tag_type;
        typedef unique_locker<Mutex,multi_process_tag> base_type;

        BOOST_NON_CONST_COPY_CONSTRUCTOR_DELETE(try_unique_locker) /*< disable copy construction >*/
        BOOST_NON_CONST_COPY_CONSTRUCTOR_DELETE(try_unique_locker) /*< disable copy asignement >*/
    
        try_unique_locker(): base_type() {}

        explicit try_unique_locker(Mutex& m_): base_type(m_, boost::interprocess::defer_lock)
        {
            this->try_lock();
        }
        try_unique_locker(Mutex& m_,force_lock_t): base_type(m_)
        {}
        try_unique_locker(Mutex& m_,adopt_lock_t): base_type(m_, boost::interprocess::accept_ownership)
        {}
        try_unique_locker(Mutex& m_,defer_lock_t): base_type(m_, boost::interprocess::defer_lock)
        {}
        try_unique_locker(Mutex& m_,try_to_lock_t): base_type(m_, boost::interprocess::try_to_lock)
        {}
            
        try_unique_locker(Mutex& m_,system_time const& target_time): base_type(m_, target_time)
        {}
        template<typename TimeDuration>
        try_unique_locker(Mutex& m_,TimeDuration const& target_time): base_type(m_, target_time)
        {}

        try_unique_locker(nothrow_timeout_t, system_time const& target_time, Mutex& m_)
            : base_type(m_, defer_lock)
        {}
        template<typename TimeDuration>
        try_unique_locker(nothrow_timeout_t, TimeDuration const& target_time, Mutex& m_)
            : base_type(m_, defer_lock)
        {}
            
        try_unique_locker(Mutex& m_,system_time const& target_time, throw_timeout_t)
            : base_type(m_, defer_lock)
        {
            this->lock_until(target_time);
        }
        template<typename TimeDuration>
        try_unique_locker(Mutex& m_,TimeDuration const& target_time, throw_timeout_t)
            : base_type(m_, defer_lock)
        {
            this->lock_for(target_time);
        }
        
        try_unique_locker(system_time const& target_time, Mutex& m_)
            : base_type(m_, defer_lock)
        {
            this->lock_until(target_time);
        }
        template<typename TimeDuration>
        try_unique_locker(TimeDuration const& target_time, Mutex& m_)
            : base_type(m_, defer_lock)
        {
            this->lock_for(target_time);
        }
        
#ifdef BOOST_HAS_RVALUE_REFS
        try_unique_locker(try_unique_locker&& other): base_type(other)
        {}
        explicit try_unique_locker(upgrade_locker<Mutex,scope_tag_type>&& other) : base_type(other)
        {}

        try_unique_locker<Mutex, scope_tag_type>&& move()
        {
            return static_cast<try_unique_locker<Mutex, scope_tag_type>&&>(*this);
        }


        try_unique_locker& operator=(try_unique_locker<Mutex, scope_tag_type>&& other)
        {
            try_unique_locker temp(other);
            swap(temp);
            return *this;
        }

        try_unique_locker& operator=(upgrade_locker<Mutex, multi_process_tag>&& other)
        {
            try_unique_locker temp(other);
            swap(temp);
            return *this;
        }
        void swap(try_unique_locker&& other)
        {
            this->base_type.swap(other);
        }
#else
        try_unique_locker(interprocess::detail::moved_object<try_unique_locker<Mutex, scope_tag_type> > other)
            : base_type(interprocess::detail::moved_object<base_type>(other.get()))
        {}
        try_unique_locker(interprocess::detail::moved_object<upgrade_locker<Mutex,scope_tag_type> > other)
            : base_type(interprocess::detail::moved_object<upgrade_locker<Mutex,scope_tag_type> >(other.get()))
        {}
        // interprocess specific
        try_unique_locker(interprocess::detail::moved_object<upgrade_locker<Mutex,scope_tag_type> > other
                    , try_to_lock_t)
            : base_type(interprocess::detail::moved_object<upgrade_locker<Mutex,scope_tag_type> >(other.get())
                        , boost::interprocess::try_to_lock)
        {}

        // interprocess specific
        try_unique_locker(interprocess::detail::moved_object<upgrade_locker<Mutex,scope_tag_type> > other
                    , system_time const& target_time)
            : base_type(interprocess::detail::moved_object<upgrade_locker<Mutex,scope_tag_type> >(other.get())
                        , target_time)
        {}

            operator interprocess::detail::moved_object<try_unique_locker<Mutex, scope_tag_type> >()
        {
            return move();
        }

        interprocess::detail::moved_object<try_unique_locker<Mutex, scope_tag_type> > move()
        {
            return interprocess::detail::moved_object<try_unique_locker<Mutex, scope_tag_type> >(*this);
        }

        try_unique_locker& operator=(interprocess::detail::moved_object<try_unique_locker<Mutex, scope_tag_type> > other)
        {
            try_unique_locker temp(other);
            swap(temp);
            return *this;
        }

        try_unique_locker& operator=(interprocess::detail::moved_object<upgrade_locker<Mutex, scope_tag_type> > other)
        {
            try_unique_locker temp(other);
            swap(temp);
            return *this;
        }
        void swap(try_unique_locker& other)
        {
            this->base_type.swap(other);
        }
        void swap(interprocess::detail::moved_object<try_unique_locker<Mutex, scope_tag_type> > other)
        {
            this->base_type.swap(interprocess::detail::moved_object<base_type>(other.get()));
        }
#endif

        ~try_unique_locker() {}

        Mutex* mutex() const
        {
            return static_cast<Mutex*>(this->base_type.mutex());
        }

        bool is_locking(lockable_type* l) const {
            return l==mutex();
        } /*< strict lockers specific function >*/

        bool owns_lock() const {
            return this->owns();
        }

        template<typename TimeDuration>
        bool try_lock_for(TimeDuration const& relative_time)
        {
            return this->timed_lock(relative_time);
        }

        bool try_lock_until(::boost::system_time const& absolute_time)
        {
            return this->timed_lock(absolute_time);
        }

        template<typename TimeDuration>
        void lock_for(TimeDuration const& relative_time)
        {
            if(!try_lock_for(relative_time)) throw timeout_exception();
        }

        void lock_until(::boost::system_time const& absolute_time)
        {
            if(!try_lock_until(absolute_time)) throw timeout_exception();
        }

        friend class shared_locker<Mutex,scope_tag_type>;
        friend class upgrade_locker<Mutex,scope_tag_type>;
    };
    
    template<typename Mutex>
    class shared_locker<Mutex,multi_process_tag>:  public shared_lock_type<Mutex>::type {
        //typename scope_tag<Mutex>::type == multi_process_tag
    public:
        typedef Mutex lockable_type;
        typedef multi_process_tag scope_tag_type;
        typedef typename shared_lock_type<Mutex>::type base_type;

        shared_locker(): base_type() {}
        BOOST_NON_CONST_COPY_CONSTRUCTOR_DELETE(shared_locker) /*< disable copy construction >*/
        BOOST_NON_CONST_COPY_CONSTRUCTOR_DELETE(shared_locker) /*< disable copy asignement >*/

        explicit shared_locker(Mutex& m_): base_type(m_)
        {}
        shared_locker(Mutex& m_,adopt_lock_t): base_type(m_, boost::interprocess::accept_ownership)
        {}
        shared_locker(Mutex& m_,defer_lock_t): base_type(m_, boost::interprocess::defer_lock)
        {}
        shared_locker(Mutex& m_,try_to_lock_t): base_type(m_, boost::interprocess::try_to_lock)
        {}
            
        shared_locker(Mutex& m_,system_time const& target_time)
            :  base_type(m_, target_time)
        {}           
        template<typename TimeDuration>
        shared_locker(Mutex& m_,TimeDuration const& target_time)
            : base_type(m_, boost::get_system_time()+target_time)
        {}

        shared_locker(nothrow_timeout_t, system_time const& target_time,Mutex& m_)
            : base_type(m_, boost::interprocess::defer_lock)
        {}
        template<typename TimeDuration>
        shared_locker(nothrow_timeout_t, TimeDuration const& target_time,Mutex& m_)
        {}

            
        shared_locker(Mutex& m_,system_time const& target_time,throw_timeout_t)
            : base_type(m_, boost::interprocess::defer_lock)
        {
            lock_until(target_time);
        }
        template<typename TimeDuration>
        shared_locker(Mutex& m_,TimeDuration const& target_time,throw_timeout_t)
        {
            lock_for(target_time);
        }
        
        shared_locker(system_time const& target_time,Mutex& m_)
            : base_type(m_, boost::interprocess::defer_lock)
        {
            lock_until(target_time);
        }
        template<typename TimeDuration>
        shared_locker(TimeDuration const& target_time,Mutex& m_)
        {
            lock_for(target_time);
        }


        shared_locker(interprocess::detail::moved_object<shared_locker<Mutex, scope_tag_type> > other)
            : base_type(interprocess::detail::moved_object<base_type>(other.get()))
        {}

        shared_locker(interprocess::detail::moved_object<unique_locker<Mutex, scope_tag_type> > other)
            : base_type(interprocess::detail::moved_object<typename unique_lock_type<Mutex>::type>(other.get()))
        {}

        shared_locker(interprocess::detail::moved_object<upgrade_locker<Mutex, scope_tag_type> > other)
            : base_type(interprocess::detail::moved_object<typename upgrade_lock_type<Mutex>::type>(other.get()))
        {}

        operator interprocess::detail::moved_object<shared_locker<Mutex,scope_tag_type> >()
        {
            return move();
        }

        interprocess::detail::moved_object<shared_locker<Mutex, scope_tag_type> > move()
        {
            return interprocess::detail::moved_object<shared_locker<Mutex, scope_tag_type> >(*this);
        }


        shared_locker& operator=(interprocess::detail::moved_object<shared_locker<Mutex, scope_tag_type> > other)
        {
            shared_locker temp(other);
            swap(temp);
            return *this;
        }

        shared_locker& operator=(interprocess::detail::moved_object<unique_locker<Mutex, scope_tag_type> > other)
        {
            shared_locker temp(other);
            swap(temp);
            return *this;
        }

        shared_locker& operator=(interprocess::detail::moved_object<upgrade_locker<Mutex, scope_tag_type> > other)
        {
            shared_locker temp(other);
            swap(temp);
            return *this;
        }

#ifdef BOOST_HAS_RVALUE_REFS
        void swap(shared_locker&& other)
        {
            this->base_type.swap(other);
        }
#else
        void swap(shared_locker& other)
        {
            this->base_type.swap(other);
        }
        void swap(boost::interprocess::detail::moved_object<shared_locker<Mutex, scope_tag_type> > other)
        {
            this->base_type.swap(interprocess::detail::moved_object<base_type>(other.get()));
        }
#endif

        Mutex* mutex() const
        {
            return static_cast<Mutex*>(this->base_type.mutex());
        }

        bool is_locking(lockable_type* l) const {
            return l==mutex();
        } /*< strict lockers specific function >*/

        bool owns_lock() const {
            return this->owns();
        }

        bool try_lock_until(boost::system_time const& absolute_time)
        {
            return this->timed_lock(absolute_time);
        }
        template<typename Duration>
        bool try_lock_for(Duration const& relative_time)
        {
            return this->timed_lock(relative_time);
        }

        template<typename TimeDuration>
        void lock_for(TimeDuration const& relative_time)
        {
            if(!try_lock_for(relative_time)) throw timeout_exception();
        }

        void lock_until(::boost::system_time const& absolute_time)
        {
            if(!try_lock_until(absolute_time)) throw timeout_exception();
        }

    };

    template<typename Mutex>
    class upgrade_locker<Mutex,multi_process_tag>:  public upgrade_lock_type<Mutex>::type {
        //typename scope_tag<Mutex>::type == multi_process_tag
    public:
        typedef Mutex lockable_type;
        typedef multi_process_tag scope_tag_type;
        typedef typename upgrade_lock_type<Mutex>::type base_type;

        upgrade_locker(): base_type() {}
        BOOST_NON_CONST_COPY_CONSTRUCTOR_DELETE(upgrade_locker) /*< disable copy construction >*/
        BOOST_NON_CONST_COPY_ASSIGNEMENT_DELETE(upgrade_locker) /*< disable copy asignement >*/

        explicit upgrade_locker(Mutex& m_): base_type(m_)
        {
        }
        upgrade_locker(Mutex& m_,adopt_lock_t): base_type(m_, boost::interprocess::accept_ownership)
        {}
        upgrade_locker(Mutex& m_,defer_lock_t): base_type(m_, boost::interprocess::defer_lock)
        {}
        upgrade_locker(Mutex& m_,try_to_lock_t): base_type(m_, boost::interprocess::try_to_lock)
        {}

        upgrade_locker(interprocess::detail::moved_object<upgrade_locker<Mutex, scope_tag_type> > other)
            : base_type(interprocess::detail::moved_object<base_type>(other.get()))
        {}

        upgrade_locker(interprocess::detail::moved_object<unique_locker<Mutex, scope_tag_type> > other)
            : base_type(interprocess::detail::moved_object<typename unique_lock_type<Mutex>::type>(other.get()))
        {}

        operator interprocess::detail::moved_object<upgrade_locker<Mutex, scope_tag_type> >()
        {
            return move();
        }

        interprocess::detail::moved_object<upgrade_locker<Mutex, scope_tag_type> > move()
        {
            return interprocess::detail::moved_object<upgrade_locker<Mutex, scope_tag_type> >(*this);
        }

        upgrade_locker& operator=(interprocess::detail::moved_object<upgrade_locker<Mutex, scope_tag_type> > other)
        {
            upgrade_locker temp(other);
            swap(temp);
            return *this;
        }

        upgrade_locker& operator=(interprocess::detail::moved_object<unique_locker<Mutex, scope_tag_type> > other)
        {
            upgrade_locker temp(other);
            swap(temp);
            return *this;
        }

        void swap(upgrade_locker& other)
        {
            this->base_type.swap(other);
        }

        ~upgrade_locker()
        {}

        Mutex* mutex() const
        {
            return static_cast<Mutex*>(this->base_type.mutex());
        }

        bool is_locking(lockable_type* l) const {
            return l==mutex();
        } /*< strict lockers specific function >*/

        bool owns_lock() const {
            return this->owns();
        }

        friend class shared_locker<Mutex, scope_tag_type>;
        friend class unique_locker<Mutex, scope_tag_type>;
    };


    template<typename Mutex>
    class upgrade_to_unique_locker<Mutex,multi_process_tag>:  public upgrade_to_unique_locker_type<Mutex>::type {
        //typename scope_tag<Mutex>::type == multi_process_tag
    public:
        typedef Mutex lockable_type;
        typedef multi_process_tag scope_tag_type;
        typedef typename upgrade_to_unique_locker_type<Mutex>::type base_type;

        BOOST_NON_CONST_COPY_CONSTRUCTOR_DELETE(upgrade_to_unique_locker) /*< disable copy construction >*/
        BOOST_NON_CONST_COPY_ASSIGNEMENT_DELETE(upgrade_to_unique_locker) /*< disable copy asignement >*/
    
        explicit upgrade_to_unique_locker(upgrade_locker<Mutex, multi_process_tag>& m_): base_type(m_)
        {}
        ~upgrade_to_unique_locker()
        {}

        upgrade_to_unique_locker(interprocess::detail::moved_object<upgrade_to_unique_locker<Mutex, scope_tag_type> > other)
            : base_type(interprocess::detail::moved_object<base_type>(other.get()))
        {}

        upgrade_to_unique_locker& operator=(interprocess::detail::moved_object<upgrade_to_unique_locker<Mutex, scope_tag_type> > other)
        {
            upgrade_to_unique_locker temp(other);
            swap(temp);
            return *this;
        }
        void swap(upgrade_to_unique_locker& other)
        {
            this->base_type.swap(other);
        }

        Mutex* mutex() const
        {
            return static_cast<Mutex*>(this->base_type.mutex());
        }

        bool is_locking(lockable_type* l) const { 
            return l==mutex(); 
        } /*< strict lockers specific function >*/

        bool owns_lock() const {
            return this->owns();
        }

    };

}
}

#endif
