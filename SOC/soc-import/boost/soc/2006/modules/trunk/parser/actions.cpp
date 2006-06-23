#include "actions.h"
#include "driver/output.h"

void
print (const context_t::iterator_type  & start, 
       const context_t::iterator_type  & end) {
	context_t::iterator_type s = start;
	std::cout << "(";
	while (s != end) {
		std::cout << s->get_value ();
		++s;
	}
	std::cout << ")" << std::endl;
}

void
break_here (context_iter_t ,context_iter_t ) {
	std::cout << "internal break" << std::endl;
}

ref_value_actor<OutputDelegate,decl_module_action>
decl_module( OutputDelegate& del ) {
	return ref_value_actor<OutputDelegate,decl_module_action>(del);
}

ref_value_actor<OutputDelegate, finish_decl_action>
finish_decl( OutputDelegate& del ) {
	return ref_value_actor<OutputDelegate, finish_decl_action>(del);
}
/*
match_token
t(token_id tk) {
	return match_token(tk);
}

nomatch_single
n(token_id tk) {
	return nomatch_single(tk);
}


nomatch_token
n(token_id t1, token_id t2) {
	vector<token_id> t;
	t.push_back (t1);
	t.push_back (t2);
	return nomatch_token(t);
}
*/

ref_value_actor<vector<string>, save_token_action>
save_as (vector<string>& value) {
	return ref_value_actor<vector<string>, save_token_action>(value);
}

ref_value_actor<OutputDelegate, import_module_action>
import_module (OutputDelegate& del) {
	return ref_value_actor<OutputDelegate, import_module_action>(del);
}

ref_value_actor<OutputDelegate, emit_action>
emit (OutputDelegate& del) {
	return ref_value_actor<OutputDelegate, emit_action>(del);
}
