//----------------------------------------------------------------------------
// Copyright (C) 2004, David B. Held and Andrei Alexandrescu
//----------------------------------------------------------------------------
#ifndef BOOST_REF_COUNTED_HPP
#define BOOST_REF_COUNTED_HPP
//----------------------------------------------------------------------------
// Headers
#include <algorithm>                                    // std::swap()
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    struct ref_counted
    {
        template <typename T>
        struct apply
        {
            typedef ref_counted_<T> type;
        };
    };
    //------------------------------------------------------------------------
    template <class StoragePolicy>
    class ref_counted_ : public StoragePolicy
    {
    public:             // Types
        typedef ownership_policy_tag                    policy_category;
        typedef copy_semantics_tag                      ownership_category;
        typedef StoragePolicy                           storage_policy;
        typedef typename storage_policy::pointer_type   pointer_type;

    protected:          // Protected Interface
                        ref_counted(void)
                            : count_(0)                 { }

                        template <typename U>
                        ref_counted_(ref_counted_<U> const& rhs)
                            : count_(BOOST_SP_CONVERT_ARGUMENT(
                                ref_counted_, rhs, count_
                            ))                          { }

        // gcc 3.0? incorrectly chooses the greedy c'tor below, instead of the
        // conversion c'tor above.  So this weaker pointer c'tor must be
        // used instead.
#if !defined(BOOST_MSVC) && !defined(__GNUC__)
                        template <typename U>
                        ref_counted_(U const&)
#else
                        ref_counted_(P const&)
#endif
                            : count_(new unsigned(1))
        {
//            count_ = static_cast<unsigned int*>(
//                SmallObject<>::operator new(sizeof(unsigned int)));
//            BOOST_ASSERT(count_);
//            *count_ = 1;
        }

                       ~ref_counted_()
		{
            if (!count_ || !--*count_)
            {
//            SmallObject<>::operator delete(count_, sizeof(unsigned int));
                delete count_;
            }
            else
            {
                storage_policy::release();
            }
        }

        pointer_type    clone(pointer_type const& val)
        {
            ++*count_;
            return val;
        }

        void            swap(ref_counted_& rhs)
        {
            std::swap(count_, rhs.count_);
        }

        static
        void            on_release(ref_counted_& sp)
        {
            if (sp.count_ && *sp.count_ > 1) throw bad_release();
        }

        static
        void            reset(ref_counted_& sp)
        {
            if (sp.count_)
            {
                --*sp.count_;
                sp.count_ = 0;
            }
        }

    public:             // Public Interface
        friend inline
        unsigned        use_count(ref_counted_ const& p)
        {
            return p.count_ ? *(p.count_) : 0U ;
        }

    private:            // Implementation
        ref_counted_&   operator=(ref_counted_ const&);
        BOOST_DECLARE_TEMPLATE_FRIEND(U, ref_counted_)

        unsigned*       count_;
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_REF_COUNTED_HPP
