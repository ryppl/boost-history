grep " Response to " lwg-active.html >i1

np++ i1

sed --file=generate.sed <i1 >temp.sed

copy /y comments.xml temp1.xml

change temp1.xml "\n  nb=\q" " nb=\q"
change temp1.xml "\n  num=\q" " num=\q"
change temp1.xml "\n  uknum=\q" " uknum=\q"
change temp1.xml "\n  type=\q" " type=\q"
change temp1.xml "\n  owner=\q" " owner=\q"
change temp1.xml "\n  issue=\q" " issue=\q"
change temp1.xml "\n  disp=\q" " disp=\q"
change temp1.xml "\n  date=\q" " date=\q"
change temp1.xml "\n  extdoc=\q" " extdoc=\q"
change temp1.xml "\n>\n<section>" ">\n<section>"

sed --file=temp.sed <temp1.xml >comments_with_numbers.xml





