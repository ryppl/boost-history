//
//  A simple "toy SQL" parser for Spirit.
//  Requires -  Spirit version 1.6.0 or later
//
//  Written by:  Andy Elvey
//  
//  This parser parses a very small subset of SQL : 
//  the "select" clause only.
//  
//  NOT YET SUPPORTED - 
//  a) Dotted names ( i.e.  filename.variablename ). This means that
//    join statements using these will not parse correctly.
//  b) Left joins. 
//  c) The "case when" statement 
//      ( e.g. case when myvar <= 5 then 'foo' 
//                  when myvar  = 6 then 'bar'  
//                  else 'baz' 
//             end 
//                  as var1  )  
//  
//  NOTE - If you use a text variable in your query (e.g. 
//   select * from mystuff where var1 = 'abc' ;  ) make sure that 
//  the variable is in **single** quotes, as shown above. 
//
//  Examples of queries that will work - 
//    select * from mydata ;  
//    select var1, var2, var3 from moredata, mystuff, yourdata ; 
//    select test as var1, foo as var2 from mystuff 
//          where a < 45 and b in (1, 2, 3) or c <= 78 ;  
//
//  ========================================================================= 
//    Permission to copy, use, modify, sell and distribute this software is
//    granted provided this copyright notice appears in all copies. This
//    software is provided "as is" without express or implied warranty, and
//    with no claim as to its suitability for any purpose.
//  =========================================================================  


//#define BOOST_SPIRIT_DEBUG  ///$$$ DEFINE THIS WHEN DEBUGGING $$$///


#include <boost/spirit/core.hpp>
#include <boost/spirit/symbols/symbols.hpp>
#include <boost/spirit/utility/lists.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

///////////////////////////////////////////////////////////////////////////////
using namespace boost::spirit;
using namespace std;

///////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------
//  Start grammar definition
//----------------------------------------------------------------------------

