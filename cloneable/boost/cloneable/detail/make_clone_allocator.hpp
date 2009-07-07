// Copyright (C) 2009 Christian Schladetsch
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_CLONEABLE_DETAIL_MAKE_CLONEABLE_ALLOCATOR_HPP
#define BOOST_CLONEABLE_DETAIL_MAKE_CLONEABLE_ALLOCATOR_HPP

#include <boost/type_traits/is_convertible.hpp>
#include <boost/cloneable/detail/prefix.hpp>
#include <boost/cloneable/allocator.hpp>

namespace boost
{
    namespace cloneable
    {
        namespace detail
        {
            namespace impl
            {
                /// adapts a given Alloc type, modelling the v1 std::allocator concept, to provide
                /// services required by abstract_allocator
                template <class Alloc>
                struct clone_allocator : abstract_allocator
                {
                    template <class Ty>
                    struct rebind
                    {
                        typedef typename Alloc::template rebind<Ty>::other other_alloc;
                        typedef clone_allocator<other_alloc> other;
                    };

                    typedef typename Alloc::template rebind<char>::other CharAlloc;

                    typedef typename Alloc::size_type size_type;
                    typedef typename Alloc::difference_type difference_type;
                    typedef typename Alloc::pointer pointer;
                    typedef typename Alloc::const_pointer const_pointer;
                    typedef typename Alloc::reference reference;
                    typedef typename Alloc::const_reference const_reference;
                    typedef typename Alloc::value_type value_type;

                    Alloc alloc;
                    clone_allocator() { }
                    template <class Other>
                    clone_allocator(const clone_allocator<Other> &other) : alloc(other.alloc) { }
                    clone_allocator(Alloc a) : alloc(a) { }

                    struct header
                    {
                        abstract_allocator::pointer allocated_ptr;
                        size_t num_bytes;
                    };

                    void destroy(pointer p, size_t = 1)
                    {
                        alloc.destroy(p);
                    }
                    void deallocate(pointer p, size_t n = 1)
                    {
                        alloc.deallocate(p, n);
                    }
                    pointer allocate(size_t n)
                    {
                        return alloc.allocate(n);
                    }
                    //template <class Init>
                    void construct(pointer p, const_reference init)
                    {
                        new (p) value_type(init);
                    }
                    size_t max_size() const
                    {
                        return alloc.max_size();
                    }

                    abstract_allocator::pointer allocate_bytes(size_t num_bytes, size_t /*alignment*/)
                    {
                        CharAlloc alloc;//(*this);
                        return alloc.allocate(num_bytes);
                        
                        // TODO: do correct alignment, store padding information so dealloc 
                        // can retrieve the originally allocated pointer

                        //CharAlloc alloc(*this);
                        //header head;
                        //head.num_bytes = sizeof(header) + num_bytes + alignment;    // don't need this much, but will do for now
                        //abstract_allocator::pointer char_ptr = alloc.allocate(head.num_bytes);
                        //head.allocated_ptr = (header *)char_ptr;
                        //*head.allocated_ptr = head;
                        //abstract_allocator::pointer base = char_ptr + sizeof(header);
                        //base += calc_padding(base, alignment);
                        //return base;
                    }

                    void deallocate_bytes(abstract_allocator::pointer ptr, size_t /*alignment*/)
                    {
                        CharAlloc alloc;//(*this);
                        alloc.deallocate(ptr, 1);
                        
                        // TODO: retreive the originally allocated pointer

                        //header *head = reinterpret_cast<header *>(ptr - sizeof(head));
                        //alloc.deallocate(head->allocated_ptr, head->num_bytes);
                    }

                };

                template <class Alloc, bool>
                struct make_clone_allocator
                {        
                    typedef clone_allocator<Alloc> type;
                };

                template <class Alloc>
                struct make_clone_allocator<Alloc, true>
                {
                    typedef Alloc type;
                };
            }

            template <class Alloc>
            struct make_clone_allocator
            {
                typedef boost::is_convertible<Alloc *, abstract_allocator *> is_convertible;
                BOOST_STATIC_CONSTANT(bool, is_clone_alloc = is_convertible::value);
                typedef typename impl::make_clone_allocator<Alloc, is_clone_alloc>::type type;
            };

        } // namespace detail

        using detail::make_clone_allocator;

        typedef make_clone_allocator<default_allocator> default_clone_allocator;

    } // namespace cloneable

} // namespace boost

#include <boost/cloneable/detail/suffix.hpp>

#endif // BOOST_CLONEABLE_DETAIL_MAKE_CLONEABLE_ALLOCATOR_HPP

//EOF
