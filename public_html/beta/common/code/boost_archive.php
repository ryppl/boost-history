<?php
/*
  Copyright 2005-2008 Redshift Software, Inc.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
*/
require_once(dirname(__FILE__) . '/boost.php');

function get_archive_location(
    $pattern,
    $vpath,
    $archive_subdir = true,
    $archive_dir = ARCHIVE_DIR,
    $archive_file_prefix = ARCHIVE_FILE_PREFIX)
{
    $path_parts = array();
    preg_match($pattern, $vpath, $path_parts);

    $version = $path_parts[1];
    $key = $path_parts[2];

    if ($archive_subdir)
    {
        $file = $archive_file_prefix . $version . '/' . $key;
    }
    else
    {
        $file = $archive_file_prefix . $key;
    }

    $archive = str_replace('\\','/', $archive_dir . '/' . $version . '.zip');
    
    return array(
        'version' => $version,
        'key' => $key,
        'file' => $file,
        'archive' => $archive
    );
}

function display_from_archive(
    $archive_location_details,
    $content_map = array(),
    $override_extractor = null)
{
    $info_map = array_merge($content_map, array(
        array('@.*@','@[.](txt|py|rst|jam|v2|bat|sh|xml|qbk)$@i','text','text/plain'),
        array('@.*@','@[.](c|h|cpp|hpp)$@i','cpp','text/plain'),
        array('@.*@','@[.]png$@i','raw','image/png'),
        array('@.*@','@[.]gif$@i','raw','image/gif'),
        array('@.*@','@[.](jpg|jpeg|jpe)$@i','raw','image/jpeg'),
        array('@.*@','@[.]css$@i','raw','text/css'),
        array('@.*@','@[.]js$@i','raw','application/x-javascript'),
        array('@.*@','@[.]pdf$@i','raw','application/pdf'),
        array('@.*@','@[.](html|htm)$@i','raw','text/html'),
        array('@.*@','@[^.](Jamroot|Jamfile|ChangeLog)$@i','text','text/plain'),
        array('@.*@','@[.]dtd$@i','raw','application/xml-dtd'),
        ));

    $preprocess = null;
    $extractor = null;
    $type = null;

    foreach ($info_map as $i)
    {
        if (preg_match($i[1],$archive_location_details['key']))
        {
            $extractor = $i[2];
            $type = $i[3];
            $preprocess = isset($i[4]) ? $i[4] : NULL;
            break;
        }
    }
    
    if ($override_extractor) $extractor = $override_extractor;

    if (!$extractor) {
        file_not_found($archive_location_details['file']);
        return;
    }

    $unzip =
      UNZIP
      .' -p '.escapeshellarg($archive_location_details['archive'])
      .' '.escapeshellarg($archive_location_details['file']);

    if($extractor == 'raw') {
        display_raw_file($unzip, $type);
        return;
    }

    $archive = new boost_archive();
    $archive->key_ = $archive_location_details['key'];

    // Note: this sets $archive->content_ with either the content or an error
    // message:
    if(!extract_file($unzip, $archive->content_)) {
        file_not_found($archive_location_details['file'], $archive->content_);
        return;
    }

    if ($preprocess) {
        $archive->content_ = call_user_func($preprocess, $archive->content_);
    }
    
    $extractor_name = $extractor.'_filter';
    $extractor_instance = new $extractor_name;
    $extractor_instance->render($archive);
}

class boost_archive
{
    var $key_ = NULL;
    var $title_ = NULL;
    var $charset_ = NULL;
    var $content_ = NULL;
}

class boost_archive_render_callbacks {
    var $extractor, $archive;
    
    function boost_archive_render_callbacks($extractor, $archive) {
        $this->extractor = $extractor;
        $this->archive = $archive;
    }

    function content_head()
    {
        $charset = $this->archive->charset_ ? $this->archive->charset_ : 'us-ascii';
        $title = $this->archive->title_ ? 'Boost C++ Libraries - '.$this->archive->title_ : 'Boost C++ Libraries';

        print <<<HTML
  <meta http-equiv="Content-Type" content="text/html; charset=${charset}" />
  <title>${title}</title>
HTML;
    }
    
    function content()
    {
        if ($this->extractor)
        {
            $this->extractor->content($this->archive);
        }
    }
}

