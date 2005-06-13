//----------------------------------------------------------------------------
// ref_linked.hpp
//
// Copyright (C) 2004, Andrei Alexandrescu and David B. Held
// Distributed under the Boost Software License, Version 1.0. (See accompany-
// ing file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//----------------------------------------------------------------------------
#ifndef BOOST_REF_LINKED_HPP
#define BOOST_REF_LINKED_HPP
//----------------------------------------------------------------------------
namespace boost
{
    //------------------------------------------------------------------------
    namespace detail
    {
        //--------------------------------------------------------------------
        class ref_node
        {
        public:             // Interface
                            ref_node(void)
                                : prev_(this), next_(this)  { }

                            ref_node(ref_node const& rhs)
                                : prev_(&rhs), next_(rhs.next_)
            {
                prev_->next_ = this;
                next_->prev_ = this;
            }

            bool            release(void)
            {
                if (next_ == this)
                {
                    BOOST_SP_INVARIANT(prev_ == this);
                    return true;
                }
                prev_->next_ = next_;
                next_->prev_ = prev_;
                return false;
            }

            void            on_release(void) const
            {
                if (next_ == this)
                {
                    BOOST_SP_INVARIANT(prev_ == this);
                }
                else
                {
                    throw bad_release();
                }
            }

            void            swap(ref_node& rhs)
            {
                if (next_ == this)
                {
                    BOOST_SP_INVARIANT(prev_ == this);
                    if (rhs.next_ == &rhs)
                    {
                        BOOST_SP_INVARIANT(rhs.prev_ == &rhs);
                        // Both lists are empty, nothing to do
                        return;
                    }
                    prev_ = rhs.prev_;
                    next_ = rhs.next_;
                    prev_->next_ = next_->prev_ = this;
                    rhs.next_ = rhs.prev_ = &rhs;
                    return;
                }
                if (rhs.next_ == &rhs)
                {
                    rhs.swap(*this);
                    return;
                }
                std::swap(prev_, rhs.prev_);
                std::swap(next_, rhs.next_);
                std::swap(prev_->next_, rhs.prev_->next_);
                std::swap(next_->prev_, rhs.next_->prev_);
            }

        private:            // Implementation
            mutable
            ref_node const* prev_;
            mutable
            ref_node const* next_;
        };
        //--------------------------------------------------------------------
    }   // namespace detail
    //------------------------------------------------------------------------
    template <class StoragePolicy>
    class ref_linked_ : public StoragePolicy
    {
    public:             // Types
        typedef copy_semantics_tag                      ownership_category;
        typedef StoragePolicy                           storage_policy;
        typedef storage_policy                          base_type;
        typedef typename storage_policy::pointer_type   pointer_type;
        typedef typename storage_policy::stored_param   stored_param;
        typedef typename storage_policy::pointer_param  pointer_param;
        typedef detail::ref_node                        ref_node;

    protected:          // Interface
                        ref_linked_(void)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_linked(void)");
        }

                        ref_linked_(ref_linked_ const& rhs)
                            : base_type(static_cast<base_type const&>(rhs)),
                            node_(rhs.node_)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_linked(ref_linked const&)");
        }

                        template <typename U>
                        ref_linked_(ref_linked_<U> const& rhs)
                            : base_type(static_cast<
                                typename ref_linked_<U>::base_type const&
                            >(rhs)),
                            node_(BOOST_SP_CONVERT_ARGUMENT(
                                ref_linked_, rhs, node_
                            ))
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_linked(ref_linked<U> const&");
        }

                        ref_linked_(stored_param p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_linked(stored_param)");
        }

                        template <typename U>
                        ref_linked_(U const& p)
                            : base_type(p)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_linked(U const&)");
        }

                        template <typename U, typename V>
                        ref_linked_(U& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_linked(U&, V&)");
        }

                        template <typename U, typename V>
                        ref_linked_(U const& p, V& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_linked(U const&, V&)");
        }

                        template <typename U, typename V>
                        ref_linked_(U& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_linked(U&, V const&)");
        }

                        template <typename U, typename V>
                        ref_linked_(U const& p, V const& d)
                            : base_type(p, d)
        {
            BOOST_SP_CONSTRUCTOR_HOOK(*this, "ref_linked(U const&, V const&)");
        }

                       ~ref_linked_(void)
        {
            BOOST_SP_DESTRUCTOR_HOOK(*this, "~ref_linked(void)");
            if (!node_.release())
            {
                storage_policy::release();
            }
        }

        static
        stored_param    clone(stored_param p)
        {
            return p;
        }

        static
        void            swap(ref_linked_& lhs, ref_linked_&rhs)
        {
            base_type::swap(lhs, rhs);
            lhs.node_.swap(rhs.node_);
        }

        static
        void            on_release(ref_linked_& sp)
        {
            sp.node_.on_release();
        }

        static
        void            reset(ref_linked_ const&)       { }

    private:            // Implementation
        ref_linked_&    operator=(ref_linked_ const&);
        BOOST_SP_DECLARE_TEMPLATE_FRIEND(U, ref_linked_, 1)

        ref_node        node_;
    };
    //------------------------------------------------------------------------
    struct ref_linked
    {
        typedef ownership_policy_tag policy_category;

        template <typename T>
        struct apply
        {
            typedef ref_linked_<T> type;
        };
    };
    //------------------------------------------------------------------------
}   // namespace boost
//----------------------------------------------------------------------------
#endif // BOOST_REF_LINKED_HPP

