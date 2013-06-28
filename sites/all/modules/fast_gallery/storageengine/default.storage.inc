<?php
//$Id:
include_once('Istorage.php');

/**
 * This function MUST be implemented!!!
 * @return Istorage
 */
function fast_gallery_get_storage() {
  return new DefaultStorage();
}

/**
 * This is the default storage engine for fast gallery. It is mainly
 * responsible for providing images and storing images. The default
 * engine uses a very simple concept of just storing them in a seperate
 * table in the db.
 * The default engine is supposed to be easy to use, fast and stable.
 */
class DefaultStorage implements Istorage {


/**
 * Returning a random image. This could be used for providing a block
 *
 * @param string $gallery
 *  the alias of a gallery to choose the image from
 * @return FGImage
 */
  public function getRandomImage() {
    $image = db_query("SELECT folder, filename FROM {fast_gallery_file} as f,
      {fast_gallery_hierarchy} AS h 
      WHERE f.foid = h.foid 
      ORDER BY RAND() LIMIT 0,1", array('fetch' => PDO::FETCH_ASSOC));

    foreach ($image as $item) {
      return new FGImage($item->folder . '/' . $item->filename);
    }
    return new FGImage('');
  }

  /**
   * Return all images that are stored on a given path
   *
   * @param $path String
   * @return array - an array of FGImages
   */
  public function getImages($path) {
  //incase we have a / at the end -> remove it.
    $arTmp = explode("/", $path);
    if ($arTmp[sizeof($arTmp)-1] == '') {
      array_pop($arTmp);
    }
    $folder = implode("/", $arTmp);
    $return = array();

    // we are going to cache the gallery content
    // so it will be served faster the next time
    if (($cache = cache_get('fast_gallery_' . $path, 'cache_fast_gallery')) && variable_get('fg_folder_cache', TRUE)) {
      $return = $cache->data;
    }
    else {

      $foid = $this->getFoid($folder);

      //incase we want to display the subfolder first
      if (variable_get('fg_folder_first', FALSE)) {
        $return = array_merge($return, $this->getFolders($foid));
      }

      $sort_order = variable_get('fg_sort_order', 'filename');
      $sort_direction = variable_get('fg_sort_direction', 'asc');

      //fetch images
      $row = db_query("SELECT filename, folder, fgf.foid FROM {fast_gallery_file} as fgf, {fast_gallery_hierarchy} as fgh
                     WHERE fgf.foid = fgh.foid 
                     AND fgh.folder = :folder 
                     ORDER BY $sort_order $sort_direction",
          array(':folder' => $folder), array('fetch' => PDO::FETCH_ASSOC));

      foreach ($row as $item) {
        $return[] = new FGImage($item['folder'] . '/' . $item['filename']);
      }

      //incase we display the subfolders last
      if (!variable_get('fg_folder_first', FALSE)) {
        $return = array_merge($return, $this->getFolders($foid));
      }

      // just incase ... store cache
      if (variable_get('fg_folder_cache', TRUE)) {
        cache_set('fast_gallery_' . $path, $return, 'cache_fast_gallery');
      }
    }
    return $return;
  }

  private function getFoid($folder) {
    $foid = db_query("SELECT foid FROM {fast_gallery_hierarchy}
                          WHERE folder=:folder 
                          ORDER BY folder", array(':folder' => $folder))->fetchField();
    return $foid;
  }

  /**
   * Get the subfolders of a given folder
   * @param int $foid
   * @return array
   */
  private function getFolders($foid) {
    $sort_order = variable_get('fg_sort_order', 'filename');
    $sort_direction = variable_get('fg_sort_direction', 'asc');

    switch($sort_order) {
      default:
        $sort_order = 'folder';
        break;
    }

    $folders = array();
    $row = db_query("SELECT folder FROM {fast_gallery_hierarchy}
                     WHERE pid = :foid 
                     ORDER BY $sort_order $sort_direction", 
        array(':foid' => $foid), array('fetch' => PDO::FETCH_ASSOC));
    foreach ($row as $folder) {
      $folders[] = new FGImage($folder['folder'], array('dir' => TRUE));
    }
    return $folders;
  }

  /**
   * implementing the interface. Passing an array of
   * FGImages and save them into the db
   * @param array $arImages
   *  an array of FGImages
   *
   */
  public function storeImages($arImages) {
    global $user;
    static $first_run = TRUE;
    static $runid;
    static $folders = array();
    $runid = variable_get('fast_gallery_runid', 0);

    //TODO: This makes the APP slow -> Rebuilding the whole
    // Gallery every time!
    if ($first_run) {
      db_truncate('fast_gallery_hierarchy')->execute();
      $first_run = FALSE;
      variable_set('fast_gallery_runid', ++$runid);
      $folders[$arImages[0]->getFolder()] = $this->storeHierarchy($arImages[0]);
    }

    foreach ($arImages as $image) {
      if ($image->isDir()) {
        $foid = $this->storeHierarchy($image);
        $folders[$image->getFolder()] = $foid;
        continue;
      }

      $foid = $folders[$image->getFolder()];
      db_merge('fast_gallery_file')
          ->key(array('foid' => $foid, 'filename' => $image->getName()))
          ->fields(array('runid' => $runid, 'filecreated' => $image->getCreationTimestamp()))
          ->execute();
    }
  }

  /**
   * (non-PHPdoc)
   * @see sites/all/modules/fast_gallery/storageengine/Istorage#clearDb()
   */
  public function clearDb() {
    db_truncate('fast_gallery_hierarchy')->execute();
    db_truncate('fast_gallery_file')->execute();
    db_truncate('cache_fast_gallery')->execute();
  }

  /**
   * (non-PHPdoc)
   * @see sites/all/modules/fast_gallery/storageengine/Istorage#removeDeletedFiles()
   */
  public function removeDeletedFiles() {
    db_query("DELETE FROM {fast_gallery_file} WHERE runid < :runid", array('runid' => variable_get('fast_gallery_runid', 0)));
  }

  /**
   * In order to retrieve the hierarchy we need to store this information
   * in a special table
   * @param FGImage $image
   * @return unknown_type
   */
  private function storeHierarchy($image) {
    static $folders = array();

    //a little cache to reduce load on db -> we only have to run this for one image per folder
    if (!in_array($image->getFolder(), $folders)) {
      $folder = db_query("SELECT folder AS c FROM {fast_gallery_hierarchy}
              WHERE folder = :folder", array(':folder' => $image->getFolder()))->fetchField();

      if (!$folder) { //incase we don't have this folder in our db
        $dbObject['folder'] = $image->getFolder();
        $dbObject['pid'] = db_query("SELECT foid FROM {fast_gallery_hierarchy}
                            WHERE folder = :folder", array(':folder' => $image->getParent()))->fetchField();
        if (!$dbObject['pid']) {
          $dbObject['pid'] = 0;
        }
        drupal_write_record('fast_gallery_hierarchy', $dbObject);

        //TODO: needs to be checked!!! needs to return the foid, so that we can make
        //the correct entry into fast_gallery
        $foid = db_query("SELECT MAX(foid) FROM {fast_gallery_hierarchy}")->fetchField();
        $folders[$foid] = $image->getFolder();
        return $foid;
      }
    }
    $ar = array_flip($folders);
    return $ar[$image->getFolder()];
  }



}