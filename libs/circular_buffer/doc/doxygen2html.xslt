<?xml version='1.0' encoding='iso-8859-1'?>

<!DOCTYPE stylesheet [
  <!ENTITY nbsp "&#160;">
  <!ENTITY space "&#32;">
]>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:param name="container"/>
  <xsl:param name="xmldir"/>
  <xsl:variable name="default-keyword">Default: </xsl:variable>
  <xsl:variable name="doxygen-version">1.4.1</xsl:variable>
  <xsl:variable name="container-ref" select="//compound[name=concat('boost::', $container) and @kind='class']/@refid"/>
  <xsl:variable name="class-file" select="concat($xmldir, '/', $container-ref, '.xml')"/>
  <xsl:variable name="class" select="document($class-file)/doxygen/compounddef[@id = $container-ref]"/>

  <xsl:template match="/">
    <xsl:if test="doxygenindex/@version != $doxygen-version">
      <xsl:message>Warning: The source documentation was generated by the doxygen version <xsl:value-of select="doxygenindex/@version"/> which differs from the reference doxygen version (<xsl:value-of select="$doxygen-version"/>).</xsl:message>
    </xsl:if>
    <html>
      <header><title>Source Code Documentation</title></header>
      <body>
        <xsl:apply-templates select="$class" mode="synopsis"/>
        <xsl:apply-templates select="$class" mode="description"/>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="linebreak">
    <br />
  </xsl:template>

  <xsl:template match="computeroutput">
    <code><xsl:apply-templates/></code>
  </xsl:template>

  <xsl:template match="para">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="ref">
    <xsl:element name="a">
      <xsl:attribute name="href"><xsl:call-template name="reference"/></xsl:attribute>
      <xsl:apply-templates/>
    </xsl:element>
  </xsl:template>

  <xsl:template match="ulink">
    <a href="{@url}"><xsl:apply-templates/></a>
  </xsl:template>

  <xsl:template match="parameteritem">
    <xsl:param name="desc" select="''"/>
    <tr>
      <td valign="top">
        <xsl:if test="$desc != ''">
          <b><xsl:value-of select="$desc"/></b>
        </xsl:if>
      </td>
      <td>
        <i><xsl:apply-templates select="parameternamelist/parametername"/></i>
        <xsl:apply-templates select="parameterdescription"/>
      </td>
    </tr>
    <xsl:apply-templates select="following-sibling[1]"/>
  </xsl:template>

  <xsl:template match="parameterlist[@kind='param']">
    <table id="table_param_desc_{generate-id(.)}">
      <xsl:apply-templates select="parameteritem[1]">
        <xsl:with-param name="desc" select="'Parameters:'"/>
      </xsl:apply-templates>
    </table>
  </xsl:template>

  <xsl:template match="parameterlist[@kind='exception']">
    <table id="table_exception_desc_{generate-id(.)}">
      <xsl:apply-templates select="parameteritem[1]">
        <xsl:with-param name="desc" select="'Throws:'"/>
      </xsl:apply-templates>
    </table>
  </xsl:template>

  <xsl:template match="simplesect[@kind='return']">
    <table id="table_return_desc_{generate-id(.)}">
      <tr><td valign="top"><b>Returns:</b></td><td><xsl:apply-templates/></td></tr>
    </table>
  </xsl:template>

  <xsl:template match="simplesect[@kind='pre']">
    <table id="table_pre_desc_{generate-id(.)}">
      <tr><td valign="top"><b>Precondition:</b></td><td><xsl:apply-templates/></td></tr>
    </table>
  </xsl:template>

  <xsl:template match="simplesect[@kind='post']">
    <table id="table_post_desc_{generate-id(.)}">
      <tr><td valign="top"><b>Postcondition:</b></td><td><xsl:apply-templates/></td></tr>
    </table>
  </xsl:template>

  <xsl:template match="simplesect[@kind='note']">
    <table id="table_note_desc_{generate-id(.)}">
      <tr><td valign="top"><b>Note:</b></td><td><xsl:apply-templates/></td></tr>
    </table>
  </xsl:template>

  <xsl:template match="briefdescription">
    <table id="table_detailed_desc_{generate-id(.)}">
      <tr><td><xsl:apply-templates/></td></tr>
    </table>
  </xsl:template>

  <xsl:template match="detaileddescription/para">
    <xsl:if test="normalize-space(text()) != ''">
      <table id="table_detailed_desc_{generate-id(.)}">
        <tr><td><xsl:apply-templates select="text()"/></td></tr>
      </table>
    </xsl:if>
    <xsl:apply-templates select="*[not(text())]"/>
  </xsl:template>

  <!-- Synopsis mode -->

  <xsl:template match="compounddef[@kind = 'class']" mode="synopsis">
    <div id="srcdoc_synopsis">
      <table id="table_synopsis" border="0" cellpadding="10">
        <tr><td>
