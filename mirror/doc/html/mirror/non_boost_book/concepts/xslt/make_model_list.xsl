<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
        <xsl:template name="make-list-of-already-done-models">
                <xsl:param name="current"/>
                <xsl:for-each select="document($ontology_src)/concept_ontology/concept[@name = $current]/model">
                        <xsl:value-of select="concat('[', @name, ']')"/>
                </xsl:for-each>
                <xsl:for-each select="document($ontology_src)/concept_ontology/role[@type='Specialization' and @subject=$current]">
                        <xsl:call-template name="make-list-of-already-done-models">
                                <xsl:with-param name="current" select="@object"/>
                        </xsl:call-template>
                </xsl:for-each>
        </xsl:template>


        <xsl:template name="make-model-list">
                <xsl:param name="concept"/>
                <xsl:param name="current"/>
                <xsl:param name="already_done"/>

                <xsl:for-each select="document($ontology_src)/concept_ontology/concept[@name = $current]/model">
                        <xsl:if test="not(contains($already_done, concat('[', @name, ']')))">
                                <tr>
                                        <td>
                                                <xsl:if test="@nolink='true'">
                                                        <code> <xsl:value-of select="@name"/> </code>
                                                </xsl:if>
                                                <xsl:if test="not(@nolink='true')">
                                                        <xsl:element name="a">
                                                                <xsl:attribute name="href">
                                                                        <xsl:if test="@url">
                                                                                <xsl:value-of select="@url"/>
                                                                        </xsl:if>
                                                                        <xsl:if test="not(@url)">
                                                                                <xsl:text>../../../../boost/</xsl:text>
                                                                                <xsl:if test="@namespace">
                                                                                        <xsl:value-of select="@namespace"/>
                                                                                </xsl:if>
                                                                                <xsl:if test="not(@namespace)">
                                                                                        <xsl:text>mirror</xsl:text>
                                                                                </xsl:if>
                                                                                <xsl:text>/</xsl:text>
                                                                                <xsl:value-of select="@name"/>
                                                                                <xsl:text>.html</xsl:text>
                                                                        </xsl:if>

                                                                </xsl:attribute>

                                                                <xsl:if test="@namespace">
                                                                        <xsl:value-of select="@namespace"/>
                                                                        <xsl:text>::</xsl:text>
                                                                </xsl:if>
                                                                <code> <xsl:value-of select="@name"/> </code>
                                                        </xsl:element>
                                                </xsl:if>
                                        </td>
                                        <td>
                                                <xsl:if test="not(notes)">
                                                        <xsl:text>-</xsl:text>
                                                </xsl:if>
                                        </td>
                                </tr>
                        </xsl:if>
                </xsl:for-each>

                <xsl:variable name="new_already_done">
                        <xsl:value-of select="$already_done"/>
                        <xsl:for-each select="document($ontology_src)/concept_ontology/concept[@name = $current]/model">
                                <xsl:value-of select="concat('[', @name, ']')"/>
                        </xsl:for-each>
                </xsl:variable>
                <!-- go through the refinements of this concept -->
                <xsl:for-each select="document($ontology_src)/concept_ontology/role[@type='Specialization' and @subject=$current]">
                        <!-- get the list of members that were already covered -->
                        <xsl:call-template name="make-model-list">
                                <xsl:with-param name="concept" select="$concept"/>
                                <xsl:with-param name="current" select="@object"/>
                                <xsl:with-param name="already_done">
                                        <xsl:value-of select="$new_already_done"/>
                                        <xsl:for-each select="preceding-sibling::node()[@type='Specialization' and @subject=$current]">
                                                <xsl:call-template name="make-list-of-already-done-models">
                                                        <xsl:with-param name="current" select="@object"/>
                                                </xsl:call-template>
                                        </xsl:for-each>
                                </xsl:with-param>
                        </xsl:call-template>
                </xsl:for-each>

        </xsl:template>
</xsl:stylesheet>
