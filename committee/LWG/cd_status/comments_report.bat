echo To see comments without issue numbers: --param no_issue "'yes'"

xsltproc --param responsible "'LWG'" %* comments_report.xsl comments.xml >comments_report.html
comments_report.html
