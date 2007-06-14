/*===========================================================================
    Copyright (c) 2007 Matias Capeletto

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
===========================================================================*/

/*********************** GroupedLinks API **********************************

In your html body use it like:
----------------------------------------------------------------------------
<!-- Include the grouped links java script api
     Remember to change this line with the path of your grouped_links.js -->

<script type="text/javascript" src="grouped_links.js"></script>

<!-- Add a form with an "id" attribute -->

<form id="boost_libs_list">

    <!-- Call the GroupedLinks "select box" with the following parameters
        (1) GroupedLinks xml definition url
        (2) form id
        (3) base url for the links
        (4) selected item [optional]                                    -->

    <script type="text/javascript">

        grouped_links_select_box('boost_libs_grouped_links.xml',
                                 'boost_libs_list',
                                 'http://www.boost.org/libs/');

    </script>

</form>
---------------------------------------------------------------------------

Read the html docs for more information.

**************************************************************************/


(function() {

// Load an xml file, and pass it to the callback function when it is ready

function asinc_load_xml(url, callback)
{
    var XMLHttpRequestObject =
        window.XMLHttpRequest ? new XMLHttpRequest()                   :
        window.ActiveXObject  ? new ActiveXObject("Microsoft.XMLHTTP") :
        false;

    if(XMLHttpRequestObject)
    {
        XMLHttpRequestObject.open("GET", url);
        XMLHttpRequestObject.onreadystatechange = function()
        {
            // Check for XMLHttpRequestObject.status == 200, it returns 0?
            if (XMLHttpRequestObject.readyState == 4 )
            {

                callback(XMLHttpRequestObject.responseXML);
                delete XMLHttpRequestObject;
                XMLHttpRequestObject = null;
            }
        }
        XMLHttpRequestObject.send(null);
    }
}

// Add the base url if it is relative

function format_url(sUrl,sBaseUrl)
{
    if( sUrl.substring(0,7) == 'http://' )
    {
        return sUrl;
    }
    return sBaseUrl + sUrl;
}

// Options for drop down list

function construct_html_select_option(sClass,sTag,sUrl,bSelected)
{
    return '<option ' + (bSelected ? 'selected ' : '') +
            'class="' + sClass + '"'  + ' value="' + sUrl + '" >'
            + sTag + '</option>\n';
}

function construct_select_option(oXmlElement,sClass,
                                 sBaseUrl,sDefaultUrl,sSelected)
{
    return construct_html_select_option(
            sClass,
            oXmlElement.getAttribute('tag'),
            oXmlElement.hasAttribute('url') ?
                format_url(oXmlElement.getAttribute('url'),sBaseUrl) :
                sDefaultUrl,
            sSelected == oXmlElement.getAttribute('tag')
    );
}

function construct_multiple_select_options(oXmlElements,sClass,
                                           sBaseUrl,sDefaultUrl,sSelected)
{
    var sOptionsHtml;
    for(var i = 0; i < oXmlElements.length; i++)
    {
        sOptionsHtml += construct_select_option(
            oXmlElements[i], sClass, sBaseUrl,sDefaultUrl,sSelected
        );
    }
    return sOptionsHtml;
}

// Populate a select block from an xml and insert the result in sId div

function select_box(sXmlUrl,sId,sBaseUrl,sSelected)
{
    asinc_load_xml(sXmlUrl, function(oXml) {

    // manage parameters

    if( sBaseUrl != '' && sBaseUrl.charAt( sBaseUrl.length-1 ) != '/' )
    {
        sBaseUrl += '/';
    }
    var sDefaultUrl = sBaseUrl + 'index';
    var oTitles = oXml.getElementsByTagName('title');
    if( sSelected == null && oTitles.length > 0 )
    {
        sSelected   = oTitles[0].getAttribute('tag');
        if( oTitles[0].hasAttribute('url') )
        {
            sDefaultUrl = oTitles[0].getAttribute('url');
        }
    }

    // Construct the select box

    var sSelectHtml =
        '<select class="grouped_links"'                                     +
               ' name="drowpdown"'                                          +
               ' size="1"'                                                  +
               ' OnChange="grouped_links_internal_go_to_url(this.form)">\n' ;

    sSelectHtml += construct_multiple_select_options(
        oTitles,
        'grouped_links_title', sBaseUrl, sDefaultUrl, sSelected
    );

    var aGroups = oXml.getElementsByTagName('group');
    for(var ig = 0; ig < aGroups.length; ig++)
    {
        sSelectHtml += construct_select_option(
            aGroups[ig],
            'grouped_links_group', sBaseUrl, sDefaultUrl, sSelected
        );

        sSelectHtml += construct_multiple_select_options(
            aGroups[ig].getElementsByTagName('item'),
            'grouped_links_item', sBaseUrl, sDefaultUrl, sSelected
        );
    }

    sSelectHtml += '</select>\n';
    document.getElementById(sId).innerHTML = sSelectHtml;

    } );
}

// Action function used when the user selects an option from the drop down list

function go_to_url(form)
{
    var formindex = form.drowpdown.selectedIndex;
    parent.self.location = form.drowpdown.options[formindex].value;
}

// Api public interface

window["grouped_links_select_box"] = select_box ;
window["grouped_links_internal_go_to_url" ] = go_to_url ;

})();

