#include "driver/config.h"
#include "map/map.h"
#include "setup.h"

#include <iterator>
#include <algorithm>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>
#include <boost/signals.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>

using namespace std;
namespace opt = boost::program_options;
using namespace boost::filesystem;
using namespace boost;

static vector<string> s_sys_inc_paths, s_inc_paths, s_force_paths;
static vector<string> s_macros, s_predef, s_undef;
static vector<string> s_filenames;
static string s_suffix("_gen");
static bool s_longlong = false, s_variadics = false, s_c99 = false, 
            s_preserve_comments = false, s_import = false;

void
configure_mapmanager (MapManager& map) {
    for (vector<string>::const_iterator it = s_sys_inc_paths.begin ();
         it != s_sys_inc_paths.end ();
         ++it) {
        map.add(path(*it));
    }
    for (vector<string>::const_iterator it = s_inc_paths.begin ();
         it != s_inc_paths.end ();
         ++it) {
        map.add(path(*it));
    }
    map.add (path ("."));
}

std::vector<std::string> configure_outputnames () {
    return s_filenames;
}

std::string configure_getsuffix () {
    return s_suffix;
}

static bool s_maps_only = false;
bool configure_procfiles () {
    return !s_maps_only;
}

bool configure_import () {
    return s_import;
}

void configure_context (context_t& context){
    using namespace boost;
    using namespace boost::wave;

    if (s_longlong)
        context.set_language (enable_long_long(context.get_language ()));
    
    if (s_variadics)
        context.set_language (enable_variadics(context.get_language ()));
        
    if (s_c99)
        context.set_language (
            static_cast<language_support>(context.get_language () 
                | support_c99));
                
    if (s_preserve_comments)
        context.set_language(
                enable_preserve_comments(context.get_language()));

    // this one isn't optional.
    context.set_language (language_support (~support_option_emit_line_directives
                                            & context.get_language()));

    for (vector<string>::const_iterator it = s_sys_inc_paths.begin ();
         it != s_sys_inc_paths.end ();
         ++it) {
        context.add_sysinclude_path (it->c_str());
    }
    
    for (vector<string>::const_iterator it = s_inc_paths.begin ();
         it != s_inc_paths.end ();
         ++it) {
        context.add_include_path (it->c_str());
    }

    context_t::iterator_type first = context.begin();
    vector<string>::const_reverse_iterator cit, rend;
    rend = s_force_paths.rend ();
    for (vector<string>::const_reverse_iterator cit = s_force_paths.rbegin(); 
         cit != rend; /**/) {
        string filename(*cit);
        first.force_include(filename.c_str(), ++cit == rend);
    }
    
    for (vector<string>::const_iterator it = s_macros.begin ();
         it != s_macros.end ();
         ++it) {
        context.add_macro_definition (it->c_str());
    }
    
    for (vector<string>::const_iterator it = s_undef.begin ();
         it != s_undef.end ();
         ++it) {
        context.remove_macro_definition (it->c_str());
    }
}

static inline pair<string, string> 
at_option_parser(string const&s)
{
    if ('@' == s[0]) 
        return std::make_pair(string("config-file"), s.substr(1));
    else
        return pair<string, string>();
}

struct token_marker {};

void validate (boost::any& v,
               const vector<string>& values,
               token_marker *, int) {
    opt::validators::check_first_occurrence(v);
    bool marked = false;
    for (vector<string>::const_iterator it = values.begin ();
         it != values.end ();
         ++it) {
        if (*it == "-")
            marked = true;
        else {
            if (marked)
                s_inc_paths.push_back(*it);
            else
                s_sys_inc_paths.push_back(*it);
        }
    }
}

