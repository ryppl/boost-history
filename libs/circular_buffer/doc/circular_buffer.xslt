<?xml version='1.0' encoding='iso-8859-1'?>
<!--
XSL transformation from the XML files generated by doxygen into HTML source documentation.
Author: Jan Gaspar (jano_gaspar[at]yahoo.com)
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  
  <xsl:import href="doxygen2html.xslt"/>
  
  <xsl:output method="xml" version="1.0" encoding="iso-8859-1" indent="yes" media-type="text/xml"/>

  <xsl:template name="template-parameters">
    <xsl:for-each select="templateparamlist/param">
      <xsl:apply-templates select="." mode="synopsis"/>
    </xsl:for-each>
  </xsl:template>
  
  <xsl:template name="public-types">
    <xsl:for-each select="sectiondef[@kind='public-type']/memberdef">
      <xsl:sort select="name"/>
      <xsl:apply-templates select="." mode="synopsis"/>
    </xsl:for-each>
  </xsl:template>
  
  <xsl:template name="member-functions">
    <xsl:for-each select="sectiondef[@kind='public-func']/memberdef[type != '']">
      <xsl:sort select="name"/>
      <xsl:apply-templates select="." mode="synopsis"/>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="exclude-method">
    <xsl:param name="name"/>
  </xsl:template>
  
  <xsl:template name="standalone-functions">
    <xsl:apply-templates select="document(concat($xmldir, '/namespaceboost.xml'))/doxygen/compounddef/sectiondef[@kind='func']/memberdef[contains(argsstring, 'circular_buffer&lt;')]" mode="synopsis">
      <xsl:with-param name="indent" select="''"/>
      <xsl:sort select="name"/>
    </xsl:apply-templates>
  </xsl:template>
  
</xsl:stylesheet>
