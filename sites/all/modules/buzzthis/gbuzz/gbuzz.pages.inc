<?php
// $Id: gbuzz.pages.inc,v 1.1 2010/06/02 17:34:33 jasonleon Exp $

function gbuzz_oauth_callback(&$form_state) {
  $form['#post']['oauth_token'] = $_GET['oauth_token'];

  $form['oauth_token'] = array(
    '#type' => 'hidden',
    '#default_value' => $_GET['oauth_token'],
  );
  
  $form['oauth_verifier'] = array(
    '#type' => 'hidden',
    '#default_value' => $_GET['oauth_verifier'],
  );
  
  return $form;
}

function gbuzz_oauth_callback_validate($form, &$form_state) {
  $key = variable_get('gbuzz_consumer_key', '');
  $secret = variable_get('gbuzz_consumer_secret', '');

  $form_state['gbuzz_oauth'] = $_SESSION['gbuzz_oauth'];
  unset($_SESSION['gbuzz_oauth']);

  $token = $form_state['gbuzz_oauth']['token'];
  if (!is_array($token) || !$key || !$secret) {
    form_set_error('oauth_token', t('Invalid Google Buzz OAuth request'));
  }

  if ($token['oauth_token'] != $form_state['values']['oauth_token']) {
    form_set_error('oauth_token', t('Invalid OAuth token.'));
  }

  module_load_include('lib.php', 'oauth');
  module_load_include('lib.php', 'gbuzz');


  $buzz = new GbuzzOAuth($key, $secret, $token['oauth_token'], $token['oauth_token_secret']);  
  $response = $buzz->get_access_token($form_state['values']['oauth_verifier']);
  $form_state['gbuzz_oauth']['response'] = $response;
}

function gbuzz_oauth_callback_submit(&$form, &$form_state) {
  
  module_load_include('inc', 'gbuzz');

  $key = variable_get('gbuzz_consumer_key', '');
  $secret = variable_get('gbuzz_consumer_secret', '');
  $tokens = $form_state['gbuzz_oauth']['response'];
  $uid = $form_state['gbuzz_oauth']['uid'];
  
  $gbuzz = new GbuzzOAuth($key, $secret, $tokens['oauth_token'], $tokens['oauth_token_secret']);
  $path = 'https://www.googleapis.com/buzz/v1/people/@me/@self';
  $response = $gbuzz->call($path, array('alt' => "json"), 'GET', TRUE);
  $response['data']['uid'] = $uid;
  $response['data'] += $tokens;
  
  $gbuzzuser = _gbuzz_user_make($response['data']);
  gbuzz_account_save($gbuzzuser);
  $form_state['redirect'] = $form_state['gbuzz_oauth']['destination'];
  $form['#programmed'] = FALSE;
}


function gbuzz_user_make_global($form_state, $account, $buzz_id) {
  module_load_include('inc', 'gbuzz');
  $buzz_account = gbuzz_account_load($buzz_id);

  $form = array();

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['buzz_id'] = array(
    '#type' => 'value',
    '#value' => $buzz_id,
  );

  if ($buzz_account->is_global) {
    $text = t('Are you sure you want to remove %display_name from the global accounts?', array('%display_name' => $buzz_account->display_name));
    $description = t('This means other users will no longer be allowed to post using this account.');
  }
  else {
    $text = t('Are you sure you want to allow other users to access the %display_name account?', array('%display_name' => $buzz_account->display_name));
    $description = t('This will allow other users to post using this account.');
  }

  return confirm_form($form, $text, 'user/'. $account->uid .'/edit/gbuzz', $description); 
}
function gbuzz_user_make_global_submit($form, &$form_state) {
  db_query("UPDATE {gbuzz_account} SET is_global = (1 - is_global) WHERE id = %n", $form_state['values']['buzz_id']);
  $form_state['redirect'] = 'user/'. $form_state['values']['uid'] .'/edit/gbuzz';
}


function gbuzz_user_settings($account) {
	module_load_include('inc', 'gbuzz');
  $output = '';
  $output .= drupal_get_form('gbuzz_add_account', $account);
  $gbuzz_accounts = gbuzz_account_load(array('uid' => $account->uid));

  if (!empty($gbuzz_accounts)) {
    $output .= drupal_get_form('gbuzz_account_list_form', $gbuzz_accounts);
  }
  return $output;
}