struct toysql_grammar :
    public grammar<toysql_grammar>
{
    template <typename ScannerT>
    struct definition
    {
        definition(toysql_grammar const& self)
        {
        
            //-----------------------------------------------------------------
            // KEYWORDS
            //-----------------------------------------------------------------
            keywords =
                "select", "distinct", "from" , "where" , "in" , 
                "and" , "or" , "as" , 
                "is" , "not" , "null" ;             
        
            //-----------------------------------------------------------------
            // OPERATORS
            //-----------------------------------------------------------------

            chlit<>     STAR('*');
            chlit<>     COMMA(',');
            chlit<>     LPAREN('(');
            chlit<>     RPAREN(')'); 
            chlit<>     SEMI(';');
            chlit<>     LT('<');
            chlit<>     GT('>');
            strlit<>    LE("<=");
            strlit<>    GE(">=");
            chlit<>     EQUAL('=');
            strlit<>    NE("!=");            


            //---------------------------------------
            // TOKENS
            //---------------------------------------
            typedef inhibit_case<strlit<> > token_t;

            token_t SELECT      = nocase_d["select"];
            token_t DISTINCT    = nocase_d["distinct"];
            token_t FROM        = nocase_d["from"];
            token_t WHERE       = nocase_d["where"];
            token_t IN_         = nocase_d["in"];
            token_t AND_        = nocase_d["and"];
            token_t OR_         = nocase_d["or"];
            token_t AS          = nocase_d["as"];
            token_t IS_         = nocase_d["is"];
            token_t NOT_        = nocase_d["not"]; 
            token_t NULL_       = nocase_d["null"];  
                  

          //-----------------------------------------
          //	Start grammar definition
          //-----------------------------------------

            identifier =
                nocase_d
                [
                    lexeme_d
                    [
                        (alpha_p >> *(alnum_p | '_'))
                    ]
                ]
            ;

            string_literal =
        lexeme_d
            [
              ch_p('\'') >>  +( anychar_p - ch_p('\'') )
              >> ch_p('\'')
            ]
            ;


            //-----------------------------------------------------------------
            // RULES
            //-----------------------------------------------------------------

            //  Now - the actual BNF grammar for the parser

            program =
             +(query) ; 
         
            query 
            = longest_d[ short_query
            | long_query ] ;
            
            short_query
            = select_stmt >> from_stmt ; 
            
            long_query
            = select_stmt >> from_stmt >> where_stmt ;
                        
            select_stmt =
             SELECT >> !(DISTINCT) 
             >> ( STAR | var_stmt ) 
            ;   

           var_stmt = longest_d[ varname 
              |  list_p( varname, COMMA ) ] ;
            
           varname = identifier >> !(as_stmt) ; 
                        
           as_stmt = AS >> alias ; 
           
           alias = identifier ;  
            
            from_stmt =
                FROM >> table_stmt 
                >> ( SEMI | where_stmt )
            ;
            
            table_stmt = longest_d[ identifier 
              |  list_p(identifier, COMMA ) ] ;  
            ;
        
        
//  Where statement 

           where_stmt = WHERE  
               >> +(stmt)
               >> SEMI ; 
                 
           stmt = longest_d [ single_stmt       
              | multi_stmt ] ;                       
                             
            single_stmt 
             =  ( join_stmt 
             |  compare_stmt ) ;              
                                                                                   
            multi_stmt 
             = list_p( single_stmt, and_or_stmt) ;                 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
             join_stmt
            = tablename >> op >> tablename ;    
                      
            tablename = identifier ;                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
            compare_stmt 
            = var_op_value_stmt
            | in_stmt 
            | null_stmt ;
             
            var_op_value_stmt 
            =  !(LPAREN) >> var >> op >> value >> !(RPAREN) ;
            
            in_stmt 
            =  var >> !(NOT_) >> IN_ >> list_stmt ;                                                 
                        
            null_stmt
            = var >> IS_ >> !(NOT_) >> NULL_ ;                          
                              
            and_or_stmt 
            = AND_ | OR_ ;  

            list_stmt
            = ( string_list | number_list ) ; 
            
            string_list 
            = LPAREN >> list_p( string_literal, COMMA )
            >> RPAREN ; 
            
            number_list 
            = LPAREN >> list_p( number, COMMA )
            >> RPAREN ; 
                                                                                                                             
            var = identifier ;                       
             
            value = longest_d[ ( string_literal 
            | number ) ] ; 
                                                                  
            number = uint_p ;  
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
            op = longest_d[ ( LT | LE | EQUAL | GE | GT | NE ) ]
            ;      
                                                                                                                                                                                                                                                                              
        }


      rule<ScannerT> const&
        start() const { return program; }
         
         
//  Declare the keywords here         
        symbols<> keywords;
        
        rule<ScannerT> 
        program, query, 
        short_query, long_query, 
        select_stmt, star, var_stmt, varname, 
        as_stmt, alias, from_stmt, table_stmt, 

        where_stmt, stmt, 
        single_stmt, multi_stmt,
        join_stmt, tablename, identifier, 
        compare_stmt, var_op_value_stmt, 
        null_stmt, in_stmt, and_or_stmt, 
        var, value, list_stmt, string_list,
        number_list, number, op, string_literal ;   
                  
    };
};


int
main(int /*argc*/, char* /*argv[]*/)
{
    cout << "/////////////////////////////////////////////////////////\n\n";
    cout << "\t\t A toy SQL parser...\n\n";
    cout << "/////////////////////////////////////////////////////////\n\n";
    cout << "Type a SQL expression \n" ; 
    cout << " (e.g.  select * from mytable where city = \'Sydney\' ; ) \n" ;
    cout << " Type [q or Q] to quit\n\n" ;

    toysql_grammar g;

    string str;
    while (getline(cin, str))
    {
        if (str[0] == 'q' || str[0] == 'Q')
            break;

        if (parse(str.c_str(), g, space_p).full)
        {
            cout << "parsing succeeded\n";
        }
        else
        {
            cout << "parsing failed\n";
        }
    }

    cout << "Bye... :-) \n\n";
    return 0;
}

