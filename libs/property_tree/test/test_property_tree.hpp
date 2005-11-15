// ----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Marcin Kalicinski
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see www.boost.org
// ----------------------------------------------------------------------------

// Intentionally no include guards (to be included more than once)

#if !defined(CHTYPE) || !defined(T) || !defined(PTREE)
#   error No character type specified
#endif

void test_debug(PTREE *)
{
    
    // Check count
    BOOST_CHECK(PTREE::debug_get_instances_count() == 0);

    {

        // Create ptrees
        PTREE pt1, pt2;
        BOOST_CHECK(PTREE::debug_get_instances_count() == 2);

        // Create PTREE
        PTREE *pt3 = new PTREE;
        BOOST_CHECK(PTREE::debug_get_instances_count() == 3);

        // Insert
        pt1.push_back(std::make_pair(T("key"), *pt3));
        BOOST_CHECK(PTREE::debug_get_instances_count() == 4);

        // Insert
        pt2.push_back(std::make_pair(T("key"), *pt3));
        BOOST_CHECK(PTREE::debug_get_instances_count() == 5);

        // Clear
        pt1.clear();
        BOOST_CHECK(PTREE::debug_get_instances_count() == 4);

        // Clear
        pt2.clear();
        BOOST_CHECK(PTREE::debug_get_instances_count() == 3);

        // Delete
        delete pt3;
        BOOST_CHECK(PTREE::debug_get_instances_count() == 2);

    }

    // Check count
    BOOST_CHECK(PTREE::debug_get_instances_count() == 0);

}

void test_constructor_destructor_assignment(PTREE *)
{
    
    {

        // Test contructor from string
        PTREE pt(T("data"));
        BOOST_CHECK(pt.data() == T("data"));
        
        PTREE pt1;
        BOOST_CHECK(PTREE::debug_get_instances_count() == 2);

        // Do insertions
        PTREE *tmp1 = pt1.put(T("key1"), T(""));
        PTREE *tmp2 = pt1.put(T("key2"), T(""));
        tmp1->put(T("key3"), T(""));
        tmp2->put(T("key4"), T(""));
        BOOST_CHECK(PTREE::debug_get_instances_count() == 6);

        // Make a copy using copy constructor
        PTREE *pt2 = new PTREE(pt1);
        BOOST_CHECK(PTREE::debug_get_instances_count() == 11);

        // Make a copy using = operator
        PTREE *pt3 = new PTREE;
        *pt3 = *pt2;
        BOOST_CHECK(PTREE::debug_get_instances_count() == 16);

        // Test self assignment
        pt1 = pt1;
        BOOST_CHECK(PTREE::debug_get_instances_count() == 16);

        // Destroy
        delete pt2;
        BOOST_CHECK(PTREE::debug_get_instances_count() == 11);

        // Destroy
        delete pt3;
        BOOST_CHECK(PTREE::debug_get_instances_count() == 6);
    
    }

    // Check count
    BOOST_CHECK(PTREE::debug_get_instances_count() == 0);

}

void test_insertion(PTREE *)
{
    
    // Do insertions
    PTREE pt;
    PTREE tmp1(T("data1"));
    PTREE tmp2(T("data2"));
    PTREE tmp3(T("data3"));
    PTREE tmp4(T("data4"));
    PTREE::iterator it1 = pt.insert(pt.end(), std::make_pair(T("key1"), tmp1));
    PTREE::iterator it2 = pt.insert(it1, std::make_pair(T("key2"), tmp2));
    PTREE::iterator it3 = it1->second.push_back(std::make_pair(T("key3"), tmp3));
    PTREE::iterator it4 = it1->second.push_front(std::make_pair(T("key4"), tmp4));
    it2->second.insert(it2->second.end(), it1->second.begin(), it1->second.end());

    // Check instance count
    BOOST_CHECK(PTREE::debug_get_instances_count() == 11);
    
    // Check contents
    BOOST_CHECK(pt.get_d(T("key1"), T("")) == T("data1"));
    BOOST_CHECK(pt.get_d(T("key2"), T("")) == T("data2"));
    BOOST_CHECK(pt.get_d(T("key1.key3"), T("")) == T("data3"));
    BOOST_CHECK(pt.get_d(T("key1.key4"), T("")) == T("data4"));
    BOOST_CHECK(pt.get_d(T("key2.key3"), T("")) == T("data3"));
    BOOST_CHECK(pt.get_d(T("key2.key4"), T("")) == T("data4"));

    // Check sequence
    PTREE::iterator it = it2;
    ++it; BOOST_CHECK(it == it1);
    ++it; BOOST_CHECK(it == pt.end());
    it = it4; 
    ++it; BOOST_CHECK(it == it3);
    ++it; BOOST_CHECK(it == it1->second.end());

}