function gbuzz_account_list_form($form_state, $gbuzz_accounts = array()) {
  $form['#tree'] = TRUE;
  $form['accounts'] = array();

  foreach ($gbuzz_accounts as $gbuzz_account) {
    $form['accounts'][] = _gbuzz_account_list_row($gbuzz_account);
  }

  if (!empty($gbuzz_accounts)) {
    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
    );
  }
  return $form;
}
function _gbuzz_account_list_row($account) {
  $form['#account'] = $account;

  $form['id'] = array(
    '#type' => 'value',
    '#value' => $account->id,
  );

  $form['thumbnail_url'] = array(
    '#type' => 'markup',
    '#value' => theme('image', $account->thumbnail_url, '', '', array(), FALSE),
  );

  $form['display_name'] = array(
    '#type' => 'markup',
    '#value' => l($account->display_name, $account->profile_url),
  );

  $form['about'] = array(
    '#type' => 'markup',
    '#value' => filter_xss($account->about),
  );

  $form['import'] = array(
    '#type' => 'checkbox',
    '#default_value' => $account->import,
  );
  
  $form['delete'] = array(
    '#type' => 'checkbox',
  );
  return $form;
}

function theme_gbuzz_account_list_form($form) {
  $header = array('', t('Name'), t('Bio'), t('Import'), t('Delete')); 
  if (user_access('make buzz accounts global')) {
    $header[] = t('Global');
  }
  $rows = array();

  foreach (element_children($form['accounts']) as $key) {
    $element = &$form['accounts'][$key];
    $row = array(
      drupal_render($element['thumbnail_url']),
      drupal_render($element['id']) . drupal_render($element['display_name']),
      drupal_render($element['about']),
      drupal_render($element['import']),
      drupal_render($element['delete']),
    );
    if (user_access('make buzz accounts global')) {
      $label = ($element['#account']->is_global) ? t('remove global') : t('make global');
      $row[] = l($label, 'user/'. $element['#account']->uid .'/edit/gbuzz/global/'. $element['#account']->id);
    }     
    $rows[] = $row;
  }

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

function gbuzz_account_list_form_submit($form, &$form_state) {
  $accounts = $form_state['values']['accounts'];
  foreach($accounts as $account) {
    if (empty($account['delete'])) {
      gbuzz_account_save($account);
    }
    else {
      gbuzz_user_delete($account['id']);
    }
  }
}


function gbuzz_add_account(&$form_state, $account = NULL) {
	if (empty($account)) {
    global $user;
    $account = $user;
  }

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  if (_gbuzz_is_oauth_active()) {
    $form['#validate'] = array('gbuzz_add_account_oauth_validate');
  }
  else {
  	$form['google_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Google profile ID'),
      '#description' => t('Your Google Buzz Id or username (without @gmail.com). Don\'t know your profile URL? <a href="@url">Click here</a>.', array('@url' => 'http://profiles.google.com/me')),
    );
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add account'),
  );
  return $form;
}

function gbuzz_add_account_validate($form, &$form_state) {
	$google_id = $form_state['values']['google_id'];
  
  if(!empty($google_id)) {
	  gbuzz_validate_buzzid($google_id);	
	}
}

function gbuzz_validate_buzzid($id) {
  form_set_error('google_id', t('Username not valid'));
}

function gbuzz_add_account_oauth_validate($form, &$form_state) {
  module_load_include('lib.php', 'oauth');
  module_load_include('lib.php', 'gbuzz');
  
  $key = variable_get('gbuzz_consumer_key', '');
  $secret = variable_get('gbuzz_consumer_secret', '');
  $uid = $form_state['values']['uid'];
  $oauth_callback = url('gbuzz/oauth', array('absolute' => TRUE));
  $gbuzz = new gbuzzOAuth($key, $secret);
  $token = $gbuzz->get_request_token($oauth_callback);
  
  $_SESSION['gbuzz_oauth']['token'] = $token;
  $_SESSION['gbuzz_oauth']['uid'] = $uid;
  $_SESSION['gbuzz_oauth']['destination'] = $_GET['q'];
  drupal_goto($gbuzz->get_authorize_url($token));

}


function buzzthis_admin_oauth() {
  $form = array();
    
  $form['oauth'] = array(
    '#title' => t('OAuth Settings'),
    '#type' => 'fieldset',
    '#description' => t('To enable OAuth, you must <a href="@url">register your application</a> and add the required keys here.', array('@url' => 'https://www.google.com/accounts/ManageDomains')),   
  );
  
  $form['oauth']['callback_url'] = array(
    '#type' => 'item',
    '#title' => t('Callback URL'),
    '#value' => url('gbuzz/oauth', array('absolute' => TRUE)),
    '#description' => 'Use this as a callback url for your Oauth response',
  );
  
  $form['oauth']['gbuzz_consumer_key'] = array(
    '#title' => t('Consumer Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('gbuzz_consumer_key', ''),
    '#description' => t('Enter the consumer key provided by Google, which will be same as your domain name'),
  );
  
  $form['oauth']['gbuzz_consumer_secret'] = array(
    '#title' => t('Consumer Secret'),
    '#type' => 'textfield',
    '#default_value' => variable_get('gbuzz_consumer_secret', ''),
  );
  
  return system_settings_form($form);  
}
