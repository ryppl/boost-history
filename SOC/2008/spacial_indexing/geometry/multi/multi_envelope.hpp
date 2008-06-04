// Geometry Library
//
// Copyright Barend Gehrels, Geodan Holding B.V. Amsterdam, the Netherlands.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef _GEOMETRY_MULTI_ENVELOPE_HPP
#define _GEOMETRY_MULTI_ENVELOPE_HPP

#include <vector>

#include <geometry/envelope.hpp>

namespace geometry
{
	namespace impl
	{
		namespace envelope
		{


			// version for multi_polygon: outer linear_ring
			template<typename B, typename O, typename S>
			inline void envelope_multi_polygon(const O& mp, B& mbr, const S& strategy)
			{
				for (typename O::const_iterator i = mp.begin(); i != mp.end(); i++)
				{
					B b;
					typename S::state_type state;
					envelope_container(i->outer(), mbr, strategy, state);
					state.envelope(mbr);
				}
			}

		} // namespace envelope
	} // namespace impl




	template<typename L,
			template<typename,typename> class V, template<typename> class A>
	inline void envelope(const multi_linestring<L, V, A>& v)
	{
		//return envelope_container<typename L::point_type>(v);
	}

	template<typename PB, typename Y,
			template<typename,typename> class V, template<typename> class A>
	inline void envelope(const multi_polygon<Y, V, A>& v, box<PB>& b)
	{
		impl::envelope::envelope_multi_polygon(v, b, strategy_traits<PB, typename Y::point_type>::envelope());
	}



} // namespace geometry


#endif // _GEOMETRY_MULTI_ENVELOPE_HPP