void test_erasing(PTREE *)
{
    
    // Do insertions
    PTREE pt;
    PTREE tmp1(T("data1"));
    PTREE tmp2(T("data2"));
    PTREE tmp3(T("data3"));
    PTREE tmp4(T("data4"));
    PTREE::iterator it1 = pt.insert(pt.end(), std::make_pair(T("key1"), tmp1));
    PTREE::iterator it2 = pt.insert(it1, std::make_pair(T("key2"), tmp2));
    it1->second.push_back(std::make_pair(T("key"), tmp3));
    it1->second.push_front(std::make_pair(T("key"), tmp4));
    it2->second.insert(it2->second.end(), it1->second.begin(), it1->second.end());
    
    // Check instance count
    BOOST_CHECK(PTREE::debug_get_instances_count() == 11);

    // Test range erase
    PTREE::iterator it = it1->second.erase(it1->second.begin(), it1->second.end());
    BOOST_CHECK(it == it1->second.end());
    BOOST_CHECK(PTREE::debug_get_instances_count() == 9);

    // Test single erase
    PTREE::size_type n = pt.erase(T("key1"));
    BOOST_CHECK(n == 1);
    BOOST_CHECK(PTREE::debug_get_instances_count() == 8);

    // Test multiple erase
    n = it2->second.erase(T("key"));
    BOOST_CHECK(n == 2);
    BOOST_CHECK(PTREE::debug_get_instances_count() == 6);

    // Test erase with no destroy
    n = pt.erase(T("key2"));
    BOOST_CHECK(n == 1);
    BOOST_CHECK(PTREE::debug_get_instances_count() == 5);

}

void test_clear(PTREE *)
{

    // Do insertions
    PTREE pt(T("data"));
    pt.push_back(std::make_pair(T("key"), PTREE(T("data"))));

    // Check instance count
    BOOST_CHECK(PTREE::debug_get_instances_count() == 2);

    // Test clear
    pt.clear();
    BOOST_CHECK(pt.empty());
    BOOST_CHECK(pt.data().empty());
    BOOST_CHECK(PTREE::debug_get_instances_count() == 1);

}

void test_pushpop(PTREE *)
{
    
    // Do insertions
    PTREE pt;
    PTREE tmp1(T("data1"));
    PTREE tmp2(T("data2"));
    PTREE tmp3(T("data3"));
    PTREE tmp4(T("data4"));
    pt.push_back(std::make_pair(T("key3"), tmp3));
    pt.push_front(std::make_pair(T("key2"), tmp2));
    pt.push_back(std::make_pair(T("key4"), tmp4));
    pt.push_front(std::make_pair(T("key1"), tmp1));
    
    // Check instance count
    BOOST_CHECK(PTREE::debug_get_instances_count() == 9);

    // Check sequence
    PTREE::iterator it = pt.begin();
    BOOST_CHECK(it->first == T("key1")); ++it;
    BOOST_CHECK(it->first == T("key2")); ++it;
    BOOST_CHECK(it->first == T("key3")); ++it;
    BOOST_CHECK(it->first == T("key4")); ++it;
    BOOST_CHECK(it == pt.end());

    // Test pops
    pt.pop_back();
    BOOST_CHECK(PTREE::debug_get_instances_count() == 8);
    BOOST_CHECK(pt.front().second.data() == T("data1"));
    BOOST_CHECK(pt.back().second.data() == T("data3"));
    pt.pop_front();
    BOOST_CHECK(PTREE::debug_get_instances_count() == 7);
    BOOST_CHECK(pt.front().second.data() == T("data2"));
    BOOST_CHECK(pt.back().second.data() == T("data3"));
    pt.pop_back();
    BOOST_CHECK(PTREE::debug_get_instances_count() == 6);
    BOOST_CHECK(pt.front().second.data() == T("data2"));
    BOOST_CHECK(pt.back().second.data() == T("data2"));
    pt.pop_front();
    BOOST_CHECK(PTREE::debug_get_instances_count() == 5);
    BOOST_CHECK(pt.empty());

}

void test_container_iteration(PTREE *)
{
    
    // Do insertions
    PTREE pt;
    pt.put(T("key3"), T(""));
    pt.put(T("key1"), T(""));
    pt.put(T("key4"), T(""));
    pt.put(T("key2"), T(""));

    // iterator
    {
        PTREE::iterator it = pt.begin();
        BOOST_CHECK(it->first == T("key3")); ++it;
        BOOST_CHECK(it->first == T("key1")); ++it;
        BOOST_CHECK(it->first == T("key4")); ++it;
        BOOST_CHECK(it->first == T("key2")); ++it;
        BOOST_CHECK(it == pt.end());
    }

    // const_iterator
    {
        PTREE::const_iterator it = pt.begin();
        BOOST_CHECK(it->first == T("key3")); ++it;
        BOOST_CHECK(it->first == T("key1")); ++it;
        BOOST_CHECK(it->first == T("key4")); ++it;
        BOOST_CHECK(it->first == T("key2")); ++it;
        BOOST_CHECK(it == pt.end());
    }

    // reverse_iterator
    {
        PTREE::reverse_iterator it = pt.rbegin();
        BOOST_CHECK(it->first == T("key2")); ++it;
        BOOST_CHECK(it->first == T("key4")); ++it;
        BOOST_CHECK(it->first == T("key1")); ++it;
        BOOST_CHECK(it->first == T("key3")); ++it;
        BOOST_CHECK(it == pt.rend());
    }

    // const_reverse_iterator
    {
        PTREE::const_reverse_iterator it = pt.rbegin();
        BOOST_CHECK(it->first == T("key2")); ++it;
        BOOST_CHECK(it->first == T("key4")); ++it;
        BOOST_CHECK(it->first == T("key1")); ++it;
        BOOST_CHECK(it->first == T("key3")); ++it;
        BOOST_CHECK(it == PTREE::const_reverse_iterator(pt.rend()));
    }

}

