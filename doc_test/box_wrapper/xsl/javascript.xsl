<?xml version="1.0" encoding="utf-8"?>
<!--
   Copyright (c) 2007 Matias Capeletto

   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at
   http://www.boost.org/LICENSE_1_0.txt)
  -->

<!--
   Templates that allows the use javascript
   Support for GroupedLinks select boxes  
  -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="http://www.w3.org/1999/xhtml"
                version="1.0">

<xsl:import href="relative-href.xsl"/>

   <!--
      include.javascript template

      Params:
         url - path to the js file (it can be relative)

      Effects:
         Include a js file in a page
   -->

   <xsl:template name = "include.javascript">
      <xsl:param name = "url"/>

      <script type="text/javascript">
         <xsl:attribute name = "src">
            <xsl:call-template name = "href.target.relative">
               <xsl:with-param name = "target" select = "$url"/>
            </xsl:call-template>
         </xsl:attribute>
      </script>
   </xsl:template>

   <!--
      grouped.links.select.box template

      Params:
         definition.xml - path to the xml definition file (it can be relative)
         form.id - string identifier, it must be unique in the page
         form.id - string identifier, it must be unique in the page

      Effects:
         Insert a GropedLinks select box

      Requires:
         grouped_links.js must be included in the page
   -->

   <xsl:template name = "grouped.links.select.box">
      <xsl:param name = "definition.xml"/>
      <xsl:param name = "element.id"/>
      <xsl:param name = "base.url"/>

      <div class="grouped_links">
         <xsl:attribute name = "id">
            <xsl:value-of select="$element.id"/>
         </xsl:attribute>
         <script type="text/javascript">
            grouped_links_select_box(
               '<xsl:call-template name="href.target.relative">
                <xsl:with-param name="target"
                                select="$definition.xml"/>
                </xsl:call-template>',
                '<xsl:value-of select="$element.id"/>',
                '<xsl:call-template name="href.target.relative">
                    <xsl:with-param name="target"
                                    select="$base.url"/>
                   </xsl:call-template>');
         </script>
      </div>
   </xsl:template>

</xsl:stylesheet>