<?php

function _gbuzz_user_make($values) {
  $account = array();
    
  if(!isset($values['uid'])) {
  	global $user;
    $values['uid'] = $user->uid;
  }

  $account['id']                 = $values['id'];
  $account['uid']                = $values['uid'];
  $account['kind']               = $values['kind'];
  $account['about']              = $values['aboutMe'];
  $account['display_name']       = $values['displayName'];
  $account['profile_url']        = $values['profileUrl'];
  $account['thumbnail_url']      = $values['thumbnailUrl'];
  $account['emails']             = serialize($values['emails']);
  $account['urls']               = serialize($values['urls']);
  $account['photos']             = serialize($values['photos']);
  $account['organizations']      = serialize($values['organizations']);
  $account['interests']          = serialize($values['interests']);
  $account['oauth_token']        = $values['oauth_token'];
  $account['oauth_token_secret'] = $values['oauth_token_secret'];
  $account['import']             = isset($values['import'])? $values['import']: 1;
  
  return $account;
}

function gbuzz_account_save($account) {
	$values = (array)$account;

  if (db_result(db_query("SELECT 1 FROM {gbuzz_account} WHERE id = %n", $values['id']))) {
    drupal_write_record('gbuzz_account', $values, array('id'));
  }
  else {
    drupal_write_record('gbuzz_account', $values);
  }    
}

function gbuzz_account_load($params, $reset = FALSE) {
  module_load_include('lib.php', 'gbuzz');
	static $accounts = array();
  if ($reset) {
  	$accounts = array();
  }  
  if (is_numeric($params)) {
  	if(isset($accounts[$params])) {
  		return $accounts[$params];
  	}
    $cond = 'id = %n';
    $args[] = $params;
  }
  else if(is_array($params)) {
  	foreach ($params as $key => $value) {
      if ($key == 'id') {
      	$cond[] = db_escape_table($key) . "= %n";
        $args[] = $value;
      }
      else if(in_array($key, array('uid', 'is_global', 'import'))) {
      	$cond[] = db_escape_table($key) . "= %d";
        $args[] = $value;
      }
      else {
  		  $cond[] = db_escape_table($key) . "= '%s'";
        $args[] = $value;
      }
  	}
    $cond = implode(' AND ', $cond);
  }
  else {
  	return FALSE;
  }
  $query = db_query("SELECT * FROM gbuzz_account WHERE ". $cond, $args);
  while ($gbuzz_account = db_fetch_array($query)) {
    $account[$gbuzz_account['id']] = $gbuzz_accounts[$gbuzz_account['id']] = new GBuzzUser($gbuzz_account);
  }

  return is_numeric($params)? $gbuzz_accounts[$params]: $gbuzz_accounts;
}

function gbuzz_get_accounts($account = NULL, $global = FALSE) {
	if (!$account) {
		global $user;
    $account = $user;
	}
  $sql = "SELECT * FROM {gbuzz_account} WHERE uid=%d ";
  
  if ($global && user_access('use global gbuzz account', $account)) {
    $sql .= " OR is_global=1";
  }
  $result = db_query($sql, $account->uid);
  while ($gbuzz_account = db_fetch_object($result)) {
    $accounts[$gbuzz_account->id] = $gbuzz_account;
  }
  return $accounts;  
}

function gbuzz_user_delete($id) {
	$sql = "DELETE FROM {gbuzz_account} WHERE id = %n";
  db_query($sql, $id);
}
/**
 *  @see http://code.google.com/apis/buzz/v1/using_rest.html#article-attachment
 */ 
function gbuzz_post_article($google_id, $title, $url) {
  $buzz_data = array(
    'data' => 
      array('object' =>
        array(
          'type' => 'note',
          'content' => $title,
          'attachments' => array(
            array(
              'type' => 'article',
              'links' => array(
                'alternate' => array(
                  array(
                    'href' => $url,
                  ),
                ),
              ),
            ), 
          ),
        ),
      ),
    );
  $json_data = json_encode($buzz_data);
  
  gbuzz_post_buzz($google_id, $json_data);  

}

function gbuzz_post_buzz($google_id, $data) {
  module_load_include('lib.php', 'oauth');
  module_load_include('lib.php', 'gbuzz');

  $account = gbuzz_account_load($google_id);

  $extra = array('data' => $data);
 
  $key = variable_get('gbuzz_consumer_key', '');
  $secret = variable_get('gbuzz_consumer_secret', '');
  $token = $account->get_oauth_tokens();
  
  $gbuzz = new GbuzzOAuth($key, $secret, $token['oauth_token'], $token['oauth_token_secret']);
  
  $path = "https://www.googleapis.com/buzz/v1/activities/@me/@self";
  
  
  $response = $gbuzz->call($path, array(), 'POST', $extra);  
  return $response;
}
