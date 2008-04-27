<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template name="get-section-depth">
		<xsl:text>3</xsl:text>
	</xsl:template>
	<!-- 
	 - template for the section node
	-->
	<xsl:template match="section">
		<xsl:variable name="heading_kind">
			<xsl:text>H</xsl:text>
			<xsl:call-template name="get-section-depth"/>
		</xsl:variable>
		<xsl:element name="{$heading_kind}">
			<xsl:value-of select="@title"/>
		</xsl:element>
		<DIV class="section">
			<xsl:apply-templates/>
		</DIV>
	</xsl:template>
</xsl:stylesheet>
