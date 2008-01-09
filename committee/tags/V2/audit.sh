
echo
echo "==== sections"
sed -e '
	/<h[1-6]>/			! d
	/^<h\([1-6]\)>.*<\/h\1>$/	d
' thread_library.html

echo
echo "==== paragraph head"
sed -e '
	/<p>/		! d
	/^<p>$/		d
' thread_library.html

echo
echo "==== paragraph tail"
sed -e '
	/<\/p>/		! d
	/^<\/p>$/	d
' thread_library.html

echo
echo "==== block head"
sed -e '
	/<dd>/		! d
	/^<dd>$/	d
' thread_library.html
sed -e '
	/<blockquote>/		! d
	/^<blockquote>$/	d
' thread_library.html

echo
echo "==== block tail"
sed -e '
	/<\/dd>/	! d
	/^<\/dd>$/	d
' thread_library.html
sed -e '
	/<\/blockquote>/	! d
	/^<\/blockquote>$/	d
' thread_library.html

echo
echo "==== example, footnote, and note head"
sed -e '
	/Editor:/		! d
	/^\[<i>Editor:<\/i>$/	d
' thread_library.html
sed -e '
	/Example:/		! d
	/^\[<i>Example:<\/i>$/	d
' thread_library.html
sed -e '
	/Footnote:/		! d
	/^\[<i>Footnote:<\/i>$/	d
' thread_library.html
sed -e '
	/Note:/			! d
	/^\[<i>Note:<\/i>$/	d
' thread_library.html

echo
echo "==== example, footnote, and note tail"
sed -e '
	/end editor/				! d
	/&mdash;<i>end editor<\/i>\]$/		d
' thread_library.html
sed -e '
	/end example/				! d
	/&mdash;<i>end example<\/i>\]$/		d
' thread_library.html
sed -e '
	/end footnote/				! d
	/&mdash;<i>end footnote<\/i>\]$/	d
' thread_library.html
sed -e '
	/end note/				! d
	/&mdash;<i>end note<\/i>\]$/		d
' thread_library.html

echo
echo "==== pre head"
sed -e '
	/<pre>/		! d
	/^<pre><code>$/	d
' thread_library.html

echo
echo "==== pre tail"
sed -e '
	/<\/pre>/		! d
	/^<\/code><\/pre>$/	d
' thread_library.html

echo
echo "==== section references"
sed -e '
	/\[[^[]*\]/		! d
	/^<h[1-6]>/		d
	/^<br>/			d
	/^<b>/			d
	/(\[[^[]*\])/		d
	/See \[thread.native\]/	d
	/\[alg.sorting\]/	d
	/\[class\])/		d
	/\[time\]/		d
' thread_library.html

echo
echo "==== no break space"
sed -e '
	/&nbsp;/		! d
	/^<br>.*href="#/	d
	/mailto:/		d
	/<h2>/			d
	/<td>&nbsp;<\/td>/	d
	/<th>&nbsp;<\/th>/	d
' thread_library.html

echo
echo "==== space at end of line"
sed -e '
	/ $/	! d
' thread_library.html

echo
echo "==== two sentences on one line"
sed -e '
	/\. /		! d
	/\. args/	d
	/i\.e\.\ /	d
' thread_library.html

echo
echo "==== double dash"
sed -e '
	/--/		! d
' thread_library.html

echo
echo "==== span"
sed -e '
	/<span/		! d
' thread_library.html

echo
echo "==== italic"
sed -e '
	/<i>/					! d
	/^\[<i>Note:<\/i>$/			d
	/^&mdash;<i>end note<\/i>]$/		d
	/^\[<i>Footnote:<\/i>$/			d
	/^&mdash;<i>end footnote<\/i>]$/	d
	/^\[<i>Example:<\/i>$/			d
	/^&mdash;<i>end example<\/i>]$/		d
	/^\[<i>Editor:<\/i>$/			d
	/^&mdash;<i>end editor<\/i>]$/		d
' thread_library.html

echo
echo "==== bold"
sed -e '
	/<b>/		! d
' thread_library.html

echo
echo "==== long code lines"
sed -e '
	1,/<pre>/		d
	/<\/pre>/,/<pre>/	d
	/.\{81,\}/		! d
				s/&lt;/</g
				s/&gt;/>/g
				s/&amp;/\&/g
				s/<[a-z]*>//g
				s/<\/[a-z]*>//g
	/.\{81,\}/		! d
' thread_library.html

echo
echo "==== long non-code lines"
sed -e '
	/<pre>/,/<\/pre>/	d
	/.\{81,\}/		! d
	/^<br>/			d
	/^<h[1-6]>/		d
' thread_library.html

