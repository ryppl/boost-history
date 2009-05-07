#ifndef BOOST_FTMPL_LIST_HPP
#define BOOST_FTMPL_LIST_HPP

namespace boost { namespace ftmpl {

namespace detail {

class list_end;

}

template< typename Head, typename Tail = detail::list_end >
struct list_t;

template< typename Head, typename Tail >
list_t< Head, Tail >& list();

template< typename Head >
list_t< Head >& list();

template< typename Head, typename Tail, typename NewFront >
list_t< NewFront, list_t< Head, Tail > >&
push_front( list_t< Head, Tail >&, NewFront& );

template< typename Head, typename TailContents >
list_t< TailContents >& pop_front( list_t< Head, list_t< TailContents >& );

template< typename Head, typename Tail >
Head& front( list_t< Head, Tail >& );

} }

#endif
