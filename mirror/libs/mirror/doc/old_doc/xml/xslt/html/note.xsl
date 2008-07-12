<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- 
	 - template for the paragraph node
	-->
	<xsl:template match="note">
		<P class="note">
			<B>Note:</B>
			<EM><xsl:apply-templates/></EM>
		</P>
	</xsl:template>
</xsl:stylesheet>
