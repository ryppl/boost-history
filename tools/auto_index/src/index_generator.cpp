// Copyright 2008 John Maddock
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "auto_index.hpp"

//
// Get a numerical ID for the next item:
//
std::string get_next_index_id()
{
   static int index_id_count = 0;
   std::stringstream s;
   s << "idx_id_" << index_id_count;
   ++index_id_count;
   return s.str();
}
//
// Validate that the container for the Index is in a valid place:
//
void check_index_type_and_placement(const std::string& parent, const std::string& container)
{
   if(container == "section")
   {
      if((parent != "appendix")
         && (parent != "article")
         && (parent != "chapter")
         && (parent != "partintro")
         && (parent != "preface")
         && (parent != "section"))
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if(container == "appendix")
   {
      if((parent != "article")
         && (parent != "book")
         && (parent != "part"))
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if(container == "index")
   {
      if((parent != "appendix")
         && (parent != "article")
         && (parent != "book")
         && (parent != "chapter")
         && (parent != "part")
         && (parent != "preface")
         && (parent != "sect1")
         && (parent != "sect2")
         && (parent != "sect3")
         && (parent != "sect4")
         && (parent != "sect5")
         && (parent != "section")
         )
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if((container == "article") || (container == "chapter") || (container == "reference"))
   {
      if((parent != "book")
         && (parent != "part"))
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if(container == "part")
   {
      if(parent != "book")
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if(container == "refsect1")
   {
      if(parent != "refentry")
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if(container == "refsect2")
   {
      if(parent != "refsect1")
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if(container == "refsect3")
   {
      if(parent != "refsect2")
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if(container == "refsection")
   {
      if((parent != "refsection") && (parent != "refentry"))
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if(container == "sect1")
   {
      if((parent != "appendix")
         && (parent != "article")
         && (parent != "chapter")
         && (parent != "partintro")
         && (parent != "preface")
         )
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if(container == "sect2")
   {
      if(parent != "sect1")
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if(container == "sect3")
   {
      if(parent != "sect2")
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if(container == "sect4")
   {
      if(parent != "sect3")
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else if(container == "sect5")
   {
      if(parent != "sect4")
         throw std::runtime_error("Error: element " + container + " can not appear inside the container " + parent + ": try using a different value for property \"auto-index-type\".");
   }
   else
   {
      throw std::runtime_error("Error: element " + container + " is unknown, and can not be used as a container for an index: try using a different value for property \"auto-index-type\".");
   }
}
//
// Generate indexes using our own internal method:
//
void generate_indexes()
{
   for(boost::tiny_xml::element_list::const_iterator i = indexes.begin(); i != indexes.end(); ++i)
   {
      boost::tiny_xml::element_ptr node = *i;
      const std::string* category = find_attr(node, "type");
      bool has_title = false;

      for(boost::tiny_xml::element_list::const_iterator k = (*i)->elements.begin(); k != (*i)->elements.end(); ++k)
      {
         if((**k).name == "title")
         {
            has_title = true;
            break;
         }
      }

      boost::tiny_xml::element_ptr navbar(new boost::tiny_xml::element());
      navbar->name = "para";
      node->elements.push_back(navbar);

      char last_c = 0;
      boost::tiny_xml::element_ptr list(new boost::tiny_xml::element());
      list->name = "variablelist";
      boost::tiny_xml::element_ptr listentry;
      boost::tiny_xml::element_ptr listitem;
      boost::tiny_xml::element_ptr sublist;
      node->elements.push_back(list);

      for(index_entry_set::const_iterator i = index_entries.begin(); i != index_entries.end(); ++i)
      {
         if((0 == category) || (category->size() == 0) || (category && (**i).category == *category))
         {
            if(std::toupper((**i).key[0]) != last_c)
            {
               std::string id_name = get_next_index_id();
               last_c = std::toupper((**i).key[0]);
               listentry.reset(new boost::tiny_xml::element());
               listentry->name = "varlistentry";
               boost::tiny_xml::attribute id;
               id.name = "id";
               id.value = id_name;
               listentry->attributes.push_back(id);
               boost::tiny_xml::element_ptr term(new boost::tiny_xml::element());
               term->name = "term";
               term->content.assign(&last_c, 1);
               listentry->elements.push_front(term);
               list->elements.push_back(listentry);
               listitem.reset(new boost::tiny_xml::element());
               listitem->name = "listitem";
               sublist.reset(new boost::tiny_xml::element());
               sublist->name = "variablelist";
               listitem->elements.push_back(sublist);
               listentry->elements.push_back(listitem);

               boost::tiny_xml::element_ptr nav(new boost::tiny_xml::element());
               nav->name = "";
               nav->content = " ";
               boost::tiny_xml::element_ptr navlink(new boost::tiny_xml::element());
               navlink->name = "link";
               navlink->content = term->content;
               boost::tiny_xml::attribute navid;
               navid.name = "linkend";
               navid.value = id_name;
               navlink->attributes.push_back(navid);
               navbar->elements.push_back(navlink);
               navbar->elements.push_back(nav);
            }
            boost::tiny_xml::element_ptr subentry(new boost::tiny_xml::element());
            subentry->name = "varlistentry";
            boost::tiny_xml::element_ptr subterm(new boost::tiny_xml::element());
            subterm->name = "term";
            if((**i).id.empty())
               subterm->content = (**i).key;
            else
            {
               boost::tiny_xml::element_ptr link(new boost::tiny_xml::element());
               link->name = "link";
               link->content = (**i).key;
               boost::tiny_xml::attribute at;
               at.name = "linkend";
               at.value = (**i).id;
               link->attributes.push_back(at);
               subterm->elements.push_back(link);
            }
            subentry->elements.push_back(subterm);
            boost::tiny_xml::element_ptr subitem(new boost::tiny_xml::element());
            subitem->name = "listitem";
            subentry->elements.push_back(subitem);
            sublist->elements.push_back(subentry);

            boost::tiny_xml::element_ptr secondary_list(new boost::tiny_xml::element());
            secondary_list->name = "simplelist";
            subitem->elements.push_back(secondary_list);

            //
            // we need to examine subsequent entries to see if they're the same as this one
            // (albeit with different "type" attributes) and if so merge with this entry
            // if we're creating the main index:
            //
            index_entry_set sub_keys((**i).sub_keys);
            if((0 == category) || (category->size() == 0) )
            {
               ++i;
               while((i != index_entries.end()) && (subterm->content == (**i).key))
               {
                  sub_keys.insert((**i).sub_keys.begin(), (**i).sub_keys.end());
                  ++i;
               }
               --i;
            }

            for(index_entry_set::const_iterator k = sub_keys.begin(); k != sub_keys.end(); ++k)
            {
               boost::tiny_xml::element_ptr member(new boost::tiny_xml::element());
               member->name = "member";
               boost::tiny_xml::element_ptr para(new boost::tiny_xml::element());
               para->name = "para";
               if((**k).id.empty())
                  para->content = (**k).key;
               else
               {
                  boost::tiny_xml::element_ptr link(new boost::tiny_xml::element());
                  link->name = "link";
                  boost::tiny_xml::attribute at;
                  at.name = "linkend";
                  at.value = (**k).id;
                  link->attributes.push_back(at);
                  if((**k).preferred)
                  {
                     boost::tiny_xml::element_ptr em(new boost::tiny_xml::element());
                     em->name = "emphasis";
                     em->content = (**k).key;
                     boost::tiny_xml::attribute b("role", "bold");
                     em->attributes.push_back(b);
                     link->elements.push_back(em);
                  }
                  else
                  {
                     link->content = (**k).key;
                  }
                  para->elements.push_back(link);
               }
               member->elements.push_back(para);
               secondary_list->elements.push_back(member);
            }
            // Remove the secondary list of there's nothing in it! :
            if(secondary_list->elements.empty())
               subitem->elements.pop_back();
         }
      }
      node->name = internal_index_type;
      boost::tiny_xml::element_ptr p(node->parent);
      while(p->name.empty())
         p = boost::tiny_xml::element_ptr(p->parent);
      check_index_type_and_placement(p->name, node->name);
      node->attributes.clear();
      if(!has_title)
      {
         boost::tiny_xml::element_ptr t(new boost::tiny_xml::element());
         t->name = "title";
         t->content = "Index";
         node->elements.push_front(t);
      }
   }
}

