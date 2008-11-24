<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
	
	<xsl:template name="make-tree">
		<xsl:param name="concept"/>
		<xsl:param name="role"/>
		<xsl:param name="caption"/>
		<xsl:param name="depth">0</xsl:param>
		<xsl:param name="part">both</xsl:param>
		<xsl:variable name="ontology_src"><xsl:value-of select="'../../ontology.xml'"/></xsl:variable>
		<xsl:variable name="inverse-role"><xsl:value-of select="document($ontology_src)/concept_ontology/relation_type[@name=$role]/@inverse"/></xsl:variable>
		
		<xsl:element name="table">
			<xsl:attribute name="class">
				<xsl:text>mirror-concepts-node-</xsl:text><xsl:value-of select="$depth"/>
			</xsl:attribute>
			<xsl:if test="$part='both'">
				<tr>
					<th class="mirror-concepts"><xsl:value-of select="$inverse-role"/>(s)</th>
					<th class="mirror-concepts">Concept</th>
					<th class="mirror-concepts"><xsl:value-of select="$role"/>(s)</th>
				</tr>
			</xsl:if>
			<tr>
				<xsl:if test="$part='subjects' or $part='both'">
					<td class="mirror-concepts-leaf-frame"><table>
					<xsl:for-each select="document($ontology_src)/concept_ontology/role[@type=$role and @object=$concept]">
						<tr><td class="mirror-concepts-leaf-frame">
							<xsl:call-template name="make-tree">
								<xsl:with-param name="concept" select="@subject"/>
								<xsl:with-param name="role" select="$role"/>
								<xsl:with-param name="depth" select="$depth + 1"/>
								<xsl:with-param name="part" select="'subjects'"/>
							</xsl:call-template>
					</td></tr>
					</xsl:for-each>
					<xsl:for-each select="document($ontology_src)/concept_ontology/role[@type=$inverse-role and @subject=$concept]">
						<tr><td class="mirror-concepts-leaf-frame">
							<xsl:call-template name="make-tree">
								<xsl:with-param name="concept" select="@object"/>
								<xsl:with-param name="role" select="$role"/>
								<xsl:with-param name="depth" select="$depth + 1"/>
								<xsl:with-param name="part" select="'subjects'"/>
							</xsl:call-template>
					</td></tr>
					</xsl:for-each>
					</table></td>
				</xsl:if>
				<td class="mirror-concept"><xsl:element name="a">
					<xsl:attribute name="class">mirror-concepts</xsl:attribute>
					<xsl:attribute name="href"><xsl:value-of select="$concept"/><xsl:text>.xml</xsl:text></xsl:attribute>
					<code><xsl:value-of select="$concept"/></code>
				</xsl:element></td>
				<xsl:if test="$part='objects' or $part='both'">
					<td class="mirror-concepts-leaf-frame"><table>
					<xsl:for-each select="document($ontology_src)/concept_ontology/role[@type=$role and @subject=$concept]">
						<tr><td class="mirror-concepts-leaf-frame">
							<xsl:call-template name="make-tree">
								<xsl:with-param name="concept" select="@object"/>
								<xsl:with-param name="role" select="$role"/>
								<xsl:with-param name="depth" select="$depth + 1"/>
								<xsl:with-param name="part" select="'objects'"/>
							</xsl:call-template>
					</td></tr>
					</xsl:for-each>
					<xsl:for-each select="document($ontology_src)/concept_ontology/role[@type=$inverse-role and @object=$concept]">
						<tr><td class="mirror-concepts-leaf-frame">
							<xsl:call-template name="make-tree">
								<xsl:with-param name="concept" select="@subject"/>
								<xsl:with-param name="role" select="$role"/>
								<xsl:with-param name="depth" select="$depth + 1"/>
								<xsl:with-param name="part" select="'objects'"/>
							</xsl:call-template>
					</td></tr>
					</xsl:for-each>
					</table></td>
				</xsl:if>
			</tr>
			<xsl:if test="$depth=0">
				<caption valign="bottom"><xsl:value-of select="$caption"/></caption>
			</xsl:if>
		</xsl:element>
	</xsl:template>

</xsl:stylesheet>
