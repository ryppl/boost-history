<?xml version='1.0'?>
<xsl:stylesheet version="1.0"
       xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<!-- ************************************************************

This stylesheet displays the information in comments.xml as an HTML
table, with anchors for each separate issue of the form US1 or UK234.
It is NOT the production stylesheet for producing the post-meeting
comment-status document.

It takes three parameters: "responsible" selects the "owner" attribute
of the comments and can thus take the values "LWG", "CWG", or "editor";
in addition, the special value "all" is accepted.  The parameter
"open_only" can be either "yes" or "no"; the former value selects only
those comments for which the "disp" attribute is empty.
The parameter "brief" can be either "yes" or "no"; if "yes" the Description
and Suggested Change columns are eliminated.

************************************************************ -->

<xsl:param name="responsible" select="'all'"/>
<xsl:param name="open_only" select="'no'"/>
<xsl:param name="brief" select="'no'"/>

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
   <CENTER>
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
     Comments
    </H1>
    <H2>
     (Rev.
     <xsl:value-of select="/document/@rev"/>,
     <xsl:value-of select="/document/@date"/>)
    </H2>
    <H3>
     William M. Miller<BR/>
     Edison Design Group, Inc.<BR/>
     <A HREF="mailto://wmm@edg.com">wmm@edg.com</A>
    </H3>
   </CENTER>
   <TABLE border="1" cellspacing="0" cellpadding="4" frame="box">
    <THEAD>
     <TR>
      <xsl:choose>

<!-- ************************************************************

The "all" version of the table contains columns for both the owner and
the <notes> element, currently used almost exclusively in LWG comments.

************************************************************ -->

       <xsl:when test="$responsible='all'">
        <TH width="4%" align="center">ID</TH>
        <TH width="6%" align="center">Ref</TH>
        <TH width="6%" align="center">Owner</TH>
        <TH width="6%" align="center">Disposition</TH>
        <TH width="19%" align="center">Description</TH>
        <TH width="19%" align="center">Suggested Change</TH>
        <TH width="4%" align="center">Issue</TH>
        <TH width="18%" align="center">Notes</TH>
        <TH width="18%" align="center">Rationale</TH>
       </xsl:when>

<!-- ************************************************************

The "LWG" version of the table contains a column for the <notes>
element but not for the owner.

************************************************************ -->

       <xsl:when test="$responsible='LWG'">
        <TH align="center">ID</TH>
        <TH align="center">Ref</TH>
        <TH align="center">Disposition</TH>
        <xsl:if test="$brief='no'">
         <TH align="center">Description</TH>
         <TH align="center">Suggested Change</TH>
        </xsl:if>
        <TH align="center">Issue</TH>
        <TH align="center">Notes</TH>
        <TH width="25%" align="center">Rationale</TH>
       </xsl:when>

<!-- ************************************************************

The "CWG" and "editor" versions of the table contain neither owner nor
<notes> information.

************************************************************ -->

       <xsl:otherwise>
        <TH width="4%" align="center">ID</TH>
        <TH width="6%" align="center">Ref</TH>
        <TH width="6%" align="center">Disposition</TH>
        <TH width="30%" align="center">Description</TH>
        <TH width="30%" align="center">Suggested Change</TH>
        <TH width="4%" align="center">Issue</TH>
        <TH width="30%" align="center">Rationale</TH>
       </xsl:otherwise>
      </xsl:choose>
     </TR>
    </THEAD>
    <TBODY>
     <xsl:choose>
      <xsl:when test="$responsible='all'">
       <xsl:apply-templates select="document/comment"/>
      </xsl:when>
      <xsl:otherwise>
       <xsl:apply-templates select="document/comment[@owner=$responsible]"/>
      </xsl:otherwise>
     </xsl:choose>
    </TBODY>
   </TABLE>
  </BODY>
 </HTML>
</xsl:template>

<xsl:template match="comment">

<!-- ************************************************************

The <comment> template adds a row to the table if the comment is
currently unresolved (i.e., the "disp" attribute is empty) or if we
are printing all comments.

************************************************************ -->

 <xsl:if test="not($open_only='yes') or @disp=''">
  <TR>
   <TD valign="top">

<!-- ************************************************************

Add an anchor to the ID so we can reference it as a URI.

************************************************************ -->

    <A>
     <xsl:attribute name="NAME">
      <xsl:value-of select="@nb"/><xsl:value-of select="@num"/>
     </xsl:attribute>
     <xsl:value-of select="@nb"/>
     <xsl:text> </xsl:text>
     <xsl:value-of select="@num"/>
    </A>

<!-- ************************************************************

If this is a UK comment, add a link to the UK C++ panel's web site for
the additional information there, using the "uknum" attribute.

************************************************************ -->

    <xsl:if test="@nb[.='UK']">
     <BR/>
     <A>
      <xsl:attribute name="HREF">
       http://cxxpanel.org.uk/ballotcomment/<xsl:value-of select="@uknum"/>
      </xsl:attribute>
      [<xsl:value-of select="@uknum"/>]
     </A>
    </xsl:if>
   </TD>
   <TD valign="top">
    <xsl:apply-templates select="section/*|section/text()"/>
    <xsl:if test="normalize-space(para)">
     &#182;
     <xsl:apply-templates select="para/*|para/text()"/>
    </xsl:if>&#160;
   </TD>
   <xsl:if test="$responsible='all'">
    <TD valign="top">
     <xsl:value-of select="@owner"/>&#160;
    </TD>
   </xsl:if>
   <TD valign="top">
    <xsl:value-of select="@disp"/>&#160;
   </TD>
   <xsl:if test="$brief='no'">
     <TD valign="top">
      <xsl:apply-templates select="description/*|description/text()"/>&#160;
     </TD>
     <TD valign="top">
      <xsl:apply-templates select="suggestion/*|suggestion/text()"/>&#160;
     </TD>
   </xsl:if>  
   <TD valign="top">
    &#160;
    <xsl:if test="@issue[.!='']">
      <A>
      <xsl:attribute name="HREF">
       lwg-active.html/#<xsl:value-of select="@issue"/>
      </xsl:attribute>
      <xsl:value-of select="@issue"/>
     </A>
    </xsl:if>
   </TD>
   <xsl:if test="$responsible='all' or $responsible='LWG'">
    <TD valign="top">
     <xsl:apply-templates select="notes/*|notes/text()"/>&#160;
    </TD>
   </xsl:if>
   <TD valign="top">
    <xsl:apply-templates select="rationale/*|rationale/text()"/>

<!-- ************************************************************

If there's a paper associated with the issue, add a reference to it.
(Note that the URI for the paper is assumed to be in the standard format,
and we just use the text from the appropriate character offset within
the URI for the text of the link.)

************************************************************ -->

    <xsl:if test="not(@extdoc='')">
     <xsl:if test="normalize-space(rationale)">
      <BR/><BR/>
     </xsl:if>
     See paper <A>
      <xsl:attribute name="HREF">
       <xsl:value-of select="@extdoc"/>
      </xsl:attribute>
      N<xsl:value-of select="substring(@extdoc,58,4)"/>
      </A>
    </xsl:if>&#160;
   </TD>
  </TR>
 </xsl:if>
</xsl:template>
</xsl:stylesheet>
