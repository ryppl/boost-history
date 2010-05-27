<?php
require_once(dirname(__FILE__) . '/../common/code/boost_archive.php');
require_once(dirname(__FILE__) . '/../common/code/boost_version.php');

function add_spirit_analytics($content) {
    if(stripos($content, '_uacct = "UA-11715441-2"') !== FALSE)
        return $content;

    $analytics = <<<EOS
<script type="text/javascript">
  var _gaq = _gaq || [];
  _gaq.push(
    ['_setAccount', 'UA-11715441-2'],
    ['_trackPageview'],
    ['_setDomainName', 'none'],
    ['_setAllowLinker', true]
    );

  (function() {
    var ga = document.createElement('script');
    ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
    ga.setAttribute('async', 'true');
    document.documentElement.firstChild.appendChild(ga);
  })();
</script>
EOS;

    $content = preg_replace(
        '@<a\s+href="(http://spirit.sourceforge.net[^"]*)"@i',
        '<a href="${1}" onclick=\'_gaq.push(["_link", "${1}"]); return false;\'',
        $content );

    return str_ireplace('</head>', $analytics.'</head>', $content);
}

$location = get_archive_location('@^[/]([^/]+)[/](.*)$@',$_SERVER["PATH_INFO"]);

if (boost_future_version($location['version'])) {
    file_not_found($location['file'],
        "Documentation for this version has not been uploaded yet. ".
        "Documentation is only uploaded when it's fully released, ".
        "you can see the documentation for a beta version or snapshot in the download.");
    return;
}

display_from_archive(
  $location,
  array(
  //~ special cases that can't be processed at all (some redirects)
  array('@.*@','@^libs/gil/doc/.*(html|htm)$@i','raw','text/html'),
  array('@.*@','@^libs/preprocessor/doc/.*(html|htm)$@i','raw','text/html'),
  array('@.*@','@^libs/test/doc/components/test_tools/reference/.*(html|htm)$@i','raw','text/html'),
  array('@.*@','@^libs/spirit/.*(html|htm)$@i','simple','text/html', 'add_spirit_analytics'),
  array('@.*@','@^libs/fusion/.*(html|htm)$@i','basic','text/html', 'add_spirit_analytics'),
  array('@.*@','@^libs/wave/.*(html|htm)$@i','raw','text/html'),
  array('@.*@','@^libs/range/doc/.*(html|htm)$@i','raw','text/html'),
  //~ special cases that can't be embeded in the standard frame
  array('@.*@','@^libs/iostreams/doc/.*(html|htm)$@i','simple','text/html'),
  array('@.*@','@^libs/serialization/doc/.*(html|htm)$@i','simple','text/html'),
  array('@.*@','@^libs/filesystem/doc/.*(html|htm)$@i','simple','text/html'),
  array('@.*@','@^libs/system/doc/.*(html|htm)$@i','simple','text/html'),
  array('@.*@','@^libs/numeric/conversion/doc/.*(html|htm)$@i','simple','text/html'),
  array('@.*@','@^libs/optional/doc/.*(html|htm)$@i','simple','text/html'),
  //~ default to processed output for libs and tools
  array('@.*@','@^libs/[^/]+/doc/html/.*(html|htm)$@i','basic','text/html'),
  array('@.*@','@^libs/[^/]+/doc/[^/]+/html/.*(html|htm)$@i','basic','text/html'),
  array('@.*@','@^libs/[^/]+/doc/[^/]+/doc/html/.*(html|htm)$@i','basic','text/html'),
  array('@.*@','@^libs.*(html|htm)$@i','basic','text/html'),
  array('@.*@','@^tools.*(html|htm)$@i','basic','text/html'),
  array('@.*@','@^doc/html/.*html$@i','basic','text/html'),
  array('@.*@','@^more/.*html$@i','basic','text/html'),
  //~ the headers are text files displayed in an embeded page
  array('@.*@','@^boost/.*$@i','cpp','text/plain')
));
