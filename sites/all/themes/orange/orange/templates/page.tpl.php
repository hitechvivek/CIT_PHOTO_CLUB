<?php

/**
 * @file
 * Orange theme's implementation to display a single Drupal page.
 */
?>
  <?php print render($page['header']); ?>
  <div id="wrapper">
    <div id="header-top">
      <?php if($site_slogan): ?>
        <span id="slogan">
          <?php print $site_slogan; ?>
        </span>
      <?php endif; ?>
      <span id="member-login">
        <?php if(isset($signup)) print $signup; ?>
       </span>
    </div><!-- end header-top -->

      <div id="header">
        <div id="logo">
        <?php if ($logo): ?>
          <a href="<?php print $front_page ?>" title="<?php print $site_title; ?>">
             <img src="<?php print $logo ?>" alt="<?php print $site_title; ?>" />
          </a>
        <?php endif; ?>
        <?php if ($site_name): ?>
           <h1 id="site-name"><a href="<?php print $front_page ?>" title="<?php print $site_name; ?>">
            <?php print $site_name; ?>
            </a></h1>
        <?php endif; ?>
        </div> <!-- end logo -->
        <?php if ($page['nav']): ?>
          <div id="nav">
            <?php print render($page['nav']); ?>
          </div>
        <?php endif; ?><!-- end nav -->
      </div> <!-- end header -->
      
      <div id="container">
        <?php if ($page['content_top']): ?>
          <div id="content-top">
            <?php print render($page['content_top']); ?>
          </div>
        <?php endif; ?><!-- end content top -->

        <div id="main">
          <?php print $breadcrumb; ?>
          <?php if($page['content_inner']): ?>
            <div id="content-inner">
              <?php print render($page['content_inner']); ?>
              <div class="clear"> </div>
            </div>
          <?php endif; ?><!-- end content inner -->

          <?php if ($page['highlight']): ?>
            <div id="highlight">
              <?php print render($page['highlight']); ?>
            </div>
          <?php endif; ?> <!-- end highlight -->
          <a name="main-content" id="main-content"></a>
          <?php if ($title): ?>
            <h1 class="title"><?php print $title; ?></h1>
          <?php endif; ?> <!-- end title -->

          <?php if ($tabs): ?>
            <div id="tabs">
            <?php if ($tabs): ?>
              <?php print render($tabs) ?>
            <?php endif; ?>
            </div> <!-- end tabs -->
          <?php endif; ?>

          <div id="content">
          <?php print $messages; ?>
          <?php print render($page['help']); ?>
          <?php if ($action_links): ?>
            <ul class="action-links"><?php print render($action_links); ?></ul>
          <?php endif; ?>
            <?php print render($page['content']); ?>
            <?php print $feed_icons ?>
          </div> <!-- end content -->
        </div><!-- end main -->
        
         <?php if($page['sidebar_first']): ?>
            <div id="sidebar">
              <?php print render($page['sidebar_first']); ?>
            </div>
          <?php endif; ?><!-- end sidebar-->
          
        <div class="clear">&nbsp; </div>
        <?php if($page['content_bottom']): ?>
        	<div id="content-bottom">
        		<?php print render($page['content_bottom']); ?>
        		<div class="clear"> &nbsp;</div>
        	</div>
        <?php endif; ?>
      </div> <!-- end container -->
    </div><!-- end wrapper -->

    <div id="footer">
      <div id="footerwrap">
        <?php if ($page['content_footer']): ?>
          <div id="content-footer">
            <?php print render($page['content_footer']); ?>
            <div class="clear">&nbsp;</div>
          </div>
        <?php endif; ?>
      </div> <!-- end footerwrap -->
    </div> <!-- end footer -->

<?php print render($page['footer']) ?>