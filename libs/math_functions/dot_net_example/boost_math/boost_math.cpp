// This is the main DLL file.

#include "stdafx.h"

#pragma warning(disable: 4400) // 'const boost_math::any_distribution ^' : const/volatile qualifiers on this type are not supported

#include "boost_math.h"

namespace boost_math
{

any_distribution::any_distribution(int t, double arg1, double arg2, double arg3)
{
   // This is where all the work gets done:
   switch(t) // index of distribution to distribution_info distributions[]
   {  // New entires must match distribution names, parameter name(s) and defaults defined below.
   case 0:
      this->reset(new concrete_distribution<boost::math::bernoulli>(boost::math::bernoulli(arg1)));
      break;
   case 1:
      this->reset(new concrete_distribution<boost::math::beta_distribution<> >(boost::math::beta_distribution<>(arg1, arg2)));
      break; // Note - no typedef, so need explicit type <> but rely on default = double.
   case 2:
      this->reset(new concrete_distribution<boost::math::binomial_distribution<> >(boost::math::binomial_distribution<>(arg1, arg2)));
      break; // Note - no typedef, so need explicit type <> but rely on default = double.
   case 3:
      this->reset(new concrete_distribution<boost::math::cauchy>(boost::math::cauchy(arg1, arg2)));
      break;
   case 4:
      this->reset(new concrete_distribution<boost::math::chi_squared>(boost::math::chi_squared(arg1)));
      break;
   case 5:
      this->reset(new concrete_distribution<boost::math::exponential>(boost::math::exponential(arg1)));
      break;
    case 6:
      this->reset(new concrete_distribution<boost::math::extreme_value>(boost::math::extreme_value(arg1)));
      break;
    case 7:
      this->reset(new concrete_distribution<boost::math::fisher_f >(boost::math::fisher_f(arg1, arg2)));
      break;
  case 8:
      this->reset(new concrete_distribution<boost::math::gamma_distribution<> >(boost::math::gamma_distribution<>(arg1, arg2)));
      break;
  case 9:
      this->reset(new concrete_distribution<boost::math::lognormal_distribution<> >(boost::math::lognormal_distribution<>(arg1, arg2)));
      break;
  case 10:
      this->reset(new concrete_distribution<boost::math::negative_binomial_distribution<> >(boost::math::negative_binomial_distribution<>(arg1, arg2)));
      break; 
  case 11:
      this->reset(new concrete_distribution<boost::math::normal_distribution<> >(boost::math::normal_distribution<>(arg1, arg2)));
      break;
   case 12:
      this->reset(new concrete_distribution<boost::math::poisson>(boost::math::poisson(arg1)));
      break;
   case 13:
      this->reset(new concrete_distribution<boost::math::rayleigh>(boost::math::rayleigh(arg1)));
      break;
   case 14:
      this->reset(new concrete_distribution<boost::math::students_t>(boost::math::students_t(arg1)));
      break;
   case 15:
      this->reset(new concrete_distribution<boost::math::triangular>(boost::math::triangular(arg1, arg2, arg3)));
      break;
   case 16:
      this->reset(new concrete_distribution<boost::math::uniform>(boost::math::uniform(arg1, arg2)));
      break;
   case 17:
      this->reset(new concrete_distribution<boost::math::weibull>(boost::math::weibull(arg1, arg2)));
      break;
   default:
      // TODO:
      // Need some proper error handling here:
      assert(0);
   }
} // any_distribution constructor.

struct distribution_info
{
   const char* name; // of distribution.
   const char* first_param; // parameters name like "degrees of freedom"
   const char* second_param; // if required, else "".
   const char* third_param; // if required, else "".
   // triangular need 3 parameters.
   // (Only the Bi-Weibull would need 5 parameters?)
   double first_default; // distribution parameter value, often 0, 0.5 or 1.
   double second_default; // 0 if there isn't a second argument.
   // Note that defaults below follow default argument in constructors,
   // if any, but need not be the same.
   double third_default; // 0 if there isn't a third argument.
};

distribution_info distributions[] = 
{ // distribution name, parameter name(s) and default(s) 
  // Order must match any_distribution constructor above!
  // Null string "" and zero default for un-used arguments.
   { "Bernoulli", "Probability", "", "",0.5, 0, 0}, // case 0
   { "Beta", "Alpha", "Beta", "", 1, 1, 0}, // case 1
   { "Binomial", "Trials", "Probability of success", "", 1, 0.5, 0}, // case 2
   { "Cauchy", "Location", "Scale", "", 0, 1, 0}, // case 3
   { "Chi_squared", "Degrees of freedom", "", "", 1, 0, 0}, // case 4
   { "Exponential", "lambda", "", "", 1, 0, 0}, // case 5
   { "Extreme value", "Location", "Scale", "", 0, 1, 0}, // case 6
   { "Fisher-F", "Degrees of freedom 1", "Degrees of freedom 2", "", 1, 1, 0}, // case 7
   { "Gamma", "Shape", "Scale", "", 1, 1, }, // case 8
   { "Lognormal", "Location", "Scale", "", 0, 1, 0}, // case 9
   { "Negative Binomial", "Successes", "Probability of success", "", 1, 0.5, 0}, // case 10
   { "Normal (Gaussian)", "Mean", "Standard Deviation", "", 0, 1, 0}, // case 11
   { "Poisson", "Mean", "", "", 1, 0, 0}, // case 12
   { "Rayleigh", "Sigma", "", "", 1, 0, 0}, // case 13
   { "Student's t", "Degrees of Freedom", "", "", 1, 0, 0}, // case 14
   { "Triangular", "Lower", "Mode", "Upper", -1, 0, +1 }, // case 15 3rd parameter!
   // 0, 0.5, 1 also said to 'standard' but this is most like an approximation to Gaussian distribution.
   { "Uniform", "Lower", "Upper", "", 0, 1, 0}, // case 16
   { "Weibull", "Shape", "Scale", "", 0, 1, 0}, // case 17
};

// How many distributions are supported:
int any_distribution::size()
{
   return sizeof(distributions) / sizeof(distributions[0]);
}

// Display name of i'th distribution:
System::String^ any_distribution::distribution_name(int i)
{
   if(i >= size())
      return "";
   return gcnew System::String(distributions[i].name);
}
// Name of first distribution parameter, or null if not supported:
System::String^ any_distribution::first_param_name(int i)
{
   if(i >= size())
      return "";
   return gcnew System::String(distributions[i].first_param);
}
// Name of second distribution parameter, or null if not supported:
System::String^ any_distribution::second_param_name(int i)
{
   if(i >= size())
      return "";
   return gcnew System::String(distributions[i].second_param);
}
// Name of third distribution parameter, or null if not supported:
System::String^ any_distribution::third_param_name(int i)
{
   if(i >= size())
      return "";
   return gcnew System::String(distributions[i].third_param);
}
// default value for first parameter:
double any_distribution::first_param_default(int i)
{
   if(i >= size())
      return 0;
   return distributions[i].first_default;
}
// default value for second parameter:
double any_distribution::second_param_default(int i)
{
   if(i >= size())
      return 0;
   return distributions[i].second_default;
}
// default value for third parameter:
double any_distribution::third_param_default(int i)
{
   if(i >= size())
      return 0;
   return distributions[i].third_default;
}

} // namespace boost_math


