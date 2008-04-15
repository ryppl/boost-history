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
	<!-- inclusions -->
	<xsl:include href="&docroot;/include.xsl"/>
	<!-- references to various things -->
	<xsl:include href="&docroot;/lib_refs.xsl"/>
	<xsl:include href="&docroot;/boost_refs.xsl"/>
</xsl:stylesheet>
