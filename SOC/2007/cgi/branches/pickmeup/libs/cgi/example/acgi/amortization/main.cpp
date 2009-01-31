//                     -- main.hpp --
//
//           Copyright (c) Darren Garvey 2007.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
////////////////////////////////////////////////////////////////
//
//[acgi_amort
//
// Amortization Calculator
// -----------------------
//
// This file uses Google cTemplate to show the benefits of using an
// HTML template engine. The code isn't commented, but should be 
// reasonably self-explanatory.
//
// It is a very basic amortization calculator.
//
#include <iostream>
#include <iomanip>
#include <boost/cgi/acgi.hpp>
#include <boost/cgi/utility.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <google/template.h>

using namespace boost::acgi;

/// Convert a string like '$250,000' into one like '250000'.
std::string string_from_currency(std::string amt)
{
  // this is much too hardcore, but it works fine...
  boost::algorithm::erase_all_regex(amt, boost::regex("[£$, ]"));
  return amt;
}

template<typename Request, typename Dictionary>
void get_prepayment_frequency(Request& req, Dictionary& dict)
{
  std::string val(has_key(req[form], "PrePmtFreq")
                       ? req[form]["PrePmtFreq"]
                       : "Monthly");
  dict.SetValue("PrePmtFreq_" + val, " selected=\"selected\"");
}

/// This function fills the dictionary and sub-dictionaries with relevant values.
template<typename Request>
void fill_amortization_dictionary(google::TemplateDictionary& dict, Request& req)
{
  dict.SetValue("SCRIPT_NAME", req.script_name());
  dict.SetValue("LoanAmt", has_key(req[form],"LoadAmt")
                              ? req[form]["LoanAmt"]
                              : "£250,000");

  dict.SetValue("YearlyIntRate", has_key(req[form],"YearlyIntRate")
                                    ? req[form]["YearlyIntRate"]
                                    : "6.000");

  boost::array<std::string, 8> year_opts
    = {{ "5", "7", "10", "20", "30", "40", "50" }};
    
  BOOST_FOREACH(std::string& year, year_opts)
  {
    dict.SetValueAndShowSection("TermYrs", year, "SELECT_TERM_YEARS");
  }
  
  get_prepayment_frequency(req, dict);

  if ( ! has_key(req[form], "Amortize") )
    dict.ShowSection("NotAmortize");
  else
  {
    double P = boost::lexical_cast<double>(string_from_currency(req[form]["LoanAmt"]));
    double i = boost::lexical_cast<double>(req[form]["YearlyIntRate"]) / 1200;
    double n = boost::lexical_cast<double>(req[form]["TermYrs"]) * 12;
    double monthly_payments = (P*i) / (1 - std::pow((1+i), -n));
    
    google::TemplateDictionary* sub_dict = dict.AddSectionDictionary("RegPmtSummary");
    sub_dict->ShowSection("RegPmtSummary");
    sub_dict->SetFormattedValue("MonthlyPmt", "%.2f", monthly_payments);

    dict.ShowSection("Amortize");

    double balance (P);
    int row_num (0);
    double interest (0);
    double principal_paid (0);
    double total_interest (0);
    do{
      google::TemplateDictionary* sub_dict2 = dict.AddSectionDictionary("PaymentEntry");
      sub_dict2->ShowSection("PaymentEntry");
      sub_dict2->SetFormattedValue("Payment", "%.2f", monthly_payments);
      sub_dict2->SetIntValue("ROW_NUM", ++row_num);
      sub_dict2->SetIntValue("ROW_TYPE", (row_num % 2) + 1);
      interest = balance * i;
      total_interest += interest;
      sub_dict2->SetFormattedValue("InterestPaid", "%.2f", interest);
      principal_paid = monthly_payments - interest;
      sub_dict2->SetFormattedValue("PrincipalPaid", "%.2f", principal_paid);
      balance -= principal_paid; // Note: balance can increase.
      sub_dict2->SetFormattedValue("Balance", "%.2f", balance);

    }while(balance > 0);

    sub_dict->SetFormattedValue("RegPmt_TotalIntPd", "%.2f", total_interest);
    sub_dict->SetFormattedValue("RegPmt_TotalPmts", "%.2f", total_interest + P);
  }
}

template<typename Request>
void write_amortization_template(Request& req, response& resp)
{
  google::TemplateDictionary dict("amortization");

  fill_amortization_dictionary(dict, req);

  google::Template* tmpl
    = google::Template::GetTemplate("../templates/acgi_amort.html", google::STRIP_WHITESPACE);

  std::string arg(req[get]["arg"]);
  if (arg.empty())
    arg = "2"; // Make this the default

  // Depending on the value of `arg`, we can write the output in
  // different ways.

  if (arg == "1")
  {
    // Output the response straight to std::cout. This won't work with
    // anything but vanilla CGI and isn't recommended. It's useful for
    // checking things work though (don't forget to write a content type
    // header - followed by a blank line - first though!).
    std::string output;
    tmpl->Expand(&output, &dict);
    std::cout<< "Content-type: text/html\r\n\r\n"
             << output;
  }else
  if (arg == "2")
  {
    // Expand the output to a string and copy the string to the response.
    // Should be expensive, but doesn't seem to impact performance hugely...
    std::string output;
    tmpl->Expand(&output, &dict);
    resp<< content_type("text/html")
        << output;
  }else
  //  if (arg == "3")
  //  {
  //    // Expand the string to a vector<const_buffer>, which should minimise any
  //    // copying of data. This requires a modified version of Google.cTemplate, but
  //    // doesn't seem to add anything to performance. Will have to check if there's a
  //    // better way to do it. 
  //    std::string s;
  //    std::vector<boost::asio::const_buffer> out;
  //
  //   tmpl->Expand(&s, &out, &dict);
  //    write(req.client(), out);
  //  }else
  if (arg == "4")
  {
    // Write the output directly to the request's client.
    std::string headers ("Content-type: text/html\r\n\r\n");
    write(req.client(), buffer(headers));
    std::string output;
    tmpl->Expand(&output, &dict);
    write(req.client(), buffer(output));
  }else
  if (arg == "5")
  {
    // An alternative to { arg == "1" }, which seems to be slightly faster.
    std::string output;
    tmpl->Expand(&output, &dict);
    const char* headers = "Content-type: text/html\r\n\r\n";
    std::cout.write(headers, strlen(headers));
    std::cout.write(output.c_str(), output.size());
  }else
  {
    resp<< "Error!";
  }
}

int main()
{
  try{
    service s;
    request req(s);
    req.load(parse_all);
    response resp;

    write_amortization_template(req, resp);
  
    return_(resp, req, 0);
  }catch(boost::system::system_error* err){
    std::cout<< "Content-type: text/plain\r\n\r\n"
             << "Error (" << err->code() << "): " << err->what();
    return 0;
  }catch(boost::system::system_error& err){
    std::cout<< "Content-type: text/plain\r\n\r\n"
             << "Error (" << err.code() << "): " << err.what();
    return 0;
  }catch(std::exception* e){
    std::cout<< "Content-type: text/html\r\n\r\n"
             << "Exception caught: " << e->what();
    return 0;
  }catch(std::exception& e){
    std::cout<< "Content-type: text/html\r\n\r\n"
             << "Exception caught: " << e.what();
    return 0;
  }catch(...){
    std::cout<< "Content-type: text/html\r\n\r\n"
             << "Unknown error!";    
    return 0;
  }
}
//]

