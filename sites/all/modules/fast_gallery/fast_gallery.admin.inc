<?php
// $Id: 

function _fast_gallery_gallery_form($key, $chid = NULL, $value = '', $title = '', $size = 10, $alias = '') {
  $form = array(
    '#tree' => TRUE,
  );
  // We'll manually set the #parents property of these fields so that
  // their values appear in the $form_state['values']['choice'] array.
  $form['chid'] = array(
    '#type' => 'value',
    '#value' => $chid,
    '#parents' => array('a_gallery', $key, 'chid'),
  );

  $form['fg_path'] = array(
    '#type' => 'textfield',
    '#default_value' => $value,
    '#size' => 30,
    '#parents' => array('a_gallery', $key, 'fg_path'),
  );

  $form['fg_alias'] = array(
    '#type' => 'textfield',
    '#default_value' => $alias,
    '#size' => 30,
    '#parents' => array('a_gallery', $key, 'fg_alias'),
  );

  $form['fg_title'] = array(
    '#type' => 'textfield',
    '#default_value' => $title,
    '#size' => 30,
    '#parents' => array('a_gallery', $key, 'fg_title'),
  );

  /*$form['weight'] = array(
   '#type' => 'weight',
   '#default_value' => $weight,
   '#delta' => $size,
   '#parents' => array('a_gallery', $key, 'weight'),
   );*/

  return $form;
}

/**
 * Callback function for admin settings
 */
