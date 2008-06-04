	template<typename MY, typename MP>
	inline void label_info_multi_polygon(const MY& my, label_option option, MP& mp)
	{
		// Just take one.
		// Alternative: take the largest one.
		// Actually the client should label_option all its polygons,
		// so this algorithm is probably not called often.
		mp.resize(0);
		if (my.size() > 0)
		{
			label_info_polygon<MP>(my.front(), option, mp);
		}
	}




	template<typename Y,
			template<typename,typename> class V, template<typename> class A,
			typename MP>
	inline void label_info(const multi_polygon<Y, V, A>& m, label_option option, MP& mp)
	{
		label_info_multi_polygon<typename Y::point_type>(m, option, mp);
	}
