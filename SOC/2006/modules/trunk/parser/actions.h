#ifndef INCLUDE_ACTIONS_H
#define INCLUDE_ACTIONS_H

#include "lexpolicies.h"
#include "driver/output.h"
#include "operations.h"
#include "xformctx.h"

#include "method_body.h"
#include "export_stmt.h"
#include "access_ctl.h"
#include "import_stmt.h"
#include "template_stmt.h"

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



// struct register_actor {
//  virtual ~register_actor () {}
//  virtual const Markup& createMarkup (const context_iter_t&  start,
//                                      const context_iter_t&  end) = 0;
//  template<typename T, typename ValueT>
//  void act (T& ref, ValueT const& value) const {
//      throw "register_actor: wrong act() getting called!";
//  }
//  
//  template<typename T, typename IterT>
//  void act (T& ref, IterT const& start, IterT const& end) const {
//      ref.add (createMarkup(start,end));
//  }
// };
template<class Transform>
struct register_actor {
    template<typename T, typename ValueT>
    void act (T& ref, ValueT const& value) const {
        throw "register_actor: wrong act() getting called!";
    }
    
    template<typename T, typename IterT>
    void act (T& ref, IterT const& start, IterT const& end) const {
        ref.add (new Transform(start,end));
    }
};

ref_value_actor<TransformContext,register_actor<MethodBodyXForm> >
method_body ( TransformContext * ctx );

ref_value_actor<TransformContext,register_actor<ExportStmtXForm> >
export_stmt ( TransformContext * ctx );

ref_value_actor<TransformContext,register_actor<AccessCtlXForm> >
access_spec ( TransformContext * ctx );

ref_value_actor<TransformContext,register_actor<AccessCtlXForm> >
access_default ( TransformContext * ctx );

ref_value_actor<TransformContext,register_actor<ImportStmtXForm> >
import_stmt ( TransformContext * ctx );

ref_value_actor<TransformContext,register_actor<TemplateStmtXForm> >
template_stmt ( TransformContext * ctx );
#endif