<pre>
namespace boost {

template &lt;<xsl:call-template name="template-parameters"/>&gt;
class <xsl:value-of select="$container"/>
{
public:
<xsl:call-template name="public-types"/><xsl:text disable-output-escaping="yes">
</xsl:text>
<xsl:apply-templates select="sectiondef[@kind='public-func']/memberdef[type = '']" mode="synopsis">
  <xsl:sort select="name"/>
</xsl:apply-templates><xsl:text disable-output-escaping="yes">
</xsl:text>
<xsl:call-template name="member-functions"/><xsl:text disable-output-escaping="yes">};

</xsl:text>
<xsl:call-template name="standalone-functions"/>
} // namespace boost
</pre>
        </td></tr>
      </table>
    </div>
  </xsl:template>

  <xsl:template match="param" mode="synopsis">
    <xsl:param name="link-prefix" select="''"/>
    <xsl:value-of select="type"/>&nbsp;<a href="{$link-prefix}#templateparam_{declname}"><xsl:value-of select="declname"/></a><xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/>
  </xsl:template>

  <xsl:template match="memberdef[@kind='typedef']" mode="synopsis">
    <xsl:param name="link-prefix" select="''"/>
    <xsl:if test="normalize-space(briefdescription) != ''">&nbsp;&nbsp;&nbsp;typedef&nbsp;<xsl:value-of select="substring('typename ', 1 div (contains(type, '::') and not(contains(type, '&gt;'))))"/>
      <xsl:choose>
        <xsl:when test="contains(type, '&gt;')"><i>implementation-defined</i>&nbsp;</xsl:when>
        <xsl:otherwise><xsl:value-of select="type"/>&nbsp;</xsl:otherwise>
      </xsl:choose>
      <a href="{$link-prefix}#{@id}"><xsl:value-of select="name"/></a>;<xsl:text disable-output-escaping="yes">
</xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template match="memberdef[@kind='function']" mode="synopsis">
    <xsl:param name="indent" select="'&nbsp;&nbsp;&nbsp;'"/>
    <xsl:param name="link-prefix" select="''"/>
    <xsl:param name="link" select="''"/>
    <xsl:variable name="too-long-args" select="string-length(argsstring) &gt; 80"/>
    <xsl:value-of select="$indent"/>
    <xsl:value-of select="substring('explicit ', 1 div (@explicit = 'yes'))"/>
    <xsl:if test="count(templateparamlist) &gt; 0">template&nbsp;&lt;<xsl:for-each select="templateparamlist/param"><xsl:value-of select="type"/>&nbsp;<xsl:value-of select="declname"/><xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/></xsl:for-each>&gt;<xsl:text disable-output-escaping="yes">
&nbsp;&nbsp;&nbsp;</xsl:text><xsl:value-of select="$indent"/>
    </xsl:if>
    <xsl:if test="type != ''"><xsl:apply-templates select="type" mode="synopsis"/>&nbsp;</xsl:if>
    <a href="{$link-prefix}#{concat(substring(@id, 1 div (string-length($link) = 0)), substring($link, 1 div (string-length($link) &gt; 0)))}">
      <xsl:value-of select="name"/>
    </a>(<xsl:for-each select="param">
    <xsl:if test="$too-long-args and not((count(preceding-sibling::param) + 1) mod 2)">
      <xsl:text disable-output-escaping="yes">
</xsl:text><xsl:value-of select="$indent"/>&nbsp;&nbsp;&nbsp;<xsl:text/>
      <xsl:if test="count(../templateparamlist) &gt; 0">&nbsp;&nbsp;&nbsp;</xsl:if>
    </xsl:if>
    <xsl:apply-templates select="type" mode="synopsis"/>&nbsp;<xsl:value-of select="declname"/>
    <xsl:value-of select="substring(concat(' = ', defval), 1 div (normalize-space(defval) != ''))"/>
    <xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/>
    </xsl:for-each>)<xsl:value-of select="substring(' const', 1 div (@const = 'yes'))"/>;<xsl:text disable-output-escaping="yes">
