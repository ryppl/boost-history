
---------------------------------------------
Release Notes -  The Spirit SQL parser
---------------------------------------------

Written by: Andy Elvey 
Released: 24th Feb 2003 
Licence: The Boost licence (see the bottom of this readme file)

---- Motivation ---- 
Having used Spirit since soon after it was first released, I was keen 
to put together an interesting little example which showed many of 
Spirit's very powerful features. Although this little "toy SQL" parser
is small, I believe that it nonetheless gives the user a good idea of 
the overall flavour of Spirit.  
  
---- Notes and Caveats ----- 
As mentioned in the code for this example, there are *several parts* of 
the SQL "select" statement which are not yet supported. 
These are as follows - 

  a) Dotted names ( i.e.  filename.variablename ). This means that
     join statements using these will not parse correctly.  
     In other words, if you type in - 
  
  select foo, bar from mytable, yourtable where 
      mytable.var1 = yourtable.var2 ; 

 - the statement won't parse.  However, if you type in - 

  select foo, bar from mytable, yourtable where 
      var1 = var2 ; 

 -  This WILL parse.  Yes, you'll ask "where's the join between the 
  tables?" :-)   My main motivation in doing this example was to put
  together a parser which accepted something close to SQL syntax. 
  The "missing join" problem is one area not yet done.   

  b) Left joins. In other words - 
    select myvar, yourvar from tab1 left join tab2 
       on tab1.foo = tab2.bar ;

  c) The "case when" statement 
      ( e.g. case when myvar <= 5 then 'foo' 
                  when myvar  = 6 then 'bar'  
                  else 'baz' 
             end 
                  as var1  )  
  
  NOTE - If you use a text variable in your query (e.g. 
   select * from mystuff where var1 = 'abc' ;  ) make sure that 
  the variable is in single quotes, as shown above. 

//  **************************************************** // 
       THE SQL PARSER IN ACTION 
  
  Examples of queries that *WILL* work with the parser -
 
    select * from mydata ;  
    select * from yourstuff where a not in (1, 2, 67) and 
       b in ('foo' , 'bar') ; 
    select var1, var2, var3 from moredata, mystuff, yourdata ; 
    select test as var1, foo as var2 from mystuff 
          where a < 45 and b >= 78 ;  

    select foo, bar as var1, baz from fred, trev where foo >= 23 ; 

//  **************************************************** //

   This SQL parser example is released under the Boost licence. 

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
//  ==================================================== //

 
