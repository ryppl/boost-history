/**
 * \file boost/mirror/factory/wx_constructor_gui/boolean.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_BOOLEAN_HPP
#define BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_BOOLEAN_HPP

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>

namespace boost {
namespace mirror {
namespace utils {

template <> 
class wx_constructor_gui<bool> : public wx_constructor_gui_utils
{
private:
	wxCheckBox* chb_ctl;
	struct chbctl_maker
	{
		inline wxCheckBox* operator()(wxWindow* parent) const
		{
			return new wxCheckBox(
				parent, 
				wxID_ANY, 
				wxEmptyString
			);
		}

		typedef wxCheckBox result_type;
	};

public:
	template <class Context, class ConstrIndex, class ParamIndex>
	wx_constructor_gui(
		wx_constructor_gui_data* parent_data, 
		Context* pc, 
		ConstrIndex ci, 
		ParamIndex pi
	): chb_ctl(make_ctl(chbctl_maker(), parent_data, pc, ci, pi))
	{ }

	inline bool operator()(void) const 
	{
		return chb_ctl->GetValue();
	}
};

} // namespace utils
} // namespace mirror
} // namespace boost

#endif //include guard

