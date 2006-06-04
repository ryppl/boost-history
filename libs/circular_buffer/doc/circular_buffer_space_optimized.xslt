<?xml version='1.0' encoding='iso-8859-1'?>
<!--
XSL transformation from the XML files generated by doxygen into HTML source documentation.
Author: Jan Gaspar (jano_gaspar[at]yahoo.com)
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:import href="doxygen2html.xslt"/>

  <xsl:output method="xml" version="1.0" encoding="iso-8859-1" indent="yes" media-type="text/xml"/>

  <xsl:variable name="override-mark" select="'!'"/>
  <xsl:variable name="link-prefix" select="'circular_buffer.html'"/>
  <xsl:variable name="circular_buffer-ref" select="//compound[name='boost::circular_buffer' and @kind='class']/@refid"/>
  <xsl:variable name="circular_buffer-file" select="concat($xmldir, '/', $circular_buffer-ref, '.xml')"/>
  <xsl:variable name="circular_buffer-reimplemented" select="document($circular_buffer-file)/doxygen/compounddef[@id = $circular_buffer-ref and @kind = 'class']//reimplementedby"/>
  <xsl:variable name="standalone-functions" select="document(concat($xmldir, '/namespaceboost.xml'))/doxygen/compounddef/sectiondef[@kind='func']"/>

  <xsl:template name="reference">
    <xsl:variable name="refid" select="$circular_buffer-reimplemented[@refid=current()/@refid]/../@id"/>
    <xsl:value-of select="concat(substring(concat($link-prefix, '#', $refid), 1 div (string-length($refid) &gt; 0)), substring(concat('#', @refid), 1 div (string-length($refid) = 0)))"/>
  </xsl:template>

  <xsl:template name="template-parameters">
    <xsl:apply-templates select="templateparamlist/param" mode="synopsis">
      <xsl:with-param name="link-prefix" select="$link-prefix"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template name="public-types">
    <xsl:for-each select="sectiondef[@kind='public-type']/memberdef | document($circular_buffer-file)/doxygen/compounddef[@id = $circular_buffer-ref and @kind = 'class']/sectiondef[@kind='public-type']/memberdef">
      <xsl:sort select="name"/>
      <xsl:if test="string-length(normalize-space(briefdescription)) &gt; 0 and count(reimplements) = 0">
        <xsl:choose>
          <xsl:when test="../../compoundname = 'boost::circular_buffer_space_optimized'">
            <xsl:apply-templates select="." mode="synopsis"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:apply-templates select="." mode="synopsis">
              <xsl:with-param name="link-prefix" select="$link-prefix"/>
            </xsl:apply-templates>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="member-functions">
    <xsl:variable name="current" select="sectiondef[@kind='public-func']/memberdef[type != '']"/>
    <xsl:for-each select="$current | document($circular_buffer-file)/doxygen/compounddef[@id = $circular_buffer-ref and @kind = 'class']/sectiondef[@kind='public-func']/memberdef[type != '']">
      <xsl:sort select="name"/>
      <xsl:variable name="briefdescription" select="normalize-space(briefdescription)"/>
      <xsl:if test="string-length($briefdescription) &gt; 0 and (starts-with($briefdescription, $override-mark) or count(reimplements) = 0)">
        <xsl:choose>
          <xsl:when test="count($current[name=current()/name]) &gt; 0 and count(param/type[ref='circular_buffer']) &gt; 0">
            <xsl:apply-templates select="$current[name=current()/name]" mode="synopsis">
              <xsl:with-param name="link-prefix" select="$link-prefix"/>
              <xsl:with-param name="link" select="@id"/>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:when test="count($current[name=current()/name]) &gt; 0 and count(param/type[ref='circular_buffer_space_optimized']) &gt; 0"/>
          <xsl:when test="../../compoundname = 'boost::circular_buffer_space_optimized'">
            <xsl:apply-templates select="." mode="synopsis"/>
          </xsl:when>
          <xsl:when test="count($current[name=current()/name and starts-with(normalize-space(briefdescription), $override-mark)]) = 0">
            <xsl:apply-templates select="." mode="synopsis">
              <xsl:with-param name="link-prefix" select="$link-prefix"/>
            </xsl:apply-templates>
          </xsl:when>
        </xsl:choose>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="standalone-functions">
    <xsl:for-each select="$standalone-functions/memberdef[contains(argsstring, 'circular_buffer_space_optimized&lt;')]">
      <xsl:sort select="name"/>
      <xsl:apply-templates select="." mode="synopsis">
        <xsl:with-param name="indent" select="''"/>
        <xsl:with-param name="link-prefix" select="$link-prefix"/>
        <xsl:with-param name="link" select="$standalone-functions/memberdef[contains(argsstring, 'circular_buffer&lt;') and name=current()/name]/@id"/>
      </xsl:apply-templates>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="template-parameters-details"/>

  <xsl:template name="public-types-details">
    <xsl:apply-templates select="sectiondef[@kind='public-type']/memberdef[not(contains(type, 'circular_buffer&lt;'))]" mode="description">
      <xsl:sort select="name"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template name="constructors-details">
    <xsl:apply-templates select="sectiondef[@kind='public-func']/memberdef[type = '']" mode="description">
      <xsl:sort select="name"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template name="member-functions-details">
    <xsl:for-each select="sectiondef[@kind='public-func']/memberdef[type != '']">
      <xsl:sort select="name"/>
      <xsl:variable name="briefdescription" select="normalize-space(briefdescription)"/>
      <xsl:if test="string-length($briefdescription) &gt; 0 and (starts-with($briefdescription, $override-mark) or count(reimplements) = 0)">
        <xsl:apply-templates select="." mode="description"/>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="standalone-functions-details"/>

</xsl:stylesheet>
