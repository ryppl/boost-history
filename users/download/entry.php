<?php
require dirname(__FILE__) . '/../../common/code/feed.php';
$_downloads = new boost_feed(dirname(__FILE__) . '/../../downloads.rss', '/users/download');
$_guid = basename($_SERVER["PATH_INFO"]);
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
  <title>Boost Downloads</title>
  <meta http-equiv="Content-Type" content="text/html; charset=us-ascii" />
  <link rel="icon" href="/favicon.ico" type="image/ico" />
  <link rel="stylesheet" type="text/css" href="/style/section-boost.css" />
  <!--[if IE]> <style type="text/css"> body { behavior: url(/style/csshover.htc); } </style> <![endif]-->
</head>

<body>
  <div id="heading">
    <?php virtual("/common/heading.html"); ?>
  </div>

  <div id="body">
    <div id="body-inner">
      <div id="content">
        <div class="section" id="intro">
          <div class="section-0">
            <div class="section-title">
              <h1>Boost Downloads</h1>
            </div>

            <div class="section-body">
              <h2><span class=
              "news-title"><?php print $_downloads->db[$_guid]['title']; ?></span>
              <span class=
              "news-date"><?php print $_downloads->db[$_guid]['date']; ?></span></h2>

              <div class="news-description">
                <?php print $_downloads->db[$_guid]['description']; ?>
              </div>
            </div>
          </div>
        </div>
      </div>

      <div id="sidebar">
        <?php virtual("/common/sidebar-common.html"); ?><?php virtual("/common/sidebar-boost.html"); ?>
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
        <p>Copyright Rene Rivera 2006.</p>
      </div><?php virtual("/common/footer-license.html"); ?>
    </div>

    <div id="footer-right">
      <?php virtual("/common/footer-banners.html"); ?>
    </div>

    <div class="clear"></div>
  </div>
</body>
</html>
