#ifndef BOOST_POLICIES_20020502_HPP
#define BOOST_POLICIES_20020502_HPP

#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <boost/checked_delete.hpp>
#include <boost/detail/shared_count.hpp>

namespace boost
{

//////////////////////////////////////////////////////////////////////////////
// class templates boost_storage
// Declarations of boost-emulating StoragePolicies.
//////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class shared_storage;

    template <typename T>
    class weak_storage;

    template <typename T>
    class intrusive_storage;

//////////////////////////////////////////////////////////////////////////////
// class template shared_storage
// A boost::shared_count StoragePolicy for boost::shared_ptr emulation
//////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class shared_storage
    {
    protected:
        typedef T* stored_type;         // the type of the pointee_ object
        typedef T* pointer_type;        // type returned by operator->
        typedef T& reference_type;      // type returned by operator*

        shared_storage()
        : pointee_(default_value()), count_()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        shared_storage(const shared_storage<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(rhs.pointee_), count_(rhs.count_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(reinterpret_cast<const shared_storage&>(rhs).pointee_)
        , count_(reinterpret_cast<const shared_storage&>(rhs).count_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        { }

        template <typename U>
        explicit shared_storage(const weak_storage<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(rhs.pointee_), count_(rhs.count_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(reinterpret_cast<const weak_storage<T>&>(rhs).pointee_)
        , count_(reinterpret_cast<const weak_storage<T>&>(rhs).count_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        { }

        template <typename U>
        shared_storage(const intrusive_storage<U>& rhs)
        : pointee_(get_impl(rhs)), count_(get_impl(rhs))
        { std::cout << "shared_storage(intrusive_storage<U>)" << std::endl; }

        template <typename U>
        explicit shared_storage(U* p)
        : pointee_(p), count_(p, checked_deleter<U>(), p)
        { std::cout << "shared_storage(U*)" << std::endl; }

#else // BOOST_NO_MEMBER_TEMPLATES

        shared_storage(const shared_storage& rhs)
        : pointee_(rhs.pointee_), count_(rhs.count_)
        { }

        explicit shared_storage(const weak_storage<T>& rhs)
        : pointee_(rhs.pointee_), count_(rhs.count_)
        { }

        shared_storage(const intrusive_storage<T>& rhs)
        : pointee_(get_impl(rhs)), count_(get_impl(rhs))
        { }

        explicit shared_storage(const stored_type& p)
        : pointee_(p), count_(p, checked_deleter<U>(), p)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        void swap(shared_storage& rhs)
        {
            std::swap(pointee_, rhs.pointee_);
            // No std specialization of shared_count
            count_.swap(rhs.count_);
        }

        pointer_type get_pointer() const
        { return pointee_; }

        reference_type get_reference() const
        { return *pointee_; }

        bool is_valid() const
        { return pointee_ != default_value(); }

    public:
        // Accessors
        friend inline pointer_type get_impl(const shared_storage& sp)
        { return sp.pointee_; }

        friend inline stored_type& get_impl_ref(shared_storage& sp)
        { return sp.pointee_; }

        friend inline const stored_type& get_impl_ref(const shared_storage& sp)
        { return sp.pointee_; }

        // boost::smart_ptr compatibility interface
        pointer_type get() const
        { return pointee_; }

        bool unique() const
        { return pointee_ == default_value(); }

        long use_count() const
        { return count_.use_count(); }

        void reset()
        { shared_storage().swap(*this); }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        // U must be complete
        template <typename U>
        void reset(U* p)
        {
            // Catch self-reset errors
            BOOST_ASSERT(p == 0 || p != pointee_);
            shared_storage(p).swap(*this);
        }

#else // BOOST_NO_MEMBER_TEMPLATES

        void reset(const stored_type& p)
        {
            // Catch self-reset errors
            BOOST_ASSERT(p == 0 || p != pointee_);
            shared_storage(p).swap(*this);
        }

#endif // BOOST_NO_MEMBER_TEMPLATES

    protected:
        // Destruction is handled by shared_count
        void destroy()
        { }

        static stored_type default_value()
        { return 0; }

    private:

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template <typename U> friend class shared_storage;
        template <typename U> friend class weak_storage;
#else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        friend class weak_storage<T>;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

        // Data
        stored_type pointee_;
        detail::shared_count count_;
    };

//////////////////////////////////////////////////////////////////////////////
// class template weak_storage
// A boost::shared_count StoragePolicy for boost::weak_ptr emulation
//////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class weak_storage
    {
    protected:
        typedef T* stored_type;         // the type of the pointee_ object
        typedef T* pointer_type;        // type returned by operator->
        typedef T& reference_type;      // type returned by operator*

        weak_storage() : pointee_(default_value()), count_()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        weak_storage(const weak_storage<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(rhs.pointee_), count_(rhs.count_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(reinterpret_cast<const weak_storage&>(rhs).pointee_)
        , count_(reinterpret_cast<const weak_storage&>(rhs).count_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        { }

        template <typename U>
        weak_storage(const shared_storage<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(rhs.pointee_), count_(rhs.count_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(reinterpret_cast<const shared_storage<T>&>(rhs).pointee_)
        , count_(reinterpret_cast<const shared_storage<T>&>(rhs).count_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        { std::cout << "weak_storage(shared_storage<U>)" << std::endl; }

#else // BOOST_NO_MEMBER_TEMPLATES

        weak_storage(const weak_storage& rhs)
        : pointee_(rhs.pointee_), count_(rhs.count_)
        { }

        weak_storage(const shared_storage<T>& rhs)
        : pointee_(rhs.pointee_), count_(rhs.count_)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        void swap(weak_storage& rhs)
        {
            std::swap(pointee_, rhs.pointee_);
            // No std specialization of weak_count
            count_.swap(rhs.count_);
        }

        pointer_type get_pointer() const
        { return pointee_; }

        reference_type get_reference() const
        { return *pointee_; }

        bool is_valid() const
        { return pointee_ != default_value(); }

    public:
        // Accessors
        friend inline pointer_type get_impl(const weak_storage& sp)
        { return sp.pointee_; }

        friend inline stored_type& get_impl_ref(weak_storage& sp)
        { return sp.pointee_; }

        friend inline const stored_type& get_impl_ref(const weak_storage& sp)
        { return sp.pointee_; }

        // boost::smart_ptr compatibility interface
        pointer_type get() const
        { return pointee_; }
        
        long use_count() const
        { return count_.use_count(); }

        void reset()
        { weak_storage().swap(*this); }

    protected:
        // Destruction is handled by weak_count
        void destroy()
        { }

        static stored_type default_value()
        { return 0; }

    private:

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template <typename U> friend class weak_storage;
        template <typename U> friend class shared_storage;
#else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        friend class shared_storage<T>;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

        // Data
        stored_type pointee_;
        detail::weak_count count_;
    };

//////////////////////////////////////////////////////////////////////////////
// class template intrusive_storage
// A boost::shared_count StoragePolicy for boost::shared_ptr emulation
//////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class intrusive_storage
    {
    protected:
        typedef T* stored_type;         // the type of the pointee_ object
        typedef T* pointer_type;        // type returned by operator->
        typedef T& reference_type;      // type returned by operator*

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        intrusive_storage(const intrusive_storage<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(rhs.pointee_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(reinterpret_cast<const intrusive_storage&>(rhs).pointee_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        {
            if (pointee_ != default_value()) intrusive_ptr_add_ref(pointee_);
        }

#else // BOOST_NO_MEMBER_TEMPLATES

        intrusive_storage(const intrusive_storage<T>& rhs)
        : pointee_(rhs.pointee_)
        {
            if (pointee_ != default_value()) intrusive_ptr_add_ref(pointee_);
        }

#endif // BOOST_NO_MEMBER_TEMPLATES

        explicit intrusive_storage(const stored_type& p = default_value())
        : pointee_(p)
        {
            std::cout << "intrusive_storage(T*): " << p << std::endl;
            if (pointee_ != default_value()) intrusive_ptr_add_ref(pointee_);
        }

        void swap(intrusive_storage& rhs)
        {
            std::swap(pointee_, rhs.pointee_);
        }

        pointer_type get_pointer() const
        { return pointee_; }

        reference_type get_reference() const
        { return *pointee_; }

        bool is_valid() const
        { return pointee_ != default_value(); }

    public:
        // Accessors
        friend inline pointer_type get_impl(const intrusive_storage& sp)
        { return sp.pointee_; }

        friend inline stored_type& get_impl_ref(intrusive_storage& sp)
        { return sp.pointee_; }

        friend inline const stored_type& get_impl_ref(const intrusive_storage& sp)
        { return sp.pointee_; }

        // boost::smart_ptr compatibility interface
        pointer_type get() const
        { return pointee_; }

        long use_count() const
        { return pointee_ == 0 ? 0 : pointee_->use_count(); }

        bool empty() const
        { return !is_valid(); }

    protected:
        // Destruction is handled by counted_base
        void destroy()
        { if (pointee_ != default_value()) intrusive_ptr_release(pointee_); }

        static stored_type default_value()
        { return 0; }

    private:

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template <typename U> friend class intrusive_ptr;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

        // Data
        stored_type pointee_;
    };

//////////////////////////////////////////////////////////////////////////////
// class template boost_ref
// Implementation of boost::*_ptr-emulating Ownership policy
//////////////////////////////////////////////////////////////////////////////

    template <typename P>
    class boost_ref
    {
    protected:
        boost_ref()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        boost_ref(const boost_ref<U>& rhs)
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        boost_ref(const boost_ref& rhs)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        P clone(const P& val)
        {
            return val;
        }

        bool release(const P&)
        {
            // This allows us to call release() in intrusive_storage, instead
            // of defining a d'tor.
            return true;
        }

        void swap(boost_ref& rhs)
        {
        }

#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        enum { destructive_copy_tag = false };
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef false_type destructive_copy_tag;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS

    private:

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template <typename U> friend class boost_ref;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

#if defined(__BORLANDC__) || defined(BOOST_MSVC)
        bool mi_ebo_hack_;
#endif // __BORLANDC__ || BOOST_MSVC
    };

}   // namespace boost

#endif // BOOST_POLICIES_20020502_HPP
