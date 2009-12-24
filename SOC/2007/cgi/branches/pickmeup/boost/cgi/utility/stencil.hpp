//                     -- stencil.hpp --
//
//            Copyright (c) Darren Garvey 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
//
// A utility class for using Google's cTemplate.
//
// Note: requires ctemplate: http://code.google.com/p/google-ctemplate/
//

#ifndef BOOST_CGI_UTILITY_STENCIL_HPP_INCLUDED_20091222_
#define BOOST_CGI_UTILITY_STENCIL_HPP_INCLUDED_20091222_

#include <boost/cgi/common/response.hpp>
#include <ctemplate/template.h>

namespace stencils {

class include_error
  : public std::logic_error
{
public:
  template<typename Str>
  include_error(Str const& section_name, Str const& filename)
     : std::logic_error(
        "Section or file name empty in stencil::include,"
        " with section_name = " + section_name + " and"
        " filename = " + filename)
  {}
};


/// A class used to represent a section in the stencil.
class section
{
public:
  typedef char                          char_type;
  typedef std::basic_string<char_type>  string_type;
  
  section() {}
  section(
      const char_type* name,
      const char_type* filename = ""
    )
    : name(name)
    , filename(filename)
  {
  }
   section(
      string_type const& name,
      string_type const& filename = ""
    )
    : name(name)
    , filename(filename)
  {
  }
  
  string_type name;
  string_type filename;
};

/// A lightweight wrapper around a ctemplate::TemplateDictionary.
class dictionary
{
public:
  typedef ctemplate::TemplateDictionary impl_type;
  typedef std::string string_type;
  typedef dictionary self_type;
  
  dictionary() {}
  dictionary(impl_type* impl)
    : impl(impl)
  {
  } 

  /// Set the varible `name` to `value` in the stencil.
  /**
   * @param name The name of the variable. Use {{name}} in the stencil.
   * @param value The value, can be any type that is supported by
   *              boost::lexical_cast. See the docs for lexical_cast:
   *              http://www.boost.org/libs/lexical_cast
   */
  template<typename T>
  self_type& set(string_type const& name, T value)
  {
    impl->SetValue(name, boost::lexical_cast<string_type>(value));
    return *this;
  }

 /// Set a variable and show a section in one go.
  template<typename T>
  self_type& set(string_type const& name, T value, section const& sec)
  {
    impl->SetValueAndShowSection(
        name,
        boost::lexical_cast<string_type>(value),
        sec.name
      );
    return *this;
  }
  
  /// Show a section.
  self_type& show(section const& sec)
  {
    if (sec.filename.empty())
      impl->ShowSection(sec.name);
    else {
      dictionary d = include(sec);
      impl->ShowSection(sec.name);
    }
    return *this;
  }

  /// Include a file into the stencil.
  /**
   * @return dictionary  The returned sub-dictionary is used to set fields in
   *                     an included stencil.
   *
   * @param section_name The name of the section, and of the marker in
   *                     the stencil. Add {{>section_name}} into the
   *                     stencil to include a file's contents.
   * @param filename     The name of the file to include. This must be set.
   */
  dictionary include(
      string_type const& section_name,
      string_type const& filename
    )
  {
    if (section_name.empty() || filename.empty())
      throw stencils::include_error(section_name, filename);
    dictionary d;
    d.impl = impl->AddIncludeDictionary(section_name);
    d.impl->SetFilename(filename);
    return d;
  }

  /// Include a file into the stencil.
  /**
   * @return dictionary  The returned sub-dictionary is used to set fields in
   *                     an included stencil.
   *
   * @param section      The section to include. Be sure to set *both* the
   *                     section name and filename first.
   */
  dictionary include(section const& sec)
  {
    if (sec.name.empty() || sec.filename.empty())
      throw stencils::include_error(sec.name, sec.filename);
    dictionary d;
    d.impl = impl->AddIncludeDictionary(sec.name);
    d.impl->SetFilename(sec.filename);
    return d;
  }

  /// Add a section into the stencil.
  dictionary add(section const& sec)
  {
    dictionary d;
    d.impl = impl->AddSectionDictionary(sec.name);
    return d;
  }

  impl_type* impl;
};

  /// Set the varible `name` to `value` in the stencil.
  template<>
  dictionary& dictionary::set(string_type const& name, string_type const& value)
  {
    impl->SetValue(name, value);
    return *this;
  }
  
