/**
 * \file boost/mirror/factory/wx_constructor_gui/regex_validator.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_REGEX_VALIDATOR_HPP
#define BOOST_MIRROR_FACTORY_WX_CONSTRUCTOR_GUI_REGEX_VALIDATOR_HPP

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/regex.h>
#include <wx/tipwin.h>

#include <assert.h>


namespace boost {
namespace mirror {
namespace utils {
namespace detail {

class RegExValidator: public wxValidator
{
protected:
	wxString pattern;
	wxRegEx regex;
	wxString message;
	wxArrayString* matches;
	wxTipWindow* tip;
	//
    bool CheckValidator(void) const
    {
        wxCHECK_MSG(
			m_validatorWindow, 
			false,
			wxT("No window associated with validator")
		);
        wxCHECK_MSG(
			m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)), 
			false,
            wxT("RegExValidator is only for wxTextCtrl's")
		);
        return true;
    }
private:
	// no assignment
    RegExValidator& operator=(const RegExValidator&);
	void Init(void)
	{
		this->Connect(
			wxEVT_CHAR,
			wxObjectEventFunction(&RegExValidator::OnChar)
		);
	}
public:
    RegExValidator(
		wxString _pattern, 
		wxString msg = wxString(wxT("invalid value")),
		wxArrayString* strings = 0
	): pattern(_pattern)
	 , regex(pattern)
	 , message(msg)
	 , matches(strings)
	 , tip(0)
	{
		Init();
	}

    RegExValidator(const RegExValidator& val)
	 : pattern(val.pattern)
	 , regex(pattern)
	 , message(val.message)
	 , matches(val.matches)
	 , tip(0)
	{
		Init();
	}

	virtual ~RegExValidator(void){ }

	// Make a clone of this validator (or return NULL) - currently necessary
	// if you're passing a reference to a validator.
	// Another possibility is to always pass a pointer to a new validator
	// (so the calling code can use a copy constructor of the relevant class).
	virtual wxObject *Clone() const 
	{
		return new RegExValidator(*this);
	}

	inline wxTextCtrl* GetControl(void)
	{
		assert(CheckValidator());
		return (wxTextCtrl *) m_validatorWindow;
	}

	inline bool QueryValue(wxString& value)
	{
	    if(!CheckValidator())
			return false;
	    // If window is disabled, simply return
	    if (GetControl()->IsEnabled())
			value.assign(GetControl()->GetValue());
		else value.clear();
		return true;
	}

    // Called when the value in the window must be validated.
    // This function can pop up an error message.
    virtual bool Validate(wxWindow *parent)
	{
		wxString val;
		if(!QueryValue(val)) return false;
		bool ok = regex.Matches(val);
		if(!ok && (tip == 0) && !wxValidator::IsSilent())
		{
			tip = new wxTipWindow(
				GetControl(),
				message,
				100, // max length
				&tip
			);
			wxBell();
		}
		return ok;
	}

    // Called to transfer data to the window
    virtual bool TransferToWindow()
	{
		return true;
	}

    // Called to transfer data from the window
    virtual bool TransferFromWindow()
	{
		wxString val;
		if(!QueryValue(val)) return false;
		if(regex.Matches(val))
		{
			if(matches != 0)
			{
				matches->clear();
				for(int i=0,n=regex.GetMatchCount();i<n;++i)
				{
					size_t start, len;
					if(!regex.GetMatch(&start, &len, i))
					{
						return false;
					}
					matches->Add(wxString(val, start, len));
				}
			}
			return true;
		}
		return false;
	}

	bool IsNavigationKey(int key_code)
	{
		return 
			key_code == WXK_END ||
			key_code == WXK_HOME ||
			key_code == WXK_LEFT ||
			key_code == WXK_RIGHT ||
			key_code == WXK_UP ||
			key_code == WXK_DOWN ;
	}

    // Filter keystrokes
    void OnChar(wxKeyEvent& event)
	{
		bool accept = false;
		wxString val;
		if(QueryValue(val))
		{
			// if we've got the value
			int key_code = event.GetKeyCode();
			// get the position of the cursor
			int pos = GetControl()->GetInsertionPoint();
			// if a navigation key was pressed
			if(IsNavigationKey(key_code))
			{
				accept = true;
			}
			// if the delete key was pressed
			else if(key_code == WXK_DELETE)
			{
				// erase the character
				val.erase(pos);
			}
			// if the backspace key was pressed
			else if(key_code == WXK_BACK)
			{
				// erase the previous character
				if(pos > 0)
					val.erase(pos-1);
			}
			else 
			{
				// if this is a regular character
				// insert it into the temporary string
				val.insert(pos, wxString(event.GetUnicodeKey(), 1));
			}
			// if the new string matches the expression
			if(regex.Matches(val))
			{
				// accept the event
				accept = true;
			}
		}
		// if the value is acceptable .. 
		if(accept)
		{
			GetControl()->SetBackgroundColour(
				wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW)
			);
		}
		else
		{
			GetControl()->SetBackgroundColour(
				wxColour(0xFF, 0xA0, 0xA0)
			);
			if(!wxValidator::IsSilent())
			{
				wxBell();
			}
		}
		// accept the event
		event.Skip();
	}
};


} // namespace detail
} // namespace utils
} // namespace mirror
} // namespace boost

#endif //include guard

