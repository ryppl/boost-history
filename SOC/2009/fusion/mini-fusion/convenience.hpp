// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#pragma once

//TODO: constexpr

#include "categories.hpp"
#include "concepts.hpp"

#include <type_traits>
#include <cstddef>

#include <boost/mpl/bool.hpp>

namespace boost{namespace fusion{namespace gsoc{
    namespace detail
    {
        template<class IteratorA,class IteratorB>class is_compareable
        {
            static_assert(std::is_same<typename IteratorA::tag,typename IteratorB::tag>::value,"Different iterator types specified");
            static_assert(std::is_same<typename IteratorA::sequence,typename IteratorB::sequence>::value,"Different iterator sequences");
        };

        template<class Arg>class always_false
        {
        public:
            typedef boost::mpl::false_ type;
        };
    }

    //begin
    namespace result_of
    {
        namespace impl
        {
            template<class Tag>class begin
            {
                static_assert(detail::always_false<Tag>::type::value, "Unsupported sequence");
            };
        }

        template<class Sequence> class begin:
            public impl::begin<typename Sequence::tag>::template apply<Sequence>
        {
        };
    }
    template<class Sequence> typename result_of::begin<typename std::remove_reference<Sequence>::type>::type begin(Sequence&& sequence)
    {
        return result_of::begin<typename std::remove_reference<Sequence>::type>::call(std::forward<Sequence>(sequence));
    }
    template<class Sequence> typename result_of::begin<const Sequence>::type begin(const Sequence& sequence)
    {
        return result_of::begin<const Sequence>::call(sequence);
    }

    //end
    namespace result_of
    {
        namespace impl
        {
            template<class Tag>class end
            {
                static_assert(detail::always_false<Tag>::type::value, "Unsupported sequence");
            };
        }

        template<class Sequence>class end:
            public impl::end<typename Sequence::tag>::template apply<Sequence>
        {
        };
    }
    template<class Sequence> typename result_of::end<typename std::remove_reference<Sequence>::type>::type end(Sequence&& sequence)
    {
        return result_of::end<typename std::remove_reference<Sequence>::type>::call(std::forward<Sequence>(sequence));
    }
    template<class Sequence> typename result_of::end<const Sequence>::type end(const Sequence& sequence)
    {
        return result_of::end<const Sequence>::call(sequence);
    }

    //advance_c
    namespace result_of
    {
        namespace impl
        {
            template<class Tag>class advance_c
            {
                static_assert(detail::always_false<Tag>::type::value, "Unsupported iterator");
            };
        }

        template<class Iterator,int Distance> class advance_c:
            public impl::advance_c<typename Iterator::tag>::template apply<Iterator,Distance>
        {
        };
    }
    template<int Distance,class Iterator> typename result_of::advance_c<Iterator,Distance>::type advance_c(const Iterator& iterator)
    {
        return result_of::advance_c<Iterator,Distance>::call(iterator);
    }

    //distance
    namespace result_of
    {
        namespace impl
        {
            template<class Tag>class distance
            {
                static_assert(detail::always_false<Tag>::type::value, "Unsupported iterator");
            };
        }

        template<class IteratorA,class IteratorB>class distance:
            private detail::is_compareable<IteratorA,IteratorB>,
            public impl::distance<typename IteratorA::tag>::template apply<IteratorA,IteratorB>
        {
        };
    }
    template<class IteratorA,class IteratorB> typename result_of::distance<IteratorA,IteratorB>::type
        distance(const IteratorA& iteratora,const IteratorB& iteratorb)
    {
        return typename result_of::distance<IteratorA,IteratorB>::type();
    }

    //next
    namespace result_of
    {
        namespace impl
        {
            template<class Tag>class next
            {
            public:
                template<class Iterator>class apply
                {
                public:
                    typedef typename result_of::advance_c<Iterator,1>::type type;

                    static type call(const Iterator& iterator)
                    {
                        return gsoc::advance_c<1>(iterator);
                    }
                };
            };
        }

        template<class Iterator>class next:
            public impl::next<typename Iterator::tag>::template apply<Iterator>
        {
        };
    }
    template<class Iterator> typename result_of::next<Iterator>::type next(const Iterator& iterator)
    {
        return result_of::next<Iterator>::call(iterator);
    }

