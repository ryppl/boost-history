<?xml version="1.0" encoding="utf-8"?>
<!--
   Copyright (c) 2007 Matias Capeletto

   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at
   http://www.boost.org/LICENSE_1_0.txt)
  -->

<!--
   Header stylesheet
  -->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="http://www.w3.org/1999/xhtml"
                version="1.0">

<xsl:import href="relative-href.xsl"/>
<xsl:import href="javascript.xsl"/>
<xsl:import href="navbar.xsl"/>

   <!--
       The style of the html pages
         *website   - like the beta.boost.org
          textlinks - old logo and links style
   -->
   <xsl:param name = "page.style" select = "'website'"/>

   <!-- Chapters and sections select boxes params -->

   <xsl:param name = "grouped.links.js" select = "'grouped_links.js'"/>

   <xsl:param name = "grouped.links.chapters.show" select = "'false'"/>
   <xsl:param name = "grouped.links.chapters.xml"  select = "'boost_libs.xml'"/>
   <xsl:param name = "grouped.links.chapters.url"  select = "''"/>

   <xsl:param name = "grouped.links.sections.show" select = "'false'"/>
   <xsl:param name = "grouped.links.sections.xml"  select = "'sections.xml'"/>
   <xsl:param name = "grouped.links.sections.url"  select = "''"/>

   <!-- Google search box params -->

   <xsl:param name = "search.box.show" select = "'false'"/>
   <xsl:param name = "search.box.cse.definition.src"
     select = "'http://www.boost.org/cse.definition.xml'"/>



   <xsl:template name = "boostbook.header">
      <xsl:param name = "prev" select = "/foo"/>
      <xsl:param name = "next" select = "/foo"/>
      <xsl:param name = "nav.context"/>

      <xsl:variable name = "home" select = "/*[1]"/>
      <xsl:variable name = "up"   select = "parent::*"/>

      <xsl:choose>
      <xsl:when test = "$page.style = 'website'">

      <!-- Include the grouped links java script api -->

      <xsl:if test = "$grouped.links.chapters.show = 'true' or
                      $grouped.links.sections.show = 'true'">
         <xsl:call-template name="include.javascript">
            <xsl:with-param name="url" select="$grouped.links.js"/>
         </xsl:call-template>
      </xsl:if>

      <div id="heading">

         <div id="heading-placard"></div>

         <!-- GroupedLinks selection boxes for All Boost Libraries and
              inner sections -->

         <div class="heading_navigation_box">
         <xsl:if test = "$grouped.links.chapters.show = 'true'">
            <xsl:call-template name = "grouped.links.select.box">
               <xsl:with-param name = "definition.xml"
                               select="$grouped.links.chapters.xml"/>
               <xsl:with-param name = "element.id" select = "'chapters_select_box'"/>
               <xsl:with-param name = "base.url"
                               select="$grouped.links.chapters.url"/>
            </xsl:call-template>
         </xsl:if>
         <xsl:if test = "$grouped.links.sections.show = 'true'">
            <xsl:call-template name = "grouped.links.select.box">
               <xsl:with-param name = "definition.xml"
                               select="$grouped.links.sections.xml"/>
               <xsl:with-param name = "element.id" select = "'sections_select_box'"/>
               <xsl:with-param name = "base.url"
                               select="$grouped.links.sections.url"/>
            </xsl:call-template>
         </xsl:if>
         </div>

         <!-- Google Custom Search linked CSE allows us to customize google
              search pages by means of a definition in xml format. By default a
              general definition is used, but libraries are encouraged to change
              it in order to add a refinement for the libs docs -->

         <div class="heading_search_box">
         <xsl:if test = "$search.box.show = 'true'">
            <xsl:call-template name = "google.searh.box">
               <xsl:with-param name = "search.box.cse.definition"
                               select = "cse.definition.src"/>
            </xsl:call-template>
         </xsl:if>
         </div>

      </div>

      <!-- Navigation -->

      <xsl:call-template name="boost.navbar">
         <xsl:with-param name = "prev" select = "$prev"/>
         <xsl:with-param name = "next" select = "$next"/>
         <xsl:with-param name = "nav.context" select = "$nav.context"/>
      </xsl:call-template>

      </xsl:when>
      <xsl:when test = "$page.style = 'textlinks'">
         <xsl:call-template name="header.textlinks">
            <xsl:with-param name = "prev" select = "$prev"/>
            <xsl:with-param name = "next" select = "$next"/>
            <xsl:with-param name = "nav.context" select = "$nav.context"/>
         </xsl:call-template>
      </xsl:when>
      </xsl:choose>

   </xsl:template>



   <!--
      google.searh.box template

      params:
         cse.definition - absolute web url of the cse definition xml

      effects:
         Insert a linked cse google search box
   -->

   <xsl:template name = "google.searh.box">
      <xsl:param name = "cse.definition"/>

      <form id="cref" action="http://google.com/cse">
         <div> Search Boost </div>
         <div>
         <input type="hidden" name="cref" value="{$cse.definition}" />
         <input class="search_box" type="text" name="q" id="q" size="40"
                maxlength="255" alt="Search Text"/>
         </div>
      </form>
   </xsl:template>

</xsl:stylesheet>
