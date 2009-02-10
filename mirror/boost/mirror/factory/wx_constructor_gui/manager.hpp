/**
 * \file boost/mirror/factory/wx_constructor_gui/manager.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_MANAGER_HPP
#define BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_MANAGER_HPP

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/choicebk.h>

#include <list>


namespace boost {
namespace mirror {
namespace utils {

template <> 
class wx_constructor_gui<void>
{
private:
	// pointer to the parent data
	wx_constructor_gui_data* parent_data;
	//
	class BookCtrl : public wxChoicebook
	{
	private:
	public:
		BookCtrl(wxWindow* parent, int id)
		 : wxChoicebook(parent, id)
		{ }

		virtual bool Validate()
		{
			// validate only the currently selected page
			wxWindow* visible_child(GetCurrentPage());
			if(visible_child)
				return visible_child->Validate();
			else return true;
		}

		virtual bool TransferDataFromWindow()
		{
			// validate only the currently selected page
			wxWindow* visible_child(GetCurrentPage());
			if(visible_child)
				return visible_child->TransferDataFromWindow();
			else return true;
		}
	};
	// the book ctrl
	BookCtrl* book_ctl;
	static inline BookCtrl* make_book_ctl(
		wx_constructor_gui_data* parent_data,
		int factory_index
	)
	{
		assert(parent_data != 0);
		// make the book ctrl
		BookCtrl* book_ctl = new BookCtrl(
			parent_data->get_window(),
			wxID_ANY
		);
		book_ctl->SetExtraStyle(
			book_ctl->GetExtraStyle() | wxWS_EX_VALIDATE_RECURSIVELY
		);
		
		// add the widget to the sizer
		parent_data->get_sizer()->Add(book_ctl, 0, wxEXPAND);
		// return the pointer
		return book_ctl;
	}
	// list of parameters for the individual pages
	::std::list<wx_constructor_gui_data> params;

	struct param_list_maker
	{
		wxString& str_ref;
		bool first_param;

		param_list_maker(wxString& _ref)
		 : str_ref(_ref)
		 , first_param(true)
		{ }

		template <
			typename Parameters,
			typename ParamIndex
		>
		inline void operator()(
			Parameters* ,
			ParamIndex pi
		)
		{
			// if it's not the first parameter
			// prepend the comma
			if(first_param) first_param = false;
			else str_ref.append(wxT(", "));
			// append the type name
			str_ref.append(
				Parameters::param<ParamIndex>::type::base_name()
			);
			// append the space
			str_ref.append(wxT(" "));
			str_ref.append(
				Parameters::base_param_name(pi)
			);
		}
	};

public:
	wx_constructor_gui(
		wx_constructor_gui_data* data, 
		int factory_index
	): parent_data(data)
	 , book_ctl(make_book_ctl(parent_data, factory_index))
	{ }

	template <class Product, class ConstructorIndex>
	inline wx_constructor_gui_data* param(Product*, ConstructorIndex ci) 
	{
		assert(parent_data);
		// create a panel and a sizer for the page
		wxPanel* panel = new wxPanel(
			book_ctl, 
			wxID_ANY
		);
		panel->SetExtraStyle(
			panel->GetExtraStyle() | wxWS_EX_VALIDATE_RECURSIVELY
		);
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		panel->SetSizer(sizer);
		//
		wxString label(
			BOOST_MIRRORED_TYPE(Product)::base_name()
		);
		label.append(wxT("("));
		BOOST_MIRRORED_CONSTRUCTORS(
			Product
		)::constructor<
				ConstructorIndex
			>::params::for_each(param_list_maker(label));
		label.append(wxT(")"));
		// add a new page
		book_ctl->AddPage(panel, label);
		//
		params.push_back(
			wx_constructor_gui_data(
				panel,
				sizer,
				parent_data
			)
		);
		return &params.back();
	}

	inline int index(void) 
	{
		return book_ctl->GetSelection();
	}

};


} // namespace utils
} // namespace mirror
} // namespace boost

#endif //include guard

