<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- 
	 - template for the include node
	-->
	<xsl:template match="include">
		<!-- try to build a url -->
		<xsl:variable name="doc_url">
			<xsl:choose>
				<xsl:when test="@section">
					<xsl:text>../sections/</xsl:text>
					<xsl:value-of select="@section"/>
					<xsl:text>.xml</xsl:text>
				</xsl:when>
				<xsl:when test="@reference_section">
					<xsl:text>../sections/reference/</xsl:text>
					<xsl:value-of select="@reference_section"/>
					<xsl:text>.xml</xsl:text>
				</xsl:when>
				<xsl:when test="@reference">
					<xsl:text>../reference/</xsl:text>
					<xsl:value-of select="@reference"/>
					<xsl:text>.xml</xsl:text>
				</xsl:when>
				<xsl:when test="@url">
					<xsl:value-of select="@url"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:if test="$doc_url">
			<!-- now open the document and apply templates to its nodes -->
			<xsl:apply-templates select="document($doc_url)"/>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet>
