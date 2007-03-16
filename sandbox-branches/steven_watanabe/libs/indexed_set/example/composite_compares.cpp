/* Boost.IndexedSet example of composite comparison predicates.
 *
 * Copyright Joaquín M López Muñoz 2003-2004. Use, modification, and distribution
 * are subject to the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/indexed_set for library home page.
 */

#if !defined(NDEBUG)
#define BOOST_INDEXED_SET_ENABLE_INVARIANT_CHECKING
#define BOOST_INDEXED_SET_ENABLE_SAFE_MODE
#endif

#include <boost/call_traits.hpp>
#include <boost/indexed_set.hpp>
#include <boost/tokenizer.hpp>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <string>

using namespace boost::indexed_sets;

/* A file record maintains some info on name and size as well
 * as a pointer to the directory it belongs (null meaning the root
 * directory.)
 */

struct file_entry
{
  file_entry(std::string name_,unsigned size_,bool is_dir_,const file_entry* dir_):
    name(name_),size(size_),is_dir(is_dir_),dir(dir_)
  {}

  std::string       name;
  unsigned          size;
  bool              is_dir;
  const file_entry* dir;

  friend std::ostream& operator<<(std::ostream& os,const file_entry& f)
  {
    os<<f.name<<"\t"<<f.size;
    if(f.is_dir)os<<"\t <dir>";
    return os;
  }
};

/* template utility to combine a key extractor and a comparison predicate
 * into a comparison predicate
 */

template<
  typename Value,
  typename KeyFromValue,
  typename Compare=std::less<BOOST_DEDUCED_TYPENAME KeyFromValue::result_type>
>
struct comp_compare:std::binary_function<Value,Value,bool>
{
  comp_compare(
    KeyFromValue key_=KeyFromValue(),
    Compare comp_=Compare()):
    key(key_),comp(comp_)
  {}

  bool operator()(const Value& x,const Value& y)const
  {
    return comp(key(x),key(y));
  }

  bool operator()(const typename KeyFromValue::result_type& x,const Value& y)const
  {
    return comp(x,key(y));
  }

  bool operator()(const Value& x,const typename KeyFromValue::result_type& y)const
  {
    return comp(key(x),y);
  }

private:
  KeyFromValue key;
  Compare      comp;
};

/* template utility to build a lexicographical comparison out of two
 * key extractors
 */

template<
  typename Value,
  typename KeyFromValue1,typename KeyFromValue2,
  typename Compare1=std::less<BOOST_DEDUCED_TYPENAME KeyFromValue1::result_type>,
  typename Compare2=std::less<BOOST_DEDUCED_TYPENAME KeyFromValue2::result_type>
>
struct lex_compare:std::binary_function<Value,Value,bool>
{
  lex_compare(
    KeyFromValue1 key1_=KeyFromValue1(),KeyFromValue2 key2_=KeyFromValue2(),
    Compare1 comp1_=Compare1(),Compare2 comp2_=Compare2()):
    key1(key1_),key2(key2_),comp1(comp1_),comp2(comp2_)
  {}

  bool operator()(const Value& x,const Value& y)const
  {
    if(comp1(key1(x),key1(y)))return true;
    if(comp1(key1(y),key1(x)))return false;
    return comp2(key2(x),key2(y));
  }

private:
  KeyFromValue1 key1;
  KeyFromValue2 key2;
  Compare1      comp1;
  Compare2      comp2;
};

/* A file system is just an indexed_set of entries with indices on file
 * and size. These indices are firstly ordered by directory, as commands
 * work on a current directory basis.
 */

typedef lex_compare<
  file_entry,
  BOOST_INDEXED_SET_MEMBER(file_entry,const file_entry*,dir),
  BOOST_INDEXED_SET_MEMBER(file_entry,std::string,name)
> name_comparer;

typedef lex_compare<
  file_entry,
  BOOST_INDEXED_SET_MEMBER(file_entry,const file_entry*,dir),
  BOOST_INDEXED_SET_MEMBER(file_entry,unsigned,size)
> size_comparer;

typedef indexed_set<
  file_entry,
  index_list<
    unique<  /* primary index sorted by name (inside the same directory) */
      identity<file_entry>,
      name_comparer
    >,
    non_unique< /* secondary index sorted by size (inside the same directory) */
      identity<file_entry>,
      size_comparer
    >
  >
> file_system;

/* typdefes to the two indices of file_system */

typedef nth_index_type<file_system,0>::type file_system_by_name;
typedef nth_index_type<file_system,1>::type file_system_by_size;

/* We build a rudimentary file system simulation out of some global
 * info and a map of commands provided to the user.
 */

static file_system fs;                             /* the one and only file system */
static file_system_by_name& fs_by_name=fs;         /* name index to fs */
static file_system_by_size& fs_by_size=get<1>(fs); /* size index to fs */
static const file_entry* current_dir=0;            /* root directory */

/* command framework */

/* A command provides an execute memfun fed with the corresponding params
 * (first param is stripped off as it serves to identify the command
 * currently being used.)
 */

typedef boost::tokenizer<boost::char_separator<char> > command_tokenizer;

class command
{
public:
  virtual ~command(){}
  virtual void execute(command_tokenizer::iterator tok1,command_tokenizer::iterator tok2)=0;
};

