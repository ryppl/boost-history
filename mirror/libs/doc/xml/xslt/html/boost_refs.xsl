<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- 
	- template for the boost node
	- this node is expanded to the word boost linking to www.boost.org
	-->
	<xsl:template match="boost">
		<A href="http://www.boost.org"><B><EM>boost</EM></B></A>
	</xsl:template>
</xsl:stylesheet>
