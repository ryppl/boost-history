//----------------------------------------------------------------------------
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See
// http://www.boost.org/LICENSE_1_0.txt)
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
    template <class StoragePolicy>
    class ref_counted_ : public StoragePolicy
    {
    public:             // Types
        typedef ownership_policy_tag                    policy_category;
        typedef copy_semantics_tag                      ownership_category;
        typedef StoragePolicy                           storage_policy;
        typedef storage_policy                          base_type;
        typedef typename storage_policy::pointer_type   pointer_type;

    protected:          // Protected Interface
                        ref_counted(void)
                            : count_(0)                 { }

                        ref_counted_(ref_counted_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs)),
                            count_(rhs.count_)          { }

                        template <typename U>
                        ref_counted_(ref_counted_<U> const& rhs)
                            : base_type(static_cast<
                                typename ref_counted_<U>::base_type const&
                            >(rhs)),
                            count_(BOOST_SP_CONVERT_ARGUMENT(
                                ref_counted_, rhs, count_
                            ))                          { }

                        template <typename U>
                        ref_counted_(U& p)
                            : base_type(p), count_(new unsigned(1))
                                                        { }

                        template <typename U>
                        ref_counted_(U const& p)
                            : base_type(p), count_(new unsigned(1))
                                                        { }

                        template <typename U, typename V>
                        ref_counted_(U& p, V& d)
                            : base_type(p, d), count_(new unsigned(1))
                                                        { }

                        template <typename U, typename V>
                        ref_counted_(U const& p, V& d)
                            : base_type(p, d), count_(new unsigned(1))
                                                        { }

                        template <typename U, typename V>
                        ref_counted_(U& p, V const& d)
                            : base_type(p, d), count_(new unsigned(1))
                                                        { }

                        template <typename U, typename V>
                        ref_counted_(U const& p, V const& d)
                            : base_type(p, d), count_(new unsigned(1))
                                                        { }

                       ~ref_counted_()
		{
            if (!count_ || !--*count_)
            {
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
    struct ref_counted
    {
        template <typename T>
        struct apply
        {
            typedef ref_counted_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_REF_COUNTED_HPP

