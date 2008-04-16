<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- 
	 - template for the paragraph node
	-->
	<xsl:template match="cpp_expr">
		<CODE class="cpp-expr">
			<xsl:apply-templates/>
		</CODE>	
	</xsl:template>
</xsl:stylesheet>
