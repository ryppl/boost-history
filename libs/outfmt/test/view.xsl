<?xml version = "1.0"?>

<xsl:stylesheet version = "1.0" xmlns:xsl = "http://www.w3.org/1999/XSL/Transform">
   <xsl:output method = "html"/>

   <xsl:template match = "data">
      <html><head>
         <title>Regression Test Results</title>
         <style>
            td{ vertical-align: top; }
            th{ background-color: #BBB; }
            a{  color: blue; }

            .table2{ background-color: lightskyblue; }

            .passed{      color: black;  font-weight: bold; }
            .failed{      color: red;    font-weight: bold; }
            .unsupported{ color: maroon; font-weight: bold; }

            .results{ color: orange;  font-weight: bold; }
            .total{   color: maroon;  font-weight: bold; }
            .expect{  color: #636;    }
            .write{   color: maroon;  }
            .read{    color: navy;    }
         </style>
      </head><body>
         <table width = "100%" style = "margin-bottom: 1pc;">
            <tr>
               <th rowspan = "2">Compiler </th>
               <th rowspan = "2">Standard Library </th>
               <th rowspan = "2">Platform </th>
               <th rowspan = "2">Boost Version </th>
               <th rowspan = "2">Run On Date </th>
               <th colspan = "2">read </th>
               <th colspan = "2">write </th>
            </tr><tr>
               <th>Pass </th><th>Fail </th>
               <th>Pass </th><th>Fail </th>
            </tr>
            <xsl:apply-templates/>
         </table>

         <xsl:apply-templates mode = "detail"/>
      </body></html>
   </xsl:template>

   <xsl:template match = "test-set">
      <tr>
         <td><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <a href = "#{generate-id(.)}"><xsl:value-of select = "@compiler"/></a>
         </td><td><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <xsl:value-of select = "@stdlib"  />
         </td><td align = "center"><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <xsl:value-of select = "@platform"/>
         </td><td align = "center"><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <xsl:value-of select = "
               round(@boostver div 100000)
            "/>.<xsl:value-of select = "
               round((@boostver div 100) mod 1000)
            "/>.<xsl:value-of select = "
               @boostver mod 100
            "/>
         </td><td align = "center"><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <xsl:value-of select = "group[1]/run[last()]/@date"/>
         </td><td><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <div class = "passed"><xsl:value-of select = "round(
               count(group/run[last()]/test[@read = 'passed']) div
               count(group/run[last()]/test[not(@read = 'unsupported')]) *
               100
            )"/>%</div>
            <div class = "passed" style = "font-weight: normal;">(<xsl:value-of select = "
               count(group/run[last()]/test[@read = 'unsupported'])
            "/> unsupported)</div>
         </td><td><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <div class = "failed"><xsl:value-of select = "round(
               count(group/run[last()]/test[@read = 'failed']) div
               count(group/run[last()]/test[not(@read = 'unsupported')]) *
               100
            )"/>%</div>
            <div class = "failed" style = "font-weight: normal;">(<xsl:value-of select = "
               count(group/run[last()]/test[@read = 'failed'])
            "/>)</div>
         </td><td><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <div class = "passed"><xsl:value-of select = "round(
               count(group/run[last()]/test[@write = 'passed']) div
               count(group/run[last()]/test[not(@write = 'unsupported')]) *
               100
            )"/>%</div>
            <div class = "passed" style = "font-weight: normal;">(<xsl:value-of select = "
               count(group/run[last()]/test[@write = 'unsupported'])
            "/> unsupported)</div>
         </td><td><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <div class = "failed"><xsl:value-of select = "round(
               count(group/run[last()]/test[@write = 'failed']) div
               count(group/run[last()]/test[not(@write = 'unsupported')]) *
               100
            )"/>%</div>
            <div class = "failed" style = "font-weight: normal;">(<xsl:value-of select = "
               count(group/run[last()]/test[@write = 'failed'])
            "/>)</div>
         </td>
      </tr>
   </xsl:template>

   <xsl:template match = "test-set" mode = "detail">
      <hr/>
      <a name = "#{generate-id(.)}"/><table width = "100%">
         <tr><td align = "right" width = "50%">Compiler:         </td><td><xsl:value-of select = "@compiler"/> </td></tr>
         <tr><td align = "right" width = "50%">Standard Library: </td><td><xsl:value-of select = "@stdlib"  /> </td></tr>
         <tr><td align = "right" width = "50%">Platform:         </td><td><xsl:value-of select = "@platform"/> </td></tr>
         <tr><td align = "right" width = "50%">Boost Version:    </td>
            <td><xsl:value-of select = "
               round(@boostver div 100000)
            "/>.<xsl:value-of select = "
               round((@boostver div 100) mod 1000)
            "/>.<xsl:value-of select = "
               @boostver mod 100
            "/></td>
         </tr>
         <tr><td colspan = "2">
            <table width = "100%">
               <tr><th>Group </th><th>Name </th><th>Read </th><th>Write </th><th>Results </th></tr>
               <xsl:apply-templates select = "group/run[last()]/test"/>
            </table>
         </td></tr>
      </table>
   </xsl:template>

   <xsl:template match = "test">
      <tr>
         <td><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <xsl:value-of select = "../../@name" />
         </td><td><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <xsl:choose>
               <xsl:when test = "@name"><xsl:value-of select = "@name"/></xsl:when>
               <xsl:otherwise>&#xA0;</xsl:otherwise>
            </xsl:choose>
         </td><td align = "center"><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <span class = "{@read}"><xsl:value-of select = "@read"/></span>
         </td><td align = "center"><xsl:if test = "( position() mod 2 ) = 1"><xsl:attribute name = "class">table2</xsl:attribute></xsl:if>
            <span class = "{@write}"><xsl:value-of select = "@write"/></span>
         </td><td>
            <xsl:choose>
               <xsl:when test = "not(@read = 'unsupported') and not(@write = 'unsupported')">
                  <pre style = "margin-bottom: 0pc;">
                     <span class = "expect"><xsl:text>E: </xsl:text><xsl:value-of select = "results/text()"/></span><xsl:text>&#x0A;</xsl:text>
                     <span class = "write" ><xsl:text>W: </xsl:text><xsl:value-of select = "write/text()"  /></span><xsl:text>&#x0A;</xsl:text>
                     <span class = "read"  ><xsl:text>R: </xsl:text><xsl:value-of select = "read/text()"   /></span>
                  </pre>
               </xsl:when><xsl:otherwise>
                  <div class = "info"><xsl:value-of select = "@reason"/></div>
               </xsl:otherwise>
            </xsl:choose>
         </td>
      </tr>
   </xsl:template>
</xsl:stylesheet>