//
// Sets up internal tables for configuring the context and mapmanager
// later on.
// returns a list of filenames.
vector<string> configure (int args, const char **argv) {
    // based mostly off of Hartmut Kaiser's boost/tools/wave/cpp.cpp.
    // a lot of it copy-pasted directly.
    opt::options_description desc_generic ("Options allowed anywhere.");
    desc_generic.add_options () 
        ("output,o", opt::value<vector<string> >()->composing(),
             "specify the name of the translated source file [arg], "
             "or to stdout [-]")
        ("suffix,s", opt::value<string>(&s_suffix)->default_value("_gen"),
             "suffix used for generated .h and .map files.  Defaults to _gen")
        ("include,I", opt::value<token_marker>()->composing (),
             "an additional include directory")
        ("sysinclude,S", opt::value<vector<string> >()->composing (),
             "an additional system include directory")
        ("forceinclude,F", opt::value<vector<string> >()->composing (),
             "force inclusion of specified file")
        ("define,D", opt::value<vector<string> >()->composing(), 
            "specify a macro to define (as macro[=[value]])")
        ("predefine,P", opt::value<vector<string> >()->composing(), 
            "specify a macro to predefine (as macro[=[value]])")
        ("undefine,U", opt::value<vector<string> >()->composing(), 
            "specify a macro to undefine")
        ("long_long", "enable long long support in C++ mode")
        ("variadics", "enable certain C99 extensions in C++ mode")
        ("c99", "enable C99 mode (implies --variadics)")
        ("preserve,p", "preserve comments in output")
        ("import,i", "import module namespaces")
        ;
        

    opt::options_description desc_cmdline 
        ("Options allowed on the command line only");
    desc_cmdline.add(desc_generic);
    desc_cmdline.add_options()
        ("help,h", "print out program usage (this message)")
        ("version,v", "print the version number")
        ("copyright,c", "print out the copyright statement")
        ("map-only", "only process .map files, do not process files")
        ("config-file", opt::value<string>(), 
            "specify a config file (alternatively: @filepath)")
        ("input-file", opt::value<vector<string> >(),
            "a file to process")
        ;
        
    opt::positional_options_description pos_opts;
    pos_opts.add("input-file", -1);
    
    opt::variables_map vm;
    
    try {    
        opt::store (opt::command_line_parser(args,(char **)argv)
                    .options(desc_cmdline)
                    .positional(pos_opts)
                    .extra_parser(at_option_parser)
                    .run (),
                    vm);
        if (vm.count("config-file")) {
            ifstream ifs (vm["config-file"].as<string>().c_str());
            if (!ifs) {
                cout << "Could not open the config file\n";
            } else {
                // Read the whole file into a string
                stringstream ss;
                ss << ifs.rdbuf();
                // Split the file content
                char_separator<char> sep("\n\r");
                tokenizer<char_separator<char> > tok(ss.str(), sep);
                vector<string> args;
                copy(tok.begin(), tok.end(), back_inserter(args));
                // Parse the file and store the options
                opt::store(opt::command_line_parser(args)
                     .options(desc_generic)
                     .run(), 
                    vm);            
            }
        }
        opt::notify (vm);
        
        // now extract the relevant data, and configure
        if (vm.count ("map-only"))
            s_maps_only = true;
         
        if (vm.count ("help"))
            cout << desc_cmdline;
        
        if (vm.count ("version")) {
            cout << "mfront version " << MFRONT_VERSION << endl;
        }
        
        if (vm.count ("copyright")) {
            cout << "mfront Copyright 2006, Boost.org" << endl;
        }
        
        
        if (vm.count ("forceinclude"))
            s_force_paths = vm["forceinclude"].as<vector<string> >();
        vector<string> paths;       
        if (vm.count ("sysinclude")) {
            paths = vm["sysinclude"].as<vector<string> >();
            copy (paths.begin (), paths.end (), back_inserter(s_sys_inc_paths));
        }
        if (vm.count ("include")) {
            paths = vm["include"].as<vector<string> >();
            copy (paths.begin(), paths.end (), back_inserter(s_inc_paths));
        }
        if (vm.count ("define"))
            s_macros = vm["define"].as<vector<string> >();
            
        if (vm.count ("predefine")) {
            paths = vm["predefine"].as<vector<string> >();
            copy (paths.begin (), paths.end (), back_inserter(s_macros));
        }
    
        if (vm.count ("undefine"))
            s_undef = vm["undefine"].as<vector<string> >();
    
        s_preserve_comments = vm.count ("preserve") > 0;
        s_longlong = vm.count ("long_long") > 0;
        s_c99 = vm.count ("c99") > 0;
        s_variadics = (vm.count ("variadics") > 0) || s_c99;
        s_import = vm.count ("import") > 0;
        
        
        if (vm.count ("output")) {
            paths = vm["output"].as<vector<string> >();
            if (paths.size () != vm.count ("input-file")) {
                cout << "Error: Mismatched count of input ("
                     << vm.count ("input-file")
                     << ") and output filenames ("
                     << paths.size ()
                     << ")" << endl;
                exit (1);
            }
            copy (paths.begin (), paths.end(), back_inserter(s_filenames));
        }
        
        if (vm.count ("input-file"))
            return vm["input-file"].as<vector<string> >();
    } catch (opt::error& e) {
        cout << e.what () << endl;
        cout << desc_cmdline;
    }
    
    return vector<string> ();
}



