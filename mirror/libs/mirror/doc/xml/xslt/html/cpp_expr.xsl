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
	<!-- 
	 - template for a multi-line source code
	-->
	<xsl:template match="cpp_source">
		<P>
			<CODE class="cpp-source">
				<xsl:apply-templates/>
			</CODE>	
		</P>
	</xsl:template>
	<!-- 
	 - template for a single line of cpp code 
	-->
	<xsl:template match="cpp_line">
		<xsl:apply-templates/><BR/>
	</xsl:template>
	<!-- 
	 - template for comment in cpp source 
	-->
	<xsl:template match="cpp_comment">
		<SPAN class="cpp-comment">
			<EM><xsl:apply-templates/></EM>
		</SPAN>	
	</xsl:template>
	<!-- 
	 - template for a preprocessor expression
	-->
	<xsl:template match="cpp_preproc">
		<SPAN class="cpp-preproc">
			<xsl:apply-templates/>
		</SPAN>	
	</xsl:template>
	<!-- 
	 - template for a cpp_indentation 
	-->
	<xsl:template match="cpp_indent">
		<DIV class="cpp-indent">
			<xsl:apply-templates/>
		</DIV>	
	</xsl:template>
</xsl:stylesheet>
