/**
 * \file boost/mirror/detail/meta_attribs_outline.hpp
 *  
 *  Meta-attribute outlines
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_META_DETAIL_META_ATTRIBS_OUTLINE_HPP
#define BOOST_MIRROR_META_DETAIL_META_ATTRIBS_OUTLINE_HPP

#include <boost/typeof/typeof.hpp>

namespace boost {
namespace mirror {
namespace detail {


template <typename MetaAttribute, template <class> class Model> 
class meta_attribute_outline
{
private:
	typedef typename MetaAttribute::type type;
	typedef typename MetaAttribute::position position;

	Model<type> model;
	typedef Model<type>* PModel;
public:
	meta_attribute_outline(Model<void>& classModel)
		: model(classModel){ }
	meta_attribute_outline(const Model<void>& classModel)
		: model(classModel){ }

	// the return value type of the getter function 
	BOOST_TYPEOF_NESTED_TYPEDEF_TPL(
		nested_result_of_get,
		(((PModel)0)->get(0, position()))
	)
	typedef typename nested_result_of_get::type result_of_get;

	// the getter member function 
	inline typename result_of_get::type get(void) const
	{
		return model.get(MetaAttribute::base_name(), position());
	}
};

#define BOOST_MIRROR_DECLARE_ATTRIB_OUTLINE(TYPE, NAME) \
	template <template <class> class Model> \
	struct NAME##_outline_holder \
	{ \
		struct outline : public mirror::detail::meta_attribute_outline< \
			mirror::meta_class_attribute< \
				Class, \
				variant_tag, \
				meta_class_attributes<Class, variant_tag>, \
				position_of_##NAME \
			>, \
			Model \
		> \
		{\
			typedef mirror::detail::meta_attribute_outline< \
				mirror::meta_class_attribute< \
					Class, \
					variant_tag, \
					meta_class_attributes<Class, variant_tag>, \
					position_of_##NAME \
				>, \
				Model \
			> base; \
			outline(Model<void>& classModel) \
			: base(classModel){ } \
			outline(const Model<void>& classModel) \
			: base(classModel){ } \
		} NAME; \
		NAME##_outline_holder(Model<void>& classModel) \
		: NAME(classModel){ } \
		NAME##_outline_holder(const Model<void>& classModel) \
		: NAME(classModel){ } \
	}; \
	template <template <class> class Model> \
	static NAME##_outline_holder<Model> get_outline_holder(Model<void>*, position_of_##NAME);


} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard

