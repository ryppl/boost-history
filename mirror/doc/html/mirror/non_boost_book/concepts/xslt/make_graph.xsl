<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
	<xsl:include href="make_tree.xsl"/>
	
	<xsl:template name="make-graph">
		<xsl:param name="concept"/>
		<xsl:param name="role"/>
		<xsl:param name="caption"/>

		<xsl:call-template name="make-tree">
			<xsl:with-param name="concept" select="$concept"/>
			<xsl:with-param name="role" select="$role"/>
			<xsl:with-param name="caption" select="$caption"/>
		</xsl:call-template>
		
	</xsl:template>

</xsl:stylesheet>
