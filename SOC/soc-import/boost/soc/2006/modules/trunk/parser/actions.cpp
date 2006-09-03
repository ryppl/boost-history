#include "actions.h"
#include "driver/output.h"

#include <iostream>

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

ref_value_actor<TransformContext,register_actor<MethodBodyXForm> >
method_body( TransformContext * ctx ) {
	return ref_value_actor<TransformContext,
	                       register_actor<MethodBodyXForm> >(*ctx);
}

ref_value_actor<TransformContext,register_actor<ExportStmtXForm> >
export_stmt ( TransformContext * ctx ) {
	std::cerr <<"[export_stmt]" << std::endl;
	
	return ref_value_actor<TransformContext,
	                       register_actor<ExportStmtXForm> >(*ctx);
}
