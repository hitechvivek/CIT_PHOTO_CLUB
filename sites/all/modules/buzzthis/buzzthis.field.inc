<?php

/**
 * Implementation of hook_field_info()
 */
function buzzthis_field_info() {
  return array(
    'buzzbutton' => array(
      'label' => t('Buzz Button'),
      'description' => t('This field display Google Buzz button on entities'),
      'default_widget' => 'buzzthis_icon',
      'default_formatter' => 'buzzthis_formatter_normal',
    ),
  );
}

/**
 * Implementation of hook_field_widget_info()
 */
function buzzthis_field_widget_info() {
  return array(
    'buzzthis_icon' => array(
      'label' => t('Buzzthis Icon'),
      'field types' => array('buzzbutton'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_DEFAULT,
        'default values' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implementaion of hook_field_formatter_info()
 */
function buzzthis_field_formatter_info() {
  return array(
    'buzzthis_formatter_normal' => array(
      'label' => t('Normal Counter+Icon'),
      'field types' => array('buzzbutton'),
    ),
    'buzzthis_formatter_small' => array(
      'label' => t('Small Counter+icon'),
      'field types' => array('buzzbutton'),
    ),
  );
}

/**
 * Implementation of hook_field_formatter_view()
 */
function buzzthis_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  switch ($display['type']) {
    case 'buzzthis_formatter_normal':
      $entity_uri = entity_uri($entity_type, $entity);
      $style = array(
        'url' => url($entity_uri['path'], array('absolute' => TRUE)),
        'type' => 'normal-count',
        'langcode' => $langcode == LANGUAGE_NONE? language_default('language'): $langcode,
      );
      
      $element = array(
        '#theme' => 'buzzthis_button',
        '#style' => $style, 
      );
    break;
    
    case 'buzzthis_formatter_small':
      $entity_uri = entity_uri($entity_type, $entity);
      
      $style = array(
        'url' => url($entity_uri['path'], array('absolute' => TRUE)),
        'type' => 'small-count',
        'langcode' => $langcode == LANGUAGE_NONE? language_default('language'): $langcode,
      );
      
      $element = array(
        '#theme' => 'buzzthis_button',
        '#style' => $style,
      );

      break; 
  }
  
  return $element;
}

/**
 * Implementation of hook_field_access()
 */
function buzzthis_field_access($op, $field, $entity_type, $entity, $account) {
  if ($field['type'] == 'buzzbutton' && $op == 'view') {
    return user_access('access buzz button', $account);
  }
  return TRUE;
}