/* available commands */

/* cd: syntax cd [.|..|<directory>] */

class command_cd:public command
{
public:
  virtual void execute(command_tokenizer::iterator tok1,command_tokenizer::iterator tok2)
  {
    if(tok1==tok2)return;
    std::string dir=*tok1++;

    if(dir==".")return;
    if(dir==".."){
      if(current_dir)current_dir=current_dir->dir;
      return;
    }

    file_system_by_name::iterator it=fs.find(file_entry(dir,0,true,current_dir));
    if(it==fs.end()){
      std::cout<<"non-existent directory"<<std::endl;
      return;
    }
    if(!it->is_dir){
      std::cout<<dir<<" is not a directory"<<std::endl;
      return;
    }
    current_dir=&*it;
  }
};
static command_cd cd;

/* ls: syntax ls [-s] */

class command_ls:public command
{
public:
  virtual void execute(command_tokenizer::iterator tok1,command_tokenizer::iterator tok2)
  {
    std::string option;
    if(tok1!=tok2)option=*tok1++;

    if(!option.empty()){
      if(option!="-s"){
        std::cout<<"incorrect parameter"<<std::endl;
        return;
      }

      /* list by size */

      file_system_by_size::iterator it0,it1;
      boost::tie(it0,it1)=fs_by_size.equal_range(
        current_dir,
        comp_compare<
          file_entry,
          BOOST_INDEXED_SET_MEMBER(file_entry,const file_entry*,dir)>());
      std::copy(it0,it1,std::ostream_iterator<file_entry>(std::cout,"\n"));

      return;
    }

    /* list by name */

    file_system_by_name::iterator it0,it1;
    boost::tie(it0,it1)=fs.equal_range(
      current_dir,
      comp_compare<
        file_entry,
        BOOST_INDEXED_SET_MEMBER(file_entry,const file_entry*,dir)>());
    std::copy(it0,it1,std::ostream_iterator<file_entry>(std::cout,"\n"));
  }
};
static command_ls ls;

/* mkdir: syntax mkdir <directory> */

class command_mkdir:public command
{
public:
  virtual void execute(command_tokenizer::iterator tok1,command_tokenizer::iterator tok2)
  {
    std::string dir;
    if(tok1!=tok2)dir=*tok1++;

    if(dir.empty()){
      std::cout<<"missing parameter"<<std::endl;
      return;
    }

    if(dir=="."||dir==".."){
      std::cout<<"incorrect parameter"<<std::endl;
      return;
    }

    if(!fs.insert(file_entry(dir,0,true,current_dir)).second){
      std::cout<<"directory already exists"<<std::endl;
      return;
    }
  }
};
static command_mkdir mkdir;

/* table of commands, a map from command names to class command pointers */

typedef std::map<std::string,command*> command_table;
static command_table cmt;

int main()
{
  /* fill the file system with some data */

  file_system::iterator it0,it1;
  
  fs.insert(file_entry("usr.cfg",240,false,0));
  fs.insert(file_entry("memo.txt",2430,false,0));
  it0=fs.insert(file_entry("dev",0,true,0)).first;
    fs.insert(file_entry("tty0",128,false,&*it0));
    fs.insert(file_entry("tty1",128,false,&*it0));
  it0=fs.insert(file_entry("usr",0,true,0)).first;
    it1=fs.insert(file_entry("bin",0,true,&*it0)).first;
      fs.insert(file_entry("bjam",172032,false,&*it1));
  it0=fs.insert(file_entry("home",0,true,0)).first;
    it1=fs.insert(file_entry("andy",0,true,&*it0)).first;
      fs.insert(file_entry("logo.jpg",5345,false,&*it1)).first;
      fs.insert(file_entry("foo.cpp",890,false,&*it1)).first;
      fs.insert(file_entry("foo.hpp",93,false,&*it1)).first;
      fs.insert(file_entry("foo.html",750,false,&*it1)).first;
      fs.insert(file_entry("a.obj",12302,false,&*it1)).first;
      fs.insert(file_entry(".bash_history",8780,false,&*it1)).first;
    it1=fs.insert(file_entry("rachel",0,true,&*it0)).first;
      fs.insert(file_entry("test.py",650,false,&*it1)).first;
      fs.insert(file_entry("todo.txt",241,false,&*it1)).first;
      fs.insert(file_entry(".bash_history",9510,false,&*it1)).first;

  /* fill the command table */

  cmt["cd"]   =&cd;
  cmt["ls"]   =&ls;
  cmt["mkdir"]=&mkdir;

  /* main looop */

  for(;;){
    /* print out the current directory and the prompt symbol */

    if(current_dir)std::cout<<current_dir->name;
    std::cout<<">";

    /* get an input line from the user: if empty, exit the program */

    std::string com;
    std::getline(std::cin,com);
    command_tokenizer tok(com,boost::char_separator<char>(" \t\n"));
    if(tok.begin()==tok.end())break; /* null command, exit */

    /* select the corresponding command and execute it */

    command_table::iterator it=cmt.find(*tok.begin());
    if(it==cmt.end()){
      std::cout<<"invalid command"<<std::endl;
      continue;
    }

    it->second->execute(++tok.begin(),tok.end());
  }
  
  return 0;
}
