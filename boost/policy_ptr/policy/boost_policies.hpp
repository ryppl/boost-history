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
    class shared_storage_;

    template <typename T>
    class weak_storage_;

    template <typename T>
    class intrusive_storage_;

    template <typename T>
    class boost_ref_;
    
//////////////////////////////////////////////////////////////////////////////
// class template shared_storage
// A boost::shared_count StoragePolicy for boost::shared_ptr emulation
//////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class shared_storage_
    {
    public:
        typedef T* stored_type;         // the type of the pointee_ object
        typedef T const*                                const_stored_type;
        typedef T* pointer_type;        // type returned by operator->
        typedef T const*                                const_pointer_type;
        typedef typename add_reference<T>::type         reference_type;
        typedef typename add_reference<T const>::type   const_reference_type;
        typedef typename call_traits<stored_type>::param_type
                                                        stored_param;
        typedef typename call_traits<pointer_type>::param_type
                                                        pointer_param;

    protected:
        shared_storage_()
        : pointee_(default_value()), count_()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        shared_storage_(const shared_storage_<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(rhs.pointee_), count_(rhs.count_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(reinterpret_cast<const shared_storage_&>(rhs).pointee_)
        , count_(reinterpret_cast<const shared_storage_&>(rhs).count_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        { }

        template <typename U>
        explicit shared_storage_(const weak_storage_<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(rhs.pointee_), count_(rhs.count_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(reinterpret_cast<const weak_storage_<T>&>(rhs).pointee_)
        , count_(reinterpret_cast<const weak_storage_<T>&>(rhs).count_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        { }

        template <typename U>
        shared_storage_(const intrusive_storage_<U>& rhs)
        : pointee_(get_impl(rhs)), count_(get_impl(rhs))
        { }

        template <typename U>
        explicit shared_storage_(U* p)
        : pointee_(p), count_(p, checked_deleter<U>() )
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        shared_storage_(const shared_storage_& rhs)
        : pointee_(rhs.pointee_), count_(rhs.count_)
        { }

        explicit shared_storage_(const weak_storage_<T>& rhs)
        : pointee_(rhs.pointee_), count_(rhs.count_)
        { }

        shared_storage_(const intrusive_storage_<T>& rhs)
        : pointee_(get_impl(rhs)), count_(get_impl(rhs))
        { }

        explicit shared_storage_(T* p)
        : pointee_(p), count_(p, checked_deleter<T>(), p)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        void swap(shared_storage_& rhs)
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
        friend inline pointer_type get_impl(const shared_storage_& sp)
        { return sp.pointee_; }

        friend inline stored_type& get_impl_ref(shared_storage_& sp)
        { return sp.pointee_; }

        friend inline const stored_type& get_impl_ref(const shared_storage_& sp)
        { return sp.pointee_; }

        // boost::smart_ptr compatibility interface
        pointer_type get() const
        { return pointee_; }

        bool unique() const
        { return pointee_ == default_value(); }

        long use_count() const
        { return count_.use_count(); }

        void reset()
        { shared_storage_().swap(*this); }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        // U must be complete
        template <typename U>
        void reset(U* p)
        {
            // Catch self-reset errors
            BOOST_ASSERT(p == 0 || p != pointee_);
            shared_storage_(p).swap(*this);
        }

#else // BOOST_NO_MEMBER_TEMPLATES

        void reset(const stored_type& p)
        {
            // Catch self-reset errors
            BOOST_ASSERT(p == 0 || p != pointee_);
            shared_storage_(p).swap(*this);
        }

#endif // BOOST_NO_MEMBER_TEMPLATES

    protected:
        // Destruction is handled by shared_count
        void destroy()
        { }

        static stored_type default_value()
        { return 0; }

        stored_param    clone(stored_param p) const
        {
            return p;
        }

        static
        void            swap(shared_storage_& lhs, shared_storage_& rhs)
        {
            lhs.swap(rhs);
        }

    private:

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template <typename U> friend class shared_storage_;
        template <typename U> friend class weak_storage_;
#else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        friend class weak_storage_<T>;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        friend class boost_ref_<shared_storage_<T> >;

        // Data
        stored_type pointee_;
        detail::shared_count count_;
    };

    //------------------------------------------------------------------------
    struct shared_storage
    {
        typedef storage_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef shared_storage_<T> type;
        };
    };
//////////////////////////////////////////////////////////////////////////////
// class template weak_storage_
// A boost::shared_count StoragePolicy for boost::weak_ptr emulation
//////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class weak_storage_
    {
    protected:
        typedef T* stored_type;         // the type of the pointee_ object
        typedef T* pointer_type;        // type returned by operator->
        typedef T& reference_type;      // type returned by operator*

        weak_storage_() : pointee_(default_value()), count_()
        { }

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        weak_storage_(const weak_storage_<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(rhs.pointee_), count_(rhs.count_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(reinterpret_cast<const weak_storage_&>(rhs).pointee_)
        , count_(reinterpret_cast<const weak_storage_&>(rhs).count_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        { }

        template <typename U>
        weak_storage_(const shared_storage_<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(rhs.pointee_), count_(rhs.count_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(reinterpret_cast<const shared_storage_<T>&>(rhs).pointee_)
        , count_(reinterpret_cast<const shared_storage_<T>&>(rhs).count_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        weak_storage_(const weak_storage_& rhs)
        : pointee_(rhs.pointee_), count_(rhs.count_)
        { }

        weak_storage_(const shared_storage_<T>& rhs)
        : pointee_(rhs.pointee_), count_(rhs.count_)
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        void swap(weak_storage_& rhs)
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
        friend inline pointer_type get_impl(const weak_storage_& sp)
        { return sp.pointee_; }

        friend inline stored_type& get_impl_ref(weak_storage_& sp)
        { return sp.pointee_; }

        friend inline const stored_type& get_impl_ref(const weak_storage_& sp)
        { return sp.pointee_; }

        // boost::smart_ptr compatibility interface
        pointer_type get() const
        { return pointee_; }
        
        long use_count() const
        { return count_.use_count(); }

        void reset()
        { weak_storage_().swap(*this); }

    protected:
        // Destruction is handled by weak_count
        void destroy()
        { }

        static stored_type default_value()
        { return 0; }

    private:

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template <typename U> friend class weak_storage_;
        template <typename U> friend class shared_storage_;
#else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        friend class shared_storage_<T>;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

        // Data
        stored_type pointee_;
        detail::weak_count count_;
    };

    //------------------------------------------------------------------------
    struct weak_storage
    {
        typedef storage_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef weak_storage_<T> type;
        };
    };
//////////////////////////////////////////////////////////////////////////////
// class template intrusive_storage
// A boost::shared_count StoragePolicy for boost::shared_ptr emulation
//////////////////////////////////////////////////////////////////////////////

    template <typename T>
    class intrusive_storage_
    {
    protected:
        typedef T* stored_type;         // the type of the pointee_ object
        typedef T* pointer_type;        // type returned by operator->
        typedef T& reference_type;      // type returned by operator*

#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        intrusive_storage_(const intrusive_storage_<U>& rhs)
# ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(rhs.pointee_)
# else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : pointee_(reinterpret_cast<const intrusive_storage_&>(rhs).pointee_)
# endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        {
            if (pointee_ != default_value()) intrusive_ptr_add_ref(pointee_);
        }

#else // BOOST_NO_MEMBER_TEMPLATES

        intrusive_storage_(const intrusive_storage_<T>& rhs)
        : pointee_(rhs.pointee_)
        {
            if (pointee_ != default_value()) intrusive_ptr_add_ref(pointee_);
        }

#endif // BOOST_NO_MEMBER_TEMPLATES

        explicit intrusive_storage_(const stored_type& p = default_value())
        : pointee_(p)
        {
            if (pointee_ != default_value()) intrusive_ptr_add_ref(pointee_);
        }

        void swap(intrusive_storage_& rhs)
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
        friend inline pointer_type get_impl(const intrusive_storage_& sp)
        { return sp.pointee_; }

        friend inline stored_type& get_impl_ref(intrusive_storage_& sp)
        { return sp.pointee_; }

        friend inline const stored_type& get_impl_ref(const intrusive_storage_& sp)
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

    //------------------------------------------------------------------------
    struct intrusive_storage
    {
        typedef storage_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef intrusive_storage_<T> type;
        };
    };
//////////////////////////////////////////////////////////////////////////////
// class template boost_ref
// Implementation of boost::*_ptr-emulating Ownership policy
//////////////////////////////////////////////////////////////////////////////

    template <typename StoragePolicy>
    class boost_ref_ : public StoragePolicy
    {
    public:
        typedef copy_semantics_tag                      ownership_category;
        typedef StoragePolicy                           storage_policy;
        typedef storage_policy                          base_type;
        typedef typename storage_policy::stored_type    stored_type;
        typedef typename storage_policy::pointer_type   pointer_type;
        typedef typename storage_policy::stored_param   stored_param;
        typedef typename storage_policy::pointer_param  pointer_param;
    protected:
        boost_ref_()
        { }

        boost_ref_(stored_type p)
                            : base_type(p)
        { }
#ifndef BOOST_NO_MEMBER_TEMPLATES

        template <typename U>
        boost_ref_(const boost_ref_<U>& rhs)
                            : base_type(static_cast<
                                typename boost_ref_<U>::base_type const&
                            >(rhs))
        { }

#else // BOOST_NO_MEMBER_TEMPLATES

        boost_ref_(const boost_ref_& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
        { }

#endif // BOOST_NO_MEMBER_TEMPLATES

        bool release(const StoragePolicy&)
        {
            // This allows us to call release() in intrusive_storage_, instead
            // of defining a d'tor.
            return true;
        }

#ifndef BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        enum { destructive_copy_tag = false };
#else // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS
        typedef false_type destructive_copy_tag;
#endif // BOOST_BCC_NON_TYPE_TEMPLATE_PARAMETERS

    private:

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        template <typename U> friend class boost_ref_;
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

#if defined(__BORLANDC__) || defined(BOOST_MSVC)
        bool mi_ebo_hack_;
#endif // __BORLANDC__ || BOOST_MSVC
    };

    //------------------------------------------------------------------------
    struct boost_ref
    {
        typedef ownership_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef boost_ref_<T> type;
        };
    };
}   // namespace boost

#endif // BOOST_POLICIES_20020502_HPP
