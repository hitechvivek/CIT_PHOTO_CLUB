<?php

/**
 * @file
 * Orange_admin theme's implementation to display a single Drupal page.
 */
?>
  <?php print render($page['header']); ?>

  <div id="wrapper">
      <div id="header">
      <?php print $breadcrumb; ?>
        <?php if ($site_name): ?>
           <h4><a href="<?php print $front_page ?>" title="<?php print $site_name; ?>">
            <?php print $site_name; ?>
            </a></h4>
        <?php endif; ?>
      </div> <!-- end header -->
      
      <div id="container">
            <?php print render($title_prefix); ?>
            <?php if ($title): ?>
              <h1 class="title"><?php print $title; ?></h1>
            <?php endif; ?>
            <?php print render($title_suffix); ?>
            <?php print render($primary_local_tasks); ?>
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
          </div> <!-- end content -->
        

      </div> <!-- end container -->
    </div><!-- end wrapper -->

<?php print render($page['footer']); ?>