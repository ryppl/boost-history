<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
	<!-- the output method -->
	<xsl:output method="html" encoding="utf-8"/>

	<!-- the root node template -->
	<xsl:template match="/">
		<html>
			<head>
				<title>Concept generalizations and specializations</title>
				<link rel="stylesheet" href="../../../../boostbook.css" type="text/css"/>
				<link rel="stylesheet" href="../css/concept_graph.css"/>

				<link rel="start" href="index.html" title="The Mirror library documentation"/>
			</head>
			<body bgcolor="white" text="black" link="#0000FF" vlink="#840084" alink="#0000FF">
				<table cellpadding="2" width="100%"><tr>
				<td valign="top"><img alt="Boost C++ Libraries" width="277" height="86" src="../../../../../../boost.png"/></td>
				<td align="center"><a href="../../../../../../index.html">Home</a></td>
				<td align="center"><a href="../../../../../../libs/libraries.htm">Libraries</a></td>
				<td align="center"><a href="http://www.boost.org/people/people.htm">People</a></td>
				<td align="center"><a href="http://www.boost.org/more/faq.htm">FAQ</a></td>
				<td align="center"><a href="../../../../../../more/index.htm">More</a></td>
				</tr></table>
				<hr/>
				<div class="spirit-nav">
					<a accesskey="h" href="../../../../index.html"><img src="../../../../../../doc/html/images/home.png" alt="Home"/></a>
				</div>
				<xsl:apply-templates/>

				<table xmlns:rev="http://www.cs.rpi.edu/~gregod/boost/tools/doc/revision" width="100%"><tr>
				<td align="left"></td>
				<td align="right"><div class="copyright-footer">Copyright © 2008 Matus; Chochlik<p>Use, modification and distribution is subject to the Boost
				Software License, Version 1.0. (See accompanying file
				<code class="filename">LICENSE_1_0.txt</code> or copy at <a href="http://www.boost.org/LICENSE_1_0.txt" target="_top">http://www.boost.org/LICENSE_1_0.txt</a>)
				</p>
				</div></td>
				</tr></table>
				<hr/>
				<div class="spirit-nav">
					<a accesskey="h" href="../../../../index.html"><img src="../../../../../../doc/html/images/home.png" alt="Home"/></a>
				</div>


			</body>
		</html>
	</xsl:template>

</xsl:stylesheet>