void test_swap(PTREE *)
{
    
    PTREE pt1, pt2;

    // Do insertions
    pt1.put(T("key1"), T(""));
    pt1.put(T("key2"), T(""));
    pt1.put(T("key1.key3"), T(""));
    pt1.put(T("key1.key4"), T(""));

    // Check counts
    BOOST_CHECK(PTREE::debug_get_instances_count() == 6);
    BOOST_CHECK(pt1.size() == 2);
    BOOST_CHECK(pt1.get_child(T("key1"))->size() == 2);
    BOOST_CHECK(pt2.size() == 0);

    // Swap
    pt2.swap(pt1);
    
    // Check counts
    BOOST_CHECK(PTREE::debug_get_instances_count() == 6);
    BOOST_CHECK(pt2.size() == 2);
    BOOST_CHECK(pt2.get_child(T("key1"))->size() == 2);
    BOOST_CHECK(pt1.size() == 0);

}

void test_sort_reverse(PTREE *)
{
    
    PTREE pt;

    // Do insertions
    pt.put(T("key2"), T("data2"));
    pt.put(T("key1"), T("data1"));
    pt.put(T("key4"), T("data4"));
    pt.put(T("key3"), T("data3"));
    pt.put(T("key3.key1"), T(""));
    pt.put(T("key4.key2"), T(""));

    // Reverse
    pt.reverse();

    // Check sequence
    {
        PTREE::iterator it = pt.begin();
        BOOST_CHECK(it->first == T("key3")); ++it;
        BOOST_CHECK(it->first == T("key4")); ++it;
        BOOST_CHECK(it->first == T("key1")); ++it;
        BOOST_CHECK(it->first == T("key2")); ++it;
        BOOST_CHECK(it == pt.end());
    }
    // Check sequence using find to check if index is ok
    {
        PTREE::iterator it = pt.begin();
        BOOST_CHECK(it == pt.find(T("key3"))); ++it;
        BOOST_CHECK(it == pt.find(T("key4"))); ++it;
        BOOST_CHECK(it == pt.find(T("key1"))); ++it;
        BOOST_CHECK(it == pt.find(T("key2"))); ++it;
        BOOST_CHECK(it == pt.end());
    }

    // Sort
    pt.sort(SortPred<PTREE>());

    // Check sequence
    {
        PTREE::iterator it = pt.begin();
        BOOST_CHECK(it->first == T("key1")); ++it;
        BOOST_CHECK(it->first == T("key2")); ++it;
        BOOST_CHECK(it->first == T("key3")); ++it;
        BOOST_CHECK(it->first == T("key4")); ++it;
        BOOST_CHECK(it == pt.end());
    }
    // Check sequence (using find to check if index is ok)
    {
        PTREE::iterator it = pt.begin();
        BOOST_CHECK(it == pt.find(T("key1"))); ++it;
        BOOST_CHECK(it == pt.find(T("key2"))); ++it;
        BOOST_CHECK(it == pt.find(T("key3"))); ++it;
        BOOST_CHECK(it == pt.find(T("key4"))); ++it;
        BOOST_CHECK(it == pt.end());
    }
    
    // Sort reverse
    pt.sort(SortPredRev<PTREE>());

    // Check sequence
    {
        PTREE::iterator it = pt.begin();
        BOOST_CHECK(it->first == T("key4")); ++it;
        BOOST_CHECK(it->first == T("key3")); ++it;
        BOOST_CHECK(it->first == T("key2")); ++it;
        BOOST_CHECK(it->first == T("key1")); ++it;
        BOOST_CHECK(it == pt.end());
    }
    // Check sequence (using find to check if index is ok)
    {
        PTREE::iterator it = pt.begin();
        BOOST_CHECK(it == pt.find(T("key4"))); ++it;
        BOOST_CHECK(it == pt.find(T("key3"))); ++it;
        BOOST_CHECK(it == pt.find(T("key2"))); ++it;
        BOOST_CHECK(it == pt.find(T("key1"))); ++it;
        BOOST_CHECK(it == pt.end());
    }

}

