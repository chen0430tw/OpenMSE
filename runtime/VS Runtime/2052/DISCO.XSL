<?xml version='1.0' ?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl">
  <xsl:template match="/">
    <table border="0" cellpadding="2" cellspacing="0" width="100%" valign="center" align="center" rules="cols">
      <xsl:choose>
        <xsl:when test="discovery/contractRef | discovery/schemaRef | discovery/discoveryRef">
          <xsl:choose>
            <xsl:when test="discovery/contractRef">
              <tr valign="center" align="left">
                <td>
                  <table border="0" cellpadding="2" cellspacing="0" width="200">
                    <tr>
                      <td class="header" colspan="2" bgcolor="#669999">Web Services</td>
                    </tr>
                  </table>
                </td>
              </tr>
              <xsl:for-each select="discovery/contractRef">
                <tr valign="center">
                  <td>
                    <table border="0" cellpadding="2" cellspacing="0">
                      <tr>
                        <td><img src="..\service.gif"/></td>
                        <td class="label" colspan="2" nowrap="true"><xsl:value-of select="@ref"/></td>
                      </tr>
                      <tr>
                        <td/>
                        <td class="label"><a><xsl:attribute name="href">VS:<xsl:value-of select="@ref"/></xsl:attribute>View Contract</a>
                        <xsl:choose>
                          <xsl:when test="@docRef">
                            <br/><a><xsl:attribute name="href">VS:<xsl:value-of select="@docRef"/></xsl:attribute>View Documentation</a>
                          </xsl:when>
                        </xsl:choose>
                        </td>
                      </tr>
                    </table>
                  </td>
                </tr>
              </xsl:for-each>      
            </xsl:when>
          </xsl:choose>
          <xsl:choose>
            <xsl:when test="discovery/schemaRef">
              <tr valign="center" align="left">
                <td class="label"><p>&#160;</p></td>
              </tr>
              <tr valign="center" align="left">
                <td>
                  <table border="0" cellpadding="2" cellspacing="0" width="200">
                    <tr>
                      <td class="header" colspan="2" bgcolor="#669999">DataSets</td>
                    </tr>
                  </table>
                </td>
              </tr>
              <xsl:for-each select="discovery/schemaRef">
                <tr valign="center">
                  <td>
                    <table border="0" cellpadding="2" cellspacing="0">
                      <tr>
                        <td><img src="..\schema.gif"/></td>
                        <td class="label" colspan="2" nowrap="true"><xsl:value-of select="@ref"/></td>
                      </tr>
                      <tr>
                        <td/>
                        <td class="label"><a><xsl:attribute name="href">VS:<xsl:value-of select="@ref"/></xsl:attribute>View Schema</a></td>
                      </tr>
                    </table>
                  </td>
                </tr>
              </xsl:for-each>
            </xsl:when>
          </xsl:choose>
          <xsl:choose>
            <xsl:when test="discovery/discoveryRef">
              <tr valign="center" align="left">
                <td class="label"><p>&#160;</p></td>
              </tr>
              <tr valign="center" align="left">
                <td class="label"><b>Linked reference groups:</b></td>
              </tr>
              <xsl:for-each select="discovery/discoveryRef">
                <tr>
                  <td>
                    <table border="0" cellpadding="2" cellspacing="0">
                      <tr>
                        <td><img src="..\disco.gif"/></td>
                        <td class="label"><a><xsl:attribute name="href">VS:<xsl:value-of select="@ref"/></xsl:attribute><xsl:value-of select="@ref"/></a></td>
                      </tr>
                    </table>
                  </td>
                </tr>
              </xsl:for-each>
            </xsl:when>
          </xsl:choose>
        </xsl:when>
        <xsl:otherwise>
          <tr valign="center">
            <td class="text">No Web References were found on this page.</td>
          </tr>
          <tr>
            <td class="text"><a href="helpwatermark.htm">Click here</a> for help on finding a Web Reference.</td>
          </tr>
        </xsl:otherwise>
      </xsl:choose>
    </table>
  </xsl:template>
</xsl:stylesheet>
