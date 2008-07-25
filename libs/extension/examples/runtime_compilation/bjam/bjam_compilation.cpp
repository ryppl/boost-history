/*
 * Boost.Extension / bjam_compilation example source:
 *         main file for bjam compilation
 *
 * (C) Copyright Jeremy Pack 2008
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/ for latest version.
 */


#include "../compilation.hpp"

#include <iostream>

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/extension/shared_library.hpp>

namespace boost {
namespace extensions {
namespace impl {
const char* boost_build_contents =
"local rule if-has-file ( file + : dir * ) \n"
"{ \n"
"    local result ;\n"
"    if $(dir) { \n"
"        result = [ GLOB $(dir) : $(file) ] ;\n"
"    } return $(result[1]:P) ; }\n"
" local boost-src = [ if-has-file configure : [ MATCH --boost=(.*)\n"
" : $(ARGV) ] $(BOOST) $(.boost-build-file:D)/../boost ] ;\n"
" local boost-build-src = [ if-has-file bootstrap.jam :\n"
" [ MATCH --boost-build=(.*) : $(ARGV) ] $(BOOST_BUILD_PATH) $(BOOST_BUILD)\n"
" $(boost-src)/tools/build/v2 ] ; BOOST ?= $(boost-src) ;\n"
" BOOST_ROOT ?= $(boost-src) ; boost-build $(boost-build-src) ;\n";

const char* jamfile_preamble =
"import type : change-generated-target-suffix ; \n"
"import type : change-generated-target-prefix ; \n"
"type.change-generated-target-suffix SHARED_LIB : : extension ; \n"
"type.change-generated-target-prefix SHARED_LIB : : lib ; \n"
"import os ; \n"
"local BOOST_ROOT = [ os.environ BOOST_ROOT ] ; \n"
"local BOOST_SANDBOX_ROOT = [ os.environ BOOST_SANDBOX_ROOT ] ; \n"
"project : requirements <include>$(BOOST_SANDBOX_ROOT) <include>$(BOOST_ROOT) \n"
"<toolset>gcc:<find-shared-library>dl \n"
"<toolset>gcc:<linkflags>\"-Wl,-rpath,'$ORIGIN'\" \n"
"<toolset>darwin:<define>DYLD_LIBRARY_PATH=./ : ; \n";

void PrintHeader(std::ostream& out, const std::string& header_location) {
  out << "#include " << header_location << std::endl;
}

}  // namespace impl
class bjam_compilation : public compilation {
public:
  virtual bool run(const std::string& library_name,
                   const std::string& external_function_contents,
                   type_map& types,
                   const std::string& earlier_file_contents = "") const {
    using namespace boost::filesystem;
    path initial_path(current_path());
    if (!compilation_directory_.empty()) {
      std::system(("cd " + compilation_directory_).c_str());
    }
    path compilation_path(current_path());
    ofstream o(compilation_path / "boost-build.jam");
    o << "# Start\n\n"
      << std::string(impl::boost_build_contents) << std::endl;
    ofstream p(compilation_path / "Jamfile.v2");
    p << "# Start\n\n"
      << std::string(impl::jamfile_preamble)
      << " lib " << library_name << " : " << library_name
      << ".cpp ;\ninstall . : " << library_name << " ;"
      << std::endl;
    p.close();
    ofstream r(compilation_path / "Jamroot");
    r << "# empty" << std::endl;
    r.close();
    ofstream cpp_file(compilation_path / (library_name + ".cpp"));
    cpp_file << "// Boost.Extension generated file" << std::endl;
    std::for_each(headers_.begin(), headers_.end(),
                  boost::bind(impl::PrintHeader, boost::ref(cpp_file), _1));
    cpp_file <<
      "\n\nextern \"C\" "
      "void BOOST_EXTENSION_EXPORT_DECL\n"
      "boost_extension_compiled_function("
      "boost::extensions::type_map& types) {\n"
      << external_function_contents << "\n}" << std::endl;
    cpp_file.close();
    int result = 0;
    if ((result = std::system("~/bin/bjam --toolset=darwin --d+2")) != 0) {
      std::cerr << "Compilation failed." << std::endl;
      return false;
    }
    std::cout << "Compilation result: " << result << std::endl;
    shared_library library("lib" + library_name + ".extension");
    if (!library.open()) {
      std::cerr << "Library not found." << std::endl;
      return false;
    }
    typedef void (*compiled_func)(type_map&);
    compiled_func func =
      library.get<void, type_map&>("boost_extension_compiled_function");
    if (!func) {
      std::cerr << "Function not found." << std::endl;
      return false;
    }
    func(types);
    std::system(("cd " + compilation_path.string()).c_str());
    return true;
  }
};
} // namespace extensions
}  // namespace boost


int main(int argc, char* argv[]) {
  using namespace boost::extensions;
  bjam_compilation c;
  c.add_header("<iostream>");
  type_map types;
  if (c.run("test_library",
            "std::cout << \"Inside the shared library: Yay!\" << std::endl;",
            types)) {
    std::cout << "Success!" << std::endl;
  } else {
    std::cout << "Failure!" << std::endl;
  }
  return 0;
}
