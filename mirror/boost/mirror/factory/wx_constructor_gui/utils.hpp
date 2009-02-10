/**
 * \file boost/mirror/factory/wx_constructor_gui/utils.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_UTILS_HPP
#define BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_UTILS_HPP

#include <wx/wx.h>
#include <wx/sizer.h>

namespace boost {
namespace mirror {
namespace utils {

class wx_constructor_gui_utils
{
protected:
	// makes the data
	template <class CtrlMaker, class Context, class ConstrIndex, class ParamIndex>
	static inline typename CtrlMaker::result_type* make_ctl(
		CtrlMaker make_ctl,
		wx_constructor_gui_data* parent_data, 
		Context* pc, 
		ConstrIndex ci, 
		ParamIndex pi
	)
	{
		// we need the parent data
		assert(parent_data != 0);
		// make a label for the static box sizer
		wxString label(
			BOOST_MIRRORED_CONSTRUCTORS(
				Context
			)::base_param_name(ci, pi)
		);
		// the panel which will be the parent of
		// the child widgets
		wxPanel* panel = new wxPanel(
			parent_data->get_window(),
			wxID_ANY
		);
		panel->SetExtraStyle(
			panel->GetExtraStyle() | wxWS_EX_VALIDATE_RECURSIVELY
		);
		// the sizer
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		panel->SetSizer(sizer);
		// add the panel to the parent sizer
		parent_data->get_sizer()->Add(
			panel,
			0, //proportion
			wxEXPAND
		);
		//
		// make the label
		sizer->Add(
			new wxStaticText(
				panel,
				wxID_ANY,
				label,
				wxDefaultPosition,
				wxDefaultSize,
				wxALIGN_RIGHT
			), 
			1,
			wxEXPAND
		);
		sizer->AddSpacer(8);
		typedef typename CtrlMaker::result_type Ctrl;
		Ctrl* ctl = make_ctl(panel);
		sizer->Add(ctl, 1);
		sizer->AddSpacer(8);
		return ctl;
	}
};


} // namespace utils
} // namespace mirror
} // namespace boost

#endif //include guard

