<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<!-- 
	- template for the boost node
	- this node is expanded to the word boost linking to www.boost.org
	-->
	<xsl:template match="boost">
		<A href="http://www.boost.org"><B><EM>boost</EM></B></A>
	</xsl:template>
	<!-- 
	 - this helper template properly formats the name of a boost
	 - library
	-->
	<xsl:template name="format-boost-lib-name">
		<xsl:param name="a_name"/>
		<xsl:param name="at_beginning" select="true()"/>
		<xsl:variable name="lc_letters">abcdefghijklmnopqrstuvwxyz</xsl:variable>
		<xsl:variable name="uc_letters">ABCDEFGHIJKLMNOPQRSTUVWXYZ</xsl:variable>
		<xsl:choose>
			<!-- if we are at the beginning -->
			<xsl:when test="$at_beginning">
				<!-- if the first character is an underscore -->
				<xsl:if test="substring($a_name, 1, 1)='_'">
					<!-- translate all charecters to uppercase and underscores to spaces -->
					<xsl:value-of select="translate(translate(substring($a_name, 2), $lc_letters, $uc_letters), '_', ' ')"/> 
				</xsl:if>
				<!-- if the first character is not an underscore -->
				<xsl:if test="substring($a_name, 1, 1)!='_'">
					<!-- capitalize the first character -->
					<xsl:value-of select="translate(substring($a_name, 1, 1), $lc_letters, $uc_letters)"/>
					<xsl:call-template name="format-boost-lib-name">
						<xsl:with-param name="a_name" select="substring($a_name, 2)"/>
						<xsl:with-param name="at_beginning" select="false()"/>
					</xsl:call-template>

				</xsl:if>
			</xsl:when>
			<!-- not at the beginning -->
			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="contains($a_name, '_')">
						<!-- if the first character is an underscore -->
						<xsl:if test="substring($a_name, 1, 1)='_'">
							<xsl:text> </xsl:text>
							<xsl:value-of select="translate(substring($a_name, 2, 1), $lc_letters, $uc_letters)"/>
							<xsl:call-template name="format-boost-lib-name">
								<xsl:with-param name="a_name" select="substring($a_name, 3)"/>
								<xsl:with-param name="at_beginning" select="false()"/>
							</xsl:call-template>
						</xsl:if>
						<!-- if the first character is not an underscore -->
						<xsl:if test="substring($a_name, 1, 1)!='_'">
							<xsl:value-of select="translate(substring-before($a_name, '_'), $uc_letters, $lc_letters)"/>
							<xsl:text> </xsl:text>
							<xsl:call-template name="format-boost-lib-name">
								<xsl:with-param name="a_name" select="substring-after($a_name, '_')"/>
								<xsl:with-param name="at_beginning" select="true()"/>
							</xsl:call-template>
						</xsl:if>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="translate($a_name, $uc_letters, $lc_letters)"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<!-- 
	- template for the boost_lib_ref node
	- this node is expanded to the name of a boost library 
	- and is linking to its documentation
	-->
	<xsl:template match="boost_lib_ref">
		<xsl:element name="A">
			<xsl:attribute name="href">
				<xsl:text>http://www.boost.org/doc/libs/</xsl:text>
			</xsl:attribute>
			<xsl:text>Boost.</xsl:text>
			<xsl:call-template name="format-boost-lib-name">
				<xsl:with-param name="a_name" select="@name"/>
			</xsl:call-template>
		</xsl:element>
	</xsl:template>
</xsl:stylesheet>