</xsl:text>
  </xsl:template>

  <xsl:template match="type" mode="synopsis">
    <xsl:for-each select="text() | ref">
      <xsl:variable name="item" select="translate(., '&space;', '')"/>
      <xsl:choose>
        <xsl:when test="$item = 'return_value_type' or $item = 'param_value_type'">value_type</xsl:when>
        <xsl:otherwise><xsl:value-of select="$item"/></xsl:otherwise>
      </xsl:choose>
      <xsl:if test="name(.) != 'ref' and position() != last()">&nbsp;</xsl:if>
    </xsl:for-each>
  </xsl:template>

  <!-- Description mode -->

  <xsl:template match="compounddef[@kind = 'class']" mode="description">
    <div id="srcdoc_params">
      <table id="table_template_params" border="1">
        <tr><th>Parameter</th><th>Description</th><th>Default</th></tr>
        <xsl:call-template name="template-parameters-details"/>
      </table>
    </div>
    <div id="srcdoc_types">
      <table id="table_public_types" border="1">
        <tr><th>Type</th><th>Description</th></tr>
        <xsl:call-template name="public-types-details"/>
      </table>
    </div>
    <div id="srcdoc_constructors">
      <table id="table_constructors" border="1">
        <xsl:call-template name="constructors-details"/>
      </table>
    </div>
    <div id="srcdoc_methods">
      <table id="table_methods" border="1">
        <xsl:call-template name="member-functions-details"/>
      </table>
    </div>
    <div id="srcdoc_functions">
      <table id="table_functions" border="1">
        <xsl:call-template name="standalone-functions-details"/>
      </table>
    </div>
  </xsl:template>

  <xsl:template match="parameteritem" mode="description">
    <tr><td><a name="templateparam_{parameternamelist/parametername}"><code><xsl:value-of select="parameternamelist/parametername"/></code></a></td>
    <xsl:choose>
      <xsl:when test="contains(parameterdescription, $default-keyword)">
        <td><xsl:value-of select="substring-before(parameterdescription, $default-keyword)"/></td>
        <td><code><xsl:value-of select="substring-after(parameterdescription, $default-keyword)"/></code></td>
      </xsl:when>
      <xsl:otherwise>
        <td><xsl:value-of select="parameterdescription"/></td>
        <td>&nbsp;</td>
      </xsl:otherwise>
    </xsl:choose></tr>
  </xsl:template>

  <xsl:template match="memberdef[@kind='typedef']" mode="description">
    <xsl:if test="normalize-space(briefdescription) != ''">
      <tr><td><a name="{@id}"><code><xsl:value-of select="name"/></code></a></td>
      <td><xsl:value-of select="briefdescription"/></td></tr>
    </xsl:if>
  </xsl:template>

  <xsl:template match="memberdef[@kind='function']" mode="description">
    <xsl:param name="link-prefix" select="''"/>
    <xsl:variable name="too-long-args" select="string-length(argsstring) &gt; 80"/>
    <tr><td><a name="{@id}" /><code><b><xsl:value-of select="substring('explicit ', 1 div (@explicit = 'yes'))"/>
    <xsl:if test="count(templateparamlist) &gt; 0">
      template&nbsp;&lt;<xsl:for-each select="templateparamlist/param"><xsl:value-of select="type"/>&nbsp;<xsl:value-of select="declname"/><xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/></xsl:for-each>&gt;<br />&nbsp;&nbsp;&nbsp;
    </xsl:if>
    <xsl:if test="type != ''"><xsl:apply-templates select="type" mode="description"/>&nbsp;</xsl:if>
    <xsl:value-of select="name"/>(<xsl:for-each select="param">
    <xsl:if test="$too-long-args and not((count(preceding-sibling::param) + 1) mod 2)">
      <br />&nbsp;&nbsp;&nbsp;
      <xsl:if test="count(../templateparamlist) &gt; 0">&nbsp;&nbsp;&nbsp;</xsl:if>
    </xsl:if>
    <xsl:apply-templates select="type" mode="description"/>&nbsp;<xsl:value-of select="declname"/>
    <xsl:value-of select="substring(concat(' = ', defval), 1 div (normalize-space(defval) != ''))"/>
    <xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/>
    </xsl:for-each>)<xsl:value-of select="substring(' const', 1 div (@const = 'yes'))"/>;</b></code><br />
    <table id="table_function_desc_{generate-id(.)}">
      <tr><td>&nbsp;&nbsp;&nbsp;</td><td>
        <xsl:apply-templates select="briefdescription"/>
        <xsl:apply-templates select="detaileddescription"/>
      </td></tr>
    </table>
    </td></tr>
  </xsl:template>

  <xsl:template match="type" mode="description">
    <xsl:for-each select="text() | ref">
      <xsl:variable name="item" select="translate(., '&space;', '')"/>
      <xsl:choose>
        <xsl:when test="$item = 'return_value_type' or $item = 'param_value_type'"><a href="{$link-prefix}#{$class[@kind = 'class']/sectiondef[@kind='public-type']/memberdef[@kind='typedef' and name='value_type']/@id}">value_type</a></xsl:when>
        <xsl:when test="contains($item, 'circular_buffer')"><xsl:value-of select="$item"/></xsl:when>
        <xsl:when test="@refid">
          <xsl:element name="a">
            <xsl:attribute name="href"><xsl:call-template name="reference"/></xsl:attribute>
            <xsl:value-of select="$item"/>
          </xsl:element>
        </xsl:when>
        <xsl:otherwise><xsl:value-of select="$item"/></xsl:otherwise>
      </xsl:choose>
      <xsl:if test="name(.) != 'ref' and position() != last()">&nbsp;</xsl:if>
    </xsl:for-each>
  </xsl:template>

</xsl:stylesheet>
