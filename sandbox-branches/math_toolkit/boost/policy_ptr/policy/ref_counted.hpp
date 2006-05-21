//----------------------------------------------------------------------------
// ref_counted.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_REF_COUNTED_HPP
#define BOOST_REF_COUNTED_HPP
//----------------------------------------------------------------------------
// Headers
#include <cstdlib>                                      // std::size_t
#include <algorithm>                                    // std::swap()
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template 
      < class StoragePolicy
      , class Counter=std::size_t //non-default type needed by refcycle_counted_
      >
    class ref_counted_ : public StoragePolicy
    {
    public:             // Types
        typedef copy_semantics_tag                      ownership_category;
        typedef StoragePolicy                           storage_policy;
        typedef storage_policy                          base_type;
        typedef typename storage_policy::pointer_type   pointer_type;
        typedef Counter                                 counter_type;
        typedef typename storage_policy::stored_param   stored_param;
        typedef typename storage_policy::pointer_param  pointer_param;

    protected:          // Protected Interface
                        ref_counted_(void)
                            : count_(0)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_counted(void)");
        }

                        ref_counted_(ref_counted_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs)),
                            count_(rhs.count_)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_counted(ref_counted const&)");
        }

                        template <typename U>
                        ref_counted_(ref_counted_<U> const& rhs)
                            : base_type(static_cast<
                                typename ref_counted_<U>::base_type const&
                            >(rhs)),
                            count_(BOOST_SP_CONVERT_ARGUMENT(
                                ref_counted_, rhs, count_
                            ))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "ref_counted(ref_counted<U> const&)"
            );
        }

                        ref_counted_(stored_param p)
                            : base_type(p), count_(new counter_type(1))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(
                *this, "ref_counted(stored_param)"
            );
        }

                        template <typename U>
                        ref_counted_(U const& p)
                            : base_type(p), count_(new counter_type(1))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_counted(U const&)");
        }

                        template <typename U, typename V>
                        ref_counted_(U& p, V& d)
                            : base_type(p, d), count_(new counter_type(1))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_counted(U&, V&)");
        }

                        template <typename U, typename V>
                        ref_counted_(U const& p, V& d)
                            : base_type(p, d), count_(new counter_type(1))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_counted(U const&, V&)");
        }

                        template <typename U, typename V>
                        ref_counted_(U& p, V const& d)
                            : base_type(p, d), count_(new counter_type(1))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_counted(U&, V const&)");
        }

                        template <typename U, typename V>
                        ref_counted_(U const& p, V const& d)
                            : base_type(p, d), count_(new counter_type(1))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_counted(U const&, V const&)");
        }

                       ~ref_counted_(void)
		{
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~ref_counted(void)");
            if (!count_ || !--*count_)
            {
                delete count_;
            }
            else
            {
                storage_policy::release();
            }
        }

        stored_param    clone(stored_param p) const
        {
            if(count_) ++*count_;
            return p;
        }

        static
        void            swap(ref_counted_& lhs, ref_counted_& rhs)
        {
            base_type::swap(lhs, rhs);
            std::swap(lhs.count_, rhs.count_);
        }

        static
        void            on_release(ref_counted_ const& sp)
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
        counter_type    use_count(ref_counted_ const& p)
        {
            return p.count_ ? *(p.count_) : 0U ;
        }

    private:            // Implementation
        ref_counted_&   operator=(ref_counted_ const&);
        BOOST_SP_DECLARE_TEMPLATE_FRIEND(U, ref_counted_, 2)

        counter_type*   count_;
    };
    //------------------------------------------------------------------------
    struct ref_counted
    {
        typedef ownership_policy_tag policy_category;

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