void test_case(PTREE *)
{
    
    // Do insertions
    PTREE pt;
    pt.put(T("key1"), T("data1"));
    pt.put(T("KEY2"), T("data2"));
    pt.put(T("kEy1.keY3"), T("data3"));
    pt.put(T("KEY1.key4"), T("data4"));

    // Check findings depending on traits type
    if (typeid(PTREE::traits_type) == typeid(boost::property_tree::ptree_traits<CHTYPE>))
    {
        BOOST_CHECK(PTREE::debug_get_instances_count() == 7);
        BOOST_CHECK(pt.get_d(T("key1"), T("")) == T("data1"));
        BOOST_CHECK(pt.get_d(T("key2"), T("")) == T(""));
        BOOST_CHECK(pt.get_d(T("key1.key3"), T("")) == T(""));
        BOOST_CHECK(pt.get_d(T("KEY1.key4"), T("")) == T("data4"));
    }
    else
    {
        BOOST_CHECK(PTREE::debug_get_instances_count() == 5);
        BOOST_CHECK(pt.get_d(T("key1"), T("1")) == pt.get_d(T("KEY1"), T("2")));
        BOOST_CHECK(pt.get_d(T("key2"), T("1")) == pt.get_d(T("KEY2"), T("2")));
        BOOST_CHECK(pt.get_d(T("key1.key3"), T("1")) == pt.get_d(T("KEY1.KEY3"), T("2")));
        BOOST_CHECK(pt.get_d(T("key1.key4"), T("1")) == pt.get_d(T("KEY1.KEY4"), T("2")));
    }

    // Do more insertions
    pt.push_back(PTREE::value_type(T("key1"), PTREE()));
    pt.push_back(PTREE::value_type(T("key1"), PTREE()));

    // Test counts
    if (typeid(PTREE::traits_type) == typeid(boost::property_tree::ptree_traits<CHTYPE>))
    {
        BOOST_CHECK(pt.count(T("key1")) == 3);
        BOOST_CHECK(pt.count(T("KEY1")) == 1);
        BOOST_CHECK(pt.count(T("key2")) == 0);
        BOOST_CHECK(pt.count(T("KEY2")) == 1);
        BOOST_CHECK(pt.count(T("key3")) == 0);
        BOOST_CHECK(pt.count(T("KEY3")) == 0);
    }
    else
    {
        BOOST_CHECK(pt.count(T("key1")) == 3);
        BOOST_CHECK(pt.count(T("KEY1")) == 3);
        BOOST_CHECK(pt.count(T("key2")) == 1);
        BOOST_CHECK(pt.count(T("KEY2")) == 1);
        BOOST_CHECK(pt.count(T("key3")) == 0);
        BOOST_CHECK(pt.count(T("KEY3")) == 0);
    }

}

void test_comparison(PTREE *)
{
    
    // Prepare original
    PTREE pt_orig;
    pt_orig.put(T("key1"), T(""));
    pt_orig.put(T("key1.key3"), T(""));
    pt_orig.put(T("key1.key4"), T(""));
    pt_orig.put(T("key2"), T(""));

    // Test originals
    {
        PTREE pt1(pt_orig);
        PTREE pt2(pt_orig);
        BOOST_CHECK(pt1 == pt2);
        BOOST_CHECK(pt2 == pt1);
        BOOST_CHECK(!(pt1 != pt2));
        BOOST_CHECK(!(pt2 != pt1));
    }

    // Test originals with modified case
    if (typeid(PTREE::traits_type) == typeid(boost::property_tree::iptree_traits<CHTYPE>))
    {
        PTREE pt1(pt_orig);
        PTREE pt2(pt_orig);
        pt1.pop_back();
        pt1.put(T("KEY2"), T(""));
        BOOST_CHECK(pt1 == pt2);
        BOOST_CHECK(pt2 == pt1);
        BOOST_CHECK(!(pt1 != pt2));
        BOOST_CHECK(!(pt2 != pt1));
    }

    // Test modified copies (both modified the same way)
    {
        PTREE pt1(pt_orig);
        PTREE pt2(pt_orig);
        pt1.put(T("key1.key5"), T("."));
        pt2.put(T("key1.key5"), T("."));
        BOOST_CHECK(pt1 == pt2);
        BOOST_CHECK(pt2 == pt1);
        BOOST_CHECK(!(pt1 != pt2));
        BOOST_CHECK(!(pt2 != pt1));
    }

    // Test modified copies (modified data)
    {
        PTREE pt1(pt_orig);
        PTREE pt2(pt_orig);
        pt1.data() = T("a");
        pt2.data() = T("b");
        BOOST_CHECK(!(pt1 == pt2));
        BOOST_CHECK(!(pt2 == pt1));
        BOOST_CHECK(pt1 != pt2);
        BOOST_CHECK(pt2 != pt1);
    }

    // Test modified copies (added subkeys with different data)
    {
        PTREE pt1(pt_orig);
        PTREE pt2(pt_orig);
        pt1.put(T("key1.key5"), T("a"));
        pt2.put(T("key1.key5"), T("b"));
        BOOST_CHECK(!(pt1 == pt2));
        BOOST_CHECK(!(pt2 == pt1));
        BOOST_CHECK(pt1 != pt2);
        BOOST_CHECK(pt2 != pt1);
    }

    // Test modified copies (added subkeys with different keys)
    {
        PTREE pt1(pt_orig);
        PTREE pt2(pt_orig);
        pt1.put(T("key1.key5"), T(""));
        pt2.put(T("key1.key6"), T(""));
        BOOST_CHECK(!(pt1 == pt2));
        BOOST_CHECK(!(pt2 == pt1));
        BOOST_CHECK(pt1 != pt2);
        BOOST_CHECK(pt2 != pt1);
    }

    // Test modified copies (added subkey to only one copy)
    {
        PTREE pt1(pt_orig);
        PTREE pt2(pt_orig);
        pt1.put(T("key1.key5"), T(""));
        BOOST_CHECK(!(pt1 == pt2));
        BOOST_CHECK(!(pt2 == pt1));
        BOOST_CHECK(pt1 != pt2);
        BOOST_CHECK(pt2 != pt1);
    }

}

