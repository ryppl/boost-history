// base_operations.cpp
#include "base_operations.h"
#include "lexpolicies.h"
#include <boost/wave/token_ids.hpp>
#include "driver/output.h"
using namespace boost::wave;
/// puts out space-separated tokens, with newlines instead for
/// right braces '}' and semicolons ';'.
void 
TokenStreamOp::operator () (OutputDelegate *out) {
	typedef std::vector<token_t>::iterator iter_t;
	std::string result;
	for (iter_t it = m_tokens.begin ();
	     it != m_tokens.end ();
	     ++it) {
		result.append (it->get_value().begin (),
		               it->get_value().end ());
		if (   (*it == T_RIGHTBRACE)
		    || (*it == T_SEMICOLON))
		    result.append ("\n");
		else
			result.append (" ");
	}
	out->text(result);
}

void
TokenOp::operator() (OutputDelegate *del) {
	std::string result;
	result.append (m_tok.get_value().begin (),
	               m_tok.get_value().end ());
	if (   (m_tok == T_RIGHTBRACE)
		|| (m_tok == T_SEMICOLON))
		result.append ("\n");
	else
		result.append (" ");
	
	del->text(result);
}

void
StringOp::operator () (OutputDelegate *del) {
	del->text(m_str);
}
