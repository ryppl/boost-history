#include <iostream>
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

void test_named_captures(const string& pattern, const string& text)
{
   regex ex(pattern);
   smatch matches;
   if (regex_match(text, matches, ex, match_extra))
      cout << pattern << " matches " << text << endl;
   else
      cout << pattern << " does not match " << text << endl;
}

void test_named_groups()
{
   test_named_captures("(?P<alpha>a)", "a");
   test_named_captures("(?P<alpha>abc)+", "abcabc");
   test_named_captures("(?P<alpha>a+)(b+)", "aaabbb");
   test_named_captures("(a+)(?P<beta>b+)", "aaabbb");
   test_named_captures("(?P<outer>(a+)b+)", "aaabbb");
   test_named_captures("(?P<outer>a+(b+))", "aaabbb");
   test_named_captures("(a+(?P<inner>b+))", "aaabbb");
   test_named_captures("(?P<outer>a+(?P<inner>b+))", "aaabbb");
   test_named_captures("a+(?P<inSiDe>b+)b+", "aaadzabbb"); //should fail
   test_named_captures("(a)(?P<alt>b|$)", "ab");
   test_named_captures("(a)(?P<alt>b|$)", "a");
}

void test_named_retrieval()
{
   smatch matches;
   regex ad("got (?P<drink>\\w+)\\?$");
   string text = "got milk?";
   if (regex_match(text, matches, ad, match_extra) && matches["drink"] == "milk")
      cout << "passed ad\n";
   else
      cout << "failed ad\n";

   regex magic("(?P<first>abra)ca(?P<second>dabra)!");
   text = "abracadabra!";
   if (regex_match(text, matches, magic, match_extra) && matches["first"] == "abra" && matches["second"] == "dabra")
      cout << "passed magic\n";
   else
      cout << "failed magic\n";

   regex poem("(?P<first>.*), (\\w+) (?P<second>.*) D.*");
   text = "Twas brillig, and the slithy toves Did gyre and gimble in the wabe";
   if (regex_match(text, matches, poem, match_extra) && 
      matches["first"] == "Twas brillig" && matches["second"] == "the slithy toves" && 
      matches["second"] == matches[3])
      cout << "passed poem\n";
   else
      cout << "failed poem\n";
}

void test_named_backrefs()
{
   test_named_captures("(?P<alpha>a)bc(?P=alpha)", "abca");
   test_named_captures("(?P<alpha>a+)bc(?P=alpha)", "aaabcaaa");
   test_named_captures("(?P<alpha>a+)bc(?P=alpha)", "aaabca"); //should fail
   test_named_captures("a(?P<alpha>b|d)(?P=alpha)", "abb");
   test_named_captures("a(?P<alpha>b|d)(?P=alpha)", "add");
   test_named_captures("a(?P<alpha>b|d)(?P=alpha)", "abd"); //should fail
   test_named_captures("(?P<name>\\w+) (.*) (?P=name)", "Jameson R. Jameson");
   test_named_captures("(a+(?P<inner>b+))(?P=inner)", "aabbbb");
   test_named_captures("(a+(?P<inner>b+))c(?P=inner)", "aabcb");
   test_named_captures("(a+(?P<inner>b+))(?P=inner)", "aabbb"); //should fail
   test_named_captures("(?P<recursive>a+(?P=recursive))(b+)", "aaabbb"); //should fail
   test_named_captures("(?P<recursive>a+(?P=recursive))(b+)", "aaabbb"); //should fail
   // the two below should (and do) cause runtime errors when the pattern compiles
   //test_named_captures("(?P=alpha)", "abca"); //should fail
   //test_named_captures("(?P=<alpha>.*)(?=beta)", "abab"); //should fail
}

int main(int argc, char * argv[])
{
   test_named_groups();
   test_named_retrieval();
   test_named_backrefs();

   return 0;
}