void test_front_back(PTREE *)
{
    
    // Do insertions
    PTREE pt;
    pt.put(T("key1"), T(""));
    pt.put(T("key2"), T(""));

    // Check front and back
    BOOST_CHECK(pt.front().first == T("key1"));
    BOOST_CHECK(pt.back().first == T("key2"));

}

void test_get_put(PTREE *)
{
    
    typedef std::basic_string<CHTYPE> str_t;

    // Temporary storage
    int tmp_int;
    long tmp_long;
    double tmp_double;
    float tmp_float;
    str_t tmp_string;
    CHTYPE tmp_char;
    bool tmp_bool;
    boost::optional<int> opt_int;
    boost::optional<long> opt_long;
    boost::optional<double> opt_double;
    boost::optional<float> opt_float;
    boost::optional<str_t> opt_string;
    boost::optional<CHTYPE> opt_char;
    boost::optional<bool> opt_bool;
    
    // Do insertions via put
    PTREE pt;
    PTREE *pt1 = pt.put(T("k1"), 1);                    // put as int
    PTREE *pt2 = pt.put(T("k2.k"), 2.5);                // put as double
    PTREE *pt3 = pt.put(T("k3.k.k"), T("ala ma kota")); // put as string
    PTREE *pt4 = pt.put(T("k4.k.k.k"), CHTYPE('c'));    // put as character
    PTREE *pt5 = pt.put(T("k5.k.k.k.f"), false);        // put as bool
    PTREE *pt6 = pt.put(T("k5.k.k.k.t"), true);         // put as bool

    // Check instances count
    BOOST_CHECK(PTREE::debug_get_instances_count() == 17);

    // Check if const char * version returns std::string
    BOOST_CHECK(typeid(pt.get_own_d(T(""))) == typeid(str_t));
    
    // Do extractions via get (throwing version)
    BOOST_CHECK(pt.get<int>(T("k1")) == 1);                             // get as int
    BOOST_CHECK(pt.get<long>(T("k1")) == 1);                            // get as long
    BOOST_CHECK(pt.get<double>(T("k2.k")) == 2.5);                      // get as double
    BOOST_CHECK(pt.get<float>(T("k2.k")) == 2.5f);                      // get as float
    BOOST_CHECK(pt.get<str_t>(T("k3.k.k")) == str_t(T("ala ma kota"))); // get as string
    BOOST_CHECK(pt.get<CHTYPE>(T("k4.k.k.k")) == CHTYPE('c'));          // get as char
    BOOST_CHECK(pt.get<bool>(T("k5.k.k.k.f")) == false);                // get as bool
    BOOST_CHECK(pt.get<bool>(T("k5.k.k.k.t")) == true);                 // get as bool

    // Do extractions via get (return bool version)
    BOOST_CHECK(pt.get_b(T("k1"), &tmp_int));                 // get as int
    BOOST_CHECK(tmp_int == 1);
    BOOST_CHECK(pt.get_b(T("k1"), &tmp_long));                // get as long
    BOOST_CHECK(tmp_long == 1);
    BOOST_CHECK(pt.get_b(T("k2.k"), &tmp_double));            // get as double
    BOOST_CHECK(tmp_double == 2.5);
    BOOST_CHECK(pt.get_b(T("k2.k"), &tmp_float));             // get as float
    BOOST_CHECK(tmp_float == 2.5f);
    BOOST_CHECK(pt.get_b(T("k3.k.k"), &tmp_string));          // get as string
    BOOST_CHECK(tmp_string == T("ala ma kota"));
    BOOST_CHECK(pt.get_b(T("k4.k.k.k"), &tmp_char));          // get as char
    BOOST_CHECK(tmp_char == CHTYPE('c'));
    BOOST_CHECK(pt.get_b(T("k5.k.k.k.f"), &tmp_bool));        // get as bool
    BOOST_CHECK(tmp_bool == false);
    BOOST_CHECK(pt.get_b(T("k5.k.k.k.t"), &tmp_bool));        // get as bool
    BOOST_CHECK(tmp_bool == true);

    // Do extractions via get (default value version)
    BOOST_CHECK(pt.get_d(T("k1"), 0) == 1);              // get as int
    BOOST_CHECK(pt.get_d(T("k1"), 0L) == 1);             // get as long
    BOOST_CHECK(pt.get_d(T("k2.k"), 0.0) == 2.5);        // get as double
    BOOST_CHECK(pt.get_d(T("k2.k"), 0.0f) == 2.5f);      // get as float
    BOOST_CHECK(pt.get_d(T("k3.k.k"), str_t()) == str_t(T("ala ma kota")));    // get as string
    BOOST_CHECK(pt.get_d(T("k3.k.k"), T("")) == T("ala ma kota"));             // get as const char *
    BOOST_CHECK(pt.get_d(T("k4.k.k.k"), CHTYPE('\0')) == CHTYPE('c'));         // get as char
    BOOST_CHECK(pt.get_d(T("k5.k.k.k.f"), true) == false);                     // get as bool
    BOOST_CHECK(pt.get_d(T("k5.k.k.k.t"), false) == true);                     // get as bool

    // Do extractions via get (optional version)
    opt_int = pt.get_o<int>(T("k1"));                   // get as int
    BOOST_CHECK(opt_int && *opt_int == 1);
    opt_long = pt.get_o<long>(T("k1"));                 // get as long
    BOOST_CHECK(opt_long && *opt_long == 1);
    opt_double = pt.get_o<double>(T("k2.k"));           // get as double
    BOOST_CHECK(opt_double && *opt_double == 2.5);
    opt_float = pt.get_o<float>(T("k2.k"));             // get as float
    BOOST_CHECK(opt_float && *opt_float == 2.5f);
    opt_string = pt.get_o<str_t>(T("k3.k.k"));          // get as string
    BOOST_CHECK(opt_string && *opt_string == str_t(T("ala ma kota")));
    opt_char = pt.get_o<CHTYPE>(T("k4.k.k.k"));         // get as char
    BOOST_CHECK(opt_char && *opt_char == CHTYPE('c'));
    opt_bool = pt.get_o<bool>(T("k5.k.k.k.f"));         // get as bool
    BOOST_CHECK(opt_bool && *opt_bool == false);
    opt_bool = pt.get_o<bool>(T("k5.k.k.k.t"));         // get as bool
    BOOST_CHECK(opt_bool && *opt_bool == true);

    // Do insertions via put_own
    pt1->put_own(short(1));                     // put as short
    pt2->put_own(2.5f);                         // put as float
    pt3->put_own(str_t(T("ala ma kota")));      // put as string
    pt4->put_own(CHTYPE('c'));                  // put as character
    pt5->put_own(false);                        // put as bool
    pt6->put_own(true);                         // put as bool

    // Do extractions via get_own (throwing version)
    BOOST_CHECK(pt1->get_own<int>() == 1);                             // get as int
    BOOST_CHECK(pt1->get_own<long>() == 1);                            // get as long
    BOOST_CHECK(pt2->get_own<double>() == 2.5);                        // get as double
    BOOST_CHECK(pt2->get_own<float>() == 2.5f);                        // get as float
    BOOST_CHECK(pt3->get_own<str_t>() == str_t(T("ala ma kota")));     // get as string
    BOOST_CHECK(pt4->get_own<CHTYPE>() == CHTYPE('c'));                // get as char
    BOOST_CHECK(pt5->get_own<bool>() == false);                        // get as bool
    BOOST_CHECK(pt6->get_own<bool>() == true);                         // get as bool

    // Do extractions via get_own (return bool version)
    BOOST_CHECK(pt1->get_own_b(&tmp_int));              // get as int
    BOOST_CHECK(tmp_int == 1);
    BOOST_CHECK(pt1->get_own_b(&tmp_long));             // get as long
    BOOST_CHECK(tmp_long == 1);
    BOOST_CHECK(pt2->get_own_b(&tmp_double));           // get as double
    BOOST_CHECK(tmp_double == 2.5);
    BOOST_CHECK(pt2->get_own_b(&tmp_float));            // get as float
    BOOST_CHECK(tmp_float == 2.5f);
    BOOST_CHECK(pt3->get_own_b(&tmp_string));           // get as string
    BOOST_CHECK(tmp_string == T("ala ma kota"));
    BOOST_CHECK(pt4->get_own_b(&tmp_char));             // get as char
    BOOST_CHECK(tmp_char == CHTYPE('c'));
    BOOST_CHECK(pt5->get_own_b(&tmp_bool));             // get as bool
    BOOST_CHECK(tmp_bool == false);
    BOOST_CHECK(pt6->get_own_b(&tmp_bool));             // get as bool
    BOOST_CHECK(tmp_bool == true);

    // Do extractions via get_own (default value version)
    BOOST_CHECK(pt1->get_own_d(0) == 1);              // get as int
    BOOST_CHECK(pt1->get_own_d(0L) == 1);             // get as long
    BOOST_CHECK(pt2->get_own_d(0.0) == 2.5);          // get as double
    BOOST_CHECK(pt2->get_own_d(0.0f) == 2.5f);        // get as float
    BOOST_CHECK(pt3->get_own_d(str_t()) == str_t(T("ala ma kota")));    // get as string
    BOOST_CHECK(pt3->get_own_d(T("")) == T("ala ma kota"));             // get as const char *
    BOOST_CHECK(pt4->get_own_d(CHTYPE('\0')) == CHTYPE('c'));           // get as char
    BOOST_CHECK(pt5->get_own_d(true) == false);                         // get as bool
    BOOST_CHECK(pt6->get_own_d(false) == true);                         // get as bool

    // Do extractions via get_own (optional version)
    opt_int = pt1->get_own_o<int>();                    // get as int
    BOOST_CHECK(opt_int && *opt_int == 1);
    opt_long = pt1->get_own_o<long>();                  // get as long
    BOOST_CHECK(opt_long && *opt_long == 1);
    opt_double = pt2->get_own_o<double>();              // get as double
    BOOST_CHECK(opt_double && *opt_double == 2.5);
    opt_float = pt2->get_own_o<float>();                // get as float
    BOOST_CHECK(opt_float && *opt_float == 2.5f);
    opt_string = pt3->get_own_o<str_t>();               // get as string
    BOOST_CHECK(opt_string && *opt_string == str_t(T("ala ma kota")));
    opt_char = pt4->get_own_o<CHTYPE>();                // get as char
    BOOST_CHECK(opt_char && *opt_char == CHTYPE('c'));
    opt_bool = pt5->get_own_o<bool>();                  // get as bool
    BOOST_CHECK(opt_bool && *opt_bool == false);
    opt_bool = pt6->get_own_o<bool>();                  // get as bool
    BOOST_CHECK(opt_bool && *opt_bool == true);

    // Do incorrect extractions (throwing version)
    try 
    { 
        pt.get<int>(T("k2.k.bogus.path"));
        BOOST_ERROR("No required exception thrown");
    } 
    catch (boost::property_tree::ptree_bad_path &) { }
    catch (...)
    {
        BOOST_ERROR("Wrong exception type thrown");
    }
    try 
    { 
        pt.get<int>(T("k2.k"));
        BOOST_ERROR("No required exception thrown");
    } 
    catch (boost::property_tree::ptree_bad_data &) { }
    catch (...)
    {
        BOOST_ERROR("Wrong exception type thrown");
    }

    // Do incorrect extractions (return bool version)
    BOOST_CHECK(pt.get_b(T("k2.k"), &tmp_int) == false);
    BOOST_CHECK(pt.get_b(T("k3.k.k"), &tmp_int) == false);
    BOOST_CHECK(pt.get_b(T("k4.k.k.k"), &tmp_int) == false);

    // Do incorrect extractions (default value version)
    BOOST_CHECK(pt.get_d(T("k2.k"), -7) == -7);
    BOOST_CHECK(pt.get_d(T("k3.k.k"), -7) == -7);
    BOOST_CHECK(pt.get_d(T("k4.k.k.k"), -7) == -7);

    // Do incorrect extractions (optional version)
    BOOST_CHECK(!pt.get_o<int>(T("k2.k")));
    BOOST_CHECK(!pt.get_o<int>(T("k3.k.k")));
    BOOST_CHECK(!pt.get_o<int>(T("k4.k.k.k")));

}

