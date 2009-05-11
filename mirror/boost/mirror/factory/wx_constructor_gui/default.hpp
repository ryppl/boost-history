/**
 * \file boost/mirror/factory/wx_constructor_gui/default.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_DEFAULT_HPP
#define BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_DEFAULT_HPP

#include <wx/wx.h>
#include <wx/sizer.h>

namespace boost {
namespace mirror {
namespace utils {

template <class Product>
class wx_constructor_gui
{
private:
	// the data 
	wx_constructor_gui_data data;
	//
	// makes the data
	template <class MetaFunctions, class FuncIndex, class ParamIndex>
	static inline wx_constructor_gui_data make_data(
		wx_constructor_gui_data* parent_data, 
		MetaFunctions mf, 
		FuncIndex fi, 
		ParamIndex pi,
		const Product* opt_default = (const Product*)0
	)
	{
		// we need the parent data
		assert(parent_data != 0);
		// make a label for the static box sizer
		wxString label(
			MetaFunctions::
			template function<FuncIndex>::params::
			template param<ParamIndex>::base_name()
		);
		// the panel which will be the parent of
		// all child widgets
		wxPanel* panel = new wxPanel(
			parent_data->get_window(),
			wxID_ANY 
		);
		panel->SetExtraStyle(
			panel->GetExtraStyle() | wxWS_EX_VALIDATE_RECURSIVELY
		);
		// the sizer
		wxStaticBoxSizer* sizer = new wxStaticBoxSizer(
			wxVERTICAL,
			panel,
			label
		);
		panel->SetSizer(sizer);
		// add the panel to the parent sizer
		parent_data->get_sizer()->Add(
			panel,
			0, //proportion
			wxEXPAND
		);
		// make and return the data
		return wx_constructor_gui_data(
			panel, 
			sizer, 
			parent_data
		);
	}

	// the mirror factory that will create the instances
	// of the Product
	typename make_factory< 
		wx_constructor_gui, 
		Product 
	>::type fact;
public:
	template <class MetaFunctions, class FuncIndex, class ParamIndex>
	wx_constructor_gui(
		wx_constructor_gui_data* parent_data, 
		MetaFunctions mf, 
		FuncIndex fi, 
		ParamIndex pi
	): data(make_data(parent_data, mf, fi, pi))
	 , fact(&data)
	{ }

	inline Product operator()(void)
	{
		return fact();
	}
};


} // namespace utils
} // namespace mirror
} // namespace boost

#endif //include guard

