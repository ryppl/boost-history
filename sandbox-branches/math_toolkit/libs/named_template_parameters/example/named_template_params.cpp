#include "boost/named_template_parameters.hpp"

#include "boost/cstdlib.hpp"

#include "boost/tuple/tuple.hpp"

#include <iostream>
#include <string>


// Example features:

// Model     
struct gl { std::string name() { return "gl"; } };
struct glx { std::string name() { return "glx"; } };
struct limited { std::string name() { return "limited"; } };

// Color
struct red { std::string name() { return "red"; } };
struct blue { std::string name() { return "blue"; } };
struct black { std::string name() { return "black"; } };

// Seats
struct leather { std::string name() { return "leather"; } };
struct no_leather { std::string name() { return "no_leather"; } };

// Transmission
struct automatic { std::string name() { return "automatic"; } };
struct manual { std::string name() { return "manual"; } };


// Specify defaults for each possible template parameter
// - your default class must inherit from boost::ntp::named_parameter_tag
//   using virtual inheritance
// - if your default value depends on the values of other functions, give it
//   the value boost:ntp::unspecified

struct default_car_features : 
  virtual public boost::ntp::named_parameter_tag {

  typedef gl Model;
  typedef red Color;
  typedef boost::ntp::unspecified Seats; // we want to make Seats depend 
                                         // on Model
  typedef automatic Transmission;

  

};

// for the default values that depend on other parameter values, define another
// default value class. Give it one template parameter (Features), and
// make it inherit from this parameter
// You will later instantiate this in a way, that you can access all 
// the user given values (as well as trivial defaults) from Features.

template <class Features> 
struct non_trivial_defaults : public Features {
  // The metafunction below encodes:

  // if (Features::Seats==unspecified) {
  //   if (Features::Model==limited) Seats = leather;
  //   else Seats = no_leather;
  // }
  // else Seats = Features::Seats;

  typedef typename
  boost::mpl::if_c<
    boost::is_same<typename Features::Seats, boost::ntp::unspecified>::value,
      typename 
        boost::mpl::if_c<
          boost::is_same<typename Features::Model, limited>::value,
          leather,
          no_leather
        >::type,
      typename Features::Seats  
  >::type Seats;
};


// define your "feature_is" classes. Note! They must inherit form
// your default class using virtual inheritance;

template <class T> struct Model_is : virtual public default_car_features 
{ typedef T Model; };

template <class T> struct Color_is : virtual public default_car_features 
{ typedef T Color; };

template <class T> struct Seats_is : virtual public default_car_features 
{ typedef T Seats; };

template <class T> struct Transmission_is : virtual public default_car_features{ typedef T Transmission; };


// use your default class as a default value for each template parameter
template <class Model = default_car_features, 
          class Color = default_car_features,
          class Seats = default_car_features, 
          class Transmission = default_car_features>
class car {
  // wrap your arguments into a tuple. If you want to allow some parameter
  // slot to accept either a named template or a traditional template, wrap
  // the template with ntp::as_named<feature_name, feature_is_template>::type
  // here Model and Color accept unnamed arguments as well
  typedef 
  boost::tuple<
    typename boost::ntp::as_named<Model, Model_is>::type, 
    typename boost::ntp::as_named<Color, Color_is>::type, 
    Seats, 
    Transmission> argument_tuple;

  // pass that to named_parameter_extractor
  typedef 
  boost::ntp::named_parameter_extractor<argument_tuple> 
  features_with_trivial_defaults;

  // now you got the user given values plus trivial defaults.
  // Apply now the default value metafunctions (to get right Seats in
  // this case)
  typedef non_trivial_defaults<features_with_trivial_defaults> features;
  
  // now all the named parameters are accessible as types in the 
  // 'features' type
  typedef typename features::Model model;
  typedef typename features::Color color;
  typedef typename features::Seats seats;
  typedef typename features::Transmission transmission;

public:
  void print_features() {
    using std::cout;
    cout << "model:        " << model().name() << '\n';
    cout << "color:        " << color().name() << '\n';
    cout << "seats:        " << seats().name() << '\n';
    cout << "transmission: " << transmission().name() << '\n';
  }
};

int main() {

    std::cout << "---\n";
    car<>().print_features();

  // Model and Color accept normal arguments
    std::cout << "---\n";
    car<limited>().print_features();

    std::cout << "---\n";
    car<glx, black>().print_features();

    std::cout << "---\n";
    car<Seats_is<leather>, Color_is<black>, Transmission_is<manual> >()
      .print_features();

    std::cout << "---\n";
    car<Transmission_is<automatic> >().print_features();

  return boost::exit_success;
};










