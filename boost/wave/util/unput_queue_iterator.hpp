/*=============================================================================
    Wave: A Standard compliant C++ preprocessor library

    Definition of the unput queue iterator
    
    Copyright (c) 2001-2004 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(UNPUT_QUEUE_ITERATOR_HPP_76DA23D0_4893_4AD5_ABCC_6CED7CFB89BC_INCLUDED)
#define UNPUT_QUEUE_ITERATOR_HPP_76DA23D0_4893_4AD5_ABCC_6CED7CFB89BC_INCLUDED

#include <list>
#include <boost/iterator_adaptors.hpp>

#include <boost/wave/wave_config.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost {
namespace wave {
namespace util {

#if !defined(BOOST_ITERATOR_ADAPTORS_VERSION) || \
    BOOST_ITERATOR_ADAPTORS_VERSION < 0x0200

template <typename TokenT, typename ContainerT>
class unput_queue_policies : public boost::default_iterator_policies
{
public:
    unput_queue_policies(ContainerT &unput_queue_)
    :   unput_queue(unput_queue_)
    {}
    
    unput_queue_policies &operator= (unput_queue_policies const &rhs)
    { 
        unput_queue = rhs.unput_queue; 
        return *this; 
    }
    
    template <typename BaseT>
    void initialize(BaseT &)
    {}

    template <typename IteratorAdaptorT>
    typename IteratorAdaptorT::reference 
    dereference(const IteratorAdaptorT &x) const
    { 
        if (x.policies().unput_queue.size() > 0)
            return x.policies().unput_queue.front();
        return *x.base(); 
    }

    template <typename IteratorAdaptorT>
    void 
    increment(IteratorAdaptorT &x)
    { 
        if (x.policies().unput_queue.size() > 0) {
        // there exist pending tokens in the unput queue
            x.policies().unput_queue.pop_front();
        }
        else {
        // the unput_queue is empty, so advance the base iterator
            ++x.base(); 
        }
    }

    template <typename IteratorAdaptorT1, typename IteratorAdaptorT2>
    bool 
    equal(const IteratorAdaptorT1 &x, const IteratorAdaptorT2 &y) const
    { 
    // two iterators are equal, if both begin() iterators of the queue objects
    // are equal and the base iterators too
        return 
           (x.policies().unput_queue.begin() == y.policies().unput_queue.begin() ||
            (0 == x.policies().queuesize() && 0 == y.policies().queuesize())) &&
            x.base() == y.base(); 
    }
    
    typename ContainerT::size_type queuesize() const 
    { return unput_queue.size(); }

    ContainerT &get_unput_queue() { return unput_queue; }
    
private:
    ContainerT &unput_queue;
};

///////////////////////////////////////////////////////////////////////////////
//
//  unput_queue_iterator
//
//      The unput_queue_iterator templates encapsulates an unput_queue together
//      with the direct input to be read after the unput queue is emptied
//
//      This version is for the old iterator_adaptors (Boost V1.30.x)
//
///////////////////////////////////////////////////////////////////////////////
template <typename IteratorT, typename TokenT, typename ContainerT>
class unput_queue_iterator 
:   public boost::iterator_adaptor<
        IteratorT, unput_queue_policies<TokenT, ContainerT>, TokenT, 
        TokenT const &, TokenT const *>
{
    typedef 
        boost::iterator_adaptor<
            IteratorT, unput_queue_policies<TokenT, ContainerT>, TokenT, 
            TokenT const &, TokenT const *
        >
        base_t;
    typedef unput_queue_policies<TokenT, ContainerT> policies_t;
        
public:
    typedef ContainerT  container_t;
    typedef IteratorT   iterator_t;
    
    unput_queue_iterator(IteratorT const &it, ContainerT &queue)
    :   base_t(it, policies_t(queue))
    {}
    
    ContainerT &get_unput_queue() { return policies().get_unput_queue(); }
    IteratorT &get_base_iterator() { return base(); }
};

#else // BOOST_ITERATOR_ADAPTORS_VERSION < 0x0200

///////////////////////////////////////////////////////////////////////////////
//
//  unput_queue_iterator
//
//      The unput_queue_iterator templates encapsulates an unput_queue together
//      with the direct input to be read after the unput queue is emptied
//
//      This version is for the new iterator_adaptors (should be released with
//      Boost V1.31.0)
//
///////////////////////////////////////////////////////////////////////////////
template <typename IteratorT, typename TokenT, typename ContainerT>
class unput_queue_iterator 
:   public boost::iterator_adaptor<
        unput_queue_iterator<IteratorT, TokenT, ContainerT>, 
            IteratorT, TokenT const, std::forward_iterator_tag>
{
    typedef boost::iterator_adaptor<
                unput_queue_iterator<IteratorT, TokenT, ContainerT>, 
                IteratorT, TokenT const, std::forward_iterator_tag>
        base_t;
        
public:
    typedef ContainerT  container_t;
    typedef IteratorT   iterator_t;
    
    unput_queue_iterator(IteratorT const &it, ContainerT &queue)
    :   base_t(it), unput_queue(queue)
    {}
    
    ContainerT &get_unput_queue() { return unput_queue; }
    ContainerT const &get_unput_queue() const { return unput_queue; }
    IteratorT &get_base_iterator() { return base_t::base_reference(); }
    IteratorT const &get_base_iterator() const { return base_t::base_reference(); }
    
    unput_queue_iterator &operator= (unput_queue_iterator const &rhs)
    { 
        if (this != &rhs) {
            unput_queue = rhs.unput_queue; 
            base_t::operator=(rhs);
        }
        return *this; 
    }

    typename base_t::reference dereference() const
    {
        if (unput_queue.size() > 0)
            return unput_queue.front();
        return *base_t::base_reference(); 
    }

    void increment()
    { 
        if (unput_queue.size() > 0) {
        // there exist pending tokens in the unput queue
            unput_queue.pop_front();
        }
        else {
        // the unput_queue is empty, so advance the base iterator
            ++base_t::base_reference(); 
        }
    }

    template <
        typename OtherDerivedT, typename OtherIteratorT, 
        typename V, typename C, typename R, typename D
    >   
    bool equal(
        boost::iterator_adaptor<OtherDerivedT, OtherIteratorT, V, C, R, D> 
        const &x) const
    {
    // two iterators are equal, if both begin() iterators of the queue objects
    // are equal and the base iterators too
        OtherDerivedT const &rhs = static_cast<OtherDerivedT const &>(x);
        return 
            (unput_queue.begin() == rhs.get_unput_queue().begin() ||
                (0 == unput_queue.size() && 0 == rhs.get_unput_queue().size())) &&
            get_base_iterator() == rhs.get_base_iterator(); 
    }

private:
    ContainerT &unput_queue;
};

#endif // BOOST_ITERATOR_ADAPTORS_VERSION < 0x0200

namespace impl {

    ///////////////////////////////////////////////////////////////////////////
    template <typename IteratorT, typename TokenT, typename ContainerT>
    struct gen_unput_queue_iterator {
    
        typedef unput_queue_iterator<IteratorT, TokenT, ContainerT> return_t;
        
        static return_t
        generate(ContainerT &queue, IteratorT const &it)
        {
            return return_t(it, queue);
        }
    };
    
    template <typename IteratorT, typename TokenT, typename ContainerT>
    struct gen_unput_queue_iterator<
        unput_queue_iterator<IteratorT, TokenT, ContainerT>, 
            TokenT, ContainerT> 
    {
        typedef unput_queue_iterator<IteratorT, TokenT, ContainerT> return_t;
        
        static return_t 
        generate(ContainerT &queue, return_t const &it)
        {
            return return_t(it.base(), queue);
        }
    };
    
    ///////////////////////////////////////////////////////////////////////////
    template <typename IteratorT>
    struct assign_iterator {
    
        static void 
        do_ (IteratorT &dest, IteratorT const &src)
        {
            dest = src;
        }
    };
    
#if !defined(BOOST_ITERATOR_ADAPTORS_VERSION) || \
    BOOST_ITERATOR_ADAPTORS_VERSION < 0x0200
    
    template <typename IteratorT, typename TokenT, typename ContainerT>
    struct assign_iterator<
        unput_queue_iterator<IteratorT, TokenT, ContainerT> > 
    {
        typedef unput_queue_iterator<IteratorT, TokenT, ContainerT> iterator_t;
        
        static void 
        do_ (iterator_t &dest, iterator_t const &src)
        {
            dest.base() = src.base();
            dest.policies() = src.policies();
        }
    };
    
#endif // BOOST_ITERATOR_ADAPTORS_VERSION < 0x0200

    ///////////////////////////////////////////////////////////////////////////
    //
    // Look for the first non-whitespace token and return this token id.
    // Note though, that the embedded unput_queues are not touched in any way!
    //
    template <typename IteratorT>
    struct next_token {
    
        static boost::wave::token_id  
        peek(IteratorT it, IteratorT end, bool skip_whitespace = true)
        {
            using namespace boost::wave;
            if (skip_whitespace) {
                for (++it; it != end; ++it) {
                    if (!IS_CATEGORY(*it, WhiteSpaceTokenType) &&
                        T_NEWLINE != token_id(*it))
                    {
                        break;  // stop at the first non-whitespace token
                    }
                }
            }
            else {
                ++it;           // we have at least to look ahead
            }
            if (it != end) 
                return token_id(*it);
            return T_EOI;
        }
    };
    
    template <typename IteratorT, typename TokenT, typename ContainerT>
    struct next_token<
        unput_queue_iterator<IteratorT, TokenT, ContainerT> > {
        
        typedef unput_queue_iterator<IteratorT, TokenT, ContainerT> iterator_t;
        
        static boost::wave::token_id 
        peek(iterator_t it, iterator_t end, bool skip_whitespace = true)
        {
            using namespace boost::wave;
            
        typename iterator_t::container_t &queue = it.get_unput_queue();
        
        // first try to find it in the unput_queue
            if (0 != queue.size()) {
            typename iterator_t::container_t::iterator cit = queue.begin();
            typename iterator_t::container_t::iterator cend = queue.end();

                if (skip_whitespace) {            
                    for (++cit; cit != cend; ++cit) {
                        if (!IS_CATEGORY(*cit, WhiteSpaceTokenType) &&
                            T_NEWLINE != token_id(*cit))
                        {
                            break;  // stop at the first non-whitespace token
                        }
                    }
                }
                else {
                    ++cit;          // we have at least to look ahead
                }
                if (cit != cend) 
                    return token_id(*cit);
            }
            
        // second try to move on into the base iterator stream
        typename iterator_t::iterator_t base_it = it.get_base_iterator();
        typename iterator_t::iterator_t base_end = end.get_base_iterator();

            if (0 == queue.size())
                ++base_it;  // advance, if the unput queue is empty

            if (skip_whitespace) {
                for (/**/; base_it != base_end; ++base_it) {
                    if (!IS_CATEGORY(*base_it, WhiteSpaceTokenType) &&
                        T_NEWLINE != token_id(*base_it))
                    {
                        break;  // stop at the first non-whitespace token
                    }
                }
            }
            if (base_it == base_end)
                return T_EOI;

            return token_id(*base_it);
        }
    };

    // Skip all whitespace characters and queue the skipped characters into the
    // given container
    template <typename IteratorT>
    inline boost::wave::token_id 
    skip_whitespace(IteratorT &first, IteratorT const &last)
    {
        using namespace cpplexer;
        
    token_id id = next_token<IteratorT>::peek(first, last, false);

        if (IS_CATEGORY(id, WhiteSpaceTokenType)) {
            do {
                ++first;
                id = next_token<IteratorT>::peek(first, last, false);
            } while (IS_CATEGORY(id, WhiteSpaceTokenType));
        }
        ++first;
        return id;
    }
    
    template <typename IteratorT, typename ContainerT>
    inline boost::wave::token_id 
    skip_whitespace(IteratorT &first, IteratorT const &last, ContainerT &queue)
    {
        using namespace cpplexer;
        queue.push_back (*first);       // queue up the current token
        
    token_id id = next_token<IteratorT>::peek(first, last, false);

        if (IS_CATEGORY(id, WhiteSpaceTokenType)) {
            do {
                queue.push_back(*++first);  // queue up the next whitespace 
                id = next_token<IteratorT>::peek(first, last, false);
            } while (IS_CATEGORY(id, WhiteSpaceTokenType));
        }
        ++first;
        return id;
    }

///////////////////////////////////////////////////////////////////////////////
}   // namespace impl

///////////////////////////////////////////////////////////////////////////////
}   // namespace util
}   // namespace wave
}   // namespace boost

#endif // !defined(UNPUT_QUEUE_ITERATOR_HPP_76DA23D0_4893_4AD5_ABCC_6CED7CFB89BC_INCLUDED)
