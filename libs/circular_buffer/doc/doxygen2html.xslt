<?xml version='1.0' encoding='iso-8859-1'?>

<!DOCTYPE stylesheet [
  <!ENTITY nbsp "&#160;">
]>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  
  <xsl:param name="container"/>
  <xsl:param name="xmldir"/>
  <xsl:variable name="doxygen-version">1.4.1</xsl:variable>
  
  <xsl:template match="/">
    <xsl:if test="doxygenindex/@version != $doxygen-version">
      <xsl:message>Warning: The source documentation was generated by the doxygen version <xsl:value-of select="doxygenindex/@version"/> which differs from the reference doxygen version (<xsl:value-of select="$doxygen-version"/>).</xsl:message>
    </xsl:if>
    <html>
      <header><title>Source Code Documentation</title></header>
      <body>
        <xsl:variable name="container-ref" select="//compound[name=concat('boost::', $container) and @kind='class']/@refid"/>
        <xsl:variable name="class-file" select="concat($xmldir, '/', $container-ref, '.xml')"/>
        <xsl:variable name="class" select="document($class-file)/doxygen/compounddef[@id = $container-ref]"/>
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
    <a href="#{@refid}"><xsl:apply-templates/></a>
  </xsl:template>
  
  <xsl:template match="ulink">
    <a href="{@url}"><xsl:apply-templates/></a>
  </xsl:template>
  
  <xsl:template match="parametername">
    <dd><i><xsl:apply-templates/></i>
    <xsl:apply-templates select="following::parameterdescription[1]"/></dd>
  </xsl:template>
  
  <xsl:template match="parameterlist[@kind='param']">
    <dl><dt><b>Parameters:</b></dt><xsl:apply-templates select="parametername"/></dl>
  </xsl:template>
  
  <xsl:template match="parameterlist[@kind='exception']">
    <dl><dt><b>Throws:</b></dt><xsl:apply-templates select="parametername"/></dl>
  </xsl:template>
  
  <xsl:template match="simplesect[@kind='return']">
    <dl><dt><b>Returns:</b></dt><dd><xsl:apply-templates/></dd></dl>
  </xsl:template>
  
  <xsl:template match="simplesect[@kind='pre']">
    <dl><dt><b>Precondition:</b></dt><dd><xsl:apply-templates/></dd></dl>
  </xsl:template>
  
  <xsl:template match="simplesect[@kind='post']">
    <dl><dt><b>Postcondition:</b></dt><dd><xsl:apply-templates/></dd></dl>
  </xsl:template>
  
  <xsl:template match="simplesect[@kind='note']">
    <dl><dt><b>Note:</b></dt><dd><xsl:apply-templates/></dd></dl>
  </xsl:template>
  
  <!-- Synopsis mode -->
    
  <xsl:template match="compounddef[@kind = 'class']" mode="synopsis">
    <div id="srcdoc_synopsis">
    <table border="0" cellpadding="5">
      <tr><td></td><td>
<pre>
namespace boost {

template &lt;<xsl:for-each select="templateparamlist/param"><xsl:value-of select="type"/>&nbsp;<xsl:value-of select="declname"/><xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/></xsl:for-each>&gt;
class <xsl:value-of select="$container"/>
{
public:
<xsl:apply-templates select="sectiondef[@kind='public-type']/memberdef" mode="synopsis"/>
<xsl:apply-templates select="sectiondef[@kind='public-func']/memberdef[type = '']" mode="synopsis">
    <xsl:sort select="name"/>
</xsl:apply-templates>
<br />
<xsl:apply-templates select="sectiondef[@kind='public-func']/memberdef[type != '']" mode="synopsis">
    <xsl:sort select="name"/>
</xsl:apply-templates>
};
<xsl:call-template name="standalone_functions"/>
} // namespace boost
</pre>
        </td></tr>
      </table>
    </div>
  </xsl:template>
  
  <xsl:template match="memberdef[@kind='typedef']" mode="synopsis">
    <xsl:if test="normalize-space(briefdescription) != ''">
        <xsl:text disable-output-escaping="yes">   typedef </xsl:text><xsl:value-of select="substring('typename ', 1 div (contains(type, '::') and not(contains(type, '&gt;'))))"/>
        <xsl:choose>
            <xsl:when test="contains(type, '&gt;')"><i>implementation-defined</i> </xsl:when>
            <xsl:otherwise><xsl:value-of select="type"/></xsl:otherwise>
        </xsl:choose>
        <xsl:text disable-output-escaping="yes"> </xsl:text>
        <a href="#{@id}"><xsl:value-of select="name"/></a><xsl:text>;
