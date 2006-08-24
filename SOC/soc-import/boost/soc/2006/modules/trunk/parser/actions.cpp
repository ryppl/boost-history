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

ref_actor<TransformContext, export_stmt_action>
export_stmt (TransformContext* ctx) {
	return ref_actor<TransformContext, export_stmt_action>(*ctx);
}

/*
ref_value_actor<OutputDelegate,decl_module_action>
decl_module( OutputDelegate* del ) {
	return ref_value_actor<OutputDelegate,decl_module_action>(*del);
}

ref_value_actor<vector<string>, save_token_action>
save_as (vector<string>& value) {
	return ref_value_actor<vector<string>, save_token_action>(value);
}

ref_value_actor<OutputDelegate, import_module_action>
import_module (OutputDelegate* del) {
	return ref_value_actor<OutputDelegate, import_module_action>(*del);
}

ref_value_actor<OutputDelegate, finish_decl_action>
finish_decl( OutputDelegate* del ) {
	return ref_value_actor<OutputDelegate, finish_decl_action>(*del);
}

ref_value_actor<OutputDelegate, emit_action>
emit (OutputDelegate* del) {
	return ref_value_actor<OutputDelegate, emit_action>(*del);
}

ref_actor<OutputDelegate, go_public_action>
go_public (OutputDelegate* del) {
	return ref_actor<OutputDelegate, go_public_action>(*del);
}

ref_actor<OutputDelegate, go_private_action>
go_private (OutputDelegate* del) {
	return ref_actor<OutputDelegate, go_private_action>(*del);
}
*/