function fast_gallery_general_settings_form($form, &$form_state) {
  $gallery_default = variable_get('fg_galleries', array());

  // Add a wrapper for the choices and more button.
  $form['gallery_wrapper'] = array(
    '#tree' => FALSE,
    '#weight' => -4,
    '#prefix' => '<div class="clearfix" id="fg-galleries-wrapper">',
    '#suffix' => '</div>',
  );

  // Container for just the poll choices.
  $form['gallery_wrapper']['a_gallery'] = array(
    '#prefix' => '<div id="fg-gallery">',
    '#suffix' => '</div>',
    '#theme' => 'fg_multiple_galleries',
  );

  // Add the current choices to the form.
  $delta = 0;
  $weight = 0;
  if (!isset($form_state['values']['a_gallery'])) {
    $gallery_count = 1;
  }
  else {
    $gallery_count = sizeof($form_state['values']['a_gallery']);
    $ar_galleries = $form_state['values']['a_gallery'];
  }

  if (isset($gallery_default)) {
    $delta = count($gallery_default);
    $weight = -$delta;
    foreach ($gallery_default as $gid => $gal) {
      $key = 'chid:' . $gid;
      $form['gallery_wrapper']['a_gallery'][$key] = _fast_gallery_gallery_form($key, $gid, $gal['fg_path'], $gal['fg_title'], $gallery_count, $gal['fg_alias']);
      //$weight = ($gal['weight'] > $weight) ? $gal['weight'] : $weight;
    }
  }

  $key = 'new:' . ($delta);
  $form['gallery_wrapper']['a_gallery'][$key] = _fast_gallery_gallery_form($key, NULL, '', '', $gallery_count, '');

  // We name our button 'poll_more' to avoid conflicts with other modules using
  // AJAX-enabled buttons with the id 'more'.
  //TODO: this still needs some work....
  /*$form['gallery_wrapper']['fg_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add gallery'),
    '#description' => t("If the amount of boxes above isn't enough, click here to add more choices."),
    '#weight' => 1,
    '#submit' => array(''), // If no javascript action.
    '#ajax' => array(
      'callback' => 'fast_gallery_ajax_handler',
      'wrapper' => 'fg-gallery',
      'method' => 'replace',
      'effect' => 'fade',
  ),
  );*/

  $form['fg_setup'] = array(
    '#type' => 'fieldset',
    '#title' => t('Setup'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $engines = module_invoke_all('fast_gallery_info');
  
  $ar_engines = array();
  foreach ($engines['storageengine'] as $engine) {
    $ar_engines[$engine['key']] = $engine['name'];
  }
  
  $form['fg_setup']['fg_storage_engine'] = array(
    '#type' => 'select',
    '#title' => t('Storage engine'),
    '#default_value' => variable_get('fg_storage_engine', FG_DEFAULT_STORAGE_ENGINE),
    '#options' => $ar_engines,
  );

  $js_framework_options = array();
  $js_framework_options[0] = t('<none>');
  $js_framework_default = 0;
  if (module_exists('thickbox'))
  $js_framework_options[1] = t('Thickbox');
  if (module_exists('lightbox2')) {
    $js_framework_default = 2;
    $js_framework_options[2] = t('Lightbox2 (no slideshow)');
    $js_framework_options[3] = t('Lightbox2 (with slideshow)');
  }

  $form['fg_setup']['fast_gallery_js_framework'] = array(
    '#type' => 'select',
    '#title' => t('Choose your Javascript framework'),
    '#options' => $js_framework_options,
    '#default_value' => variable_get('fast_gallery_js_framework', $js_framework_default),
    '#description' => t('Thickbox and Lightbox are the two options for displaying images.'),
  );

  $ic_link = l('Imagecache', 'admin/build/imagecache');
  $form['fast_gallery_operations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Operations'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => FALSE,
    '#description' => t('Remember to first click Save Settings (if you made any changes) before performing the below operations. Clear the database whenever you change the gallery path. Rescan the database whenever you add images, delete images, move images, or restructure the hierarchy.  Flush Internal Cache to delete 
      and force recaching of Internal Cache thumbnails aswell as the {cache_fast_gallery} db table. If you are using Imagecache, you need to flush 
      thumbnails on the Imagecache settings page: ') . $ic_link ,
  );

  $form['fast_gallery_operations']['fast_gallery_clear'] = array(
    '#type' => 'button',
    '#value' => t('Clear DB'),
    '#ajax' => array(
      'callback' => 'fast_gallery_ajax_handler',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array(
        'type' => 'bar',
        'message' => t('Clearing DB')
  ),
      'wrapper' => 'attach-wrapper',
  )
  );

  $form['fast_gallery_operations']['fast_gallery_rescan'] = array(
    '#type' => 'button',
    '#value' => t('Rescan'),
    '#ajax' => array(
      'callback' => 'fast_gallery_ajax_handler',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array(
        'type' => 'bar',
        'message' => t('Scan for new pictures...')
  ),
      'wrapper' => 'attach-wrapper',
  )
  );

  $form['fast_gallery_operations']['fast_gallery_flush_thumbs'] = array(
    '#type' => 'button',
    '#value' => t('Flush Internal Cache'),
    '#ajax' => array(
      'callback' => 'fast_gallery_ajax_handler',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array(
        'type' => 'bar',
        'message' => t('Flushing thumbnails')
  ),
      'wrapper' => 'attach-wrapper',
  )
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * validation function for the settinsg form
 * @param $form
 * @param $form_state
 */
function fast_gallery_general_settings_form_validate($form, &$form_state) {
  foreach ($form_state['values']['a_gallery'] as $key => $gallery) {
    $path = $gallery['fg_path'];

    //incase we have a path -> check if it's valid
    if (!is_dir($path) && $path != '') {
      form_error($form['gallery_wrapper']['a_gallery'][$key]['fg_path'], t('This path does not exist.'));
    }
    
    //incase there's a path make sure the path ends with a slash
    if ($path != '') {
      $last_token = substr($path, -1);
      if ($last_token != '/') {
        $path .= '/';
        $form_state['values']['a_gallery'][$key]['fg_path'] = $path;
      }
    }

    //incase there's a path -> check if there's an alias
    if ($form_state['values']['a_gallery'][$key]['fg_alias'] == '' && $form_state['values']['a_gallery'][$key]['fg_path'] != '') {
      form_error($form['gallery_wrapper']['a_gallery'][$key]['fg_alias'], t('Please enter an alias'));
    }
  }
}

/**
 * submitting the form making sure we save all the relevant information
 * @param $form
 * @param $form_state
 * @return unknown_type
 */
function fast_gallery_general_settings_form_submit($form, &$form_state) {
  $ar_galleries = array();
  foreach ($form_state['values']['a_gallery'] as $gallery) {
    if ($gallery['fg_path'] != '') {
      $ar_galleries[$gallery['fg_path']] = $gallery;
    }
  }
  variable_set('fg_galleries', $ar_galleries); //save the galleries
  
  $engines = module_invoke_all('fast_gallery_info');
  variable_set('fg_storage_engine', $engines['storageengine'][$form_state['values']['fg_storage_engine']]); //save the storageengine

  db_truncate('cache_fast_gallery')->execute();
  menu_rebuild();//prepare the paths for newly built galleries
}

/**
 * Ajax handler when hitting some of the buttons
 * @param $form
 * @param $form_state
 */
function fast_gallery_ajax_handler($form, $form_state) {
  $fg = fast_gallery_get_FastGallery();

  switch ($form_state['clicked_button']['#id']) {
    case 'edit-fast-gallery-flush-thumbs': //clear cache
      $cache = fast_gallery_get_cache();
      cache_clear_all('*', 'cache_fast_gallery', TRUE);
      $cache->flushThumbs();
      break;
    case 'edit-fast-gallery-rescan': //reindex images
      $fg->rescanGallery();
      break;
    case 'edit-fast-gallery-clear': //clear db
      $fg->clearDb();
      break;
    case 'edit-fg-more':
      $choice_form = $form['gallery_wrapper']['a_gallery'];
      return $choice_form;
      break;
  }
}

/**
 * theming the form for adding new galleries
 * @param $key
 * @param $chid
 * @param $value
 * @param $votes
 * @param $weight
 * @param $size
 * @return unknown_type
 */
function theme_fg_multiple_galleries($variables) {
  $form = $variables['form'];

  //drupal_add_tabledrag('fg-galleries-table', 'order', 'sibling', 'poll-weight');

  $delta = 0;
  $rows = array();
  $headers = array(
  //  '',
  t('Path to Gallery'),
  t('Path Alias'),
  t('Title of Gallery'),
  //t('Weight'),
  );

  foreach (element_children($form) as $key) {
    $delta++;
    // Set special classes for drag and drop updating.
    //$form[$key]['weight']['#attributes']['class'] = array('poll-weight');

    // Build the table row.
    $row = array(
      'data' => array(
    //array('class' => array('choice-flag')),
    drupal_render($form[$key]['fg_path']),
    drupal_render($form[$key]['fg_alias']),
    drupal_render($form[$key]['fg_title']),
    //drupal_render($form[$key]['weight']),
    ),
    //'class' => array('draggable'),
    );

    // Add any additional classes set on the row.
    if (!empty($form[$key]['#attributes']['class'])) {
      $row['class'] = array_merge($row['class'], $form[$key]['#attributes']['class']);
    }

    $rows[] = $row;
  }

  $output = theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => 'fg-galleries-table')));
  $output .= drupal_render_children($form);
  return $output;
}