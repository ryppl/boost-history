#include <boost/introspection/introspection.hpp>

namespace boost { namespace introspection {

using namespace std;
using namespace boost;

const std::vector< qualified_member<member> >&
class_type::candidates(const std::string& member_name) const
{
  build_inheritance_graph();
  
  map<std::string, vector< qualified_member<member> > >::iterator iter(qualified_member_map_.find(member_name));
  
  if (iter == qualified_member_map_.end())
    throw member_not_found(name(), member_name);

  vector< qualified_member<member> >& candidates(iter->second);
  
  if (candidates.size() == 1)
    return candidates;
    
  for (int candidate_index = 0; candidate_index < (int) candidates.size(); ++candidate_index)
  {
    qualified_member<member> candidate(candidates[candidate_index]); // don't take ref into vector because it may shrink
    
    for (int shadow_index = 0; shadow_index < (int) candidates.size(); )
    {
      qualified_member<member>& shadow(candidates[shadow_index]);

      if (candidate.path()->size() < shadow.path()->size()
        && mismatch(candidate.path()->begin(), candidate.path()->end(), shadow.path()->begin()).first == candidate.path()->end())
      {
        candidates.erase(candidates.begin() + shadow_index);
        
        if (candidate_index > shadow_index)
          --candidate_index;
      }
      else
      {
        ++shadow_index;
      }
    }
  }
  
  return candidates;
}

void class_type::dump_inheritance_graph(std::ostream& os) const
{
  build_inheritance_graph();
  os << name();
  dump_inheritance_graph_bases(os, bases_);
}

void class_type::dump_inheritance_graph_bases(std::ostream& os, const vector< intrusive_ptr<base> >& bases)
{
  os << '(';

  const char* sep = "";
  vector< intrusive_ptr<base> >::const_iterator base_iter(bases.begin()), base_last(bases.end());
  
  while (base_iter != base_last)
  {
    os << sep;
    sep = ",";
    
    if ((*base_iter)->virtual_)
      os << '*';
      
    os << (*base_iter)->class_->name();
    
    dump_inheritance_graph_bases(os, (*base_iter)->bases_);
    
    ++base_iter;
  }
  
  os << ')';
}

void class_type::dump_inheritance_tree(std::ostream& os) const
{
  build_inheritance_graph();
  os << name() << endl;
  dump_inheritance_tree(os, 1, bases_);
}

void class_type::dump_inheritance_tree(std::ostream& os, int indent, const vector< intrusive_ptr<base> >& bases)
{
  vector< intrusive_ptr<base> >::const_iterator base_iter(bases.begin()), base_last(bases.end());
  
  while (base_iter != base_last)
  {
    for (int i = 0; i < indent; i++)
      os << "  ";
      
    if ((*base_iter)->virtual_)
      os << "virtual ";
      
    os << (*base_iter)->class_->name() << endl;
    
    dump_inheritance_tree(os, indent + 1, (*base_iter)->bases_);
    
    ++base_iter;
  }
}

void class_type::build_inheritance_graph() const
{
  if (!collect_qualified_members_done_)
  {
    class_type* myself(const_cast<class_type*>(this));
    std::vector<const direct_base*> path;
    map<const class_type*, intrusive_ptr<base> > virtual_bases;
    build_inheritance_graph(myself, myself->bases_, path, virtual_bases);
    collect_qualified_members_done_ = true;
  }
}

void class_type::build_inheritance_graph(class_type* complete_class, vector< intrusive_ptr<base> >& bases,
  vector<const direct_base*>& path, map<const class_type*, intrusive_ptr<base> >& virtual_bases) const
{
  inheritance_path this_path(new vector<const direct_base*>(path));

  vector<member*>::const_iterator member_iter(direct_members_.begin()), member_last(direct_members_.end());
  
  while (member_iter != member_last)
  {
    complete_class->qualified_members_.push_back(qualified_member<member>(this_path, *member_iter));
    complete_class->qualified_member_map_[(*member_iter)->name()].push_back(qualified_member<member>(this_path, *member_iter));
    ++member_iter;
  }
    
  vector<direct_base*>::const_iterator base_iter(direct_bases_.begin()), base_last(direct_bases_.end());
  
  while (base_iter != base_last)
  {
    const direct_base* direct(*base_iter);
    path.push_back(direct);
    
    intrusive_ptr<base> new_base;
    
    if (direct->is_virtual())
    {
      intrusive_ptr<base>& vbase = virtual_bases[direct->class_];
      
      if (vbase)
      {
        bases.push_back(vbase);
      }
      else
      {
        new_base = new base;
        new_base->virtual_ = true;
        vbase = new_base;
      }
    }
    else
    {
      new_base = new base;
      new_base->virtual_ = false;
    }
    
    if (new_base)
    {
      new_base->class_ = direct->class_;
      new_base->path_ = inheritance_path(new vector<const direct_base*>(path));
      direct->class_->build_inheritance_graph(complete_class, new_base->bases_, path, virtual_bases);
      bases.push_back(new_base);
    }
         
    path.pop_back();
    ++base_iter;
  }
}

void* class_type::adjust(const inheritance_path& path, void* address) const
{
  std::vector<const direct_base*>::const_iterator iter(path->begin()), last(path->end());
  
  while (iter != last)
    address = (*iter++)->upcast_(address);
    
  return address;
}

} }
