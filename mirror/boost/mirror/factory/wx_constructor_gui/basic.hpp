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
#include <boost/mirror/factory/wx_constructor_gui/regex_validator.hpp>

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
		wxString pattern;
		wxString message;

		text_ctl_maker(
			const wxString& pat,
			const wxString& msg
		): pattern(pat)
		 , message(msg)
		{ }

		inline wxTextCtrl* operator()(wxWindow* parent) const
		{
			if(pattern.empty())
				return new wxTextCtrl(
					parent, 
					wxID_ANY
				);
			else return new wxTextCtrl(
				parent, 
				wxID_ANY,
				wxEmptyString,
				wxDefaultPosition,
				wxDefaultSize,
				0, 
				detail::RegExValidator(pattern, message)
			);
		}
		typedef wxTextCtrl result_type;
	};
protected:
	wxString get_text(void) const
	{
		return text_ctl->GetValue();
	}

	template <typename Default>
	void init(const Default* opt_def)
	{
		if(opt_def) text_ctl->SetValue(*opt_def);
	}

	template <typename Default>
	void init(const wxString& fmt, const Default* opt_def)
	{
		if(opt_def) text_ctl->SetValue(
			wxString::Format(
				fmt,
				*opt_def
			) 
		);
	}
public:
	template <class MetaFunctions, class FuncIndex, class ParamIndex>
	wx_constructor_gui_basic(
		wx_constructor_gui_data* parent_data, 
		MetaFunctions mf, 
		FuncIndex fi, 
		ParamIndex pi,
		Product const* opt_default,
		wxString pattern,
		wxString message
	): text_ctl(
		make_ctl(
			text_ctl_maker(pattern, message), 
			parent_data, 
			mf, 
			fi, 
			pi
		)
	){ }
};

#define BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI( \
	PRODUCT, \
	CONSTRUCTOR_BODY, \
	FUNCTOR_BODY, \
	PATTERN, \
	MESSAGE \
) \
template <> \
class wx_constructor_gui< PRODUCT > \
 : public wx_constructor_gui_basic< PRODUCT > \
{ \
private: \
	typedef wx_constructor_gui_basic< PRODUCT > \
		base_class; \
public: \
	template <class MetaFunctions, class FuncIndex, class ParamIndex> \
	wx_constructor_gui( \
		wx_constructor_gui_data* parent_data, \
		MetaFunctions mf, \
		FuncIndex fi, \
		ParamIndex pi, \
		PRODUCT const* opt_default = (PRODUCT const*)0 \
	): base_class( \
		parent_data, \
		mf, \
		fi, \
		pi, \
		opt_default, \
		wxT( PATTERN ), \
		wxT( MESSAGE ) \
	) CONSTRUCTOR_BODY \
 \
	inline PRODUCT operator()(void) FUNCTOR_BODY \
};

BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI(
	cts::bstring,
	{init(opt_default);},
	{return cts::bstring(get_text().c_str());},
	"",
	""
)

#define BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT( \
	TYPE, \
	TEMP_TYPE, \
	FORMAT_STRING, \
	CONVERSION, \
	PATTERN, \
	MESSAGE \
) BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI( \
	TYPE, \
	{init(wxT(FORMAT_STRING), opt_default);}, \
	{ \
		TEMP_TYPE result; \
		if(!get_text().CONVERSION(&result)) \
			throw ::std::bad_cast(); \
		return TYPE(result); \
	}, \
	PATTERN, \
	MESSAGE \
)

#define BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_UINT(TYPE) \
	BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT( \
		TYPE, \
		unsigned long, \
		"%ul", \
		ToULong, \
		"^[+]?[[:digit:]]+$", \
		"invalid unsigned integer" \
	)

#define BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_INT(TYPE) \
	BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT( \
		TYPE, \
		long, \
		"%dl", \
		ToLong, \
		"^[+-]?[[:digit:]]+$", \
		"invalid integer" \
	)

#define BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_FLOAT(TYPE) \
	BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT( \
		TYPE, \
		double, \
		"%f", \
		ToDouble, \
		"^[+-]?[[:digit:]]+\\.?[[:digit:]]*$", \
		"invalid floating point number" \
	)

BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_UINT(unsigned short)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_UINT(unsigned int)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_UINT(unsigned long)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_INT(short)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_INT(int)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_INT(long)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_FLOAT(float)
BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_FLOAT(double)


#undef BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_UINT
#undef BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_INT
#undef BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_FLOAT
#undef BOOST_MIRROR_FACTORY_PLUGINS_SPECIALIZE_WX_CONSTR_GUI_WX_CONVERT

} // namespace utils
} // namespace mirror
} // namespace boost

#endif //include guard