function display_raw_file($unzip, $type) {
    header('Content-type: '.$type);
    ## header('Content-Disposition: attachment; filename="downloaded.pdf"');
    $file_handle = popen($unzip,'rb');
    fpassthru($file_handle);
    $exit_status = pclose($file_handle);
    
    // Don't display errors for a corrupt zip file, as we seemd to
    // be getting them for legitimate files.

    if($exit_status > 3)
        echo 'Error extracting file: '.unzip_error($exit_status);
};

function extract_file($unzip, &$content) {
    $file_handle = popen($unzip,'r');
    $text = '';
    while ($file_handle && !feof($file_handle)) {
        $text .= fread($file_handle,8*1024);
    }
    $exit_status = pclose($file_handle);

    if($exit_status == 0) {
        $content = $text;
        return true;
    }
    else {
        $content = strstr($_SERVER['HTTP_HOST'], 'beta') ? unzip_error($exit_status) : null;
        return false;
    }
}

class text_filter
{
    function content($archive)
    {
        print "<h3>".htmlentities($archive->key_)."</h3>\n";
        print "<pre>\n";
        print htmlentities($archive->content_);
        print "</pre>\n";
    }

    function render($archive) {
        $archive->title_ = htmlentities($archive->key_);

        display_template(new boost_archive_render_callbacks($this, $archive));
    }
}

class cpp_filter
{
    function content($archive)
    {
        $text = htmlentities($archive->content_);
        
        print "<h3>".htmlentities($archive->key_)."</h3>\n";
        print "<pre>\n";
        $root = dirname(preg_replace('@([^/]+/)@','../',$archive->key_));
        $text = preg_replace(
            '@(#[ ]*include[ ]+&lt;)(boost[^&]+)@Ssm',
            '${1}<a href="'.$root.'/${2}">${2}</a>',
            $text );
        $text = preg_replace(
            '@(#[ ]*include[ ]+&quot;)(boost[^&]+)@Ssm',
            '${1}<a href="'.$root.'/${2}">${2}</a>',
            $text );
        print $text;
        print "</pre>\n";
    }

    function render($archive) {
        $archive->title_ = htmlentities($archive->key_);

        display_template(new boost_archive_render_callbacks($this, $archive));
    }
}

class html_base_filter
{
    function html_init($archive)
    {
        preg_match('@text/html; charset=([^\s"\']+)@i',$archive->content_,$charset);
        if (isset($charset[1]))
        {
            $archive->charset_ = $charset[1];
        }
        
        preg_match('@<title>([^<]+)</title>@i',$archive->content_,$title);
        if (isset($title[1]))
        {
            $archive->title_ = $title[1];
        }
    }
}

class boost_book_html_filter extends html_base_filter
{
    function content($archive)
    {
        $text = prepare_html($archive->content_);
        
        $text = substr($text,strpos($text,'<div class="spirit-nav">'));
        $text = substr($text,0,strpos($text,'</body>'));
        $text = str_replace('<hr>','',$text);
        $text = str_replace('<table width="100%">','<table class="footer-table">',$text);
        $text = str_replace('<table xmlns:rev="http://www.cs.rpi.edu/~gregod/boost/tools/doc/revision" width="100%">','<table class="footer-table">',$text);
        $text = preg_replace(
            '@[\s]+(border|cellpadding|cellspacing|width|height|valign|frame|rules|naturalsizeflag|background)=[^\s>]+@i',
            '',
            $text );
        /* */
        for ($i = 0; $i < 8; $i++) {
            $text = preg_replace(
                '@<img src="[\./a-z]*images/(prev|up|home|next|tip|note|warning|important|caution|sidebar|hint|alert)\.png" alt="([^"]+)"([ /]*)>@Ssm',
                '<img src="/gfx/space.png" alt="${2}" class="${1}_image" />',
                $text );
        }
        /* */
        
        print $text;
    }

    function render($archive) {
        $this->html_init($archive);
        display_template(new boost_archive_render_callbacks($this, $archive));
    }
}

class boost_libs_filter extends html_base_filter
{
    function content($archive)
    {
        return $archive->content_;
    }
    
    function render($archive)
    {
        $this->html_init($archive);
        $text = extract_html_body($archive->content_);
        if($text) {
            $text = prepare_html($text);
            $text = remove_html_banner($text);
            $text = prepare_themed_html($text);
            $archive->content_ = $text;
            
            display_template(new boost_archive_render_callbacks($this, $archive));
        }
        else {
            print $archive->content_;
        }
    }
}

