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
#include <boost/spirit/phoenix/core.hpp>
#include <boost/spirit/phoenix/bind.hpp>
#include <boost/spirit/phoenix/function.hpp>
#include <boost/spirit/phoenix/operator.hpp>

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
//using namespace boost::phoenix;
using namespace boost::wave;

void print (const context_t::iterator_type  & start, 
            const context_t::iterator_type  & end);

// just a little debugging support for decl_grammar
void break_here (context_iter_t ,context_iter_t );


struct decl_module_action {
	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {
		ref.push_header ();
		ref.out ((format ("\nnamespace %s {") % value.get_value ()).str ());
	}

	template<typename T, typename IterT>
	void act (T& ref, IterT const& start, IterT const& end) const {
		ref.push_header ();
		ref.out ((format ("\nnamespace %s {") % start->get_value()).str ());
	}
};

struct finish_decl_action {
	template<typename T>
	struct result { typedef void type; };

/*	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {
		// we want the iterated range!
		std::cout << "finish_decl: what do I do with this?" << std::endl;
		throw value;
	}*/
	
	template<typename T, typename IterT>
	//void act (T& ref, IterT const& start, IterT const& end) const {
	void operator()(T& ref, IterT const& start, IterT const& end) const {
		ref.out (start, end);
		ref.pop ();
	}
};

struct emit_action {
	template<typename T>
	struct result { typedef void type; }; 
	
/*	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {
		// we want the iterated range!
		std::cout << "emit: what do I do with this?" << std::endl;
		throw value;
	} */
	
	template<typename T, typename IterT>
	//void act (T& ref, IterT const& start, IterT const& end) const {
	void operator()(T& ref, IterT const& start, IterT const& end) const {
		ref.out (start, end);
	}	
};

struct swap_header_action {
	template< typename T>
	void act (T& ref) const {
		ref.swap_header ();
	}	
};

struct swap_source_action {
	template< typename T>
	void act (T& ref) const {
		ref.swap_source ();
	}	
};


struct import_module_action {
	template< typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {
		string s(value.get_value ().begin (), value.get_value().end ());
		ref.include_module (s);
	}
	template<typename T, typename IterT>
	void act (T& ref, IterT const& start, IterT const& end) const {
		string s(start->get_value ().begin (), start->get_value().end ());
		ref.include_module (s);
	}
};

struct save_token_action {
	template<typename Container, typename Value>
	void act (Container& ref, Value const& value) const {
		string s(value.get_value ().begin (), value.get_value().end ());
		ref.push_back (s);
	}
	template<typename Container, typename Iter>
	void act (Container& ref, Iter const& start, Iter const& end) const {
		string s(start->get_value ().begin (), start->get_value().end ());
		ref.push_back (s);
	}
};

ref_actor<OutputDelegate, swap_header_action>
go_public (OutputDelegate& del);

ref_actor<OutputDelegate, swap_source_action>
go_private (OutputDelegate& del);

ref_value_actor<vector<string>, save_token_action>
save_as (vector<string>& value);

ref_value_actor<OutputDelegate, import_module_action>
import_module (OutputDelegate& del);

boost::phoenix::function<emit_action> emit;
boost::phoenix::function<finish_decl_action> finish_decl;

// ref_value_actor<OutputDelegate, emit_action>
// emit (OutputDelegate& del);

ref_value_actor<OutputDelegate,decl_module_action>
decl_module( OutputDelegate& del );

// ref_value_actor<OutputDelegate, finish_decl_action>
// finish_decl( OutputDelegate& del );

#endif
