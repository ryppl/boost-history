
#include "../algo_traits.hpp"

namespace boost
{ 

#if 1
    //#ifdef  BOOST_NO_FUNCTION_TEMPLATE_ORDERING
    // We need the tags because the compiler will not know how to
    // choose between begin( pair<T1,T2>& ) and begin( Container& )...
    // Instead we rely on full class specialization which is
    // supported by most compilers

    namespace detail 
    {

        
        
        // tag classes for dispatch 
        class container_algo_pair_tag
        {
        };
        class container_algo_container_tag
        {
        };
        class container_algo_array_tag
        {
        };
        class container_algo_iter_tag
        {
        };

        template <typename Container>
        struct container_algo_make_tag
        {
            typedef container_algo_container_tag       tag;
            typedef typename Container::size_type      size_type;
            typedef typename Container::iterator       result_type;
            typedef typename Container::const_iterator const_result_type;
        };

        template <typename T>
        struct container_algo_make_tag< std::pair<T,T> >
        {
            typedef container_algo_pair_tag            tag;
            typedef typename iterator_traits<T>::difference_type size_type;
            typedef T                                  result_type;
            typedef T                                  const_result_type;
        };
        template <typename T>
        struct container_algo_make_tag< const std::pair<T,T> >
        : public container_algo_make_tag< std::pair<T,T> >
        {
        };

        template< typename T, size_t sz >
        struct container_algo_make_tag< T[sz] >
        {
            typedef container_algo_array_tag                  tag;
//            typedef typename array_traits<T[sz]>::size_type   size_type;
//            typedef typename array_traits<T[sz]>::iterator    result_type;
            typedef typename algo_traits<T[sz]>::size_type   size_type; 
            typedef typename algo_traits<T[sz]>::iterator     result_type;
            typedef const result_type                         const_result_type; 
        };

        template< typename Iterator >
        struct container_algo_make_tag;

        // for container_tag
        template <typename Container>
        inline typename
        container_algo_make_tag<Container>::result_type
        begin(Container& c, container_algo_container_tag const&)
        {
            return c.begin();
        }

        template <typename Container>
        inline typename
        container_algo_make_tag<Container>::const_result_type
        begin(const Container& c, container_algo_container_tag const&)
        {
            return c.begin();
        }

        template <typename Container>
        inline typename
        container_algo_make_tag<Container>::result_type
        end(Container& c, container_algo_container_tag const&)
        {
            return c.end();
        }

        template <typename Container>
        inline typename        
        container_algo_make_tag<Container>::const_result_type
        end(const Container& c, container_algo_container_tag const&)
        {
            return c.end();
        }

        template <typename Container>
        inline typename
        container_algo_make_tag<Container>::size_type
        size(const Container& c, container_algo_container_tag const&)
        {
            return c.size();
        }

        // for pair_tag
        template <typename Iter1, typename Iter2>
        inline typename
        container_algo_make_tag< std::pair<Iter1,Iter2> >::result_type
        begin(const std::pair<Iter1, Iter2>& p, container_algo_pair_tag const&)
        {
            return p.first;
        }

        template <typename Iter1, typename Iter2>
        inline typename
        container_algo_make_tag< std::pair<Iter1, Iter2> >::const_result_type
        end(const std::pair<Iter1, Iter2>& p, container_algo_pair_tag const&)
        {
            return p.second;
        }

        template <typename Iter1, typename Iter2>
        inline typename
        container_algo_make_tag< std::pair<Iter1,Iter2> >::size_type
        size(const std::pair<Iter1, Iter2>& p, container_algo_pair_tag const&)
        {
            return std::distance(p.first, p.second);
        }

        // for array_tag
        template< typename T, size_t sz >
        inline typename 
        container_algo_make_tag<T[sz]>::result_type
        begin( T t[sz] )
        {
            return begin( t );
        }

        template< typename T, size_t sz >
        inline typename
        container_algo_make_tag<T[sz]>::result_type 
        begin( const T t[sz] )
        {
            return begin( t );
        }

        template< typename T, size_t sz >
        inline typename
        container_algo_make_tag<T[sz]>::result_type 
        end( T t[sz] )
        {
            return end( t );
        }

        template< typename T, size_t sz >
        inline typename
        container_algo_make_tag<T[sz]>::result_type 
        end( const T t[sz] )
        {
            return end( t );
        }

        template< typename T, size_t sz >
        inline typename
        container_algo_make_tag<T[sz]>::size_type
        size( const T t[sz] )
        {
            return size( t );
        }

    } // namespace detail

    template <typename T>
    inline typename
    detail::container_algo_make_tag<T>::result_type
    begin(T& t)
    {
        return detail::begin(t, typename 
                             detail::container_algo_make_tag<T>::tag()); 
    }

    template <typename T>
    inline typename
    detail::container_algo_make_tag<T>::const_result_type
    begin(const T& t)
    {
        return detail::begin(t, typename 
                             detail::container_algo_make_tag<T>::tag()); 
    }

    template <typename T>
    inline typename
    detail::container_algo_make_tag<T>::result_type
    end(T& t)
    {
        return detail::end(t, typename 
                           detail::container_algo_make_tag<T>::tag()); 
    }

    template <typename T>
    inline typename
    detail::container_algo_make_tag<T>::const_result_type
    end(const T& t)
    {
        return detail::end(t, typename 
                           detail::container_algo_make_tag<T>::tag());
    }

    template <typename T>
    inline typename
    detail::container_algo_make_tag<T>::size_type
    size(const T& t)
    {
        return detail::size(t, typename 
                            detail::container_algo_make_tag<T>::tag()); 
    }



#else
    // The compiler will handle it fine like this

    template <typename Container>
    typename Container::iterator
    begin(Container& c)
    {
        return c.begin();
    }

    template <typename Container>
    typename Container::const_iterator
    begin(const Container& c)
    {
        return c.begin();
    }

    template <typename Container>
    typename Container::iterator
    end(Container& c)
    {
        return c.end();
    }

    template <typename Container>
    typename Container::const_iterator
    end(const Container& c)
    {
        return c.end();
    }

    template <typename Container>
    typename Container::size_type
    size(const Container& c)
    {
        return c.size();
    }

    template <typename Iter1, typename Iter2>
    Iter1 begin(const std::pair<Iter1, Iter2>& p)
    {
        return p.first;
    }

    template <typename Iter1, typename Iter2>
    Iter2 end(const std::pair<Iter1, Iter2>& p)
    {
        return p.second;
    }

    template <typename Iter1, typename Iter2>
    typename boost::detail::iterator_traits<Iter1>::difference_type
    size(const std::pair<Iter1, Iter2>& p)
    {
        return std::distance(p.first, p.second);
    }
#endif

} // namespace 'boost'
