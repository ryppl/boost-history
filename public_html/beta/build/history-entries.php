<?php

echo "History entries\n";
echo "===============\n";

require_once(dirname(__FILE__) . '/../common/code/boost_feed.php');
$_history = new boost_feed(dirname(__FILE__) . '/../feed/history.rss', '/users/history');
foreach($_history->db as $_guid => $_value) {
    echo "Building $_guid\n";

    ob_start();
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
  <title><?php print $_history->db[$_guid]['title']; ?></title>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <link rel="icon" href="/favicon.ico" type="image/ico" />
  <link rel="stylesheet" type="text/css" href="/style-v2/section-boost.css" />
  <style type="text/css">
/*<![CDATA[*/
  #content .news-description ul {
    list-style: none;
  }
  #content .news-description ul ul {
    list-style: circle;
  }
  /*]]>*/
  </style>
  <!--[if IE 7]> <style type="text/css"> body { behavior: url(/style-v2/csshover3.htc); } </style> <![endif]-->
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
              <h1><?php print $_history->db[$_guid]['title']; ?></h1>
            </div>

            <div class="section-body">
              <h2><span class=
              "news-title"><?php print $_history->db[$_guid]['title']; ?></span></h2>

              <p><span class=
              "news-date"><?php print $_history->db[$_guid]['date']; ?></span></p>

              <?php $_history->echo_download_table($_guid); ?>

              <div class="news-description">
                <?php print $_history->db[$_guid]['description']; ?>
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
        <p>Revised $<?php ?>Date$</p>
      </div>

      <div id="copyright">
        <p>Copyright Rene Rivera 2006-2007.</p>
      </div><?php virtual("/common/footer-license.html"); ?>
    </div>

    <div id="footer-right">
      <?php virtual("/common/footer-banners.html"); ?>
    </div>

    <div class="clear"></div>
  </div>
</body>
</html>
<?php
    $_page = ob_get_contents();
    ob_end_clean();
    
    file_put_contents(dirname(__FILE__) . "/../users/history/$_guid.html", $_page);
}

echo "\n";

?>
