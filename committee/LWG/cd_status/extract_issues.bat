@echo off
rem Extract issues from issues list

if {%1}=={} goto usage
if {%2}=={} goto usage

copy /y %1 temp1 >nul
chg temp1 "</h3>\r\n" "</h3>" >nul
grep "<h3><a" temp1 >temp2
grep "#%2" temp2 >temp3
chg temp3 "\q></a>" "\r\n" >nul
grep "<h3><a" temp3 >temp4
chg temp4 "<h3><a name=\q" "" >nul
type temp4

goto done

:usage
  echo Usage: extract_issues issues-list-path issue-type

:done
