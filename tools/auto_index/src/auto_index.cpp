// Copyright 2008 John Maddock
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <set>
#include <cstring>
#include <boost/array.hpp>
#include <boost/exception/all.hpp>
#include "auto_index.hpp"

std::string infile, outfile, prefix, last_primary, last_secondary;
std::set<index_info> index_terms;
std::set<std::pair<std::string, std::string> > found_terms;
bool no_duplicates = false;
bool verbose = false;
bool use_section_names = true;
index_entry_set index_entries;
boost::tiny_xml::element_list indexes;
std::list<id_rewrite_rule> id_rewrite_list;
bool internal_indexes = false;
std::string internal_index_type = "section";
boost::regex debug;
file_scanner_set_type file_scanner_set;

int help()
{
   std::cout << "Please refer to the documentation for the correct command line syntax" << std::endl;
   return 1;
}

void eat_whitespace(std::istream & is)
{
   char c = is.peek();
   while(std::isspace(c))
   {
      is.get(c);
      c = is.peek();
   }
}

void eat_block(std::string& result, std::istream & is)
{
   //
   // everything until we get to a closing '>':
   //
   char c;
   while(is.get(c) && c != '>')
   {
      result += c;
      if(c == '\\')
      {
         is.get(c);
         result += c;
      }
   }
   result += c;
}

std::string get_header(std::istream & is)
{
   //
   // We need to get any leading <? and <! elements:
   //
   std::string result;
   eat_whitespace(is);
   if(is.get() != '<')
      throw std::runtime_error("Invalid leading markup in XML file found");
   char c = is.peek();
   while((c == '?') || (c == '!'))
   {
      result += '<';
      eat_block(result, is);
      eat_whitespace(is);
      if(is.get() != '<')
         throw std::runtime_error("Invalid leading markup in XML file found");
      c = is.peek();
      result += '\n';
   }
   return result;
}
//
// Find attribute named "name" in node "node":
//
const std::string* find_attr(boost::tiny_xml::element_ptr node, const char* name)
{
   for(boost::tiny_xml::attribute_list::const_iterator i = node->attributes.begin();
      i != node->attributes.end(); ++i)
   {
      if(i->name == name)
         return &(i->value);
   }
   return 0;
}
//
// Get the ID of the current block scope, basically
// move up the XML tree until we find a valid ID:
//
const std::string* get_current_block_id(node_id const* id)
{
   while((id->id == 0) && (id->prev))
      id = id->prev;
   return id->id;
}
//
// Get the title of the current block scope, basically
// move up the XML tree until we find a valid title:
//
const std::string& get_current_block_title(title_info const* id)
{
   while((id->title.size() == 0) && (id->prev))
      id = id->prev;
   return id->title;
}
//
// Get all the content under this node, with any inline XML
// stripped out:
//
std::string get_consolidated_content(boost::tiny_xml::element_ptr node)
{
   std::string result(node->content);
   for(boost::tiny_xml::element_list::const_iterator i = node->elements.begin();
      i != node->elements.end(); ++i)
   {
      result += " ";
      result += get_consolidated_content(*i);
   }
   static const boost::regex e("(^[[:space:]]+)|([[:space:]]+)|([[:space:]]+$)");
   return regex_replace(result, e, "(?2 )", boost::regex_constants::format_all);
}
//
// Rewrite a title based on any rewrite rules we may have:
//
std::string rewrite_title(const std::string& title, const std::string& id)
{
   for(std::list<id_rewrite_rule>::const_iterator i = id_rewrite_list.begin(); i != id_rewrite_list.end(); ++i)
   {
      if(i->base_on_id)
      {
         if(regex_match(id, i->id))
            return i->new_name;
      }
      else
      {
         if(regex_match(title, i->id))
            return regex_replace(title, i->id, i->new_name);
      }
   }
   return title;
}