    //prior
    namespace result_of
    {
        namespace impl
        {
            template<class Tag>class prior
            {
            public:
                template<class Iterator>class apply
                {
                public:
                    typedef typename result_of::advance_c<Iterator,-1>::type type;

                    static type call(const Iterator& iterator)
                    {
                        return gsoc::advance_c<-1>(iterator);
                    }
                };
            };
        }

        template<class Iterator>class prior:
            public impl::prior<typename Iterator::tag>::template apply<Iterator>
        {
        };
    }
    template<class Iterator> typename result_of::prior<Iterator>::type prior(const Iterator& iterator)
    {
        return result_of::prior<Iterator>::call(iterator);
    }

    //equal_to
    namespace result_of
    {
        namespace impl
        {
            template<class Tag>class equal_to
            {
            public:
                template<class IteratorA,class IteratorB>class apply
                {
                public:
                    typedef mpl::bool_<std::is_same<
                        typename std::add_const<IteratorA>::type,
                        typename std::add_const<IteratorB>::type>::value> type;
                };
            };
        }

        template<class IteratorA,class IteratorB>class equal_to:
            private detail::is_compareable<IteratorA,IteratorB>,
            public impl::equal_to<typename IteratorA::tag>::template apply<IteratorA,IteratorB>
        {
        };
    }
    template<class IteratorA,class IteratorB> typename result_of::equal_to<IteratorA,IteratorB>::type
        equal_to(const IteratorA& iteratora,const IteratorB& iteratorb)
    {
        return typename result_of::equal_to<IteratorA,IteratorB>::type();
    }

    //size
    namespace result_of
    {
        namespace impl
        {
            template<class Tag>class size
            {
            public:
                template<class Sequence>class apply:
                    public result_of::distance<typename result_of::begin<Sequence>::type,typename result_of::end<Sequence>::type>
                {
                };
            };
        }

        template<class Sequence>class size :
            public impl::size<typename Sequence::tag>::template apply<Sequence>
        {
        };
    }
    template<class Sequence> typename result_of::size<Sequence>::type size(const Sequence&)
    {
        return typename result_of::size<Sequence>::type();
    }

    //empty
    namespace result_of
    {
        namespace impl
        {
            template<class Tag>class empty
            {
            public:
                template<class Sequence> class apply
                {
                public:
                    typedef mpl::bool_<result_of::equal_to<
                        typename result_of::begin<Sequence>::type,
                        typename result_of::end<Sequence>::type>::type::value> type;
                };
            };
        }

        template<class Sequence>class empty:
            public impl::empty<typename Sequence::tag>::template apply<Sequence>
        {
        };
    }
    template<class Sequence> typename result_of::empty<Sequence>::type empty(const Sequence&)
    {
        return typename result_of::empty<Sequence>::type();
    }

    //value_of
    namespace result_of
    {
        namespace impl
        {
            template<class Tag>class value_of
            {
                static_assert(detail::always_false<Tag>::type::value, "Unsupported iterator");
            };
        }

        template<class Iterator>class value_of:
            public impl::value_of<typename Iterator::tag>::template apply<Iterator>
        {
        };
    }

    //deref
    namespace result_of
    {
        namespace impl
        {
            template<class Tag>class deref
            {
                static_assert(detail::always_false<Tag>::type::value, "Unsupported iterator");
            };
        }

        template<class Iterator>class deref:
            public impl::deref<typename Iterator::tag>::template apply<Iterator>
        {
        };
    }
    template<class Iterator>typename result_of::deref<Iterator>::type deref(const Iterator& iterator)
    {
        return result_of::deref<Iterator>::call(iterator);
    }

    //at_c
    namespace result_of
    {
        namespace impl
        {
            template<class Tag>class at_c
            {
            public:
                template<class Container,std::size_t Index>class apply
                {
                public:
                    typedef typename result_of::deref<
                        typename result_of::advance_c<typename result_of::begin<Container>::type,Index>::type>::type type;

                    static type call(Container& container)
                    {
                        return gsoc::deref(gsoc::advance_c<Index>(gsoc::begin(container)));
                    }
                };
            };
        }

        template<class Container,std::size_t Index>class at_c:
            public impl::at_c<typename Container::tag>::template apply<Container,Index>
        {
        };
    }

    template<std::size_t Index,class Container>typename result_of::at_c<const Container,Index>::type at_c(const Container& container)
    {
        return result_of::at_c<const Container,Index>::call(container);
    }

    template<std::size_t Index,class Container>typename result_of::at_c<Container,Index>::type at_c(Container& container)
    {
        return result_of::at_c<Container,Index>::call(container);
    }
}}}

