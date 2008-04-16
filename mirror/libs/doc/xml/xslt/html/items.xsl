<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- 
	- template for the items node
	- items node represents a item list 
	-->
	<xsl:template match="items">
		<UL>
			<xsl:apply-templates/>
		</UL>
	</xsl:template>
	<!-- 
	- template for the item node
	- item node represents a single item in a item list 
	-->
	<xsl:template match="item">
		<LI>
			<xsl:if test="@title">
				<B><xsl:value-of select="@title"/>:</B>
			</xsl:if>
			<xsl:apply-templates/>
		</LI>	
	</xsl:template>
</xsl:stylesheet>
