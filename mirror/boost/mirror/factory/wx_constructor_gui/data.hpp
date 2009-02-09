/**
 * \file boost/mirror/factory/wx_constructor_gui/data.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_DATA_HPP
#define BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_DATA_HPP

#include <wx/wx.h>
#include <wx/sizer.h>

namespace boost {
namespace mirror {
namespace utils {

class wx_constructor_gui_data
{
private:
	wx_constructor_gui_data* parent_data;
	wxWindow* parent_window;
	wxSizer* sizer;
public:
	wx_constructor_gui_data(
		wxWindow* _window,
		wxSizer* _sizer,
		wx_constructor_gui_data* _data
	): parent_data(_data)
	 , parent_window(_window)
	 , sizer(_sizer)
	{ }

	inline wx_constructor_gui_data* get_parent_data(void) const
	{
		return parent_data;
	}

	inline wxWindow* get_window(void) const
	{
		return parent_window;
	}

	inline wxSizer* get_sizer(void) const
	{
		return sizer;
	}
};

} // namespace utils
} // namespace mirror
} // namespace boost

#endif //include guard

