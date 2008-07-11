<?xml version="1.0" encoding="utf-8" ?>
<!DOCTYPE stylesheet [
<!ENTITY docroot "./html">
]>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- document structure -->
	<xsl:include href="&docroot;/doc_page.xsl"/>
	<xsl:include href="&docroot;/section.xsl"/>
	<xsl:include href="&docroot;/paragraph.xsl"/>
	<xsl:include href="&docroot;/note.xsl"/>
	<xsl:include href="&docroot;/items.xsl"/>
	<xsl:include href="&docroot;/links.xsl"/>
	<xsl:include href="&docroot;/feature_ref.xsl"/>
	<!-- font style-related -->
	<xsl:include href="&docroot;/emph.xsl"/>
	<xsl:include href="&docroot;/cpp_expr.xsl"/>
	<xsl:include href="&docroot;/filepath.xsl"/>
	<!-- inclusions -->
	<xsl:include href="&docroot;/include.xsl"/>
	<!-- references to various things -->
	<xsl:include href="&docroot;/lib_refs.xsl"/>
	<xsl:include href="&docroot;/boost_refs.xsl"/>
</xsl:stylesheet>
