grep -E "(<p><b>Addresses )|(name=)" lwg-active.html >i1
change i1 "\r\n<p><b>Addresses " "$"
grep "\$" <i1 >i2

np++ i2

sed --file=generate.sed <i2 >temp.sed

del comments.xml.bak 2>nul
ren comments.xml comments.xml.bak

sed --file=temp.sed <comments.xml.bak >comments.xml





