// Boost.Range library
//
// Copyright Neil Groves 2008. Use, modification and
// distribution is subject to the Boost Software Licence, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// For more information, see http://www.boost.org/libs/range
//

#ifndef BOOST_RANGE_COUNTING_RANGE_HPP_INCLUDED
#define BOOST_RANGE_COUNTING_RANGE_HPP_INCLUDED

#include <boost/range.hpp>
#include <boost/iterator/counting_iterator.hpp>

namespace boost
{

	template<class Incrementable, class Elem>
	inline range<counting_iterator<Incrementable> >
	counting_range(Elem first, Elem last)
	{
		typedef counting_iterator<Incrementable> counting_iterator_t;
		typedef range<counting_iterator_t> result_t;
		return result_t(
			make_counting_iterator(first),
			make_counting_iterator(last));
	}

	template<class Range>
	inline range<counting_iterator<typename range_iterator<Range>::type> >
	counting_range(const Range& r)
	{
		typedef counting_iterator<typename range_iterator<Range>::type> counting_iterator_t;
		typedef range<counting_iterator_t> result_t;
		return result_t(
			counting_iterator_t(begin(r)),
			counting_iterator_t(end(r)));
	}

	template<class Range>
	inline range<counting_iterator<typename range_iterator<Range>::type> >
	counting_range(Range& r)
	{
		typedef counting_iterator<typename range_iterator<Range>::type> counting_iterator_t;
		typedef range<counting_iterator_t> result_t;
		return result_t(
			counting_iterator_t(begin(r)),
			counting_iterator_t(end(r)));
	}
} // namespace boost

#endif // include guard
