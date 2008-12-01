<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
	<xsl:param name="ontology_src" select="'../ontology.xml'"/>
	<!-- the output method -->
	<xsl:output method="html" encoding="utf-8"/>

	<!-- include the root node template -->
	<xsl:include href="../xslt/document_root.xsl"/>

	<!-- include the template that makes the concept graph -->
	<xsl:include href="../xslt/make_graph.xsl"/>

	<!-- include the template that makes the alphabetical list of concepts-->
	<xsl:include href="../xslt/make_concept_list.xsl"/>

	<!-- include the template that makes the list of concept models-->
	<xsl:include href="../xslt/make_model_list.xsl"/>


	<xsl:template name="make-list-of-already-done-members">
		<xsl:param name="current"/>
		<xsl:param name="member_kind"/>
		<xsl:for-each select="document($ontology_src)/concept_ontology/concept[@name = $current]/*[name() = $member_kind]">
			<xsl:value-of select="concat('[', @name, ']')"/>
		</xsl:for-each>
		<xsl:for-each select="document($ontology_src)/concept_ontology/role[@type='Specialization' and @object=$current]">
			<xsl:call-template name="make-list-of-already-done-members">
				<xsl:with-param name="current" select="@subject"/>
				<xsl:with-param name="member_kind" select="$member_kind"/>
			</xsl:call-template>
		</xsl:for-each>
	</xsl:template>


	<xsl:template name="find-and-mark-member-items">
		<xsl:param name="current"/>
		<xsl:param name="member_kind"/>
		
		<xsl:choose>
			<xsl:when test="count(document($ontology_src)/concept_ontology/concept[@name = $current]/*[name() = $member_kind]) &gt; 0">
				<xsl:text>X</xsl:text>
			</xsl:when>
			<xsl:otherwise>
			<xsl:for-each select="document($ontology_src)/concept_ontology/role[@type='Specialization' and @object=$current]">
				<xsl:call-template name="find-and-mark-member-items">
					<xsl:with-param name="current" select="@subject"/>
					<xsl:with-param name="member_kind" select="$member_kind"/>
				</xsl:call-template>
			</xsl:for-each>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="has-member-items">
		<xsl:param name="concept"/>
		<xsl:param name="member_kind"/>
		<xsl:variable name="marks">
			<xsl:call-template name="find-and-mark-member-items">
				<xsl:with-param name="current" select="$concept"/>
				<xsl:with-param name="member_kind" select="$member_kind"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:value-of select="string-length($marks) &gt; 0"/>
	</xsl:template>
	

	<xsl:template name="make-member-items">
		<xsl:param name="concept"/>
		<xsl:param name="current"/>
		<xsl:param name="already_done"/>
		<xsl:param name="member_kind"/>

		<!-- print out the typedefs of the current concept -->
		<xsl:for-each select="document($ontology_src)/concept_ontology/concept[@name = $current]/*[name() = $member_kind]">
			<xsl:if test="not(contains($already_done, concat('[', @name, ']')))">
			<tr>
				<td><code>
					<xsl:variable name="this_concept" select="@concept"/>
					<xsl:variable name="this_is_known_concept" select="document($ontology_src)/concept_ontology/concept[@name = $this_concept]"/>
					<xsl:if test="not($this_is_known_concept)">
						<xsl:value-of select="$this_concept"/>
					</xsl:if>
					<xsl:if test="$this_is_known_concept">
						<xsl:element name="a">
							<xsl:attribute name="href">
								<xsl:if test="@url">
									<xsl:value-of select="@url"/>
								</xsl:if>
								<xsl:if test="not(@url)">
									<xsl:value-of select="$this_concept"/>
									<xsl:text>.xml</xsl:text>
								</xsl:if>
							</xsl:attribute>

							<xsl:value-of select="$this_concept"/>
						</xsl:element>
					</xsl:if>
				</code></td>
				<td><code>
					<xsl:if test="$member_kind = 'member_typedef'">
						<strong>T</strong><xsl:text>::</xsl:text>
					</xsl:if>
					<xsl:if test="$member_kind = 'member_function'">
						<strong>x</strong><xsl:text>.</xsl:text>
					</xsl:if>
					
					
					<xsl:if test="$member_kind = 'free_function' or $member_kind='metafunction'">
						<xsl:if test="@namespace or @subnamespace">
							<xsl:value-of select="@namespace | @subnamespace"/>
							<xsl:text>::</xsl:text>
						</xsl:if>
					</xsl:if>
					<xsl:value-of select="@name"/>
					<xsl:if test="$member_kind = 'metafunction' or template_param">
						<xsl:text>&lt; </xsl:text>
						<xsl:for-each select="metafn_param | template_param">
							<xsl:variable name="templ_param_concept" select="@name"/>
							<xsl:if test="@placeholder='true'">
								<strong>T</strong>
							</xsl:if>
							<xsl:choose>
								<xsl:when test="document($ontology_src)/concept_ontology/concept[@name = $templ_param_concept]">
									<xsl:element name="a">
										<xsl:attribute name="href">
											<xsl:value-of select="@name"/>
											<xsl:text>.xml</xsl:text>
										</xsl:attribute>
										<xsl:value-of select="@name"/>
									</xsl:element>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="@name"/>
								</xsl:otherwise>
							</xsl:choose>
							<xsl:if test="position()!=last()">
							<xsl:text>, </xsl:text>
							</xsl:if>
						</xsl:for-each>
						<xsl:text> &gt;::type</xsl:text>
					</xsl:if>
					<xsl:if test="$member_kind = 'member_function' or $member_kind = 'free_function'">
						<xsl:text>(</xsl:text>
						<xsl:for-each select="fn_param">
							<xsl:variable name="fn_param_concept" select="@concept"/>
							<xsl:element name="span">
								<xsl:attribute name="title">
									<xsl:value-of select="@concept"/>
								</xsl:attribute>
								<xsl:choose>
									<xsl:when test="document($ontology_src)/concept_ontology/concept[@name = $fn_param_concept]">
										<xsl:element name="a">
											<xsl:attribute name="href">
												<xsl:value-of select="@concept"/>
												<xsl:text>.xml</xsl:text>
											</xsl:attribute>
											<xsl:value-of select="@name"/>
										</xsl:element>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="@name"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:element>
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
			<xsl:for-each select="document($ontology_src)/concept_ontology/concept[@name = $current]/*[name() = $member_kind]">
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
						<xsl:call-template name="make-list-of-already-done-members">
							<xsl:with-param name="current" select="@subject"/>
							<xsl:with-param name="member_kind" select="$member_kind"/>
						</xsl:call-template>
					</xsl:for-each>
				</xsl:with-param>
				<xsl:with-param name="member_kind" select="$member_kind"/>
			</xsl:call-template>
		</xsl:for-each>


	</xsl:template>


	<xsl:template name="make-member-item-table">
		<xsl:param name="concept"/>
		<xsl:param name="member_kind"/>
		<xsl:param name="caption"/>
		<xsl:param name="concept_column_heading"/>
		<xsl:variable name="has_items">
			<xsl:call-template name="has-member-items">
				<xsl:with-param name="concept" select="$concept"/>
				<xsl:with-param name="member_kind" select="$member_kind"/>
			</xsl:call-template>
		</xsl:variable>

		<xsl:if test="$has_items='true'">
			<div class="table">
				<p class="title"><b><xsl:value-of select="$caption"/></b></p>
				<div class="table-contents">
					<table class="table">
						<tr>
							<th><xsl:value-of select="$concept_column_heading"/></th>
							<th>Expression</th>
							<th>Notes</th>
						</tr>
						<xsl:call-template name="make-member-items">
							<xsl:with-param name="concept" select="$concept"/>
							<xsl:with-param name="current" select="$concept"/>
							<xsl:with-param name="member_kind" select="$member_kind"/>
						</xsl:call-template>
					</table>
				</div>
			</div>
		</xsl:if>
	</xsl:template>


	<xsl:template name="has-any-member-items-helper">
		<xsl:param name="concept"/>
		<xsl:call-template name="has-member-items">
			<xsl:with-param name="concept" select="$concept"/>
			<xsl:with-param name="member_kind" select="'member_typedef'"/>
		</xsl:call-template>
		<xsl:call-template name="has-member-items">
			<xsl:with-param name="concept" select="$concept"/>
			<xsl:with-param name="member_kind" select="'member_function'"/>
		</xsl:call-template>
		<xsl:call-template name="has-member-items">
			<xsl:with-param name="concept" select="$concept"/>
			<xsl:with-param name="member_kind" select="'free_function'"/>
		</xsl:call-template>
		<xsl:call-template name="has-member-items">
			<xsl:with-param name="concept" select="$concept"/>
			<xsl:with-param name="member_kind" select="'metafunction'"/>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template name="has-any-member-items">
		<xsl:param name="concept"/>
		<xsl:variable name="subresults">
			<xsl:call-template name="has-any-member-items-helper">
				<xsl:with-param name="concept" select="$concept"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:value-of select="contains($subresults, 'true')"/>
	</xsl:template>
	

	<!-- makes the typedef table -->
	<xsl:template name="make-typedef-table">
		<xsl:param name="concept"/>
		<xsl:call-template name="make-member-item-table">
			<xsl:with-param name="concept" select="$concept"/>
			<xsl:with-param name="member_kind" select="'member_typedef'"/>
			<xsl:with-param name="caption" select="'Member types'"/>
			<xsl:with-param name="concept_column_heading" select="'Concept'"/>
		</xsl:call-template>
	</xsl:template>

	<!-- makes the member function table -->
	<xsl:template name="make-mem-fn-table">
		<xsl:param name="concept"/>
                <xsl:call-template name="make-member-item-table">
                        <xsl:with-param name="concept" select="$concept"/>
                        <xsl:with-param name="member_kind" select="'member_function'"/>
                        <xsl:with-param name="caption" select="'Member functions'"/>
                        <xsl:with-param name="concept_column_heading" select="'Return value concept'"/>
                </xsl:call-template>
	</xsl:template>

	<!-- makes the free function table -->
	<xsl:template name="make-free-fn-table">
		<xsl:param name="concept"/>
                <xsl:call-template name="make-member-item-table">
                        <xsl:with-param name="concept" select="$concept"/>
                        <xsl:with-param name="member_kind" select="'free_function'"/>
                        <xsl:with-param name="caption" select="'Free functions'"/>
                        <xsl:with-param name="concept_column_heading" select="'Return value concept'"/>
                </xsl:call-template>
	</xsl:template>

	<!-- makes the metafunction function table -->
	<xsl:template name="make-meta-fn-table">
		<xsl:param name="concept"/>
                <xsl:call-template name="make-member-item-table">
                        <xsl:with-param name="concept" select="$concept"/>
                        <xsl:with-param name="member_kind" select="'metafunction'"/>
                        <xsl:with-param name="caption" select="'Metafunctions'"/>
                        <xsl:with-param name="concept_column_heading" select="'Result concept'"/>
                </xsl:call-template>
	</xsl:template>

	<xsl:template name="make-models-table">
		<xsl:param name="concept"/>
		<div class="table">
			<p class="title"><b>Models</b></p>
			<div class="table-contents">
				<table class="table">
					<tr>
						<th>Model</th>
						<th>Notes</th>
					</tr>
					<xsl:call-template name="make-model-list">
						<xsl:with-param name="concept" select="$concept"/>
						<xsl:with-param name="current" select="$concept"/>
					</xsl:call-template>
				</table>
			</div>
		</div>

	</xsl:template>

	<!-- the template for the concept node -->
	<xsl:template match="/concept">
		<xsl:variable name="concept" select="@name"/>

		<h2>Concept browser (The <xsl:value-of select="$concept"/> concept)</h2>
		<!-- make a list of all concepts -->
		<xsl:call-template name="make-concept-list"/>

		<!-- make the genealization/specialization "graph" -->
		<div>
		<p class="title"><b>The direct and indirect generalizations and specializations of the <code><xsl:value-of select="@name"/></code> concept.</b></p>
		<xsl:call-template name="make-graph">
			<xsl:with-param name="concept" select="$concept"/>
			<xsl:with-param name="role" select="'Specialization'"/>
		</xsl:call-template>
		</div>

		<xsl:variable name="has_expressions">
			<xsl:call-template name="has-any-member-items">
				<xsl:with-param name="concept" select="$concept"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:if test="$has_expressions='true'">
			<p>For each type <code><strong>T</strong></code> which is a model of the <xsl:value-of select="$concept"/> concept and for each variable <code><strong>x</strong></code> which is of <code><strong>T</strong></code> type, the following expressions are valid.</p>
		</xsl:if>
	
		<xsl:call-template name="make-typedef-table">
			<xsl:with-param name="concept" select="$concept"/>
		</xsl:call-template>
		<xsl:call-template name="make-mem-fn-table">
			<xsl:with-param name="concept" select="$concept"/>
		</xsl:call-template>
		<xsl:call-template name="make-free-fn-table">
			<xsl:with-param name="concept" select="$concept"/>
		</xsl:call-template>
		<xsl:call-template name="make-meta-fn-table">
			<xsl:with-param name="concept" select="$concept"/>
		</xsl:call-template>
		<xsl:call-template name="make-models-table">
			<xsl:with-param name="concept" select="$concept"/>
		</xsl:call-template>

	</xsl:template>

</xsl:stylesheet>
