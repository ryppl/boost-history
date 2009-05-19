// Pinhole demo source
//
// Copyright Jared McIntyre 2007.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include "property_group.hpp"
#include "property_manager.hpp"
#include "find.hpp"

using namespace std;
using namespace boost;
using namespace boost::pinhole;
        
// I can hide these two line if I don't do everything in headers
shared_ptr<property_manager> property_manager::m_instance(new property_manager);
event_source* event_source::m_instance = 0;

class SubGroup : public property_group
{
public:
    SubGroup(property_group* pParentproperty_group) : property_group("SubGroup", pParentproperty_group)
    {
            add_property("Name", "name", BOOST_SETTER_NONE, BOOST_GETTER(&SubGroup::GetName));
    }

    string GetName()
    {
        return "SubGroup";
    }
};

// For an object to be a property group, it must inherit from property_group. You could also
// use the PropertGroupWrapper to create properties without inheriting, but it isn't as nice to work with,
// and I don't have any examples here. We currently only use it to create logical property groups under an
// existing property group that don't have an object to match up with them. I probably need to make an
// example to have that make any sense.
class test_group : public property_group
{
public:
    enum eExample
    {
        Example1,
        Example2
    };
    
    // The property_group constructor requires you to give the class a name and it's parent property_group if
    // it has one. The name is used by the property system for path based lookups, but our code uses it,
    // especially in the property debugging user interface. If your object never has a parent property group,
    // as is the case here, default it to NULL and it will become a root property group. To see an example
    // of parenting, see the definition of SubGroup above, and the use below. If you wanted to make SubGroup
    // a root property group, you would just pass it NULL as parent.
    //
    // A root property group is one that shows up in the root list in property_manager. This has important
    // implications in tree iteration and path lookup.
    test_group() : property_group("test_group", NULL),
        a_sub_group(this)
    {
        // Setup default values for our variables.
        name = "Hello World!";
        iVal = 123;
        fFloat = 0.0f;
        
        // Simple hooking up of getter/setters as properties
        add_property("Name", "name", BOOST_SETTER(&test_group::SetName), BOOST_GETTER(&test_group::GetName));
        add_property("iVal", "name", BOOST_SETTER(&test_group::SetIVal), BOOST_GETTER(&test_group::GetIVal));
        
        // Slightly more advanced. Converts a function that takes two parameters into multiple valid getters
        // for the property system by hard-coding one of the values passed in.
        add_property("example1", "name", BOOST_SETTER_NONE, boost::bind(&test_group::GetExample, this, Example1));
        add_property("example2", "name", BOOST_SETTER_NONE, boost::bind(&test_group::GetExample, this, Example2));
        
        // Here is an example of how to expose a variable without associated getter/setter functions
        add_property("variable", "name", BOOST_SETTER_VAR(fFloat), BOOST_GETTER_VAR(fFloat));
    }
    
    string GetName()
    {
        return name;
    }
    
    void SetName(string newName)
    {
        name = newName;
    }
    
    int GetExample(eExample eVal)
    {
        switch(eVal)
        {
            case Example1:
                return 1;
            case Example2:
                return 2;
        }
        
        return 0;
    }
    
    int GetIVal()
    {
        return iVal;
    }
    
    void SetIVal(int value)
    {
        iVal = value;
    }
    
    float fFloat;
    string name;
    int iVal;
    SubGroup a_sub_group;
};

