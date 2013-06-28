<?php
//$Id:

/**
 * Callback function default storage engine
 * @return html
 */
function fast_gallery_default_config() {
  return drupal_get_form('fast_gallery_default_config_form');
}

/**
 * Form definition for default storage engine configuration
 * @return array
 */
function fast_gallery_default_config_form() {
  $items = array();
  $items['fg_folder_first'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display folders first'),
    '#default_value' => variable_get('fg_folder_first', FALSE),
    '#description' => t('If checked folders will be displayed first'),
  );
  
  $items['fg_folder_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Gallery Cache'),
    '#default_value' => variable_get('fg_folder_cache', TRUE),
    '#description' => t('If checked, the galleries will be cached. Cached will be cleared upon recreation of the gallery.'),
  );
  
  $items['fg_sort_order'] = array(
    '#type' => 'select',
    '#title' => t('Sort by'),
    '#options' => array('filename' => 'name', 'filecreated' => 'filecreation'),
    '#default_value' => variable_get('fg_sort_order', array('filename')),
    '#description' => t('Specify how to order your gallery. If you choose the filecreation
      option, it\'s only going to affect the images and not the folder sorting. '),
  );
  
  $items['fg_sort_direction'] = array(
    '#type' => 'select',
    '#title' => t('Sort by'),
    '#options' => array('asc' => 'ascending', 'desc' => 'descending'),
    '#default_value' => variable_get('fg_sort_direction', array('descending')),
    '#description' => t('Which direction should it be ordered'),
  );
  
  return system_settings_form($items);
}