<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- 
	 - template for the link_set node
	-->
	<xsl:template match="link_set">
		<DIV class="linkset">
			<xsl:apply-templates/>
		</DIV>
	</xsl:template>
	<!-- 
	 - template for the link node
	-->
	<xsl:template match="link">
		<xsl:variable name="source_url">
			<xsl:choose>
				<xsl:when test="@section">
					<xsl:text>../sections/</xsl:text>
					<xsl:value-of select="@section"/>
					<xsl:text>.xml</xsl:text>
				</xsl:when>
				<xsl:when test="@page">
					<xsl:text>../pages/</xsl:text>
					<xsl:value-of select="@page"/>
					<xsl:text>.xml</xsl:text>
				</xsl:when>
				<xsl:when test="@url">
					<xsl:value-of select="@url"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="link_title">
			<xsl:choose>
				<xsl:when test="@title">
					<xsl:value-of select="@title"/>
				</xsl:when>
				<xsl:when test="@section">
					<xsl:value-of select="document($source_url)/section/@title"/>
				</xsl:when>
				<xsl:when test="@page">
					<xsl:value-of select="document($source_url)/doc_page/@title"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text>--INVALID LINK--</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="parent::link_set">
				<xsl:element name="A">
					<xsl:attribute name="class">
						<xsl:text>linkset-link</xsl:text>
					</xsl:attribute>
					<xsl:attribute name="href">
						<xsl:value-of select="$source_url"/>
					</xsl:attribute>
					<xsl:value-of select="$link_title"/>
				</xsl:element>
				<BR/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:element name="A">
					<xsl:attribute name="href">
						<xsl:value-of select="$source_url"/>
					</xsl:attribute>
					<xsl:value-of select="$link_title"/>
				</xsl:element>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>