void test_get_child_put_child(PTREE *)
{
    
    typedef std::basic_string<CHTYPE> str_t;

    PTREE pt(T("ala ma kota"));
    
    // Do insertions via put_child
    PTREE pt1, pt2, pt3;
    pt1.put_child(T("k1"), NULL);
    pt1.put_child(T("k2.k"), NULL);
    pt2.put_child(T("k1"), &pt);
    pt2.put_child(T("k2.k"), &pt);

    // Const references to test const versions of methods
    const PTREE &cpt1 = pt1, &cpt2 = pt2;

    // Do correct extractions via get_child (throwing version)
    BOOST_CHECK(pt1.get_child(T("k1"))->empty());
    BOOST_CHECK(pt1.get_child(T("k2.k"))->empty());
    BOOST_CHECK(*pt2.get_child(T("k1")) == pt);
    BOOST_CHECK(*pt2.get_child(T("k2.k")) == pt);
    BOOST_CHECK(cpt1.get_child(T("k1"))->empty());
    BOOST_CHECK(cpt1.get_child(T("k2.k"))->empty());
    BOOST_CHECK(*cpt2.get_child(T("k1")) == pt);
    BOOST_CHECK(*cpt2.get_child(T("k2.k")) == pt);

    // Do correct extractions via get_child (return bool version)
    PTREE *tmp;
    const PTREE *ctmp;
    BOOST_CHECK(pt1.get_child_b(T("k1"), &tmp) == true);
    BOOST_CHECK(tmp->empty());
    BOOST_CHECK(pt1.get_child_b(T("k2.k"), &tmp) == true);
    BOOST_CHECK(tmp->empty());
    BOOST_CHECK(pt2.get_child_b(T("k1"), &tmp) == true);
    BOOST_CHECK(*tmp == pt);
    BOOST_CHECK(pt2.get_child_b(T("k2.k"), &tmp) == true);
    BOOST_CHECK(*tmp == pt);
    BOOST_CHECK(cpt1.get_child_b(T("k1"), &ctmp) == true);
    BOOST_CHECK(ctmp->empty());
    BOOST_CHECK(cpt1.get_child_b(T("k2.k"), &ctmp) == true);
    BOOST_CHECK(ctmp->empty());
    BOOST_CHECK(cpt2.get_child_b(T("k1"), &ctmp) == true);
    BOOST_CHECK(*ctmp == pt);
    BOOST_CHECK(cpt2.get_child_b(T("k2.k"), &ctmp) == true);
    BOOST_CHECK(*ctmp == pt);

    // Do correct extractions via get_child (default value version)
    BOOST_CHECK(pt1.get_child_d(T("k1"), NULL) != NULL);
    BOOST_CHECK(pt1.get_child_d(T("k2.k"), NULL) != NULL);
    BOOST_CHECK(*pt2.get_child_d(T("k1"), NULL) == pt);
    BOOST_CHECK(*pt2.get_child_d(T("k2.k"), NULL) == pt);
    BOOST_CHECK(cpt1.get_child_d(T("k1"), NULL) != NULL);
    BOOST_CHECK(cpt1.get_child_d(T("k2.k"), NULL) != NULL);
    BOOST_CHECK(*cpt2.get_child_d(T("k1"), NULL) == pt);
    BOOST_CHECK(*cpt2.get_child_d(T("k2.k"), NULL) == pt);

    // Do correct extractions via get_child (optional version)
    boost::optional<PTREE *> opt;
    boost::optional<const PTREE *> copt;
    opt = pt1.get_child_o(T("k1"));
    BOOST_CHECK(opt);
    opt = pt1.get_child_o(T("k2.k"));
    BOOST_CHECK(opt);
    opt = pt2.get_child_o(T("k1"));
    BOOST_CHECK(opt && **opt == pt);
    opt = pt2.get_child_o(T("k2.k"));
    BOOST_CHECK(opt && **opt == pt);
    copt = cpt1.get_child_o(T("k1"));
    BOOST_CHECK(copt);
    copt = cpt1.get_child_o(T("k2.k"));
    BOOST_CHECK(copt);
    copt = cpt2.get_child_o(T("k1"));
    BOOST_CHECK(copt && **copt == pt);
    copt = cpt2.get_child_o(T("k2.k"));
    BOOST_CHECK(copt && **copt == pt);
    
    // Do incorrect extractions via get_child (throwing version)
    try 
    { 
        pt.get_child(T("k2.k.bogus.path"));
        BOOST_ERROR("No required exception thrown");
    } 
    catch (boost::property_tree::ptree_bad_path &) { }
    catch (...)
    {
        BOOST_ERROR("Wrong exception type thrown");
    }

    // Do incorrect extractions via get_child (return bool version)
    tmp = &pt1;
    BOOST_CHECK(pt.get_child_b(T("k2.k.bogus.path"), &tmp) == false);
    BOOST_CHECK(tmp == &pt1);

    // Do incorrect extractions via get_child (default value version)
    BOOST_CHECK(pt.get_child_d(T("k2.k.bogus.path"), &pt3) == &pt3);

    // Do incorrect extractions via get_child (optional version)
    BOOST_CHECK(!pt.get_child_o(T("k2.k.bogus.path")));

}

