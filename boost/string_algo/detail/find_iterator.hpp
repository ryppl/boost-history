//  Boost string_algo library find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_ITERATOR_DETAIL_HPP
#define BOOST_STRING_FIND_ITERATOR_DETAIL_HPP


#include <boost/string_algo/config.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/find.hpp>
#include <boost/iterator.hpp>

namespace boost {
    namespace string_algo { 
        namespace detail {

//  find_iterator policies -----------------------------------------------//

            // match find policy
            struct match_find_policy
            {
                template<
                    typename InputIteratorT,
                    typename FinderT >
                struct policy
                {
                    typedef InputIteratorT input_iterator_type;
                    typedef iterator_range<InputIteratorT> match_type;
                
                    // Construction
                    policy(
                            input_iterator_type Begin,
                            input_iterator_type End, 
                            const FinderT& Finder ) :
                        m_Finder(Finder), 
                        m_Match(Begin,Begin),
                        m_End(End)
                    {
                        advance();
                    }
                    
                    // Access
                    const match_type& match() const
                    {
                        return m_Match;
                    }

                    // Operations
                    void advance()
                    {
                        m_Match=m_Finder(m_Match.end(),m_End);
                    }

                    // Comparison
                    bool operator==( const policy& Other ) const
                    {
                        return 
                            (m_End==Other.m_End) &&
                            (m_Match==Other.m_Match);
                    }

                private:
                    FinderT m_Finder;
                    match_type m_Match;
                    input_iterator_type m_End;
                };
            };

            // split find policy
            struct split_find_policy
            {
                template<
                    typename InputIteratorT,
                    typename FinderT >
                struct policy
                {
                    typedef InputIteratorT input_iterator_type;
                    typedef iterator_range<InputIteratorT> match_type;
                
                    // Construction
                    policy(
                            input_iterator_type Begin,
                            input_iterator_type End, 
                            const FinderT& Finder ) :
                        m_Finder(Finder), 
                        m_Match(Begin,Begin),
                        m_Next(Begin),
                        m_End(End)
                    {
                        advance();
                    }
                    
                    // Access
                    const match_type& match() const
                    {
                        return m_Match;
                    }

                    // Operations
                    void advance()
                    {
                        match_type FindMatch=m_Finder( m_Next, m_End );
                        m_Match=make_range( m_Next, FindMatch.begin() );
                        m_Next=FindMatch.end();
                    }

                    // Comparison
                    bool operator==( const policy& Other ) const
                    {
                        return 
                            (m_End==Other.m_End) &&
                            (m_Next==Other.m_Next) &&
                            (m_Match==Other.m_Match);
                    }

                private:
                    FinderT m_Finder;
                    match_type m_Match;
                    input_iterator_type m_Next;
                    input_iterator_type m_End;
                };
            };

//  find_iterator class  -----------------------------------------------//
        
            // find iterator
            template< 
                typename InputIteratorT,
                typename FinderT,
                typename FindIterPolicyT=match_find_policy >
            class find_iterator : 
                public  boost::iterator<
                    std::forward_iterator_tag,
                    iterator_range<InputIteratorT> >
            {
                typedef BOOST_STRING_TYPENAME
                    FindIterPolicyT::policy<InputIteratorT,FinderT> policy_type;
                typedef BOOST_STRING_TYPENAME
                    policy_type::input_iterator_type input_iterator_type;
                typedef BOOST_STRING_TYPENAME
                    policy_type::match_type match_type;
            
            public:
                // Constructors
                find_iterator( 
                        input_iterator_type Begin,
                        input_iterator_type End, 
                        const FinderT& Finder ) :
                    m_Policy( Begin, End, Finder ) {}

                bool operator==( const find_iterator& Other ) const
                {
                    return m_Policy==Other.m_Policy;
                }
                bool operator!=( const find_iterator& Other ) const
                {
                    return !(m_Policy==Other.m_Policy);
                }
                const match_type& operator*() const
                {
                    return m_Policy.match();
                }
                const match_type* operator->() const
                {
                    return &m_Policy.match();
                }
                find_iterator& operator++()
                {
                    m_Policy.advance();
                    return *this;
                }
                find_iterator& operator++(int)
                {
                    find_iterator<
                        InputIteratorT,
                        FinderT,
                        FindIterPolicyT> tmp=*this;

                    m_Policy.advance();
                    return tmp;
                }

            private:
                policy_type m_Policy;
            };

        } // namespace detail
    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_FIND_ITERATOR_DETAIL_HPP
