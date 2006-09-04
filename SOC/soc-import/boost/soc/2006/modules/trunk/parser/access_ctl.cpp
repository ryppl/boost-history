#include "access_ctl.h"
#include "base_operations.h"
#include "lexpolicies.h"
#include "xformctx.h"
#include <boost/wave/token_ids.hpp>
#include <boost/format.hpp>

#include <iostream>

using namespace std;
using namespace boost::wave;
using boost::format;


// class AccessCtlXForm : public TransformStage {
// 	enum access_mode { mPublic, mPrivate } m_access;
// 	enum parse_mode { mNotFound, mColon, mFound } m_parse;

AccessCtlXForm::
AccessCtlXForm (const context_iter_t& s,
	                const context_iter_t& e)
  : TransformStage (s,e), m_access(mPrivate), m_parse(mNotFound) {}

void 
AccessCtlXForm::
at_start (TransformContext *ctx) {
// 	Operation_p prefix = Operation_p( new StringOp (
// 	   (format ("/*start %s*/") % (m_access == mPrivate? "private":"public")).str ()));
// 	ctx->add_header(prefix);
// 	ctx->add_source(prefix);
}

void 
AccessCtlXForm::
at_end (TransformContext *ctx) {
	if (m_access == mPrivate) {
		Operation_p suffix = Operation_p( new StringOp (
		   (format ("\n}\nusing namespace %s;\n") % m_ns_name).str ()));
		ctx->add_header(suffix);
		ctx->add_source(suffix);
	}
// 	Operation_p suffix = Operation_p( new StringOp (
// 	   (format ("/*end %s*/") % (m_access == mPrivate? "private":"public")).str ()));
// 	ctx->add_header(suffix);
// 	ctx->add_source(suffix);
}
  
OperationPair 
AccessCtlXForm::
process_token (const token_t& tok, TransformContext *ctx) {
	OperationPair result;
	switch (m_parse) {
		case mColon:
			if (tok == T_COLON)
				m_parse = mFound;
			break;
		case mNotFound:
			if (tok == T_PUBLIC || tok == T_PRIVATE) {
				m_parse = mColon;
				m_access = (tok == T_PUBLIC)? mPublic:mPrivate;
				// eat the token, with result's default values of
				// (null, null) ops.
				break;
			} else if (!whitespace(tok)) {
// 				cerr << "[access_ctl: no permission found. got token " 
// 				     << string (tok.get_value().begin (),
// 				                tok.get_value().end ()) << endl;
				m_parse = mFound;
				// we use the default value of m_access, and
				// fall through.
			} else {
// 				cerr << "[access_ctl: skipping token '" 
// 				     << string (tok.get_value().begin (),
// 				                tok.get_value().end ()) <<"'"<< endl;
			}
		case mFound: {
			insert_private (ctx);
			m_parse=mNormal;
			// fall through.
		case mNormal: 
			result.header = Operation_p(new TokenOp(tok));
			result.source = result.header;
			break;
		}
			
	}
	return result;
}

void 
AccessCtlXForm::
insert_private (TransformContext * ctx) {
	static int s_counter;
	if (m_access == mPrivate) {
		// this should pop up before the current token's
		// emitted, which is t
		m_ns_name = (format("_private_%d") % s_counter++).str();
		Operation_p prefix = Operation_p (
			new StringOp ( 
				(format("\n// private section\nnamespace %s {\n")
				 % m_ns_name).str ()));
		ctx->add_header(prefix);
		ctx->add_source(prefix);
	} else {
// 		cerr << "[access_ctl: found a public section]" << endl;
	}
}
							 
OperationPair 
AccessCtlXForm::
process_upstream (OperationPair p, TransformContext *ctx) {
	if (m_parse != mFound) {
		insert_private(ctx);
		m_parse = mFound;
	}
	return p;
}
								
AccessCtlXForm::
~AccessCtlXForm () {}

