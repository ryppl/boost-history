#include <iostream>
#include <boost/cgi/acgi.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <google/template.h>

using namespace boost::acgi;

std::string string_from_currency(std::string amt)
{
  using namespace boost::algorithm;
  
  erase_all_regex(amt, boost::regex("[$, ]")); // this is much too hardcore...
  return amt;
}


int main()
{
  service s;
  request req(s);
  req.load(true);
  response resp;

  google::TemplateDictionary dict("example");
  if (req.POST("LoanAmt").empty())
    dict.SetValue("LoanAmt", "$250,000");
  else
    dict.SetValue("LoanAmt", req.POST("LoanAmt"));

  if (req.POST("YearlyIntRate").empty())
    dict.SetValue("YearlyIntRate", "6.000");
  else
    dict.SetValue("YearlyIntRate", req.POST("YearlyIntRate"));

  boost::array<std::string, 8> year_opts
    = {{ "5", "7", "10", "20", "30", "40", "50" }};
    
  BOOST_FOREACH(std::string& year, year_opts)
  {
    dict.SetValueAndShowSection("TermYrs", year, "SELECT_TERM_YEARS");
  }

  if (req.POST("Amortize").empty())
    dict.ShowSection("NotAmortize");
  else
  {
    try {
      double P = boost::lexical_cast<double>(string_from_currency(req.POST("LoanAmt")));
      double i = boost::lexical_cast<double>(boost::algorithm::trim_copy(req.POST("YearlyIntRate"))) / 1200;
      double n = boost::lexical_cast<double>(req.POST("TermYrs")) * 12;

      //resp<< "P = " << P << ", i = " << i << ", n = " << n << ", ";
      double monthly_payments = (P*i) / (1 - std::pow((1+i), -n));

      //resp<< monthly_payments;
      
      google::TemplateDictionary* sub_dict = dict.AddSectionDictionary("RegPmtSummary");
      sub_dict->ShowSection("RegPmtSummary");
      sub_dict->SetFormattedValue("MonthlyPmt", "%.2f", monthly_payments);

      dict.ShowSection("Amortize");

      double balance = P;
      int row_num = 0;
      double interest;
      double principal_paid;
      double total_interest;
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

    }catch(boost::bad_lexical_cast& blc)
    {
      std::cout<< "Bad lexical_cast: " << blc.what() << std::endl;
      return 0;
    }

  }

  google::Template* tmpl = google::Template::GetTemplate("example.tpl", google::STRIP_WHITESPACE);

  std::string output;
  tmpl->Expand(&output, &dict);
  
  resp<< content_type("text/html")
      << output;

  resp.send(req.client());

  return 0;
}

