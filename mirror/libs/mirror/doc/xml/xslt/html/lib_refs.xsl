<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- 
	 - template for the lib_name node
	 - this node expands to the name of the library without the 'library' word
	-->
	<xsl:template match="lib_name_ref">
		<EM>
			<xsl:variable name="lib_info_url">../_library.xml</xsl:variable>
			<xsl:value-of select="document($lib_info_url)/library/name"/>
		</EM>
	</xsl:template>
	<xsl:template match="Lib_name_ref">
		<EM>
			<xsl:variable name="lib_info_url">../_library.xml</xsl:variable>
			<xsl:value-of select="document($lib_info_url)/library/Name"/>
		</EM>
	</xsl:template>
</xsl:stylesheet>