class boost_frame1_filter extends html_base_filter
{
    function content($archive)
    {
        $text = prepare_html($archive->content_);
        
        $text = substr($text,strpos($text,'<div class="spirit-nav">'));
        $text = substr($text,0,strpos($text,'</body>'));
        for ($i = 0; $i < 8; $i++) {
            $text = preg_replace(
                '@<img src="[\./]*images/(.*\.png)" alt="(.*)"([ ][/])?>@Ssm',
                '<img src="/style-v2/css_0/${1}" alt="${2}" />',
                $text );
        }
        $text = str_replace('<hr>','',$text);
        $text = str_replace('<table width="100%">','<table class="footer-table">',$text);
        $text = preg_replace(
            '@[\s]+(border|cellpadding|cellspacing|width|height|valign|frame|rules|naturalsizeflag|background)=[^\s>]+@i',
            '',
            $text );
        
        print $text;
    }

    function render($archive) {
        $this->html_init($archive);
        display_template(new boost_archive_render_callbacks($this, $archive));
    }
}

class simple_filter extends html_base_filter
{
    function render($archive)
    {
        print prepare_html($archive->content_);
    }
}

class basic_filter extends html_base_filter
{
    function render($archive)
    {
        $text = prepare_html($archive->content_);
        $text = remove_html_banner($text);

        $is_xhtml = preg_match('@<!DOCTYPE[^>]*xhtml@i', $text);
        $tag_end = $is_xhtml ? '/>' : '>';
        
        $sections = preg_split('@(</head>|<body[^>]*>)@i',$text,-1,PREG_SPLIT_DELIM_CAPTURE);

        $body_index = 0;
        $index = 0;
        foreach($sections as $section) {
            if(stripos($section, '<body') === 0) {
                $body_index = $index;
                break;
            }
            ++$index;
        }

        if(!$body_index) {
            print($text);
        }
        else {
            $index = 0;
            foreach($sections as $section) {
                print($section);
                if($index == 0) {
                    print '<link rel="icon" href="/favicon.ico" type="image/ico"'.$tag_end;
                    print '<link rel="stylesheet" type="text/css" href="/style-v2/section-basic.css"'.$tag_end;
                }
                else if($index == $body_index) {
                    virtual("/common/heading-doc.html");
                }
                ++$index;
            }
        }
    }
}

/* File Not Found */

function file_not_found($file, $message = null)
{
    header("HTTP/1.0 404 Not Found");
    display_template(new file_not_found_render_callbacks($file, $message));
}

class file_not_found_render_callbacks
{
    var $file, $message;
    
    function file_not_found_render_callbacks($file, $message) {
        $this->file = $file;
        $this->message = $message;
    }

    function content_head()
    {
        print <<<HTML
  <meta http-equiv="Content-Type" content="text/html; charset=us-ascii" />
  <title>Boost C++ Libraries - 404 Not Found</title>
HTML;
    }
    
    function content()
    {
        print '<h1>404 Not Found</h1><p>File "' . $this->file . '" not found.</p>';
        if($this->message) {
            print '<p>Unzip error: '.htmlentities($this->message).'</p>';
        }
    }
}

function extract_html_body($text) {
    preg_match('@<body[^>]*>@i',$text,$body_begin,PREG_OFFSET_CAPTURE);
    preg_match('@</body>@i',$text,$body_end,PREG_OFFSET_CAPTURE);
    if (!isset($body_begin[0]))
    {
        //~ Attempt to recover some content from illegal HTML that is missing the body tag.
        preg_match('@</head>@i',$text,$body_begin,PREG_OFFSET_CAPTURE);
    }
    if (!isset($body_begin[0]))
    {
        //~ Attempt to recover some content from illegal HTML that is missing the body tag.
        preg_match('@<html[^>]*>@i',$text,$body_begin,PREG_OFFSET_CAPTURE);
    }
    if (!isset($body_begin[0]))
    {
        //~ Attempt to recover some content from illegal HTML that is missing the body tag.
        preg_match('@<(hr|div|img|p|h1|h2|h3|h4)[^>]*>@i',$text,$body_begin,PREG_OFFSET_CAPTURE);
    }
    if (!isset($body_begin[0]))
    {
        return;
    }
    else if (!isset($body_end[0]))
    {
        $text = substr($text,
            $body_begin[0][1]+strlen($body_begin[0][0]));
    }
    else
    {
        $text = substr($text,
            $body_begin[0][1]+strlen($body_begin[0][0]),
            $body_end[0][1]-($body_begin[0][1]+strlen($body_begin[0][0])) );
    }

    return $text;
}

