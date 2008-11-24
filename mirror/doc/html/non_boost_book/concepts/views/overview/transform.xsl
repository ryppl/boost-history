<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
	<xsl:param name="ontology_src" select="'../../ontology.xml'"/>
	<!-- the output method -->
	<xsl:output method="html" encoding="utf-8"/>

	<!-- include the root node template -->
	<xsl:include href="../../xslt/document_root.xsl"/>

	<!-- include the template that makes the concept graph -->
	<xsl:include href="../../xslt/make_graph.xsl"/>


	<xsl:template name="make-list-of-already-done">
		<xsl:param name="current"/>
		<xsl:param name="member_type"/>
		<xsl:for-each select="document($ontology_src)/concept_ontology/concept[@name = $current]/*[name() = $member_type]">
			<xsl:value-of select="concat('[', @name, ']')"/>
		</xsl:for-each>
		<xsl:for-each select="document($ontology_src)/concept_ontology/role[@type='Specialization' and @object=$current]">
			<xsl:call-template name="make-list-of-already-done">
				<xsl:with-param name="current" select="@subject"/>
				<xsl:with-param name="member_type" select="$member_type"/>
			</xsl:call-template>
		</xsl:for-each>
	</xsl:template>


	<xsl:template name="make-member-items">
		<xsl:param name="concept"/>
		<xsl:param name="current"/>
		<xsl:param name="already_done"/>
		<xsl:param name="member_type"/>

		<!-- print out the typedefs of the current concept -->
		<xsl:for-each select="document($ontology_src)/concept_ontology/concept[@name = $current]/*[name() = $member_type]">
			<xsl:if test="not(contains($already_done, concat('[', @name, ']')))">
			<tr>
				<td><code>
					<xsl:element name="a">
						<xsl:attribute name="href"><xsl:value-of select="@concept"/>.xml</xsl:attribute>
						<xsl:value-of select="@concept"/>
					</xsl:element>
				</code></td>
				<td><code>
					<xsl:value-of select="@name"/>
					<xsl:if test="$member_type = 'member_function'">
						<xsl:text>(</xsl:text>
						<xsl:for-each select="fn_param">
							<xsl:value-of select="@concept"/>
							<xsl:text> </xsl:text>
							<xsl:value-of select="@name"/>
							<xsl:if test="position()!=last()">
							<xsl:text>, </xsl:text>
							</xsl:if>
						</xsl:for-each>
						<xsl:text>)</xsl:text>
					</xsl:if>
				</code></td>
				<td>
					<xsl:value-of select="desc"/>
					<xsl:if test="$concept != $current">
						<xsl:if test="desc/text()"><br/></xsl:if>
						<xsl:text>Inherited from </xsl:text>
						<xsl:element name="a">
							<xsl:attribute name="href">
								<xsl:value-of select="$current"/>
								<xsl:text>.xml</xsl:text>
							</xsl:attribute>
							<code><xsl:value-of select="$current"/></code>
						</xsl:element>
					</xsl:if>
					<xsl:if test="not(desc/text() or $concept != $current)">
						<xsl:text>-</xsl:text>
					</xsl:if>
				</td>
			</tr>
			</xsl:if>
		</xsl:for-each>

		<xsl:variable name="new_already_done">
			<xsl:value-of select="$already_done"/>
			<xsl:for-each select="document($ontology_src)/concept_ontology/concept[@name = $current]/*[name() = $member_type]">
				<xsl:value-of select="concat('[', @name, ']')"/>
			</xsl:for-each>
		</xsl:variable>

		<!-- go through the generalizations of this concept --> 
		<xsl:for-each select="document($ontology_src)/concept_ontology/role[@type='Specialization' and @object=$current]">
			<!-- get the list of members that were already covered -->
			<xsl:call-template name="make-member-items">
				<xsl:with-param name="concept" select="$concept"/>
				<xsl:with-param name="current" select="@subject"/>
				<xsl:with-param name="already_done">
					<xsl:value-of select="$new_already_done"/>
					<xsl:for-each select="preceding-sibling::node()[@type='Specialization' and @object=$current]">
						<xsl:call-template name="make-list-of-already-done">
							<xsl:with-param name="current" select="@subject"/>
							<xsl:with-param name="member_type" select="$member_type"/>
						</xsl:call-template>
					</xsl:for-each>
				</xsl:with-param>
				<xsl:with-param name="member_type" select="$member_type"/>
			</xsl:call-template>
		</xsl:for-each>


	</xsl:template>

	<!-- makes the typedef table -->
	<xsl:template name="make-typedef-table">
		<xsl:param name="concept"/>
		<div class="table">
			<p class="title"><b>Typedefs</b></p>
			<div class="table-contents">
				<table class="table">
					<tr>
						<th>Concept</th>
						<th>Name</th>
						<th>Notes</th>
					</tr>
					<xsl:call-template name="make-member-items">
						<xsl:with-param name="concept" select="$concept"/>
						<xsl:with-param name="current" select="$concept"/>
						<xsl:with-param name="member_type">member_typedef</xsl:with-param>
					</xsl:call-template>
				</table>
			</div>
		</div>
	</xsl:template>

	<!-- makes the member function table -->
	<xsl:template name="make-mem-fn-table">
		<xsl:param name="concept"/>
		<div class="table">
			<p class="title"><b>Member functions</b></p>
			<div class="table-contents">
				<table class="table">
					<tr>
						<th>Returned concept</th>
						<th>Name</th>
						<th>Notes</th>
					</tr>
					<xsl:call-template name="make-member-items">
						<xsl:with-param name="concept" select="$concept"/>
						<xsl:with-param name="current" select="$concept"/>
						<xsl:with-param name="member_type">member_function</xsl:with-param>
					</xsl:call-template>
				</table>
			</div>
		</div>
	</xsl:template>

	<!-- the template for the concept node -->
	<xsl:template match="/concept">
		<xsl:variable name="concept" select="@name"/>

		<!-- make the genealization/specialization "graph" -->
		<div >
		<p class="title"><b>The direct and indirect generalizations and specializations of the <code><xsl:value-of select="@name"/></code> concept.</b></p>
		<xsl:call-template name="make-graph">
			<xsl:with-param name="concept" select="$concept"/>
			<xsl:with-param name="role" select="'Specialization'"/>
		</xsl:call-template>
		</div>

		<xsl:call-template name="make-typedef-table">
			<xsl:with-param name="concept" select="$concept"/>
		</xsl:call-template>
		<xsl:call-template name="make-mem-fn-table">
			<xsl:with-param name="concept" select="$concept"/>
		</xsl:call-template>

	</xsl:template>

	<!-- -->

</xsl:stylesheet>
