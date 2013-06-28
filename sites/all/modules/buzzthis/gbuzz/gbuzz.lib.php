<?php
// $Id: gbuzz.lib.php,v 1.1 2010/06/02 17:34:33 jasonleon Exp $

/**
 * Exception handling class.
 */
class GbuzzException extends Exception {
}

class Gbuzz {
  protected $format = 'json';
  protected $host = 'www.google.com';

  /**
   * Perform a GET/POST request
   */
  protected function request($url, $params = array(), $method = 'GET', $extra = array()) {
    $data = '';
    $headers = array();
    if (count($params) > 0) {
      if ($method == 'GET') {
        $url .= '?'. http_build_query($params, '', '&');
      }
      else {
        $data = $extra['data'];
        $headers = $extra['headers'];
        if (!isset($headers['Content-type'])) {
        	$headers['Content-type'] = 'application/json';
        }
      }
    }

    $response = drupal_http_request($url, $headers, $method, $data);
//    drupal_set_message(print_r($response,1));
    if (!$response->error) {
      return $response->data;
    }
    else {
      $error = $response->error;
      $data = $this->parse_response($response->data);
      if ($data['error']) {
        $error = $data['error'];
      }
//      throw new GbuzzException($error);
    }
  }

  protected function parse_response($response, $format = NULL) {
    if (empty($format)) {
      $format = $this->format;
    }

    switch ($format) {
      case 'json':
        return json_decode($response, TRUE);
    }
  }
  

  protected function create_url($path) {
    $url =  'https://'. $this->host .'/'. $path;
    return $url;
  }
    
}

class GbuzzOAuth extends Gbuzz {
	protected $signature_method;
  protected $consumer;
  protected $token;
  
  public function __construct($consumer_key, $consumer_secret, $oauth_token = NULL, $oauth_token_secret = NULL) {
    $this->signature_method = new OAuthSignatureMethod_HMAC_SHA1();
    $this->consumer = new OAuthConsumer($consumer_key, $consumer_secret);
    if (!empty($oauth_token) && !empty($oauth_token_secret)) {
      $this->token = new OAuthConsumer($oauth_token, $oauth_token_secret);
    }
  }
  
  public function get_request_token($oauth_callback = NULL) {
    $params = array();
    if (!empty($oauth_callback)) {
    	$params = array('oauth_callback' => $oauth_callback);
    }
 
    $url = $this->create_url('accounts/OAuthGetRequestToken');
    $params += array('scope' => 'https://www.googleapis.com/auth/buzz');
 
    try {
      $response = $this->auth_request($url, $params);
    }
    catch (GbuzzException $e) {
    }
 
    parse_str($response, $token);
    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
    return $token;
  }

  public function get_access_token($oauth_verifier = FALSE) {
    $params = array();
 
    if (!empty($oauth_verifier)) {
      $params['oauth_verifier'] = $oauth_verifier;
    }
    
    $url = $this->create_url('accounts/OAuthGetAccessToken');
    try {
      $response = $this->auth_request($url, $params);
    }
    catch (GbuzzException $e) {
    }
 
    parse_str($response, $token);
 
    $this->token = new OAuthConsumer($token['oauth_token'], $token['oauth_token_secret']);
 
    return $token;
 
  }
  
  /**
   * Method for calling any twitter api resource
   */
  public function call($path, $params = array(), $method = 'GET', $extra = array()) {
    try {
      $response = $this->auth_request($path, $params, $method, $extra);
    }
    catch (GbuzzException $e) {
      return FALSE;
    }

    if (!$response) {
      return FALSE;
    }
    
    return $this->parse_response($response);
  }

  public function auth_request($url, $params = array(), $method = 'GET', $extra = array()) {
    
    $request = OAuthRequest::from_consumer_and_token($this->consumer, $this->token, $method, $url, $params);
    $request->sign_request($this->signature_method, $this->consumer, $this->token);
    switch ($method) {
      case 'GET':
        return $this->request($request->to_url());
      case 'POST':
	     $headers['Authorization'] = substr($request->to_header(), 14);
	     $extra += array('headers' => $headers);
       return $this->request($request->get_normalized_http_url(), $request->get_parameters(), 'POST', $extra);
    }
  }
  
  public function get_authorize_url($token) {
    global $base_url;
    
    $url = $this->create_url('accounts/OAuthAuthorizeToken');
    $url .= '?oauth_token=' . $token['oauth_token'];
    $url .= '&scope=https://www.googleapis.com/auth/buzz&domain='.$base_url;
    return $url;
  }
}

class GBuzzUser {
  
  public $id;
  public $uid;
  public $kind;
  public $about;
  public $display_name;
  public $profile_url;
  public $thumbnail_url;
  public $emails;
  public $urls;
  public $photos;
  public $organizations;
  public $interests;
  public $is_global;
  public $import;
  protected $oauth_token;
  protected $oauth_token_secret;
  
  public function __construct($values = array()) {

    $this->id = $values['id'];
    $this->uid = $values['uid'];
    $this->kind = $values['kind'];
    $this->about = $values['about'];
    $this->display_name = $values['display_name'];
    $this->profile_url = $values['profile_url'];
    $this->thumbnail_url = $values['thumbnail_url'];
    $this->emails = unserialize($values['emails']);
    $this->urls = unserialize($values['urls']);
    $this->photos = unserialize($values['photos']);
    $this->organizations = unserialize($values['organizations']);
    $this->interests = unserialize($values['interests']);
    $this->oauth_token = $values['oauth_token'];
    $this->is_global = $values['is_global'];
    $this->import = $values['import'];
    $this->oauth_token_secret = $values['oauth_token_secret'];
    
  }
  public function get_oauth_tokens() {
    return array('oauth_token' => $this->oauth_token, 'oauth_token_secret' => $this->oauth_token_secret);
  }
}
