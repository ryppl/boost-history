/**
 * \file boost/mirror/factory/wx_constructor_gui/basic.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_BASIC_HPP
#define BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_BASIC_HPP

#include <wx/wx.h>
#include <wx/sizer.h>

namespace boost {
namespace mirror {
namespace utils {

template <class Product> 
class wx_constructor_gui_basic : public wx_constructor_gui_utils
{
private:
	wxTextCtrl* text_ctl;

	struct text_ctl_maker
	{
		inline wxTextCtrl* operator()(wxWindow* parent) const
		{
			return new wxTextCtrl(parent, wxID_ANY);
		}

		typedef wxTextCtrl result_type;
	};
protected:
	wxString get_text(void) const
	{
		return text_ctl->GetValue();
	}
public:
	template <class Context, class ConstrIndex, class ParamIndex>
	wx_constructor_gui_basic(
		wx_constructor_gui_data* parent_data, 
		Context* pc, 
		ConstrIndex ci, 
		ParamIndex pi
	): text_ctl(make_ctl(text_ctl_maker(), parent_data, pc, ci, pi))
	{ }
};

#define BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI( \
	PRODUCT, \
	FUNCTOR_BODY \
) \
template <> \
class wx_constructor_gui< PRODUCT > \
 : public wx_constructor_gui_basic< PRODUCT > \
{ \
private: \
	typedef wx_constructor_gui_basic< PRODUCT > \
		base_class; \
public: \
	template <class Context, class ConstrIndex, class ParamIndex> \
	wx_constructor_gui( \
		wx_constructor_gui_data* parent_data, \
		Context* pc, \
		ConstrIndex ci, \
		ParamIndex pi \
	): base_class(parent_data, pc, ci, pi) \
	{ } \
 \
	inline PRODUCT operator()(void) FUNCTOR_BODY \
};

BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI(
	cts::bstring,
	{return cts::bstring(get_text().c_str());}
)

#define BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT( \
	TYPE, \
	TEMP_TYPE, \
	CONVERSION \
) BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI( \
	TYPE, \
	{ \
		TEMP_TYPE result; \
		if(!get_text().CONVERSION(&result)) \
			throw ::std::bad_cast(); \
		return TYPE(result); \
	} \
)

BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT(unsigned char, unsigned long, ToULong)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT(short, long, ToLong)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT(unsigned short, unsigned long, ToULong)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT(int, long, ToLong)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT(unsigned int, unsigned long, ToULong)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT(long, long, ToLong)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT(unsigned long, unsigned long, ToULong)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT(float, double, ToDouble)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT(double, double, ToDouble)

} // namespace utils
} // namespace mirror
} // namespace boost

#endif //include guard

