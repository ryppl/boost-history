<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
	
	<xsl:template name="find-longest-concept-name-helper">
		<xsl:param name="current_max">0</xsl:param>
		<xsl:variable name="current_length" select="string-length(@name)"/>
		<xsl:choose>
			<xsl:when test="$current_length&gt;$current_max">
				<xsl:for-each select="following-sibling::concept[position()=1]">
					<xsl:call-template name="find-longest-concept-name-helper">
						<xsl:with-param name="current_max">
							<xsl:value-of select="$current_length"/>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:for-each select="following-sibling::concept[position()=1]">
					<xsl:call-template name="find-longest-concept-name-helper">
						<xsl:with-param name="current_max">
							<xsl:value-of select="$current_max"/>
						</xsl:with-param>
					</xsl:call-template>
				</xsl:for-each>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:if test="not(following-sibling::concept)">
			<xsl:choose>
				<xsl:when test="$current_length &gt; $current_max">
					<xsl:value-of select="$current_length"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$current_max"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template name="find-longest-concept-name">
		<xsl:for-each select="document($ontology_src)/concept_ontology/concept[position()=1]">
		<xsl:call-template name="find-longest-concept-name-helper"/>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="make-concept-list-padding">
		<xsl:param name="count"/>
		<xsl:if test="$count &gt; 0">
			<xsl:call-template name="make-concept-list-padding">
				<xsl:with-param name="count" select="$count - 1"/>
			</xsl:call-template>
		</xsl:if>
		<xsl:text>_</xsl:text>
	</xsl:template>
	
	<xsl:template name="make-concept-list">
		<xsl:variable name="ontology_src"><xsl:value-of select="'../ontology.xml'"/></xsl:variable>
		<xsl:variable name="max_name_length">
			<xsl:call-template name="find-longest-concept-name"/>
		</xsl:variable>
		<div class="table">
			<div class="table-contents">
			<table>
				<tr> <th>Alphabetical list of concepts</th> </tr>
				<tr> <td> <div> <code>
				<xsl:for-each select="document($ontology_src)/concept_ontology/concept">
					<xsl:sort select="@name" data-type="text" order="ascending"/>
						<xsl:element name="span">
						<xsl:attribute name="class">mirror-concept-list-item</xsl:attribute>
							<xsl:element name="a">
								<xsl:attribute name="href">
									<xsl:value-of select="@name"/>
									<xsl:text>.xml</xsl:text>
								</xsl:attribute>
								<xsl:call-template name="make-concept-list-padding">
									<xsl:with-param name="count" select="$max_name_length - string-length(@name)"/>
								</xsl:call-template>
								<xsl:value-of select="@name"/>
							<xsl:text> </xsl:text>
							</xsl:element>
						</xsl:element>
				</xsl:for-each>
				</code> </div> </td> </tr>
			</table>
			</div>
		</div>
	</xsl:template>

</xsl:stylesheet>
