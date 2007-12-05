<?php
require_once(dirname(__FILE__) . '/../common/code/archive_file.php');

$_file = new archive_file('@^[/]([^/]+)[/](.*)$@',$_SERVER["PATH_INFO"],array(
  array('@^boost-build/index[.]html$@i','raw','text/html'),
  array('@[.](html|htm)$@i','boost_book_html','text/html')
  ),false,false);

if (!$_file->is_raw()) {
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
  <?php $_file->content_head(); ?>
  <link rel="icon" href="/favicon.ico" type="image/ico" />
  <link rel="stylesheet" type="text/css" href="/style/section-doc.css" />
  <!--[if IE]> <style type="text/css"> body { behavior: url(/style/csshover.htc); } </style> <![endif]-->
  <?php #~ pwn_head(); ?>
  <style type="text/css">
/*<![CDATA[*/
div.sidebar {
  margin: 0em 0em 1em 1em !important;
  width: 40%;
  float: right;
  clear: right;
}
  /*]]>*/
  </style>

</head><!-- <?php print $_file->file_; ?> -->

<body>
  <div id="heading">
    <?php virtual("/common/heading.html");?>
  </div>

  <div id="body">
    <div id="body-inner">
      <div id="content">
        <div class="section" id="docs">
          <div class="section-0">
            <div class="section-body">
              <?php $_file->content(); ?>
            </div>
          </div>
        </div>
      </div>

      <div class="clear"></div>
    </div>
  </div>

  <div id="footer">
    <div id="footer-left">
      <div id="revised">
        <p>Revised $Date$</p>
      </div>

      <div id="copyright">
        <p>Copyright Beman Dawes, David Abrahams, 1998-2005.</p>

        <p>Copyright Rene Rivera 2004-2007.</p>
      </div><?php virtual("/common/footer-license.html");?>
    </div>

    <div id="footer-right">
      <?php virtual("/common/footer-banners.html");?>
    </div>

    <div class="clear"></div>
  </div>
</body>
</html><?php } ?>