  /// Set a variable and show a section in one go.
  template<>
  dictionary& dictionary::set(
      string_type const& name,
      string_type const& value,
      section const& sec
    )
  {
    impl->SetValueAndShowSection(name, value, sec.name);
    return *this;
  }
  
 }

namespace boost { namespace cgi { namespace common {

  using namespace stencils;

class stencil
  : public boost::cgi::common::response
{
public:
  typedef stencil                       self_type;
  typedef boost::cgi::common::response  base_type;
  typedef ctemplate::Template           stencil_type;
  typedef ctemplate::TemplateDictionary impl_type;
  typedef stencils::section             section;
  typedef stencils::dictionary          dictionary;
  
  enum strip
  {
    do_not_strip = ctemplate::DO_NOT_STRIP,
    strip_blank_lines = ctemplate::STRIP_BLANK_LINES,
    strip_whitespace = ctemplate::STRIP_WHITESPACE
  };
  
  stencil(impl_type* parent_dict)
    : impl(parent_dict->MakeCopy("response"))
  {
  }
  
  stencil(string_type const& root_dir = "")
    : impl(new impl_type("response"))
  {
    if (!root_dir.empty())
      ctemplate::Template::SetTemplateRootDirectory(root_dir);
  }
  
  /// Get the implementation type of the template.
  impl_type& native() { return *impl; }

  bool expand(
      string_type const& template_name,
      enum strip strip_option = strip_blank_lines
    )
  {
    // Save the data already stored in the response.
    string_type content(str());
    // Clear the response body (but not headers).
    clear(false);

    // Get hold of the template to output.
    tmpl = ctemplate::Template::GetTemplate(
              template_name,
              (ctemplate::Strip)strip_option
           );
           
    if (!tmpl)
      return false;
    
    // Add the response content back into the template.
    set("content", content);
    
    // Expand the template and write it to the response.
    string_type body;
    tmpl->Expand(&body, impl.get());
    write(body);

    // All ok.
    return true;
  }
  
  dictionary include(
      string_type const& section_name,
      string_type const& filename
    )
  {
    dictionary d;
    d.impl = impl->AddIncludeDictionary(section_name);
    d.impl->SetFilename(filename);
    return d;
  }

  dictionary include(section const& sec)
  {
    dictionary d;
    d.impl = impl->AddIncludeDictionary(sec.name);
    d.impl->SetFilename(sec.filename);
    return d;
  }

  dictionary add(section const& sec)
  {
    dictionary d;
    d.impl = impl->AddSectionDictionary(sec.name);
    return d;
  }

  /// Set the varible `name` to `value` in the stencil.
  /**
   * @param name The name of the variable. Use {{name}} in the stencil.
   * @param value The value, can be any type that is supported by
   *              boost::lexical_cast. See the docs for lexical_cast:
   *              http://www.boost.org/libs/lexical_cast
   */
  template<typename T>
  self_type& set(string_type const& name, T value)
  {
    impl->SetValue(name, boost::lexical_cast<string_type>(value));
    return *this;
  }

  /// Set a variable and show a section in one go.
  template<typename T>
  self_type& set(string_type const& name, T value, section const& sec)
  {
    impl->SetValueAndShowSection(
        name,
        boost::lexical_cast<string_type>(value),
        sec.name
      );
    return *this;
  }
  
  /// Show a section.
  self_type& show(section const& sec)
  {
    if (sec.filename.empty())
      impl->ShowSection(sec.name);
    else {
      dictionary d = include(sec);
      impl->ShowSection(sec.name);
    }
    return *this;
  }

  stencil_type* tmpl;
  boost::scoped_ptr<impl_type> impl;
  bool expanded;
};

  /// Set the varible `name` to `value` in the stencil.
  template<>
  stencil& stencil::set(string_type const& name, string_type const& value)
  {
    impl->SetValue(name, value);
    return *this;
  }

  /// Set a variable and show a section in one go.
  template<>
  stencil& stencil::set(
      string_type const& name,
      string_type const& value,
      section const& sec
    )
  {
    impl->SetValueAndShowSection(name, value, sec.name);
    return *this;
  }

} } } // namespace boost::cgi::common

namespace boost { namespace cgi {

  using common::stencil;
  
} } // namespace boost::cgi

#endif // BOOST_CGI_UTILITY_STENCIL_HPP_INCLUDED_20091222_
