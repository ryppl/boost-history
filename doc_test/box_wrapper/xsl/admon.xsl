<?xml version="1.0" encoding="utf-8"?>
<!--
   Copyright (c) 2002 Douglas Gregor <doug.gregor -at- gmail.com>
  
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at
   http://www.boost.org/LICENSE_1_0.txt)
  -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="http://www.w3.org/1999/xhtml"
                version="1.0">

<!-- Not needed?

xsl:import href="http://docbook.sourceforge.net/release/xsl/current/html/admon.xsl"/

-->

<xsl:import href="relative-href.xsl"/>


   <xsl:template name="admon.graphic.icon">
      <xsl:param name="node" select="."/>

      <xsl:variable name="admon.icon.type">
         <xsl:choose>
            <xsl:when test="local-name($node)='note'">note</xsl:when>
            <xsl:when test="local-name($node)='warning'">warning</xsl:when>
            <xsl:when test="local-name($node)='caution'">caution</xsl:when>
            <xsl:when test="local-name($node)='tip'">tip</xsl:when>
            <xsl:when test="local-name($node)='important'">important</xsl:when>
            <xsl:otherwise>note</xsl:otherwise>
         </xsl:choose>
      </xsl:variable>

      <div class="admonition-icon">
          <div class="{$admon.icon.type}-icon" />
      </div>

   </xsl:template>



   <!-- overwrites docbook graphical.admonition -->

   <xsl:template name="graphical.admonition">

      <xsl:variable name="admon.type">
         <xsl:choose>
            <xsl:when test="local-name(.)='note'">Note</xsl:when>
            <xsl:when test="local-name(.)='warning'">Warning</xsl:when>
            <xsl:when test="local-name(.)='caution'">Caution</xsl:when>
            <xsl:when test="local-name(.)='tip'">Tip</xsl:when>
            <xsl:when test="local-name(.)='important'">Important</xsl:when>
            <xsl:otherwise>Note</xsl:otherwise>
         </xsl:choose>
      </xsl:variable>

      <xsl:variable name="alt">
         <xsl:call-template name="gentext">
            <xsl:with-param name="key" select="$admon.type"/>
         </xsl:call-template>
      </xsl:variable>

      <div class="{name(.)}">

         <!-- top box wrapper -->
         <div class="box-outer-wrapper">
         <div class="box-top-left" />
         <div class="box-top-right" />
         <div class="box-top" />
         <div class="box-inner-wrapper">
         <!-- top box wrapper -->

         <div class="admonition-graphic">
            <xsl:call-template name="admon.graphic.icon"/>
         </div>
         <div class="admonition-body">
         <div class="admonition-title">
            <xsl:call-template name="anchor"/>
            <xsl:if test="$admon.textlabel != 0 or title">
               <xsl:apply-templates select="." mode="object.title.markup"/>
            </xsl:if>
         </div>
         <div class="admonition-content">
            <xsl:apply-templates/>
         </div>
         </div>

         <!-- bottom box wrapper -->
         </div>
         <div class="box-bottom-left" />
         <div class="box-bottom-right" />
         <div class="box-bottom" />
         </div>
         <!-- bottom box wrapper -->

      </div>

   </xsl:template>


</xsl:stylesheet>