void test_path_separator(PTREE *)
{
    
    // Check instances count
    BOOST_CHECK(PTREE::debug_get_instances_count() == 0);

    // Do insertions
    PTREE pt;
    pt.put(T("key1"), T("1"));
    pt.put(T("key2.key"), T("2"));
    pt.put(T("key3.key.key"), T("3"));
    pt.put(CHTYPE('/'), T("key4"), T("4"));
    pt.put(CHTYPE('/'), T("key5/key"), T("5"));
    pt.put(CHTYPE('/'), T("key6/key/key"), T("6"));

    // Check instances count
    BOOST_CHECK(PTREE::debug_get_instances_count() == 13);

    // Do correct extractions
    BOOST_CHECK(pt.get_d(T("key1"), 0) == 1);
    BOOST_CHECK(pt.get_d(T("key2.key"), 0) == 2);
    BOOST_CHECK(pt.get_d(T("key3.key.key"), 0) == 3);
    BOOST_CHECK(pt.get_d(CHTYPE('/'), T("key4"), 0) == 4);
    BOOST_CHECK(pt.get_d(CHTYPE('/'), T("key5/key"), 0) == 5);
    BOOST_CHECK(pt.get_d(CHTYPE('/'), T("key6/key/key"), 0) == 6);

    // Do incorrect extractions
    BOOST_CHECK(pt.get_d(T("key2/key"), 0) == 0);
    BOOST_CHECK(pt.get_d(T("key3/key/key"), 0) == 0);
    BOOST_CHECK(pt.get_d(CHTYPE('/'), T("key5.key"), 0) == 0);
    BOOST_CHECK(pt.get_d(CHTYPE('/'), T("key6.key.key"), 0) == 0);

}

