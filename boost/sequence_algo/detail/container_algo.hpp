// (C) Copyright Thorsten Ottosen 2002-2003. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// (C) Copyright Jeremy Siek 2001. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.


#include "../algo_traits.hpp"

namespace boost
{ 

    ///////////////////////////////////////////////////////////////////////////
    // begin/end for stl containers
    ///////////////////////////////////////////////////////////////////////////
    
//     namespace detail
//     {
// 	template <typename Container>
// 	inline typename
// 	container_traits<Container>::iterator
// 	begin(Container& c, container_algo_container_tag const&)
// 	{
// 	    return c.begin();
// 	}
	
// 	template <typename Container>
// 	inline typename
// 	container_traits<Container>::const_iterator
// 	begin(const Container& c, container_algo_container_tag const&)
// 	{
// 	    return c.begin();
// 	}
	
// 	template <typename Container>
// 	inline typename
// 	container_traits<Container>::iterator
// 	end(Container& c, container_algo_container_tag const&)
// 	{
//             return c.end();
//         }

//         template <typename Container>
//         inline typename        
//         container_traits<Container>::const_iterator
//         end(const Container& c, container_algo_container_tag const&)
//         {
//             return c.end();
//         }

//         template <typename Container>
//         inline typename
//         container_traits<Container>::size_type
//         size(const Container& c, container_algo_container_tag const&)
//         {
//             return c.size();
//         }

//         // for pair_tag
//         template <typename Iter1, typename Iter2>
//         inline typename
//         container_traits< std::pair<Iter1,Iter2> >::iterator
//         begin(const std::pair<Iter1, Iter2>& p, container_algo_pair_tag const&)
//         {
//             return p.first;
//         }

//         template <typename Iter1, typename Iter2>
//         inline typename
//         container_traits< std::pair<Iter1, Iter2> >::const_iterator
//         end(const std::pair<Iter1, Iter2>& p, container_algo_pair_tag const&)
//         {
//             return p.second;
//         }

//         template <typename Iter1, typename Iter2>
//         inline typename
//         container_traits< std::pair<Iter1,Iter2> >::size_type
//         size(const std::pair<Iter1, Iter2>& p, container_algo_pair_tag const&)
//         {
//             return std::distance(p.first, p.second);
//         }

// #ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

//         // for array_tag
//         template< typename T, std::size_t sz >
//         inline typename 
//         container_traits<T[sz]>::iterator
//         begin( T (&t)[sz], const container_algo_array_tag& )
//         {
//             return t;
//         }

//         template< typename T, std::size_t sz >
//         inline typename
// 	container_traits<T[sz]>::const_iterator 
//         begin( const T (&t)[sz], const container_algo_array_tag&  )
//         {
//             return t;
//         }

//         template< typename T, std::size_t sz >
//         inline typename
//         container_traits<T[sz]>::iterator 
//         end( T (&t)[sz], const container_algo_array_tag&  )
//         {
//             return t + sz;
//         }

//         template< typename T, std::size_t sz >
//         inline typename
//         container_traits<T[sz]>::const_iterator 
//         end( const T (&t)[sz], const container_algo_array_tag&  )
//         {
//             return t + sz;
//         }

//         template< typename T, std::size_t sz >
//         inline typename
//         container_traits<T[sz]>::size_type
//         size( const T (&t)[sz], const container_algo_array_tag&  )
//         {
//             return sz;
//         }

// #endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

//     } // namespace detail

//     template <typename T>
//     inline typename
//     container_traits<T>::iterator
//     begin(T& t)
//     {
//         return begin(t, typename 
//                              container_traits<T>::tag()); 
//     }

//     template <typename T>
//     inline typename
//     container_traits<T>::const_iterator
//     begin(const T& t)
//     {
//         return begin(t, typename 
//                              container_traits<T>::tag()); 
//     }

//     template <typename T>
//     inline typename
//     container_traits<T>::iterator
//     end(T& t)
//     {
//         return end(t, typename 
//                            container_traits<T>::tag()); 
//     }

//     template <typename T>
//     inline typename
//     container_traits<T>::const_iterator
//     end(const T& t)
//     {
//         return end(t, typename 
//                            container_traits<T>::tag());
//     }

//     template <typename T>
//     inline typename
//     container_traits<T>::size_type
//     size(const T& t)
//     {
//         return size(t, typename 
//                             container_traits<T>::tag()); 
//     }


} // namespace 'boost'
