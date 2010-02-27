echo Make link from extract_issues output

if {%1}=={} goto usage
if {%2}=={} goto usage

sed "s|[0-9]*|<p><a href=*%2#&*>&</a></p>|" %1 >temp5
chg temp5 "*" "\q"
type temp5

goto done

:usage
  echo Usage: make_link source-path URL

:done
