<?php
// $Id:

/**
 *
 * @author rschaer
 *
 */


class FGImage {
  //$fid
  private $path, $name, $folder, $parent, $fid, $isDir, $filetype, $created;

  /**
   * Constructor
   * @param $path
   *  The path to the image
   * @param $options
   *  an assoziative array containing some options. The following options are possible:
   *    - dir: can be true or false. If set to true the image behaves as a dir! Default is set to false
   */
  public function __construct($path, $options = array()) {
    $this->setPath($path);
    // Split up the Image Path, so we can work on it
    $arPath = explode('/', $path);
    if ($arPath[count($arPath)-1] == '') {
      unset($arPath[count($arPath)-1]);
      $options['dir'] = TRUE;
    }

    $this->setName($arPath);

    if (isset($options['dir'])) {
      $this->setIsDir($options['dir']);
    }
    else {
      $this->setIsDir(FALSE);
      $this->setFileType($arPath[count($arPath) - 1]);
    }

    $this->setFolder($arPath);
    $this->setParent($arPath);
    $this->setCreationTimestamp();
  }

  /**
   * Render the html for displaying the image
   * @return string html
   */
  public function renderHtml() {
    $cache = fast_gallery_get_cache();
    $path = $this->getPath();
    if ($this->isDir) { //incase a dir we return a special image
      $path = drupal_get_path('module', 'fast_gallery') . '/images/folder.png';
      return theme('image', array('path' => $path, 'title' => t('folder')));
    }
    else if($this->getFileType() == 'pdf') {
      $path = drupal_get_path('module', 'fast_gallery') . '/images/doc.screenshot.jpg';
      $cache->createthumb($path, 150, 100);
      return theme('image', array('path' => $path . '.thumb', 'title' => t('PDF')));
    }
    else {
      $cache->createthumb($path, 150, 100);
      return theme('image', array('path' => $path . '.thumb', 'title' => $this->getName()));
    }
  }

  public function createArrayVersion() {
    $image = array(
      'path' => $this->getPath(),
      'name' => $this->getName(),
      'folder' => $this->getFolder(),
      'filetyp' => $this->getFileType(),
      'created' => $this->getCreationTimestamp(),
    );
    return $image;
  }
  /**
   * Given a path and index n, removes n number of elements from the end of
   * the path (delimited by '/').
   *
   * @param path_ar
   *   Path.
   * @param n
   *   Number of elements to remove from end of the path.
   */
  private function substractFromPath($path_ar, $n) {
    $path_ar = array_slice($path_ar, 0, - $n);
    return implode('/', $path_ar);
  }


  // All the accessor mehtods
  // -------------------------

  public function setCreationTimestamp($timestamp = 0) {
    if ($timestamp == 0) {
      if (!file_exists($this->getPath())) {
        drupal_set_message(t('The file %file doesn\'t exist. Line %line in %file',
        array('%line' => __LINE__, '%file' => __FILE__, '%file' => $this->getPath())), 'error');
        return false;
      }
      $this->created = filectime($this->getPath());
    }
    else {
      $this->created = $timestamp;
    }
  }

  public function getCreationTimestamp() {
    return $this->created;
  }

  public function setIsDir($isDir) {
    if ($isDir) {
      $this->isDir = TRUE;
    }
    else {
      $this->isDir = FALSE;
    }
  }

  public function isDir() {
    return $this->isDir;
  }

  public function getFid() {
    return $this->fid;
  }

  public function setFid($fid) {
    $this->fid = $fid;
  }

  public function getFolder() {
    return $this->folder;
  }

  public function setFolder($arPath) {
    if (!$this->isDir()) {
      $this->folder = $this->substractFromPath($arPath, 1);
    }
    else {
      $this->folder = implode("/", $arPath);
    }
  }

  public function getParent() {
    return $this->parent;
  }

  public function setParent($arPath) {
    if ($this->isDir()) {
      $this->parent = $this->substractFromPath($arPath, 1);
    }
    else {
      $this->parent = $this->substractFromPath($arPath, 2);
    }
  }
  /**
   * Getter method
   * @return string $path
   */
  public function getPath() {
    return $this->path;
  }

  /**
   * Setter method
   * @param $path string
   */
  public function setPath($path) {
    $this->path = $path;
  }

  /**
   * Getter method
   * @return string $path
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Setter method
   * @param $name string
   */
  public function setName($arPath) {
    if (!$this->isDir()) {
      $this->name = $arPath[count($arPath)-1];
    }
  }

  /**
   * setter method
   * @param $path - the path of the file
   */
  public function setFileType($path) {
    $ending = explode(".", $path);
    $ending = $ending[count($ending) - 1];
    $this->filetype = $ending;
  }

  /**
   * getter method
   * @return string
   */
  public function getFileType() {
    return $this->filetype;
  }

}