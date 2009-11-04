<?xml version='1.0'?>
<xsl:stylesheet version="1.0"
       xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<!-- ************************************************************

This stylesheet displays the information in comments.xml as an HTML
table, with anchors for each separate issue of the form US1 or UK234.
It is NOT the production stylesheet for producing the post-meeting
comment-status document.

It takes two parameters: "responsible" selects the "owner" attribute
of the comments and can thus take the values "LWG", "CWG", or "editor";
in addition, the special value "all" is accepted.  The parameter
"open_only" can be either "yes" or "no"; the former value selects only
those comments for which the "disp" attribute is empty.

************************************************************ -->

<xsl:param name="responsible" select="'all'"/>
<xsl:param name="open_only" select="'no'"/>
<xsl:param name="no_issue" select="'no'"/>

<xsl:output method="html" encoding="us-ascii"/>

<xsl:template match="*|@*"><xsl:copy><xsl:apply-templates select="@*|*|text()"/></xsl:copy></xsl:template>

<xsl:template match="/">
 <HTML>
  <HEAD>
   <TITLE>
    C++ CD1
    <xsl:if test="$open_only='yes'">
     Unresolved
    </xsl:if>
    <xsl:choose>
     <xsl:when test="$responsible='LWG'">
      Library
     </xsl:when>
     <xsl:when test="$responsible='CWG'">
      Core Language
     </xsl:when>
     <xsl:when test="$responsible='editor'">
      Editorial
     </xsl:when>
    </xsl:choose>
    Comments
   </TITLE>
  </HEAD>
  <BODY>
    <H1>
     C++ CD1
     <xsl:if test="$open_only='yes'">
      Unresolved
     </xsl:if>
     <xsl:choose>
      <xsl:when test="$responsible='LWG'">
       Library
      </xsl:when>
      <xsl:when test="$responsible='CWG'">
       Core Language
      </xsl:when>
      <xsl:when test="$responsible='editor'">
       Editorial
      </xsl:when>
     </xsl:choose>
     Comment Status
    </H1>
     <p>
     This is an unofficial document used to track work-in-progress. Its contents 
     are subject to change.</p>
     <hr/>
     <xsl:choose>
      <xsl:when test="$responsible='all'">
       <xsl:apply-templates select="document/comment"/>
      </xsl:when>
      <xsl:otherwise>
       <xsl:apply-templates select="document/comment[@owner=$responsible]">
         <xsl:sort select="translate(section,'-',' ')"/>
         <xsl:sort select="para" data-type="number"/>
       </xsl:apply-templates>
     </xsl:otherwise>
     </xsl:choose>
    ---- End ----
  </BODY>
 </HTML>
</xsl:template>

<xsl:template match="comment">

<!-- ************************************************************

The <comment> template adds a row to the table if the comment is
currently unresolved (i.e., the "disp" attribute is empty) or if we
are printing all comments.

************************************************************ -->

 <xsl:if test="(not($open_only='yes') or @disp='')
   and (not($no_issue='yes') or (@issue='' and not(@disp='rejected') and not(@disp='duplicate')))">

 
<!-- ************************************************************

Add an anchor to the ID so we can reference it as a URI.

************************************************************ -->

   <h3>Comment
    <A>
     <xsl:attribute name="NAME">
      <xsl:value-of select="@nb"/><xsl:value-of select="@num"/>
     </xsl:attribute>
     <xsl:value-of select="@nb"/>
     <xsl:value-of select="@num"/>
    </A>
   </h3> 
    

<!-- ************************************************************

If this is a UK comment, add a link to the UK C++ panel's web site for
the additional information there, using the "uknum" attribute.

************************************************************ -->

    <xsl:if test="@nb[.='UK']">
     <b>UKref: </b>
     <A>
      <xsl:attribute name="HREF">
       http://cxxpanel.org.uk/ballotcomment/<xsl:value-of select="@uknum"/>
      </xsl:attribute>
      <xsl:value-of select="@uknum"/>
     </A>
     &#160;
    </xsl:if>
    
    <b>Section: </b>
    <xsl:apply-templates select="section/*|section/text()"/>
    
    <xsl:if test="normalize-space(para)">
     &#182;
     <xsl:apply-templates select="para/*|para/text()"/>
    </xsl:if>&#160;

   <xsl:if test="$responsible='all'">

     <xsl:value-of select="@owner"/>

   </xsl:if>
   
   <b>Type: </b>
    <xsl:value-of select="@type"/>&#160;
    
   <b>Issue: </b>
    <xsl:if test="@issue[.!='']">
      <A>
      <xsl:attribute name="HREF">
       lwg-active.html/#<xsl:value-of select="@issue"/>
      </xsl:attribute>
      <xsl:value-of select="@issue"/>
     </A>
    </xsl:if>
    &#160;
   
   <b>Disposition: </b>
    <xsl:value-of select="@disp"/>&#160;
    
   <p><b>Description</b></p>
    <xsl:apply-templates select="description/*|description/text()"/>
   
   <p><b>Suggestion</b></p>
    <xsl:apply-templates select="suggestion/*|suggestion/text()"/>
    
   <xsl:if test="normalize-space(notes)">   
    <p><b>Notes</b></p>
     <xsl:apply-templates select="notes/*|notes/text()"/>
   </xsl:if>  
   
   <xsl:if test="normalize-space(rationale)">   
    <p><b>Rationale</b></p>
     <xsl:apply-templates select="rationale/*|rationale/text()"/>
   </xsl:if>  

<!-- ************************************************************

If there's a paper associated with the issue, add a reference to it.
(Note that the URI for the paper is assumed to be in the standard format,
and we just use the text from the appropriate character offset within
the URI for the text of the link.)

************************************************************ -->

    <xsl:if test="not(@extdoc='')">
     <xsl:if test="normalize-space(rationale)">
      <P/>
     </xsl:if>
     See paper <A>
      <xsl:attribute name="HREF">
       <xsl:value-of select="@extdoc"/>
      </xsl:attribute>
      N<xsl:value-of select="substring(@extdoc,58,4)"/>
      </A>
    </xsl:if>&#160;

<br/>
&#160;
<hr/>
 
 </xsl:if>
</xsl:template>
</xsl:stylesheet>
