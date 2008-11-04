#include <iostream>
#include <boost/test/minimal.hpp>
#include <boost/introspection/introspection.hpp>

class animal
{
public:
  animal() : age(1), shadow(1000) { }
  virtual ~animal() { }
  const animal* animal_part() const { return this; }
//private:
  int age;
  int shadow;
};

class mammal : public virtual animal
{
public:
  mammal() : teats(2), ambiguous(1001), shadow(1002) { }
  int teats;
  int ambiguous;
  int shadow;
  const mammal* mammal_part() const { return this; }
private:
};

class carnivore : public virtual animal
{
public:
  carnivore() : victims(17), ambiguous(1003) { }
  int victims;
  int ambiguous;
  const carnivore* carnivore_part() const { return this; }
private:
};

class dog : public mammal, public carnivore
{
public:
  dog() : name_("Droopy") { }
  dog(const std::string& name) : name_(name) { }
  std::string name_;
  const dog* dog_part() const { return this; }
  std::string name() const { return name_; }
  void name(const std::string& n) { name_ = n; }
  std::string bark() { return "bow wow wow"; }
};

// =========================================================

using namespace std;
using namespace boost::introspection;

#ifdef _WIN32

#include <windows.h>
#include <boost/iostreams/stream.hpp>

using namespace boost::iostreams;

class debug_output_sink : public boost::iostreams::sink {
public:
    debug_output_sink(int) { }
    std::streamsize write(const char* s, std::streamsize n);
};

streamsize debug_output_sink::write(const char* s, streamsize n)
{
  TCHAR chunk[1000 + 1];
  streamsize remain(n);
  
  while (remain)
  {
    streamsize chunk_size(min(remain, sizeof chunk - 1));
    *copy(s, s + chunk_size, chunk) = 0;
    OutputDebugString(chunk);
    remain -= chunk_size;
  }
  
  return n;
}

#endif

int test_main( int, char *[] )
{
#ifdef _WIN32
  static stream_buffer<debug_output_sink> buf(0);
  cout.rdbuf(&buf);
#endif
   
  introspection_class<animal>::name("animal");
  introspection_class<animal>::variable("age", &animal::age);
  introspection_class<animal>::variable("shadow", &animal::shadow);
  introspection_class<animal>::function<const animal* (animal::*)(void) const>("animal_part", &animal::animal_part);
  
  BOOST_INTROSPECTION_CLASS(mammal);
  introspection_class<mammal>::base<animal>(virtual_);
  BOOST_INTROSPECTION_MEMBER_VARIABLE(mammal, teats);
  BOOST_INTROSPECTION_MEMBER_VARIABLE(mammal, ambiguous);
  BOOST_INTROSPECTION_MEMBER_VARIABLE(mammal, shadow);
  
  BOOST_INTROSPECTION_CLASS(carnivore);
  BOOST_INTROSPECTION_VIRTUAL_BASE(carnivore, animal);
  BOOST_INTROSPECTION_MEMBER_VARIABLE(carnivore, victims);
  BOOST_INTROSPECTION_MEMBER_VARIABLE(carnivore, ambiguous);
  
  BOOST_INTROSPECTION_CLASS(dog);
  BOOST_INTROSPECTION_BASE(dog, mammal);
  BOOST_INTROSPECTION_BASE(dog, carnivore);
  BOOST_INTROSPECTION_DEFAULT_CONSTRUCTOR(dog);
  BOOST_INTROSPECTION_CONSTRUCTOR_1(dog, const std::string&);
  BOOST_INTROSPECTION_MEMBER_VARIABLE(dog, name_);
  BOOST_INTROSPECTION_OVERLOADED_MEMBER_FUNCTION(dog, name, string (dog::*)() const);
  BOOST_INTROSPECTION_OVERLOADED_MEMBER_FUNCTION(dog, name, void (dog::*)(const string&));
  BOOST_INTROSPECTION_MEMBER_FUNCTION(dog, bark);

  introspection_class<dog>::instance()->dump_inheritance_tree(cout);
  
  {
    ostringstream opath;
    introspection_class<dog>::instance()->dump_inheritance_graph(opath);
    string path(opath.str());
    cout << path << endl;
    BOOST_CHECK(path == "dog(mammal(*animal()),carnivore(*animal()))");
  }
  
  dog droopy;

  { 
    const_object o(downcast(&droopy));
   
    member_variable<string> dog_name_(o, "name_");
    cout << o[dog_name_] << endl;
    BOOST_CHECK(&o[dog_name_] == &droopy.name_);
    BOOST_CHECK(o[dog_name_] == droopy.name_);

    member_variable<int> dog_age(o, "age");
    cout << o[dog_age] << endl;
    BOOST_CHECK(&droopy.age == &o[dog_age]);

    member_variable<int> dog_teats(o, "teats");
    cout << o[dog_teats] << endl;
    BOOST_CHECK(&droopy.teats == &o[dog_teats]);

    member_variable<int> dog_victims(o, "victims");
    cout << o[dog_victims] << endl;
    BOOST_CHECK(&droopy.victims == &o[dog_victims]);
    
    member_variable<int> dog_shadow(o, "shadow");
    BOOST_CHECK(&droopy.shadow == &o[dog_shadow]);

    try
    {  
      member_variable<int> ambiguous(o, "ambiguous");
      BOOST_ERROR("ambiguity not detected");
    }
    catch (ambiguous)
    {
    }
    catch (...)
    {
      BOOST_ERROR("wrong exception during ambiguity");
    }
    
    member_function<string (object::*)() const> dog_name(o, "name");
    BOOST_CHECK(droopy.name() == o[dog_name]());

    BOOST_CHECK(droopy.animal_part() == o.function<const animal* (object::*)() const>("animal_part")());
  }
  
  { 
    object o(downcast(&droopy));
    
    member_function<string (object::*)()> dog_bark(o, "bark");
    BOOST_CHECK(droopy.bark() == o[dog_bark]());

    // Can we call const member functions on non-const objects too?    
    member_function<string (object::*)() const> dog_name(o, "name");
    BOOST_CHECK(droopy.name() == o[dog_name]());
    
    BOOST_CHECK(droopy.animal_part() == o.function<const animal* (object::*)() const>("animal_part")());
  }
  
  {
    object o("dog");
    member_function<string (object::*)() const> dog_name(o, "name");
    BOOST_CHECK(droopy.name() == o[dog_name]());

    member_function<void (object::*)(const string&)> set_dog_name(o, "name");
    o[set_dog_name]("Rantanplan");
    
    BOOST_CHECK("Rantanplan" == o[dog_name]());
  }
  
  {
    object o(make<const std::string&>("dog", "Rantanplan"));
    member_function<string (object::*)() const> dog_name(o, "name");
    BOOST_CHECK("Rantanplan" == o[dog_name]());
  }

  cout << flush;
      
  return 0;
}
