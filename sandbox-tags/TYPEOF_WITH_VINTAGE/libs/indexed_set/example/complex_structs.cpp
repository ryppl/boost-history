/* Boost.IndexedSet example: complex searches and foreign keys.
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

#include <boost/indexed_set.hpp>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <string>

using boost::indexed_set;
using namespace boost::indexed_sets;

/* This small utility that composes two key extractors will be
 * used througout the example.
 */

template<class KeyExtractor1,class KeyExtractor2>
struct compose_key
{
public:
  typedef typename KeyExtractor1::result_type result_type;

  compose_key(
    const KeyExtractor1& key1_=KeyExtractor1(),const KeyExtractor2& key2_=KeyExtractor2()):
    key1(key1_),key2(key2_)
  {}

  template<typename Arg>
  result_type operator()(Arg& arg)const
  {
    return key1(key2(arg));
  }

private:
  KeyExtractor1 key1;
  KeyExtractor2 key2;
};

/* tags for accessing several indices defined below */

struct model{};
struct manufacturer{};
struct price{};

/* a manufacturer struct just holds the name of the manufacturer */

struct car_manufacturer
{
  std::string name;

  car_manufacturer(const std::string& name_):name(name_){}
};

/* car_model holds the model of car, its price and a pointer to the
 * manufacturer. The pointer thing eliminates duplication of the same
 * data among cars of the same manufacturer.
 */

struct car_model
{
  std::string             model;
  const car_manufacturer* manufacturer;
  int                     price;

  car_model(const std::string& model_,const car_manufacturer* manufacturer_,int price_):
    model(model_),manufacturer(manufacturer_),price(price_)
  {}

  friend std::ostream& operator<<(std::ostream& os,const car_model& c)
  {
    os<<c.manufacturer->name<<" "<<c.model<<" $"<<c.price<<std::endl;
    return os;
  }
};

/* see Advanced topics: Use of member_offset for info on BOOST_INDEXED_SET_MEMBER */

/* Car manufacturers are stored in an indexed_set with one single index on
 * the name member. This is functionally equivalent to an std::set, though
 * in this latter case we woud have to define a non-default comparison
 * predicate (with indexed_set, member<> does the work for us.)
 */

typedef indexed_set<
  car_manufacturer,
  index_list<
    unique<BOOST_INDEXED_SET_MEMBER(car_manufacturer,std::string,name)>
  >
> car_manufacturer_table;

/* Define an indexed_set of car_models with following indices:
 *   - a unique index sorted by car_model::model,
 *   - a non-unique index sorted by car_model::manufacturer; note the
 *     non-standard manufacturer_extractor used.
 *   - a non-unique index sorted by car_model::price.
 */

typedef indexed_set<
  car_model,
  index_list<
    unique<tag<model>,BOOST_INDEXED_SET_MEMBER(car_model,std::string,model)>,
    non_unique<
      tag<manufacturer>,
      compose_key<
        BOOST_INDEXED_SET_MEMBER(car_manufacturer,const std::string,name),
        BOOST_INDEXED_SET_MEMBER(car_model,const car_manufacturer *,manufacturer)
      >
    >,
    non_unique<tag<price>,BOOST_INDEXED_SET_MEMBER(car_model,int,price)>
  >
> car_table;

/* We call a *view* to an indexed_set storing pointers instead of
 * actual objects. These views are used in the complex search performed
 * in the program. Resorting to indexed_sets of pointers eliminates
 * unnecessary copying of objects, and provides us with an opportunity
 * to show how BOOST_INDEXED_SET_MEMBER can be used with pointer
 * type elements.
 * car_table_price_view indexes (pointers to) car_models by price.
 */

typedef indexed_set<
  const car_model*,
  index_list<
    non_unique<BOOST_INDEXED_SET_MEMBER(car_model,const int,price)>
  >
> car_table_price_view;

/* car_table_manufacturer_view indexes (pointers to) car_models by manufacturer */

typedef indexed_set<
  const car_model*,
  index_list<
    non_unique<
      compose_key<
        BOOST_INDEXED_SET_MEMBER(car_manufacturer,const std::string,name),
        BOOST_INDEXED_SET_MEMBER(car_model,const car_manufacturer * const,manufacturer)
      >
    >
  >
> car_table_manufacturer_view;