struct string_cmp
{
   bool operator()(const char* a, const char* b)const
   {
      return std::strcmp(a, b) < 0;
   }
};
//
// Determine whether this node can contain an indexterm or not:
//
bool can_contain_indexterm(const char* name)
{
   static const boost::array<const char*, 257> names = 
   {
      "abbrev", "accel", "ackno", "acronym", "action", "answer", "appendix", "appendixinfo", "application", 
      "article", "articleinfo", "artpagenums", "attribution", "authorinitials", "bibliocoverage", "bibliodiv", 
      "biblioentry", "bibliography", "bibliographyinfo", "biblioid", "bibliomisc", "bibliomixed", "bibliomset", 
      "bibliorelation", "biblioset", "bibliosource", "blockinfo", "blockquote", "bookinfo", "bridgehead", "callout", 
      "caution", "chapter", "chapterinfo", "citation", "citebiblioid", "citetitle", "city", "classname", "classsynopsisinfo", 
      "code", "collabname", "command", "computeroutput", "confdates", "confnum", "confsponsor", "conftitle", "constant", 
      "constraintdef", "contractnum", "contractsponsor", "contrib", "corpauthor", "corpcredit", "corpname", "country", 
      "database", "date", "dedication", "edition", "email", "emphasis", "entry", "envar", "errorcode", "errorname", "errortext", 
      "errortype", "example", "exceptionname", "fax", "figure", "filename", "firstname", "firstterm", "foreignphrase", 
      "formalpara", "funcparams", "funcsynopsisinfo", "function", "glossary", "glossaryinfo", "glossdef", "glossdiv", 
      "glossentry", "glosssee", "glossseealso", "glossterm", "guibutton", "guiicon", "guilabel", "guimenu", "guimenuitem", 
      "guisubmenu", "hardware", "highlights", "holder", "honorific", "important", "index", "indexinfo", "informalexample", 
      "informalfigure", "initializer", "interface", "interfacename", "invpartnumber", "isbn", "issn", "issuenum", "itemizedlist", 
      "itermset", "jobtitle", "keycap", "keycode", "keysym", "label", "legalnotice", "lineage", "lineannotation", 
      /*"link", */"listitem", "literal", "literallayout", "lotentry", "manvolnum", "markup", "medialabel", "member", 
      "methodname", "modespec", "modifier", "mousebutton", "msgaud", "msgexplan", "msglevel", "msgorig", "msgtext", "note", 
      "objectinfo", "olink", "option", "optional", "orderedlist", "orgdiv", "orgname", "otheraddr", "othername", "package", 
      "pagenums", "para", "parameter", "partinfo", "partintro", "phone", "phrase", "pob", "postcode", "preface", "prefaceinfo", 
      "procedure", "productname", "productnumber", "programlisting", "prompt", "property", "pubdate", "publishername", 
      "pubsnumber", "qandadiv", "qandaset", "question", "quote", "refentry", "refentryinfo", "refentrytitle", "referenceinfo", 
      "refmeta", "refmiscinfo", "refpurpose", "refsect1", "refsect1info", "refsect2", "refsect2info", "refsect3", "refsect3info", 
      "refsection", "refsectioninfo", "refsynopsisdiv", "refsynopsisdivinfo", "releaseinfo", "remark", "returnvalue", 
      "revdescription", "revnumber", "revremark", "screen", "screeninfo", "sect1", "sect1info", "sect2", "sect2info", "sect3", 
      "sect3info", "sect4", "sect4info", "sect5", "sect5info", "section", "sectioninfo", "seg", "segtitle", "seriesvolnums", 
      "setindex", "setindexinfo", "setinfo", "sgmltag", "shortaffil", "sidebar", "sidebarinfo", "simpara", "simplesect", 
      "state", "step", "street", "structfield", "structname", "subtitle", "surname", "symbol", "synopsis", "systemitem", 
      "table", "task", "taskprerequisites", "taskrelated", "tasksummary", "td", "term", "termdef", "th", "tip", /*"title",*/ 
      "titleabbrev", "tocback", "tocentry", "tocfront", "token", "type", "ulink", "uri", "userinput", "variablelist", 
      "varname", "volumenum", "warning", "wordasword", "year"
   };
   static std::set<const char*, string_cmp> permitted;

   if(permitted.empty())
   {
      for(unsigned i = 0; i < names.size(); ++i)
      {
         permitted.insert(names[i]);
      }
   }

   return permitted.find(name) != permitted.end();
}
//
// Decide whether to flatten this node for searching purposes:
//
bool should_flatten_node(const char* name)
{
   //
   // The list of nodes to flatten is basically the list of elements that
   // can appear inside a <section> - see http://www.docbook.org/tdg/en/html/section.html.
   // In other words basically anything at the level of a paragraph/table/listing etc.
   //
   static const boost::array<const char*, 57> names = 
   {
      "title", "subtitle", "titleabbrev", 
      "toc", "lot", "glossary", "bibliography", 
      /*"calloutlist", "glosslist", "bibliolist", "itemizedlist", "orderedlist", 
      "segmentedlist", "simplelist", "variablelist",*/ "caution", "important", "note", 
      "tip", "warning", "literallayout", "programlisting", "programlistingco", 
      "screen", "screenco", "screenshot", "synopsis", "cmdsynopsis", "funcsynopsis", 
      "classsynopsis", "fieldsynopsis", "constructorsynopsis", 
      "destructorsynopsis", "methodsynopsis", "formalpara", "para", "simpara", 
      "address", "blockquote", "graphic", "graphicco", "mediaobject", 
      "mediaobjectco", "informalequation", "informalexample", "informalfigure", 
      "informaltable", "equation", "example", "figure", "table", "msgset", "procedure", 
      "sidebar", "qandaset", "task", "productionset", "constraintdef", "anchor", 
      "bridgehead", "remark", "highlights", "abstract", "authorblurb", "epigraph"
      /*"biblioentry", "bibliomixed", "callout", "glossentry", "listitem", "seg", "seglistitem", "member",
      "term", */
   };
   static std::set<const char*, string_cmp> terminals;

   if(terminals.empty())
   {
      for(unsigned i = 0; i < names.size(); ++i)
      {
         terminals.insert(names[i]);
      }
   }

   return terminals.find(name) != terminals.end();
}
std::string unescape_xml(const std::string& s)
{
   boost::regex e("&(?:(quot)|(amp)|(apos)|(lt)|(gt));");
   return regex_replace(s, e, "(?1\")(?2&)(?3\')(?4<)(?5>)", boost::regex_constants::format_all);
}
//
// This does most of the work: process the node pointed to, and any children
// that it may have:
//
void process_node(boost::tiny_xml::element_ptr node, node_id* prev, title_info* pt, bool seen = false)
{
   //
   // Store the current ID and title as nested scoped objects:
   //
   node_id id = { 0, prev };
   id.id = find_attr(node, "id");
   title_info title = { "", pt};
   bool flatten = should_flatten_node(node->name.c_str());

   if((node->name == "title") && (id.prev->id))
   {
      //
      // This actually sets the title of the enclosing scope, 
      // not this tag itself:
      //
      title.prev->title = get_consolidated_content(node);
      if(verbose)
         std::cout << "Indexing section: " << title.prev->title << std::endl;
   }
   if(node->name == "anchor")
   {
      if(node->parent.lock()->name == "title")
      {
         // We have a title with a nested anchor ID, change the ID of our parents parent to match:
         id.prev->prev->id = id.id;
      }
   }
   else if(node->name == "index")
   {
      // Keep track of all the indexes we see:
      indexes.push_back(node);
      if(node->parent.lock()->name == "para")
         node->parent.lock()->name = "";
   }
   else if(node->name == "primary")
   {
      last_primary = get_consolidated_content(node);
   }
   else if(node->name == "secondary")
   {
      last_secondary = get_consolidated_content(node);
   }
   else if((node->name == "see") && internal_indexes)
   {
      std::cerr << "WARNING: <see> in XML source will be ignored for the index generation" << std::endl;
   }
   else if((node->name == "seealso") && internal_indexes)
   {
      std::cerr << "WARNING: <seealso> in XML source will be ignored for the index generation" << std::endl;
   }
   else if((node->name == "tertiary") && internal_indexes)
   {
      std::cerr << "WARNING: <tertiary> in XML source will be ignored for the index generation" << std::endl;
   }

   std::string flattenned_text;
   const std::string* ptext;
   if(flatten)
   {
      flattenned_text = unescape_xml(get_consolidated_content(node));
      ptext = &flattenned_text;
   }
   else
   {
      ptext = &(node->content);
   }

   //
   // Search content for items: we only search if the content is not empty, 
   // and the content is not whitespace alone, and we haven't already searched this
   // text in one of our parent nodes that got flattened.
   //
   static const boost::regex space_re("[[:space:]]+");
   if(!seen && ptext->size() && !regex_match(*ptext, space_re))
   {
      // Save block ID and title in case we find some hits:
      const std::string* pid = get_current_block_id(&id);
      const std::string& rtitle = get_current_block_title(&title);
      const std::string simple_title = rewrite_title(rtitle, *pid);
      // Scan for each index term:
      for(std::set<index_info>::const_iterator i = index_terms.begin();
            i != index_terms.end(); ++i)
      {
         if(regex_search(*ptext, i->search_text))
         {
            //
            // We need to check to see if this term has already been indexed
            // in this zone, in order to prevent duplicate entries, also check
            // that any constrait placed on the term's ID is satisfied:
            //
            std::pair<std::string, std::string> item_index(*pid, i->term);
            if(((no_duplicates == false) || (0 == found_terms.count(item_index))) 
               && (i->search_id.empty() || regex_search(*pid, i->search_id)))
            {
               // We have something to index!
               found_terms.insert(item_index);

               if(!debug.empty() && (regex_match(i->term, debug) || regex_match(rtitle, debug) || regex_match(simple_title, debug)))
               {
                  std::cout << "Debug term found, in block with ID: " << *pid << std::endl;
                  std::cout << "Current section title is: " << rtitle << std::endl;
                  std::cout << "The main index entry will be : " << simple_title << std::endl;
                  std::cout << "The indexed term is: " << i->term << std::endl;
                  std::cout << "The search regex is: " << i->search_text << std::endl;
                  std::cout << "The section constraint is: " << i->search_id << std::endl;
                  std::cout << "The index type for this entry is: " << i->category << std::endl;
               }

               if(use_section_names && (simple_title != i->term))
               {
                  //
                  // First off insert index entry with primary term
                  // consisting of the section title, and secondary term the
                  // actual index term, this gets skipped if the title and index 
                  // term are the same:
                  //
                  if(internal_indexes == false)
                  {
                     // Insert an <indexterm> into the XML:
                     boost::tiny_xml::element_ptr p(new boost::tiny_xml::element());
                     p->name = "indexterm";
                     boost::tiny_xml::element_ptr prim(new boost::tiny_xml::element());
                     prim->name = "primary";
                     prim->elements.push_front(boost::tiny_xml::element_ptr(new boost::tiny_xml::element()));
                     prim->elements.front()->content = simple_title;
                     p->elements.push_front(prim);

                     boost::tiny_xml::element_ptr sec(new boost::tiny_xml::element());
                     sec->name = "secondary";
                     sec->elements.push_front(boost::tiny_xml::element_ptr(new boost::tiny_xml::element()));
                     sec->elements.front()->content = i->term;
                     p->elements.push_back(sec);
                     try{
                        // Insert the Indexterm:
                        boost::tiny_xml::element_ptr parent(node->parent);
                        while(!can_contain_indexterm(parent->name.c_str()))
                           parent = parent->parent.lock();
                        parent->elements.push_front(p);
                     }
                     catch(const std::exception&)
                     {
                        std::cerr << "Unable to find location to insert <indexterm>" << std::endl;
                     }
                  }
                  // Track the entry in our internal index:
                  index_entry_ptr item1(new index_entry(simple_title));
                  index_entry_ptr item2(new index_entry(i->term, *pid));
                  index_entry_set::iterator pos = index_entries.insert(item1).first;
                  (**pos).sub_keys.insert(item2);
               }
               //
               // Now insert another index entry with the index term
               // as the primary key, and the section title as the 
               // secondary key, this one gets assigned to the 
               // appropriate index category if there is one:
               //
               bool preferred_term = false;
               if(internal_indexes == false)
               {
                  // Insert <indexterm> into the XML:
                  boost::tiny_xml::element_ptr p2(new boost::tiny_xml::element());
                  p2->name = "indexterm";
                  if(i->category.size())
                  {
                     p2->attributes.push_back(boost::tiny_xml::attribute("type", i->category));
                  }
                  boost::tiny_xml::element_ptr prim2(new boost::tiny_xml::element());
                  prim2->name = "primary";
                  prim2->elements.push_front(boost::tiny_xml::element_ptr(new boost::tiny_xml::element()));
                  prim2->elements.front()->content = i->term;
                  p2->elements.push_front(prim2);

                  boost::tiny_xml::element_ptr sec2(new boost::tiny_xml::element());
                  sec2->name = "secondary";
                  sec2->elements.push_front(boost::tiny_xml::element_ptr(new boost::tiny_xml::element()));
                  sec2->elements.front()->content = rtitle;
                  p2->elements.push_back(sec2);
                  try{
                     // Insert the Indexterm:
                     boost::tiny_xml::element_ptr parent(node->parent);
                     while(!can_contain_indexterm(parent->name.c_str()))
                     {
                        // If the search text was found in a title then make it a preferred term:
                        if(parent->name == "title")
                           preferred_term = true;
                        parent = parent->parent.lock();
                     }
                     if(preferred_term)
                     {
                        boost::tiny_xml::attribute a("significance", "preferred");
                        p2->attributes.push_back(a);
                     }
                     parent->elements.push_front(p2);
                  }
                  catch(const std::exception&)
                  {
                     std::cerr << "Unable to find location to insert <indexterm>" << std::endl;
                  }
               }
               
               // Track the entry in our internal index:
               try{
                  // figure out if it's preferred or not:
                  boost::tiny_xml::element_ptr parent(node->parent);
                  while(!can_contain_indexterm(parent->name.c_str()))
                  {
                     // If the search text was found in a title then make it a preferred term:
                     if(parent->name == "title")
                     {
                        preferred_term = true;
                     }
                     parent = parent->parent.lock();
                  }
               }
               catch(const std::exception&){}

               index_entry_ptr item3(new index_entry(i->term));
               if(i->category.size())
                  item3->category = i->category;
               index_entry_ptr item4(new index_entry(rtitle, *pid));
               item4->preferred = preferred_term;
               index_entry_set::iterator pos = index_entries.insert(item3).first;
               (**pos).sub_keys.insert(item4);
            }
         }
      }
   }
   //
   // Recurse through children:
   //
   for(boost::tiny_xml::element_list::const_iterator i = node->elements.begin();
      i != node->elements.end(); ++i)
   {
      process_node(*i, &id, &title, flatten || seen);
   }
   //
   // Process manual index entries last of all:
   //
   if(node->name == "indexterm")
   {
      // Track the entry in our internal index:
      const std::string* pid = get_current_block_id(&id);
      const std::string* attr = find_attr(node, "type");
      index_entry_ptr item1;
      if(last_secondary.size())
         item1.reset(new index_entry(last_primary));
      else
         item1.reset(new index_entry(last_primary, *pid));
      if(index_entries.find(item1) == index_entries.end())
      {
         index_entries.insert(item1);
      }
      if(last_secondary.size())
      {
         index_entry_ptr item2(new index_entry(last_secondary, *pid));
         (**index_entries.find(item1)).sub_keys.insert(item2);
      }
      if(attr && attr->size())
      {
         (**index_entries.find(item1)).category = *attr;
      }
      last_primary = "";
      last_secondary = "";
   }
}

