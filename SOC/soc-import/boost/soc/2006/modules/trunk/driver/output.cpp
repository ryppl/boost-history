#include "output.h"
//#include "output_priv.h"
#include <set>
#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace boost;
using namespace boost::filesystem;

#pragma mark -

//
//  this is how I debug bad values of 'this'.  Sometimes hard to track
// down with templates or lambda functions.
//  _List is designed to be callable from gdb.  Only reason check() calls
// it is to make sure the linker doesn't kill it off.
static std::set<OutputDelegate *> s_delegates;
//OutputDelegate* OutputDelegate::s_current;

static bool _Find (OutputDelegate *d) {
	return s_delegates.count (d) > 0;
}

static void _Add (OutputDelegate *d) {
	if (_Find (d)) {
		cerr << "WARNING: multiple constructions of OutputDelegate" << endl;
	} else {
		s_delegates.insert (d);
	}
}

static void _Remove (OutputDelegate *d) {
	if (!_Find (d)) {
		cerr << "WARNING: multiple destructions of OutputDelegate" << endl;
	} else {
		s_delegates.erase (d);
	}
}

static void _List (void) {
	std::set<OutputDelegate*>::iterator it;
	fprintf (stderr, "_List: %d instances.\n", s_delegates.size ());
	for (it = s_delegates.begin (); it != s_delegates.end (); ++it) {
		fprintf (stderr, "Instance at 0x%08x\n", *it);
	}
	fflush (stderr);
}

#pragma mark -

void
OutputDelegate::
check () {
	if (!_Find(this)) {
		_List ();
		throw "Invalid instance!";
	} else if (m_emitted) {
		throw "Already emitted!";
	}
}

OutputDelegate::
OutputDelegate (ostream& s, MapManager *m) 
 : m_stream(s), m_mmgr(m), m_emitted(false) {
	_Add (this);
 }

OutputDelegate::
~OutputDelegate () {
	_Remove (this);
}


void 
OutputDelegate::
text (const string& s) {
	m_text.push_back(s);
}


void 
OutputDelegate::
include (const string& module_name) {
	m_includes.push_back(module_name);
}

void 
OutputDelegate::
emit () {
	vector<string>::iterator it;
	m_emitted = true;
	for (it = m_includes.begin ();
	     it != m_includes.end ();
	     ++it) {
		set<path>  names = m_mmgr->lookup(*it);
		if (names.empty()) {
			cerr << "failed to find module '" << *it << "'" << endl;
		}
		for (set<path>::iterator n = names.begin ();
		     n != names.end ();
		     ++n) {
			if (ends_with(n->leaf(), ".h"))
				m_stream << "#include \"" << n->leaf () << "\"\n";
		}
	}
	
	
	m_stream << endl;
	for (it = m_text.begin ();
	     it != m_text.end ();
	     ++it) {
		m_stream << *it;
	}
}

/*
	//
	// Older API
	void push_source () {
		check ();
		m_mode.push (out_source);
	}
	
	void push_header () {
		check ();
		m_mode.push (out_header);
	}
	
	void pop () {
		check ();
		m_mode.pop ();
	}

	//
	// the next two replace the topmost element with either header or source.
	void swap_header () {
		check ();
		pop ();
		push_header ();
	}
	
	void swap_source () {
		check ();
		pop ();
		push_source ();
	}
	
*/
/*
void 
OutputDelegate::
import_module (std::string & name) {
	include_module(name);
}

// by default, we're in private mode, like a class.

void 
OutputDelegate::
begin_module (std::string & name) {
	Context::Emitter_sp p ( new Context::StringEmitter (
	    (format ("\n// module %s\nnamespace %s {\n") % name % name).str ()
	));
	m_impl->put_header (p);
	m_impl->put_source (p);
	// go_public will clobber the top of the stack, so we'll add a new level first.
	m_impl->m_mode.push(Context::out_none);
	go_private ();
}

void 
OutputDelegate::
end_module () {
	// close off any private sections.
	m_impl->close_private ();
	// close off the namespace %s {
	m_impl->put_header (m_impl->m_close_bracket);
	m_impl->put_source (m_impl->m_close_bracket);
}


void 
OutputDelegate::
go_public () {
	m_impl->close_private ();
	// makes you miss pascal's with (x) {..} statements...
	m_impl->m_mode.pop();
	m_impl->m_mode.push(Context::out_public);
}

void 
OutputDelegate::
go_private () {
	if (m_impl->m_mode.top () != Context::out_private) {
		m_impl->m_mode.pop();
		m_impl->m_mode.push(Context::out_private);
		// open up a private namespace for the private code.
		m_impl->put ((format ("\nnamespace private_%d {\n") % (m_impl->m_priv_cnt++)).str ());
		m_impl->m_in_priv_ns = true;
	}
}

#pragma mark -

void
OutputDelegate::
out (context_iter_t start, context_iter_t end)  {
	check ();
	m_impl->put(new Context::RangeEmitter(std::make_pair(start,end)));
}

void
OutputDelegate::
out (token_t text)  {
	check ();
	token_t::string_type t = text.get_value ();
	out (std::string(t.begin (), t.end ()));
}

void
OutputDelegate::
out (string text) {
	check ();
	m_impl->put(new Context::StringEmitter (text));
}

void
OutputDelegate::
include_module (std::string module) {
	check ();
	m_impl->m_includes.insert(module);
}



void
OutputDelegate::
emit () {
	if (m_impl->m_emitted) {
		throw "Already emitted!";
	}
	
	// first, generate our include text.
	string include_text;
	for (set<string>::iterator it = m_impl->m_includes.begin ();
	     it != m_impl->m_includes.end ();
	     ++it) {
		// lookup the module name
		set<path>  names = m_impl->m_map.lookup(*it);
		for (set<path>::iterator n = names.begin ();
		     n != names.end ();
		     ++n) {
			if (ends_with(n->leaf(), ".h"))
				include_text.append ((format("#include \"%s\"\n") % n->leaf ()).str());	
		}
	}
	include_text.append ("\n");

	typedef std::vector< boost::shared_ptr<Context::Emitter> >::iterator iter_t;
	
	//
	// Now, the header.	
	// TODO: an #ifdef pair probably required.
	m_impl->header << include_text;
	for (iter_t it= m_impl->m_header_text.begin (); it != m_impl->m_header_text.end (); ++it)
		(**it)(m_impl->header);
	// failed attempt at using some of boost's functional abilities...
// 	for_each (m_header_text.begin (), m_header_text.end (),
// 	          bind<void> (*_1, m_header)); //_1(m_header));
	
	//
	// Finally, the source.
	m_impl->source << include_text;
	for (iter_t it= m_impl->m_source_text.begin (); it != m_impl->m_source_text.end (); ++it)
		(**it)(m_impl->source);
// 	for_each (m_source_text.begin (), m_source_text.end (),
// 	          bind<void>(Emitter::operator(), _1, m_source));

	m_impl->m_emitted = true;
}

#pragma mark -
*/
// void
// OutputDelegate::
// out (token_t single) {
// 	token_t::string_type t = single.get_value();
// 	m_impl->source << std::string(t.begin (), t.end ()) << " ";	
// }


