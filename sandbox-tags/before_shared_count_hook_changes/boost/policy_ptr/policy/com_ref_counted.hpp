//----------------------------------------------------------------------------
// com_ref_counted.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_COM_REF_COUNTED_HPP
#define BOOST_COM_REF_COUNTED_HPP
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    template <class StoragePolicy>
    struct com_ref_counted_ : public StoragePolicy
    {
    public:             // Types
        typedef copy_semantics_tag                      ownership_category;
        typedef StoragePolicy                           storage_policy;
        typedef storage_policy                          base_type;
        typedef typename storage_policy::pointer_type   pointer_type;

    protected:          // Protected Interface
                        com_ref_counted_(void)          { }

                        com_ref_counted_(com_ref_counted_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs))
                                                        { }

                        template <typename U>
                        com_ref_counted_(com_ref_counted_<U> const& rhs)
                            : base_type(static_cast<
                                typename com_ref_counted_<U>::base_type const&
                            >(rhs))                     { }

                        template <typename U>
                        com_ref_counted_(U& p)
                            : base_type(p)              { }

                        template <typename U>
                        com_ref_counted_(U const& p)
                            : base_type(p)              { }

                        template <typename U, typename V>
                        com_ref_counted_(U& p, V& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        com_ref_counted_(U const& p, V& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        com_ref_counted_(U& p, V const& d)
                            : base_type(p, d)           { }

                        template <typename U, typename V>
                        com_ref_counted_(U const& p, V const& d)
                            : base_type(p, d)           { }

                       ~com_ref_counted_(void)
        {
            pointer_type p(get_impl(*this));
            if (p) p->Release();
            storage_policy::release();
        }

        static
        pointer_type    clone(pointer_type const& p)
        {
            if (p) p->AddRef();
            return p;
        }

        static
        void            swap(com_ref_counted_& lhs, com_ref_counted_& rhs)
        {
            base_type::swap(lhs, rhs);
        }

        static
        void            on_release(com_ref_counted_ const&)
                                                        { }

        static
        void            reset(com_ref_counted_ const&)  { }
    };
    //------------------------------------------------------------------------
    struct com_ref_counted
    {
        typedef ownership_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef com_ref_counted_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_COM_REF_COUNTED_HPP
