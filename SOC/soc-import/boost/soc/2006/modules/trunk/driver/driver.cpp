#include "../parser/generator.h"
#include "driver.h"
#include "output.h"

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

int
Driver::
execute (int args, const char ** argv) {
	if (args == 1) 
		return 0;
	vector<string> includes;
	vector<string> files;
	char buffer[1024];
	// scan the argument list for includes to add in
	
	for (int i=1; i<args; i++) {
		cout << "processing arg " << argv[i] << endl;
		strncpy (buffer, argv[i], sizeof (buffer));
		// options:
		// -I<include> or -I <include>
		// we won't be very smart about the .map file searching for now.
		if (!strncmp(buffer, "-I", 2)) {
			// see if the path is longer than this, or if not.
			char * p = strip (buffer);
			if (strlen(p) > 2) {
				includes.push_back (string (p+2));
			} else if (i+1 < args) {
				strncpy(buffer, argv[++i], sizeof (buffer));
				includes.push_back (strip(buffer));
				continue;
			}
		} else {
			cout <<"  filename: " << buffer << endl;
			files.push_back(buffer);
		}
	}

	typedef vector<string>::iterator  vec_iter_t;
	for (vec_iter_t file = files.begin (); 
	     file != files.end ();
	     ++file) {
	    cout << "processing file " << *file << endl;
		ifstream f(file->c_str());
		string instring;
		f.unsetf(ios::skipws);
		
		instring = string(istreambuf_iterator<char>(f.rdbuf()),
		                  istreambuf_iterator<char>());
		                  
		context_t ctx (instring.begin (), instring.end (), file->c_str());
		               
		for (vec_iter_t include = includes.begin ();
		     include != includes.end ();
		     ++include)
			ctx.add_include_path(include->c_str());
		
		// calculate header and generated filename names.
		// upon successful generation, put them the .map file.

		try {
			string header_n = replace_suffix(*file, "_gen.h");
			string source_n = replace_suffix(*file, "_gen.cpp");
			
			ofstream header(header_n.c_str());
			ofstream source(source_n.c_str());
			OutputDelegate del (header, source);
			
// 			Generator g(instring.c_str(),del);
			Generator g(ctx,del);
			vector<string> namespaces = g.execute ();
			cout << "The following entries are going to the mapfile:" << endl;
			for (vec_iter_t map = namespaces.begin ();
			     map != namespaces.end ();
			     ++map) {
				cout << *map << ": " << header_n << " " << source_n << ";\n" ;
			}
			cout << "-done processing " << *file << endl;
		} catch (...) {
			cout << "failed processing " << *file << ". sorry." << endl;
			return 1;
		}
		
	}
	return 0;
}