int main()
{
  car_manufacturer_table cmt;

  /* Fill the car_manufacturer table and keep pointers to the
   * elements inserted.
   */

  const car_manufacturer * cadillac=&*(cmt.insert(car_manufacturer("Cadillac")).first);
  const car_manufacturer * ford    =&*(cmt.insert(car_manufacturer("Ford")).first);
  const car_manufacturer * bmw     =&*(cmt.insert(car_manufacturer("BMW")).first);
  const car_manufacturer * audi    =&*(cmt.insert(car_manufacturer("Audi")).first);

  car_table ct;

  /* Fill the car_model_table. We use the previously initialized
   * pointers to the elements of cmt.
   */

  ct.insert(car_model("XLR",cadillac,76200));
  ct.insert(car_model("SRX",cadillac,38690));
  ct.insert(car_model("CTS",cadillac,30695));
  ct.insert(car_model("Escalade",cadillac,54770));
  ct.insert(car_model("ESV",cadillac,57195));
  ct.insert(car_model("EXT",cadillac,52045));
  ct.insert(car_model("Deville",cadillac,45195));
  ct.insert(car_model("Seville",cadillac,46330));

  ct.insert(car_model("ZX2",ford,15355));
  ct.insert(car_model("Thunderbird",ford,43995));
  ct.insert(car_model("Windstar",ford,35510));
  ct.insert(car_model("Focus",ford,19630));
  ct.insert(car_model("Taurus",ford,24290));
  ct.insert(car_model("Mustang",ford,39900));
  ct.insert(car_model("Crown Victoria",ford,30370));

  ct.insert(car_model("325i",bmw,27800));
  ct.insert(car_model("545i",bmw,54300));
  ct.insert(car_model("745i",bmw,68500));
  ct.insert(car_model("M3 coupe",bmw,46500));
  ct.insert(car_model("Z4 roadster 3.0i",bmw,40250));
  ct.insert(car_model("X5 4.4i",bmw,49950));

  ct.insert(car_model("A4 1.8T",audi,25940));
  ct.insert(car_model("TT Coupe",audi,33940));
  ct.insert(car_model("A6 3.0",audi,36640));
  ct.insert(car_model("Allroad quattro 2.7T",audi,40640));
  ct.insert(car_model("A8 L",audi,69190));

  std::cout<<"enter a car manufacturer"<<std::endl;
  std::string cm;
  std::getline(std::cin,cm);
  car_manufacturer_table::iterator icm=cmt.find(cm); /* check for manufacturer */
  if(icm==cmt.end()){
    std::cout<<"no such manufacturer in the table"<<std::endl;
    return 0;
  }

  std::cout<<"enter a minimum price"<<std::endl;
  int min_price;
  std::cin>>min_price;
  std::cout<<"enter a maximum price"<<std::endl;
  int max_price;
  std::cin>>max_price;

  {
    /* method 1 */

    /* find all the cars for the manufacturer given */

    iterator_type<car_table,manufacturer>::type ic0,ic1;
    boost::tuples::tie(ic0,ic1)=get<manufacturer>(ct).equal_range(cm);

    /* construct a view (indexed by price) with these */

    car_table_price_view ctpv;
    while(ic0!=ic1){
      ctpv.insert(&*ic0);
      ++ic0;
    }

    /* select the cars in the range given */

    car_table_price_view::iterator ictpv0=ctpv.lower_bound(min_price);
    car_table_price_view::iterator ictpv1=ctpv.upper_bound(max_price);
    if(ictpv0==ictpv1){
      std::cout<<"no cars in the range given"<<std::endl;
      return 0;
    }

    /* list them */

    std::cout<<"listing by method 1"<<std::endl;
    while(ictpv0!=ictpv1){
      std::cout<<**ictpv0;
      ++ictpv0;
    }
    std::cout<<std::endl;
  }

  {
    /* method 2 will give the same results */

    /* find the cars in the range given */

    iterator_type<car_table,price>::type ic0,ic1;
    ic0=get<price>(ct).lower_bound(min_price);
    ic1=get<price>(ct).upper_bound(max_price);

    /* construct a view with these */

    car_table_manufacturer_view ctmv;
    while(ic0!=ic1){
      ctmv.insert(&*ic0);
      ++ic0;
    }

    /* select the cars with given manufacturer */

    car_table_manufacturer_view::iterator ictmv0,ictmv1;
    boost::tuples::tie(ictmv0,ictmv1)=ctmv.equal_range(cm);
    if(ictmv0==ictmv1){
      std::cout<<"no cars in the range given"<<std::endl;
      return 0;
    }

    /* list them */

    std::cout<<"listing by method 2"<<std::endl;
    while(ictmv0!=ictmv1){
      std::cout<<**ictmv0;
      ++ictmv0;
    }
    std::cout<<std::endl;
  }

  return 0;
}
