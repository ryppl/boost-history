#include "setup.h"
#include "generator.h"
#include "decl_grammar.h"
#include "xformctx.h"

#include <iostream>
#include <fstream>

using namespace std;
using namespace boost::spirit;
using namespace boost::wave;

struct skip_parser : public grammar <skip_parser> {
    template<typename ScannerT>
    struct definition {
        rule<ScannerT> skip;
        definition (skip_parser const& self) {
            skip = ( ch_p(T_SPACE) | ch_p(T_NEWLINE) | ch_p(T_SPACE2) 
                     | ch_p(T_CCOMMENT) | ch_p(T_CPPCOMMENT) );
            BOOST_SPIRIT_DEBUG_RULE(skip);
        }
        rule<ScannerT> const& start () { return skip; }
    };

};

static vector<string> execute (context_t&, OutputDelegate*, OutputDelegate*);

vector<string>
SourceGenerator::
execute(OutputDelegate *header, OutputDelegate *source) {
    return ::execute(c, header, source);
}

static
std::vector<std::string>
execute (context_t& ctx, OutputDelegate *header, OutputDelegate *source) {
    std::vector<std::string> retval;
    TransformContext xform;
    decl_grammar g (xform);
    skip_parser s;
    
    context_iter_t start, end;
    start = ctx.begin ();
    end = ctx.end ();
    // default to emit in the source code.
//  del.push_source ();

    // this'll mark up the text, stored in the transformcontext
    if (parse (start, end, g, s).hit) {
        // it parsed, now we begin the output process.
        xform.output(start, end, header, source);
//      context_iter_t it;
//      for (it = start; it != end; ++it) {
//          xform.set_position (it);
//          del.out (*it);
//      }
        cerr << "parsed" << endl;
    } else {
        cerr << "not parsed." << endl;
    }
    return xform.all_modules();
}
