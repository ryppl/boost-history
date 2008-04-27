<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- 
	 - template for the paragraph node
	-->
	<xsl:template match="filepath">
		<CODE class="filepath">
			<xsl:text>"</xsl:text>
			<xsl:apply-templates/>
			<xsl:text>"</xsl:text>
		</CODE>	
	</xsl:template>
</xsl:stylesheet>