function prepare_html($text) {
    $text = preg_replace(
        '@href="?http://www.boost.org/?([^"\s]*)"?@i',
        'href="/${1}"',
        $text );
    $text = preg_replace(
        '@href="?http://boost.org/?([^"\s]*)"?@i',
        'href="/${1}"',
        $text );
    $text = preg_replace(
        '@href="?(?:\.\./)+people/(.*\.htm)"?@i',
        'href="/users/people/${1}l"',
        $text );
    $text = preg_replace(
        '@href="?(?:\.\./)+(LICENSE_[^"\s]*\.txt)"?@i',
        'href="/${1}"',
        $text );
    $text = preg_replace(
        '@<a\s+(class="[^"]+")?\s*href="?(http|mailto)(:[^"\s]*)"?@i',
        '<a class="external" href="${2}${3}"',
        $text );
    
    return $text;
}

function remove_html_banner($text) {

    # nasty code, because (?!fubar) causes an ICE...
    preg_match('@<table[^<>]*>?@i',$text,$table_begin,PREG_OFFSET_CAPTURE);
    preg_match('@</table>@i',$text,$table_end,PREG_OFFSET_CAPTURE);
    if (isset($table_begin[0]) && isset($table_end[0])) {
        $table_contents_start = $table_begin[0][1] + strlen($table_begin[0][0]);
        $table_contents = substr($text, $table_contents_start,
            $table_end[0][1] - $table_contents_start);

        if(strpos($table_contents, 'boost.png') !== FALSE) {
            preg_match('@<td[^<>]*>?([^<]*<(h[12]|p).*?)</td>@is', $table_contents,
                $table_contents_header, PREG_OFFSET_CAPTURE);
            
            $head = substr($text, 0, $table_begin[0][1]);
            $header = isset($table_contents_header[1]) ? $table_contents_header[1][0] : '';
            $tail = substr($text, $table_end[0][1] + strlen($table_end[0][0]));
            $tail = preg_replace('@^\s*<hr\s*/?>\s*@', '', $tail);
                
            $text = $head.$header.$tail;
            return $text;
        }
    }

    $parts = preg_split('@(?=<(p|blockquote))@', $text, 2);
    $header = $parts[0];
    $content = $parts[1];
    
    $header = preg_replace('@(<h\d>\s*)<img[^>]*src="(\.\.\/)*boost\.png"[^>]*>@', '$1', $header);    
    $header = preg_replace('@<img[^>]*src="(\.\.\/)*boost\.png"[^>]*>\s*<[hb]r.*?>@', '', $header);

    return $header.$content;
}

