/**
 * \file examples/factories/simple_gui.cpp
 *
 *  This example shows how to use the configurable factory
 *  with a input user interface implementation to generically 
 *  construct an instance of classes with non default constructors.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include <boost/shared_ptr.hpp>
#include <boost/mirror/factory/wx_constructor_gui.hpp>
#include "./tetrahedron.hpp"


namespace boost {
namespace mirror {
namespace utils {

} // namespace utils
} // namespace mirror
} // namespace boost


class simple_gui_test_app : public wxApp
{
	virtual bool OnInit();
};

IMPLEMENT_APP(simple_gui_test_app)


bool simple_gui_test_app::OnInit()
{
	using namespace ::boost::mirror;
	// we'll be constructing the tetrahedron
	typedef ::test::tetrahedron T;
	// we'll use the wxWidgets-based dialog
	utils::wx_factory_dialog<T> fact(
		0, 
		wxT("GUI test")
	);
	try
	{
		// use the factory to create a tetrahedron
		::boost::shared_ptr<T> t(fact.create());
		// if we were successful
		if(t)
		{
			// show some info 
			wxString message;
			message.Printf(
				wxT("Area of the base: %f\nVolume: %f"), 
				t->base.area(),
				t->volume()
			);
			wxMessageBox(
				message,
				wxT("Tetrahedron info")
			);
		}
	}
	catch(::std::exception& e)
	{
		wxString message;
		wxMessageBox(
			wxString::FromAscii(e.what()),
			wxT("Construction failed"),
			wxOK | wxICON_ERROR
		);
	}
	catch(...)
	{
		wxMessageBox(
			wxT("Unknown error"),
			wxT("Construction failed"),
			wxOK | wxICON_ERROR
		);
	}
	// finito
	return true;
}