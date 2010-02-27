@echo off

if {%1}=={} goto usage
if {%2}=={} goto usage

sed "s@[0-9]*@/owner=~LWG~ issue=~&~/ s/disp=~[a-z]*~/disp=~%2~/@" %1 >temp6
chg temp6 "~" "\q" >nul
type temp6
goto done

:usage
  echo Generate sed script to set comments.xml status
  echo Usage: generate_status_sed source-path status
  echo source-path format is file of issue numbers such as from extract_issues.bat
  echo status is accepted, modified, or rejected
  echo Resulting script can be invoked: sed --file=temp.sed

:done