function prepare_themed_html($text) {
    $text = preg_replace(
        '@(<a[^>]+>[\s]*)?<img.*boost\.png[^>]*>([\s]*</a>)?@i',
        '',
        $text );
    $text = preg_replace(
        '@<img(.*)align="?right"?[^>]*>@i',
        '<img${1} class="right-inset" />',
        $text );
    $text = preg_replace(
        '@<img(.*)align="?absmiddle"?[^>]*>@i',
        '<img${1} class="inline" />',
        $text );
    /* Remove certain attributes */
    $text = preg_replace(
        '@[\s]+(border|cellpadding|cellspacing|width|height|valign|align|frame|rules|naturalsizeflag|background)=("[^"]*"?|\'[^\']*\'?|[^\s/>]+)@i',
        '',
        $text );
    $text = preg_replace(
        '@<table[\s]+(border)[^\s>]*@i',
        '<table',
        $text );
    $text = preg_replace(
        '@<[/]?(font|hr)[^>]*>@i',
        '',
        $text );
    $text = preg_replace(
        '@<([^\s]+)[\s]+>@i',
        '<${1}>',
        $text );
    $text = _preg_replace_bounds(
        '@<blockquote>[\s]*(<pre>)@i','@(</pre>)[\s]*</blockquote>@i',
        '${1}','${1}',
        $text );
    $text = _preg_replace_bounds(
        '@<blockquote>[\s]*(<p>)@i','@(</p>)[\s]*</blockquote>@i',
        '${1}','${1}',
        $text );
    $text = _preg_replace_bounds(
        '@<blockquote>[\s]*(<table>)@i','@(</table>)[\s]*</blockquote>@i',
        '${1}','${1}',
        $text );
    $text = _preg_replace_bounds(
        '@<blockquote>[\s]*<li>@i','@</li>[\s]*</blockquote>@i',
        '<ul><li>','</li></ul>',
        $text );
    $text = _preg_replace_bounds(
        '@(?:<blockquote>[\s]*)+<h2>@i','@</h2>(?:[\s]*</blockquote>)+@i',
        '<h2>','</h2>',
        $text );
    $text = preg_replace(
        '@(<a name=[^\s>]+[\s]*>)[\s]*(</?[^a])@i',
        '${1}</a>${2}',
        $text );
    $text = preg_replace(
        '@<table>([\s]+<tr>[\s]+<td>.*_arr.*</td>[\s]+<td>.*</td>[\s]+<td>.*</td>[\s]+</tr>[\s]+)</table>@i',
        '<table class="pyste-nav">${1}</table>',
        $text );
    $text = preg_replace(
        '@<table>([\s]+<tr>[\s]+<td)[\s]+class="note_box">@i',
        '<table class="note_box">${1}>',
        $text );
    $text = preg_replace(
        '@<table>([\s]+<tr>[\s]+<td[\s]+class="table_title">)@i',
        '<table class="toc">${1}',
        $text );
    $text = preg_replace(
        '@src=".*theme/u_arr\.gif"@i',
        'src="/gfx/space.png" class="up_image"',
        $text );
    $text = preg_replace(
        '@src=".*theme/l_arr\.gif"@i',
        'src="/gfx/space.png" class="prev_image"',
        $text );
    $text = preg_replace(
        '@src=".*theme/r_arr\.gif"@i',
        'src="/gfx/space.png" class="next_image"',
        $text );
    $text = preg_replace(
        '@src=".*theme/u_arr_disabled\.gif"@i',
        'src="/gfx/space.png" class="up_image_disabled"',
        $text );
    $text = preg_replace(
        '@src=".*theme/l_arr_disabled\.gif"@i',
        'src="/gfx/space.png" class="prev_image_disabled"',
        $text );
    $text = preg_replace(
        '@src=".*theme/r_arr_disabled\.gif"@i',
        'src="/gfx/space.png" class="next_image_disabled"',
        $text );
    $text = preg_replace(
        '@src=".*theme/note\.gif"@i',
        'src="/gfx/space.png" class="note_image"',
        $text );
    $text = preg_replace(
        '@src=".*theme/alert\.gif"@i',
        'src="/gfx/space.png" class="caution_image"',
        $text );
    $text = preg_replace(
        '@src=".*theme/bulb\.gif"@i',
        'src="/gfx/space.png" class="tip_image"',
        $text );
    $text = preg_replace(
        '@<img src=".*theme/(?:bullet|lens)\.gif">@i',
        '',
        $text );
    $text = preg_replace(
        '@(<img src=".*theme/(?:arrow)\.gif")>@i',
        '${1} class="inline">',
        $text );
    return $text;
}

// Display the content in the standard boost template

function display_template($callbacks) {
    $_file = $callbacks;
    include(dirname(__FILE__)."/template.php");
}

// Return a readable error message for unzip exit state.

function unzip_error($exit_status) {
    switch($exit_status) {
    case 0: return 'No error.';
    case 1: return 'One  or  more  warning  errors  were  encountered.';
    case 2: return 'A generic error in the zipfile format was detected.';
    case 3: return 'A severe error in the zipfile format was detected.';
    case 4: return 'Unzip was unable to allocate memory for one or more buffers during program initialization.';
    case 5: return 'Unzip was unable to allocate memory or unable to obtain a tty to read the decryption password(s).';
    case 6: return 'Unzip was unable to allocate memory during decompression to disk.';
    case 7: return 'Unzip was unable to allocate memory during in-memory decompression.';
    case 9: return 'The specified zipfile was not found.';
    case 10: return 'Invalid options were specified on the command line.';
    case 11: return 'No matching files were found.';
    case 50: return 'The disk is (or was) full during extraction.';
    case 51: return 'The end of the ZIP archive was encountered prematurely.';
    case 80: return 'The user aborted unzip prematurely with control-C (or similar).';
    case 81: return 'Testing or extraction of one or more files failed due to unsupported compression methods or unsupported decryption.';
    case 82: return 'No files were found due to bad decryption password(s).';
    default: return 'Unknown unzip error code: ' + $exit_status;
    }
}
?>
