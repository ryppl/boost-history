# input file as prepared by add_issue_number.bat

s/<h3><a name="\([0-9]*\)"[^\$]*\$\(..\) \([0-9\.]*\)<\/b><\/p>/\/<comment nb="\2" num="\3"\/ s% issue="[^"]*"% issue="\1"%/g
