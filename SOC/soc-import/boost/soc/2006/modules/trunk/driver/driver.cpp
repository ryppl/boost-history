#include "driver.h"
#include <boost/filesystem/operations.hpp>
#include <boost/wave/cpp_exceptions.hpp>
#include <boost/wave/cpplexer/cpplexer_exceptions.hpp>
#include "parser/generator.h"
#include "config.h"
#include "output.h"
#include "map/map.h"

using namespace boost;
using namespace boost::wave;
using namespace boost::filesystem;
using namespace std;

static string replace_suffix (string src, const char * suffix) {
	int loc = src.find_last_of(".");
	if (loc)
		src.replace(loc, src.size()-loc, suffix);
	else
		src.append(suffix);
	return src;
}

static char * strip (char * src) {
	char * p = src;
	while (isspace(*p))
		p++;
	// go back from the end and kill any spaces there.
	char * e = p + strlen(p) - 1;
	while (isspace(*e))
		e--;
	*++e = 0;
	return p;
}

/*
static
ostream& operator<<(ostream& o, const vector<string>& v) {
	typedef vector<string>::const_iterator  vec_iter_t;
	
	for (vec_iter_t it = v.begin ();
	     it != v.end ();
	     ++it) {
		o << " " << *it << endl;
	}
	
	return o;
}
*/
int
Driver::
execute (int args, const char ** argv) {
	if (args == 1) 
		return 0;
	vector<string> local_includes;
	vector<string> files;
	char buffer[1024];
	// scan the argument list for includes to add in
	// eventually replace this with boost::program_options 
	for (int i=1; i<args; i++) {
		strncpy (buffer, argv[i], sizeof (buffer));
		// options:
		// -I<include> or -I <include>
		// we won't be very smart about the .map file searching for now.
		if (!strncmp(buffer, "-I", 2)) {
			// see if the path is longer than this, or if not.
			char * p = strip (buffer);
			if (strlen(p) > 2) {
				local_includes.push_back (string (p+2));
			} else if (i+1 < args) {
				strncpy(buffer, argv[++i], sizeof (buffer));
				local_includes.push_back (strip(buffer));
				continue;
			}
		} else {
			files.push_back(buffer);
		}
	}

	typedef vector<string>::const_iterator  vec_iter_t;
	
	for (vec_iter_t file = files.begin (); 
	     file != files.end ();
	     ++file) {

		MapManager maps(path(file->c_str()));
		
		ifstream f(file->c_str());
		string instring;
		f.unsetf(ios::skipws);
		
		instring = string(istreambuf_iterator<char>(f.rdbuf()),
		                  istreambuf_iterator<char>());
		                  
		context_t ctx (instring.begin (), instring.end (), file->c_str());

		::configure_context(ctx);
		::configure_mapmanager(maps);
	     
		for (vec_iter_t it= local_includes.begin ();
		     it != local_includes.end ();
		     ++it)
			ctx.add_include_path(it->c_str());

		// calculate header and generated filename names.
		// upon successful generation, put them the .map file.

		try {
			string header_n = replace_suffix(*file, "_gen.h");
			string source_n = replace_suffix(*file, "_gen.cpp");
			
			ofstream header(header_n.c_str());
			ofstream source(source_n.c_str());
			OutputDelegate del (header, source, maps);
			header << "// " << header_n << endl;
			source << "// " << source_n << endl;
			cout << "Processing file " << *file << "...";
			SourceGenerator g(ctx,del);
			vector<string> namespaces = g.execute ();
			del.emit ();
			for (vec_iter_t map = namespaces.begin ();
			     map != namespaces.end ();
			     ++map) {
				maps.put(*map, path(header_n));
				maps.put(*map, path(source_n));
			}
			
			// compilers seem happier with a newline as the last char.
			// well, at least some sort of whitespace as the last line (off by 1 bugs in
			// lexers aren't as infrequent as they should be).
			header << endl;
			source << endl;
		} 
		catch (wave::cpplexer::lexing_exception& e) {
			cout << *file 
			     << ": " << e.description ()
			     << endl;
			return 1;
		}
		catch (wave::macro_handling_exception& e) {
			cout << *file 
			     << "(" << e.file_name() << " " 
			     << e.line_no() <<":" << e.column_no() << ")"
			     << ": macro expansion failed: "
			     << e.get_related_name () 
			     << endl;
			return 1;
		}
		catch (wave::preprocess_exception& e) {
			cout << *file 
			     << "(" << e.file_name() << " " 
			     << e.line_no() <<":" << e.column_no() << ")"			
			     << ": " << e.description ()
			     << endl;
			 return 1;
		}
		catch (std::exception& e) {
			cout << *file << ": " << e.what () << endl;
			return 1;
		}
		
	}
	return 0;
}