void test_precision(PTREE *)
{
    
    typedef double real;
    
    // Precise PI value
    real pi = real(3.1415926535897932384626433832795028841971);
    
    // Put and get
    PTREE pt;
    pt.put_own(pi);
    real pi2 = pt.get_own<real>();
    
    // Test if precision is "good enough", i.e. if stream precision increase worked
    real error = std::abs(pi - pi2) *
        std::pow(real(std::numeric_limits<real>::radix), 
                 real(std::numeric_limits<real>::digits));
    BOOST_CHECK(error < 100);

}

void test_locale(PTREE *)
{
    
    try
    {
    
        // Write strings in english and french locales
        PTREE pt;
#ifdef BOOST_WINDOWS
        std::locale loc_english("english");
        std::locale loc_french("french");
#else
        std::locale loc_english("en_GB");
        std::locale loc_french("fr_FR");
#endif
        pt.put(T("english"), 1.234, loc_english);
        pt.put(T("french"), 1.234, loc_french);

        // Test contents
        BOOST_CHECK(pt.get<PTREE::data_type>(T("english")) == T("1.234"));
        BOOST_CHECK(pt.get<PTREE::data_type>(T("french")) == T("1,234"));

    }
    catch (boost::property_tree::ptree_error &)
    {
        throw;
    }
    catch (std::runtime_error &e)
    {
        std::cerr << "\"en_GB\" and/or \"fr_FR\" locale not supported by the platform. "
                     "Skipping locale tests (caught std::runtime_error with message " << 
                     e.what() << ").\n";
    }

}

void test_leaks(PTREE *)
{
    BOOST_CHECK(PTREE::debug_get_instances_count() == 0);
}
