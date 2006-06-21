#ifndef INCLUDE_ACTIONS_H
#define INCLUDE_ACTIONS_H

#include "lexpolicies.h"
#include "driver/output.h"
#include <boost/format.hpp>

#include <boost/spirit.hpp>
#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator.hpp>
#include <boost/spirit/symbols.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/spirit/actor.hpp>

#include <boost/wave.hpp>
#include <boost/wave/token_ids.hpp>
#include <boost/wave/grammars/cpp_grammar_gen.hpp>
#include <boost/wave/util/pattern_parser.hpp>
#include <boost/wave/cpp_exceptions.hpp>

#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>

using namespace std;
using namespace boost;
using namespace boost::spirit;
using namespace boost::wave;

void print (const context_t::iterator_type  & start, 
            const context_t::iterator_type  & end);


struct match_token : public parser<match_token> {
	typedef match_token self_t;
	
	match_token (token_id id) : m_id(id) {}
	
	template<typename ScannerT>
	typename parser_result<self_t, ScannerT>::type
	parse (ScannerT const& scan) const {
		if (!scan.at_end ()) {
			typename ScannerT::iterator_t save = scan.first;
			++scan;
			if (*save == m_id)
				return scan.create_match (1, nil_t(), save, scan.first);
		} 
		return scan.no_match ();
	}
private:
	token_id   m_id;
};

struct nomatch_single : public parser<nomatch_single> {
	typedef nomatch_single self_t;
	
	nomatch_single (token_id id) : m_id(id) {}

	template<typename ScannerT>
	typename parser_result<self_t, ScannerT>::type
	parse (ScannerT const& scan) const {
		typename ScannerT::iterator_t save = scan.first;
		int len = 0;
		if (!scan.at_end ()) {
			++scan;
			++len;
			if (*save == m_id)
				return scan.no_match ();
		} 
		// if we hit the end, this is equivalent to end_p().
		return scan.create_match (len, nil_t(), save, scan.first);
	}
private:
	token_id   m_id;
};	
/*
struct nomatch_token : public parser<nomatch_token> {
	typedef nomatch_token self_t;
	
//	nomatch_token (token_id id)  {m_ids.push_back(id);}
	nomatch_token (vector<token_id> ids) : m_ids(ids) {}
		
	template<typename ScannerT>
	typename parser_result<self_t, ScannerT>::type
	parse (ScannerT const& scan) const {
		typename ScannerT::iterator_t save = scan.first;
		int len = 0;
		if (!scan.at_end ()) {
			++scan;
			++len;
			for (vector<token_id>::const_iterator it = m_ids.begin ();
			     it != m_ids.end ();
			     ++it) {
				if (*save == *it)
					return scan.no_match ();
			}
		} 
		// if we hit the end, this is equivalent to end_p().
		return scan.create_match (len, nil_t(), save, scan.first);
	}
private:
	vector<token_id>   m_ids;
};
*/
//
// t(FOO) matches FOO
// n(FOO) matches anything but FOO.
match_token    t(token_id tk);
nomatch_single  n(token_id tk);
//nomatch_token  n(token_id t1, token_id t2);

// just a little debugging support for decl_grammar
void break_here (context_iter_t ,context_iter_t );


struct decl_module_action {
	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {
		ref.push_header ();
		ref.out ((format ("\nnamespace %s /*act[1]*/") % value.get_value ()).str ());
	}

	template<typename T, typename IterT>
	void act (T& ref, IterT const& start, IterT const& end) const {
		ref.push_header ();
		ref.out ((format ("\nnamespace %s /*act[2]*/") % start->get_value()).str ());
	}
};

struct finish_decl_action {
	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {
		// we want the iterated range!
		throw value;
	}
	
	template<typename T, typename IterT>
	void act (T& ref, IterT const& start, IterT const& end) const {
		ref.out (string ("/* finish_decl_action { */"));
		ref.out (start, end);
		ref.out (string ("/* } finish_decl_action*/ "));
	}
};

struct save_token_action {
	template<typename Container, typename Value>
	void act (Container& ref, Value const& value) const {
		ref.push_back (value.get_value());
	}
	template<typename Container, typename Iter>
	void act (Container& ref, Iter const& start, Iter const& end) const {
		string s(start->get_value ().begin (), start->get_value().end ());
		ref.push_back (s);
	}
};

ref_value_actor<vector<string>, save_token_action>
save_as (vector<string>& value) {
	return ref_value_actor<vector<string>, save_token_action>(value);
}

ref_value_actor<OutputDelegate,decl_module_action>
decl_module( OutputDelegate& del );

ref_value_actor<OutputDelegate, finish_decl_action>
finish_decl( OutputDelegate& del );

// void begin_export (context_iter_t ,context_iter_t);
// void decl_module (context_iter_t ,context_iter_t);
/* *it is the identifier
  we push a header context, then emit a 'namespace Blah', which
  nicely prefixes what we put out next from the skip_block in export_stmt.
  Also we add the module name to our module list, for returning by 
*/

// void finish_decl (context_iter_t ,context_iter_t);
/* iterators for the entire block,
   we pop() the I/O stack when we're done, and copy the text over to the header.
   */
#endif
