<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:variable name="lib_info_url">../_library.xml</xsl:variable>
	<!-- 
	 - template that prints out the links to pages related this page
	-->
	<xsl:template name="format-related-page-links">
		<DIV class="related-pages">
			Related pages: [<A href="index.xml">Home</A>][&lt;&lt;][&lt;][^][&gt;][&gt;&gt;]
		</DIV>
	</xsl:template>
	<!-- 
	 - template that prints out the licensing notice
	-->
	<xsl:template name="format-licensing-notice">
		<DIV class="licensing-notice">
			<xsl:text>Distributed under the Boost Software License, Version 1.0.</xsl:text>
			<xsl:text>(See accompanying file LICENSE_1_0.txt or copy at </xsl:text>
			<A href="http://www.boost.org/LICENSE_1_0.txt">http://www.boost.org/LICENSE_1_0.txt</A>
			<xsl:text>)</xsl:text>
		</DIV>
	</xsl:template>
	<!-- 
	 - template that prints out the revision notice
	-->
	<xsl:template name="format-revision-notice">
		<DIV class="revision-notice">
			<xsl:for-each select="document($lib_info_url)/library/revisions/revision[position()=last()]">
				<xsl:text>Last revision (</xsl:text>
				<xsl:value-of select="@major"/>
				<xsl:text>.</xsl:text>
				<xsl:value-of select="@minor"/>
				<xsl:text>.</xsl:text>
				<xsl:value-of select="@micro"/>
				<xsl:text>) Date: </xsl:text>
				<xsl:value-of select="substring(@id, 1,4)"/>
				<xsl:text>-</xsl:text>
				<xsl:value-of select="substring(@id, 5,2)"/>
				<xsl:text>-</xsl:text>
				<xsl:value-of select="substring(@id, 7,2)"/>
			</xsl:for-each>
		</DIV>
	</xsl:template>
	<!-- 
	 - template that prints out the copyright notice
	-->
	<xsl:template name="format-copyright-notice">
		<DIV class="copyright-notice">
			<xsl:text>Copyright (C) </xsl:text>
			<xsl:for-each select="document($lib_info_url)/library/authors/author">
				<xsl:if test="position()!=1">
					<xsl:if test="position()!=last()">
						<xsl:text>, </xsl:text>
					</xsl:if>
					<xsl:if test="position()=last()">
						<xsl:text> and </xsl:text>
					</xsl:if>
				</xsl:if>
				<xsl:value-of select="name"/>
				<xsl:text> </xsl:text>
				<xsl:value-of select="surname"/>
			</xsl:for-each>
			<xsl:text>, </xsl:text>
			<xsl:for-each select="document($lib_info_url)/library/date">
				<xsl:choose>
					<xsl:when test="from = to">
						<xsl:value-of select="from"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="from"/>
						<xsl:text> - </xsl:text>
						<xsl:value-of select="to"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:for-each>
		</DIV>
	</xsl:template>
	<!-- 
	 - template for the root node 
	 - just applies the templates to the xml nodes
	-->
	<xsl:template match="/">
		<xsl:apply-templates/>
	</xsl:template>
	<!-- 
	 - template for the root node 
	 - generates the basic html document skeleton and applies
	 - the templates to the xml nodes
	-->
	<xsl:template match="doc_page">
		<HTML>
			<HEAD>
				<TITLE>
					<xsl:value-of select="document($lib_info_url)/library/title"/>
					<xsl:text> - </xsl:text>
					<xsl:value-of select="@title"/>
				</TITLE>
				<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=utf-8"/>
				<META NAME="author" CONTENT="Matus Chochlik"/>
				<LINK REL="stylesheet" HREF="../../css/mirror.css" TYPE="text/css"/>
				<LINK REL="Index" HREF="index.html" TYPE="text/html"/>
			</HEAD>
			<BODY> 
				<H1><xsl:value-of select="@title"/></H1>
				<xsl:call-template name="format-copyright-notice"/>
				<xsl:call-template name="format-licensing-notice"/>
				<xsl:call-template name="format-related-page-links"/>
				<xsl:apply-templates/>
				<xsl:call-template name="format-related-page-links"/>
				<xsl:call-template name="format-revision-notice"/>
			</BODY>
		</HTML>
	</xsl:template>
</xsl:stylesheet>
