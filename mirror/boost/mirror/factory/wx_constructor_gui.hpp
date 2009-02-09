/**
 * \file boost/mirror/factory/wx_constructor_gui.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_HPP
#define BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_HPP

#include <boost/mirror/factory.hpp>

#include <boost/mirror/factory/wx_constructor_gui/data.hpp>
#include <boost/mirror/factory/wx_constructor_gui/utils.hpp>
#include <boost/mirror/factory/wx_constructor_gui/default.hpp>
#include <boost/mirror/factory/wx_constructor_gui/basic.hpp>
#include <boost/mirror/factory/wx_constructor_gui/boolean.hpp>
#include <boost/mirror/factory/wx_constructor_gui/manager.hpp>

#include <wx/statline.h>

#include <assert.h>

namespace boost {
namespace mirror {
namespace utils {

template <class Product>
class wx_factory_dialog
{
private:
	// the dialog 
	wxDialog* dialog;

	// the data passed to the factory
	wx_constructor_gui_data data;
	wx_constructor_gui_data make_data(wxWindow* parent)
	{
		assert(parent != 0);
		//
		// create the main panel od the dialog
		wxPanel* panel = new wxPanel(parent, wxID_ANY);
		// create the main sizer
		wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
		// top padding
		main_sizer->AddSpacer(8);
		// create the horizontal padding sizer
		wxBoxSizer* hz_padding_sizer = new wxBoxSizer(wxHORIZONTAL);
		// add left padding
		hz_padding_sizer->AddSpacer(8);
		// the main sizer
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		panel->SetSizer(sizer);
		hz_padding_sizer->Add(panel, 1, wxEXPAND);
		// add the right padding 
		hz_padding_sizer->AddSpacer(16);
		// add the padding sizer to the main sizer
		main_sizer->Add(hz_padding_sizer, 1, wxEXPAND);
		// add the padding above the buttons
		main_sizer->AddSpacer(8);
		main_sizer->Add(
			new wxStaticLine(
				parent, 
				wxID_ANY,
				wxDefaultPosition,
				wxDefaultSize,
				wxLI_HORIZONTAL
			), 
			0,
			wxEXPAND
		);
		main_sizer->AddSpacer(8);
		//
		// now add the buttons
		main_sizer->Add(
			dialog->CreateStdDialogButtonSizer(wxOK | wxCANCEL),
			0,
			wxEXPAND
		);
		// add bottom padding
		main_sizer->AddSpacer(16);
		// assign the main sizer
		parent->SetSizer(main_sizer);
		//
		// make and return the data
		return wx_constructor_gui_data(panel, sizer, 0);
	}

	// the factory
	factory< wx_constructor_gui, Product > fact;
public:
	wx_factory_dialog(wxWindow* parent, wxString caption)
	 : dialog(new wxDialog(parent, wxID_ANY, caption))
	 , data(make_data(dialog))
	 , fact(&data, 0)
	{
		// and resize the dialog
		dialog->GetSizer()->SetSizeHints(dialog);
	}

	~wx_factory_dialog(void)
	{
		dialog->Destroy();
	}

	inline Product* create(void)
	{
		assert(dialog != 0);
		// center the dialog 
		dialog->Center();
		// show it modally
		if(dialog->ShowModal() == wxID_OK)
		{
			return fact.new_();
		}
		return 0;
	}
};

} // namespace utils
} // namespace mirror
} // namespace boost

#endif //include guard