</xsl:text>
</xsl:if>
  </xsl:template>
  
  <xsl:template match="memberdef[@kind='function']" mode="synopsis">
    <xsl:variable name="long-args" select="string-length(argsstring) &gt; 70"/>
    <xsl:text disable-output-escaping="yes">
   </xsl:text>
    <xsl:value-of select="substring('explicit ', 1 div (@explicit = 'yes'))"/>
    <xsl:if test="count(templateparamlist) &gt; 0">
        <xsl:text disable-output-escaping="yes">template </xsl:text>&lt;<xsl:for-each select="templateparamlist/param"><xsl:value-of select="type"/>&nbsp;<xsl:value-of select="declname"/><xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/></xsl:for-each>&gt;<xsl:text disable-output-escaping="yes">
      </xsl:text>
    </xsl:if>
    <xsl:value-of select="substring(concat(type, ' '), 1 div (type != ''))"/>
    <a href="#{@id}">
        <xsl:value-of select="name"/>
    </a>(<xsl:for-each select="param">
        <xsl:if test="not((count(preceding-sibling::param) + 1) mod 3) and $long-args">
        <xsl:text disable-output-escaping="yes">
      </xsl:text>
            <xsl:if test="count(../templateparamlist) &gt; 0">
                <xsl:text disable-output-escaping="yes">   </xsl:text>
            </xsl:if>
        </xsl:if>
        <xsl:apply-templates select="type" mode="synopsis"/>
        <xsl:text disable-output-escaping="yes"> </xsl:text>
        <xsl:value-of select="declname"/>
        <xsl:value-of select="substring(concat(' = ', defval), 1 div (normalize-space(defval) != ''))"/>
        <xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/>
    </xsl:for-each>)<xsl:value-of select="substring(' const', 1 div (@const = 'yes'))"/>;<xsl:text></xsl:text>
  </xsl:template>

  <xsl:template match="type" mode="synopsis">
    <xsl:for-each select="text() | ref">
        <xsl:choose>
        <xsl:when test="name(.) = 'ref'">
            <xsl:value-of select="."/>
        </xsl:when>
        <xsl:otherwise>
            <xsl:choose>
                <xsl:when test=". = 'return_value_type' or . = 'param_value_type'">
                    <xsl:text>value_type</xsl:text>
                </xsl:when>
                <xsl:when test=". = ' &amp;'">
                    <xsl:text>&amp;</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="."/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:otherwise>
    </xsl:choose>
    </xsl:for-each>
  </xsl:template>
  
  <!-- Description mode -->
  
  <xsl:template match="compounddef[@kind = 'class']" mode="description">
    <hr align="left" size="1" />
    <h3><a name="consructors">Constructors/Desctructor</a></h3>
    <xsl:apply-templates select="sectiondef[@kind='public-func']/memberdef[type = '']" mode="description">
        <xsl:sort select="name"/>
    </xsl:apply-templates>
    <h3><a name="typedefs">Typesdefs</a></h3>
    <xsl:apply-templates select="sectiondef[@kind='public-type']/memberdef" mode="description">
      <xsl:sort select="name"/>
    </xsl:apply-templates>
    <h3><a name="members">Members</a></h3>
    <xsl:apply-templates select="sectiondef[@kind='public-func']/memberdef[type != '']" mode="description">
      <xsl:sort select="name"/>
    </xsl:apply-templates>
  </xsl:template>
  
  <xsl:template match="memberdef[@kind='function' or @kind='typedef']" mode="description">
    <a name="{@id}" />
    <b><pre>&nbsp;<xsl:value-of select="substring('explicit ', 1 div (@explicit = 'yes'))"/>
      <xsl:if test="count(templateparamlist) &gt; 0">
          <xsl:text disable-output-escaping="yes">template </xsl:text>&lt;<xsl:for-each select="templateparamlist/param"><xsl:value-of select="type"/>&nbsp;<xsl:value-of select="declname"/><xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/></xsl:for-each>&gt;
      </xsl:if>
      <xsl:apply-templates select="type" mode="description"/>
      <xsl:value-of select="substring(' ', 1 div (normalize-space(type) != ''))"/>
          <xsl:value-of select="name"/>(<xsl:for-each select="param">
          <xsl:apply-templates select="type" mode="description"/>&nbsp;<xsl:value-of select="declname"/>
          <xsl:value-of select="substring(concat(' = ', defval), 1 div (normalize-space(defval) != ''))"/>
          <xsl:value-of select="substring(', ', 1 div (count(following-sibling::param) != 0))"/>
      </xsl:for-each>)<xsl:value-of select="substring(' const', 1 div (@const = 'yes'))"/>;</pre></b>
    <dl>
      <dd>
        <xsl:apply-templates select="briefdescription"/>
        <xsl:apply-templates select="detaileddescription"/>
      </dd>
    </dl>
    <hr align="left" size="1" />
  </xsl:template>

  <xsl:template match="type" mode="description">
    <xsl:for-each select="text() | ref">
        <xsl:choose>
        <xsl:when test="name(.) = 'ref'">
            <a href="#{@refid}"><xsl:value-of select="."/></a>
        </xsl:when>
        <xsl:otherwise>
            <xsl:choose>
                <xsl:when test=". = 'return_value_type' or . = 'param_value_type'">
                    <a href="#{../../../../sectiondef[@kind='public-type']/memberdef[name = 'value_type']/@id}">value_type</a>
                </xsl:when>
                <xsl:when test=". = ' &amp;'">
                    <xsl:text>&amp;</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="."/>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:otherwise>
    </xsl:choose>
    </xsl:for-each>
  </xsl:template>
  
  <!-- To be overwritten -->
  
  <xsl:template name="standalone_functions">
  </xsl:template>
  
</xsl:stylesheet>
