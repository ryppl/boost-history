<?php
/*
  Copyright 2005-2006 Redshift Software, Inc.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
*/
require_once(dirname(__FILE__) . '/boost_config.php');


function _preg_replace_bounds($front_regex,$back_regex,$front_replace,$back_replace,$text)
{
    $offset = 0;
    $result = '';
    while (TRUE)
    {
        $subject = substr($text,$offset);
        if (preg_match($front_regex,$subject,$begin,PREG_OFFSET_CAPTURE) == 0 ||
            preg_match($back_regex,$subject,$end,PREG_OFFSET_CAPTURE,
                $begin[0][1]+strlen($begin[0][0])) == 0
            )
        { break; }
        else
        {
            $result .= substr($subject,0,$begin[0][1]);
            $result .= preg_replace($front_regex,$front_replace,$begin[0][0]);
            $result .= substr(
                $subject,
                $begin[0][1]+strlen($begin[0][0]),
                $end[0][1]-($begin[0][1]+strlen($begin[0][0])) );
            $result .= preg_replace($back_regex,$back_replace,$end[0][0]);
            $offset += $end[0][1]+strlen($end[0][0]);
        }
    }
    if ($result === '') { return $text; }
    else { return $result . substr($text,$offset); }
}

class boost_irc_stats
{
    var $head_content_ = NULL;
    var $content_ = NULL;
    
    function boost_irc_stats($uri)
    {
        $context = NULL;
        $this->content_ = file_get_contents($uri,false,$context);
        
        if ($this->content_ && $this->content_ != '')
        {
            $this->_init_html();
        }
        
        $this->head_content_ .= <<<HTML
  
  <!-- IRC STATS URI == '${uri}' -->
  
HTML
            ;
    }
    
    function content_head()
    {
        print $this->head_content_;
    }
    
    function content()
    {
        if ($this->content_ && $this->content_ != '')
        {
            $this->_content_html();
        }
    }

    function _init_html()
    {
        preg_match('@text/html; charset=([^\s"]+)@i',$this->content_,$charset);
        if (isset($charset[1]))
        {
            $this->head_content_ .= <<<HTML
  <meta http-equiv="Content-Type" content="text/html; charset=${charset[1]}" />
HTML
                ;
        }
        else
        {
            $this->head_content_ .= <<<HTML
  <meta http-equiv="Content-Type" content="text/html; charset=us-ascii" />
HTML
                ;
        }
        
        preg_match('@<title>([^<]+)</title>@i',$this->content_,$title);
        if (isset($title[1]))
        {
            $this->head_content_ .= <<<HTML
  <title>Boost C++ Libraries - ${title[1]}</title>
HTML
                ;
        }
        else
        {
            $this->head_content_ .= <<<HTML
  <title>Boost C++ Libraries - Wiki</title>
HTML
                ;
        }
    }
    
    function _content_html()
    {
        $text = $this->content_;
        
        $text = preg_replace(
            '@href="?http://www.boost.org/?([^"\s]*)"?@i',
            'href="/${1}"',
            $text );
        $text = preg_replace(
            '@href="?(?:\.\./)+people/(.*\.htm)"?@i',
            'href="/users/people/${1}l"',
            $text );
        $text = preg_replace(
            '@href="?(?:\.\./)+(LICENSE_.*\.txt)"?@i',
            'href="/${1}"',
            $text );
        $text = preg_replace(
            '@<a\s+(class="[^"]+")?\s*href="?(http|mailto)(:[^"\s]*)"?@i',
            '<a class="external" href="${2}${3}"',
            $text );
        
        preg_match('@<body>@i',$text,$body_begin,PREG_OFFSET_CAPTURE);
        preg_match('@</body>@i',$text,$body_end,PREG_OFFSET_CAPTURE);
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
        
        $text = preg_replace(
            '@<[/]?(font|hr)[^>]*>@i',
            '',
            $text );
        $text = preg_replace(
            '@[\s]+(border|cellpadding|cellspacing|width|height|valign|align|frame|rules|naturalsizeflag|background|wrap)=[^\s>]+@i',
            '',
            $text );
        
        print $text;
    }
}
?>