int main (int argc, char * const argv[])
{
    test_group test;

    // This is an example of basic runtime type-safe property accessors (get/set)
    cout << test.get<string>("Name") << endl;
    test.set("Name", string("newName"));
    cout << test.get<string>("Name") << endl;

    // This is an example of accessors that return strings for all types. This is very
    // useful when you need to write code that doesn't know anything about the types
    // it is processing over. Examples of this are generic user interfaces that show
    // all the property groups and their properties in the system so that they can be
    // manipulated when the system is running. The UI can't know what the types are, but
    // using this method it can still display them, and allow them to be changed.
    string i = test.get_as_string("iVal");
    cout << "Fetched by string: " << i << endl;
    test.set_as_string("iVal", "20");
    i = test.get_as_string("iVal");
    cout << "Fetched by string: " << i << endl;

    // This is an example of an accessor that is getting data from a function that isn't
    // a simple getter function. Each one of these properties is actually calling the same
    // function, but with different default parameters passed in. See the definition of
    // test_group for more details.
    cout << "Example1: " << test.get<int>("example1") << endl;
    cout << "Example2: " << test.get<int>("example2") << endl;

    // This is fetching from a property that is exposing a variable directly. There aren't any
    // getter/setter methods for it, though you could add them later if you needed to change
    // the default behavior and the users wouldn't have to change.
    test.set("variable", 1.234567f);
    cout << "Fetched wrapped variable: " << test.get<float>("variable") << endl;

    // Here is an example of fetching a child property group based on a search path. The
    // You can also iterate through all the children, and thus walk the entire property
    // tree if you would like, or if you're code doesn't know what the hierarchy looks like
    // when it is compiled.
    property_group* found_property_group = select_single_node(test, "SubGroup.Name=SubGroup");
    cout << "Subproperty_group Name: " << found_property_group->get<string>("Name") << endl;

    // Here is an example of the above, but using a full path from the root of the property
    // group tree. Note that we use the value "newName" for Name since we changed the name
    // in an earlier example.
    found_property_group = select_single_node("/test_group.Name=newName/SubGroup.Name=SubGroup");
    cout << "Subproperty_group Name: " << found_property_group->get<string>("Name") << endl;

    // Here is an example of iterating through all the child propety groups.
    // ** I don't like the way this works today, so I'm leaving the example out for now.

    // Here we see that we can get to the test_group instance from the property_manager.
    property_manager* manager = property_manager::instance();

    // Here is an example of iterating through all the root propety groups.
    property_manager::iterator itrRoot    = manager->begin();
    property_manager::iterator itrRootEnd = manager->end();
    for( ; itrRoot != itrRootEnd; ++itrRoot )
    {
        cout << "Found Root Item: " << (*itrRoot)->get_name() << endl;;
    }

    // All properties can have metadata associated to them. In the current form we store
    // instances of the Editor class with each property (see "Areas that need work" below
    // for where I admit this is too specific of a design). If you don't specify the Editor
    // object in your call to add_property, the property system tries to find a default Editor
    // based on the property type. None of the above examples specify an Editor, so they are
    // all defaulted.
    //
    // Here is an example of accessing metadata for a specified property.
    // ** I don't like the way this works today, so I'm leaving the example out for now.

    /*
    Areas that need work:

    1) Algorithms: We currently only have one path based search algorithm. This expects a single matching value and
    throws otherwise. This is a useful search algorithm as is, but we need more. We need one that returns iterators
    over all the items found. We need a version of that that returns them as they exist in the system, and one that
    returns them sorted based on a given criteria. I also imagine there may be other algorithms for processing entire
    property trees, or sub-trees. For example, we have a serialization algorithm that walks the entire tree and stores
    the values of all properties. Given the same tree structure, it can re-walk the tree and set the values to their
    stored values.

    2) More compile time optimizations: There are a lot of powerful things you can do by creating properties at runtime.
    However, our experience shows that most properties are simple, and everything about them is known at compile time.
    There should be some template mechanism we could use for creating this type of property that would reduce the amount
    of time that is used at runtime to setup the property without reducing the utility of the system to create properties
    purely at runtime.

    3) Property Manager Singleton: Currently the property manager is a singleton. This has been extremely helpful in
    allowing us to build out the full property group tree and access it from anywhere. However, some people have
    an aversion to this pattern, and there are cases where you may want multiple managers (if you have multiple copies
    of your data layer, you probably don't want all those copies combined into the same property group tree). We haven't
    really thought much further on how to do this better and are looking for insights.
     
    4) Safe Property Group: This is something I've been toying around with for a while. Currently it is advantagious to
    get a property group and hold on to it. However, since property groups are retrieved largely as pointers, this can
    be dangerious. I can't use shared_ptr since the object that it is pointing to may have been created on the stack,
    and it won't have control over deletion (among other issues). I have a prototype class that wraps the property group
    and listens to the property manager to see when it is destroyed, but I'm not sure I like that idea (and it is part of
    our C++/CLI wrapper, so it isn't something I can share).

    5) Documentation: We currently have doxygen documented objects for this code. If we are going to submit to boost, we
    would need quickbook docs on top of that that show how to use everything.
    
    Discussion Points:
     
    1) Header Only: This is currently built as a library since the property_manager is a singleton, and the basic functionality
    relies on that. If this fact changes, I'd like to look at making this header only.
     
    2) Right now we have several different begin/end function pairs in property_group for accessing the different data in
    the system. Is that really the best way to do it? Whould we be passing back const references to the collections instead?
    If we did that, then what dop we do about retrieving a list of all the property names. That is really just keys to a map,
    but we can hide that complexity from the user using the technique we use now.
     
    Dreams:
     
    1) Events: Wouldn't it be cool if you could hook to an event that was fired whenever a specified property value
    changed. Since the system is really just wrapping existing function calls, I haven't come up with a way to do this
    that doesn't add a lot of work to property implementers, and would have high risk of being buggy. Any ideas?
     
    2) Thread Safety: Since the system is really just wrapping existing function calls, any thread safety is tied to
    those function calls. I currently have no way of exposing complex techniques. Any ideas?
    */
    
    return 0;
}
