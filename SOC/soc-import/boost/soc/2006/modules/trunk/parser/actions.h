#ifndef INCLUDE_ACTIONS_H
#define INCLUDE_ACTIONS_H

#include "lexpolicies.h"
#include "driver/output.h"
#include "xformctx.h"

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
#include <assert.h>

using namespace std;
using namespace boost;
using namespace boost::spirit;
//using namespace boost::phoenix;
using namespace boost::wave;

void print (const context_t::iterator_type  & start, 
            const context_t::iterator_type  & end);

// just a little debugging support for decl_grammar
void break_here (context_iter_t ,context_iter_t );

struct go_public_action {
	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {}
	
	template<typename T, typename IterT>
	void act (T& ref, IterT const& start, IterT const& end) const {}
};

ref_actor<TransformContext, go_public_action>
go_public (TransformContext* ctx);


struct go_private_action {
	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {}
	
	template<typename T, typename IterT>
	void act (T& ref, IterT const& start, IterT const& end) const {}
};

ref_actor<TransformContext, go_private_action>
go_private (TransformContext* ctx);

struct method_body_action {
	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {}
	
	template<typename T, typename IterT>
	void act (T& ref, IterT const& start, IterT const& end) const {}
};

ref_actor<TransformContext, method_body_action>
method_body (TransformContext* ctx);

struct import_stmt_action {
	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {}
	
	template<typename T, typename IterT>
	void act (T& ref, IterT const& start, IterT const& end) const {}
};

ref_actor<TransformContext, import_stmt_action>
import_stmt (TransformContext* ctx);

struct export_stmt_action {
	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {
		assert(0 && "export_stmt: act without iters called");
	}
	
	template<typename T, typename IterT>
	void act (T& ref, IterT const& start, IterT const& end) const {
		ref.module_export(start,end);
	}
};

ref_actor<TransformContext, export_stmt_action>
export_stmt (TransformContext* ctx);

/*

struct decl_module_action {
	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {
		string s(value.get_value ().begin (), value.get_value().end ());
		ref.begin_module (s);
// 		ref.push_header ();
// 		ref.out ((format ("\nnamespace %s {") % value.get_value ()).str ());
	}

	template<typename T, typename IterT>
	void act (T& ref, IterT const& start, IterT const& end) const {
		string s(start->get_value ().begin (), start->get_value().end ());
		ref.begin_module (s);
// 		ref.push_header ();
// 		ref.out ((format ("\nnamespace %s {") % start->get_value()).str ());
	}
};




struct finish_decl_action {
	template<typename T>
	struct result { typedef void type; };
	
	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {
		//ref.out(value); -- we ignore the closing brace coming in.
		ref.end_module ();
	// we want the iterated range!
// 		std::cout << "finish_decl: what do I do with this?" << std::endl;
// 		throw value;
	} 
	
	template<typename T, typename IterT>
// 	void operator()(OutputDelegate * const & ref, 
// 	                IterT const& start, 
// 	                IterT const& end) const {
	void act (T& ref, IterT const& start, IterT const& end) const {
		//ref.out (start, end);
		ref.end_module ();
// 		ref.pop ();
	}
};

struct emit_action {
	template<typename T>
	struct result { typedef void type; }; 
	
	template<typename T, typename ValueT>
	void act (T& ref, ValueT const& value) const {
		ref.out(value);
		// we want the iterated range!
// 		std::cout << "emit: what do I do with this?" << std::endl;
// 		throw value;
	} 
	
	template<typename T, typename IterT>
	void act (T& ref, IterT const& start, IterT const& end) const {
// 	void operator()(T& ref, IterT const& start, IterT const& end) const {
		ref.out (start, end);
	}	
};

// struct go_public_action {
// 	template< typename T>
// 	void act (T& ref) const {
// 		ref.go_public ();
// 	}	
// };

// struct go_private_action {
// 	template< typename T>
// 	void act (T& ref) const {
// 		ref.go_private ();
// 	}	
// };


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

ref_actor<OutputDelegate, go_public_action>
go_public (OutputDelegate* del);

ref_actor<OutputDelegate, go_private_action>
go_private (OutputDelegate* del);

ref_value_actor<vector<string>, save_token_action>
save_as (vector<string>& value);

ref_value_actor<OutputDelegate, import_module_action>
import_module (OutputDelegate* del);

// boost::phoenix::function<emit_action> emit;
// boost::phoenix::function<finish_decl_action> finish_decl;

ref_value_actor<OutputDelegate, emit_action>
emit (OutputDelegate* del);

ref_value_actor<OutputDelegate, finish_decl_action>
finish_decl( OutputDelegate* del );

ref_value_actor<OutputDelegate,decl_module_action>
decl_module( OutputDelegate* del );

*/

#endif
