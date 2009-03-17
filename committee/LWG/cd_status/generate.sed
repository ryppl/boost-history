#                                                                            \/<comment nb="\2" num="\3"\/ s% issue="[^"]*"% issue="\1"%

s/<h3><a name="\([0-9]*\)"><\/a>[^\.]*\. Response to \(..\) \([0-9]*\)<\/h3>/\/<comment nb="\2" num="\3"\/ s% issue="[^"]*"% issue="\1"%/g