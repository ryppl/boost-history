<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:param name="macro_prefix">
		<xsl:text>BOOST_MIRROR_</xsl:text>
	</xsl:param>
	<!-- 
	 - template for the feature reference
	-->
	<xsl:template match="feature_doc">
		<DIV class="feature-doc">
			<xsl:element name="A">
				<xsl:attribute name="NAME">
					<xsl:value-of select="macro/@name|other/@name"/>
				</xsl:attribute>
			</xsl:element>
		<H4 class="feature-doc">
			<xsl:choose>
				<xsl:when test="macro">
					<xsl:value-of select="$macro_prefix"/>
					<xsl:value-of select="macro/@name"/>
					<xsl:text> macro</xsl:text>
				</xsl:when>
				<xsl:when test="other">
					<xsl:value-of select="other/@name"/>
					<xsl:text> - </xsl:text>
					<xsl:value-of select="other/@description"/>
				</xsl:when>
			</xsl:choose>
		</H4>
		<xsl:apply-templates/>
		</DIV>
	</xsl:template>

	<!-- 
	 - template for the macro definition/description formatting
	-->
	<xsl:template match="macro">
		<H5>Synopsis</H5>
		<DIV class="feature-synopsis">
		<CODE>
			<xsl:value-of select="$macro_prefix"/>
			<xsl:value-of select="@name"/>
			<xsl:text>(</xsl:text>
			<xsl:for-each select="arg">
				<xsl:value-of select="@name"/>
				<xsl:if test="position() != last()">
					<xsl:text>, </xsl:text>
				</xsl:if>
			</xsl:for-each>
			<xsl:text>)</xsl:text>
		</CODE>
		</DIV>
		<DL>
		<xsl:for-each select="arg">
			<DT><CODE><xsl:value-of select="@name"/></CODE></DT>
			<DD><xsl:apply-templates/></DD>
		</xsl:for-each>
		</DL>
	</xsl:template>
	<!-- 
	 - template for the requirements
	-->
	<xsl:template match="requirements">
		<H5><xsl:text>Required headers:</xsl:text></H5>
		<xsl:for-each select="header">
			<CODE><SPAN class="cpp-preproc">
				<xsl:text>#include &lt;</xsl:text>
				<xsl:element name="A">
					<xsl:attribute name="href">
						<xsl:text>../../../../../../boost/mirror/</xsl:text>
						<xsl:value-of select="."/>
					</xsl:attribute>
					<xsl:text>boost/mirror/</xsl:text>
					<xsl:value-of select="."/>
				</xsl:element>
				<xsl:text>&gt;</xsl:text>
			</SPAN></CODE>
		</xsl:for-each>
	</xsl:template>
	
	<!-- 
	 - example
	-->
	<xsl:template match="example">
		<H5><xsl:text>Example:</xsl:text></H5>
		<xsl:apply-templates/>
	</xsl:template>

	<!-- 
	 - reference to a feature
	-->
	<xsl:template match="feat_ref">
		<xsl:variable name="doc_url">
			<xsl:text>../reference/</xsl:text>
			<xsl:value-of select="."/>
			<xsl:text>.xml</xsl:text>
		</xsl:variable>
		<SPAN class="feature-ref">
			<xsl:element name="A">
				<xsl:attribute name="href">
					<xsl:text>#</xsl:text>
					<xsl:value-of select="."/>
				</xsl:attribute>
				<xsl:attribute name="title">
					<xsl:for-each select="document($doc_url)/feature_doc/requirements/header">
						<xsl:text>#include &lt;boost/mirror/</xsl:text>
						<xsl:value-of select="."/>
						<xsl:text>&gt;</xsl:text>
					</xsl:for-each>
				</xsl:attribute>
				<xsl:choose>
					<xsl:when test="document($doc_url)/feature_doc/macro">
						<xsl:value-of select="$macro_prefix"/>
					</xsl:when>
				</xsl:choose>
				<xsl:value-of select="."/>
			</xsl:element>
		</SPAN>
	</xsl:template>

	<!-- 
	 - mirror_header
	-->
	<xsl:template match="mirror_header">
		<xsl:element name="A">
			<xsl:attribute name="href">
				<xsl:text>../../../../../../boost/mirror/</xsl:text>
				<xsl:value-of select="."/>
			</xsl:attribute>
			<xsl:text>boost/mirror/</xsl:text>
			<xsl:value-of select="."/>
		</xsl:element>
	</xsl:template>

</xsl:stylesheet>