void process_nodes(boost::tiny_xml::element_ptr node)
{
   node_id id = { 0, };
   title_info t = { "", 0 };
   process_node(node, &id, &t);
}

int main(int argc, char* argv[])
{
   try{

   if(argc < 2)
      return help();

   //
   // Process arguments:
   //
   for(int i = 1; i < argc; ++i)
   {
      if(std::strncmp(argv[i], "in=", 3) == 0)
      {
         infile = argv[i] + 3;
      }
      else if(std::strncmp(argv[i], "out=", 4) == 0)
      {
         outfile = argv[i] + 4;
      }
      else if(std::strncmp(argv[i], "scan=", 5) == 0)
      {
         scan_file(argv[i] + 5);
      }
      else if(std::strncmp(argv[i], "script=", 7) == 0)
      {
         process_script(argv[i] + 7);
      }
      else if(std::strcmp(argv[i], "--no-duplicates") == 0)
      {
         no_duplicates = true;
      }
      else if(std::strcmp(argv[i], "--no-section-names") == 0)
      {
         use_section_names = false;
      }
      else if(std::strcmp(argv[i], "--internal-index") == 0)
      {
         internal_indexes = true;
      }
      else if(std::strcmp(argv[i], "--verbose") == 0)
      {
         verbose = true;
      }
      else if(std::strncmp(argv[i], "prefix=", 7) == 0)
      {
         prefix = argv[i] + 7;
      }
      else if(std::strncmp(argv[i], "index-type=", 11) == 0)
      {
         internal_index_type = argv[i] + 11;
      }
      else
      {
         std::cerr << "Unrecognised option " << argv[i] << std::endl;
         return 1;
      }
   }

   if(infile.empty())
   {
      return help();
   }
   if(outfile.empty())
   {
      return help();
   }

   std::ifstream is(infile.c_str());
   if(!is.good())
   {
      std::cerr << "Unable to open XML data file " << argv[1] << std::endl;
      return 1;
   }
   //
   // We need to skip any leading <? and <! elements:
   //
   std::string header = get_header(is);
   boost::tiny_xml::element_ptr xml = boost::tiny_xml::parse(is, "");
   is.close();

   std::cout << "Indexing " << index_terms.size() << " terms..." << std::endl;

   process_nodes(xml);

   if(internal_indexes)
      generate_indexes();

   std::ofstream os(outfile.c_str());
   os << header << std::endl;
   boost::tiny_xml::write(*xml, os);

   }
   catch(boost::exception& e)
   {
      std::cerr << diagnostic_information(e);
   }
   catch(const std::exception& e)
   {
      std::cerr << e.what() << std::endl;
      return 1;
   }
   catch(const std::string& s)
   {
      std::cerr << s << std::endl;
      return 1;
   }

   return 0;
}
