<?php
include_once('Istorage.php');

/**
 * This function MUST be implemented!!!
 * @return Istorage
 */
function fast_gallery_get_storage() {
  return new DefaultStorage();
}


class NodeStorage implements Istorage {
  /**
   * Return all images that are stored on a given path
   * 
   * @param $path String
   * @return array - an array of FGImages
   */
  public function getImages($path) {
    
  }
  
  /**
   * 
   * @param array $arImages
   *  an array of FGImages
   *  
   */
  public function storeImages($arImages) {
    
  }
}