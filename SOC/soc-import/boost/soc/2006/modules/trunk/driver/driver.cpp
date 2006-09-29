#include "driver.h"
#include <boost/filesystem/operations.hpp>
#include <boost/wave/cpp_exceptions.hpp>
#include <boost/wave/cpplexer/cpplexer_exceptions.hpp>
#include <boost/algorithm/string.hpp>
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
    if (loc > 0)
        src.replace(loc, src.size()-loc, suffix);
    else
        src.append(suffix);
    return src;
}

static bool validate_filename (string s) {
    if (! iends_with(s, ".cpp")) {
        cout << "Ignoring " << s << ": not a .cpp" << endl;
        return false;
    } else {
        return true;
    }
}

namespace {
    class NameGenerator {
    public:
        virtual ~NameGenerator () {}
        virtual string get_name(const string& n, int i) = 0;
    };
    
    class ArrayNameGen : public NameGenerator {
        vector<string> m_names;
    public:
        ArrayNameGen (const vector<string>& v) : m_names(v) {}
        virtual string get_name(const string&, int i) { return m_names[i]; }
    };
    
    class SuffixNameGen : public NameGenerator {
        string m_suffix;
    public:
        SuffixNameGen (const string& s) : m_suffix(s) {}
        virtual string get_name(const string& s, int i) {
            return replace_suffix (s, m_suffix.c_str ());
        }
    };
}

int
Driver::
execute (int args, const char ** argv) {
    if (args <= 1) { 
        cout << "No arguments given, try mfront --help" << endl;
        return 0;
    }
    vector<string> files = configure(args, argv);
    
    auto_ptr<NameGenerator> namegen(new SuffixNameGen (configure_getsuffix ()));

    typedef vector<string>::const_iterator  vec_iter_t;
    vector<string> outputs = configure_outputnames ();
    
    if (!outputs.empty ()) {
        //switch name generation methods
        namegen.reset (new ArrayNameGen (outputs));
    }
    
    int cnt = 0;
    for (vec_iter_t file = files.begin (); 
         file != files.end ();
         ++file, ++cnt) {

        MapManager maps(path(file->c_str()));
        ::configure_mapmanager(maps);
        
        if (!configure_procfiles())
            continue;
        
        ifstream f(file->c_str());
        string instring;
        f.unsetf(ios::skipws);
        
        instring = string(istreambuf_iterator<char>(f.rdbuf()),
                          istreambuf_iterator<char>());
                          
        context_t ctx (instring.begin (), instring.end (), file->c_str());

        ::configure_context(ctx);
         
        // calculate header and generated filename names.
        // upon successful generation, put them the .map file.

        try {
            if (!validate_filename(*file))
                continue;
            
            string header_n = 
                replace_suffix(namegen->get_name(*file,cnt), ".h");
                
            string source_n = 
                replace_suffix(namegen->get_name(*file,cnt), ".cpp");
            
            if (header_n == *file || source_n == *file) {
                cerr << "Refusing to overwrite input file with output for " 
                     << *file << endl;
                continue;
            }
            
            ofstream header(header_n.c_str());
            ofstream source(source_n.c_str());
            
            OutputDelegate header_del (*file, header, &maps);
            OutputDelegate source_del (*file, source, &maps);
            
            header << "// " << header_n << endl;
            source << "// " << source_n << endl;
            source << "#include \"" << header_n << "\"" << endl;
            cerr << "Processing file " << *file << "...";
            SourceGenerator g(ctx);
            vector<string> namespaces = g.execute (&header_del, &source_del);
            header_del.emit ();
            source_del.emit ();
            
            for (vec_iter_t map = namespaces.begin ();
                 map != namespaces.end ();
                 ++map) {
                maps.put(*map, path(header_n));
                maps.put(*map, path(source_n));
            }
            
            // compilers seem happier with a newline as the last char.
            // well, at least some sort of whitespace as the last line (off by 
            // 1 bugs in lexers aren't as infrequent as they should be).
            header << endl;
            source << endl;
        } 
        catch (wave::cpplexer::lexing_exception& e) {
            cerr << *file 
                 << ": " << e.description ()
                 << endl;
            return 1;
        }
        catch (wave::macro_handling_exception& e) {
            cerr << *file 
                 << "(" << e.file_name() << " " 
                 << e.line_no() <<":" << e.column_no() << ")"
                 << ": macro expansion failed: "
                 << e.get_related_name () 
                 << endl;
            return 1;
        }
        catch (wave::preprocess_exception& e) {
            cerr << *file 
                 << "(" << e.file_name() << " " 
                 << e.line_no() <<":" << e.column_no() << ")"           
                 << ": " << e.description ()
                 << endl;
             return 1;
        }
        catch (std::exception& e) {
            cerr << *file << ": " << e.what () << endl;
            return 1;
        }
        
    }
    return 0;
}



