/* $Id:  */

-------------
-- SUMMARY --
-------------
The Fast Gallery module is aimed at new Drupal user who just want a nice 
Gallery without having to dig further into the Drupal modules and API. Further
it comes in very handy for users who want to sync a webgallery based on
a file system.

Easy, Fast, Reliable.

For a full description of the module, visit the project page:
  http://drupal.org/project/fast_gallery

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/fast_gallery
  
------------------
-- Installation --
------------------
The installation is very easy:
1. Install the module
2. Go to admin/config/fast_gallery/general and set up at least one
   gallery (Path to gallery, path alias and title of gallery) and
   select a storage engine (Default is just fine). Save the configuration.
3. Hit the rescan button ... your gallery is all set up and ready to go.
4. There are a few other options config/fast_gallery/default to 
   customize your gallery. Feel free to set up as many galleries as you want
  
--------------  
-- Features --
--------------
1. Synchronize gallery with filesystem on cron run

------------------------  
-- API for Developers --
------------------------
What does the API do? It's very simple: You can build your own storage engine.
So the question is, what does the storage engine do? The storages engine is 
actually doing everything. Just core Fast Gallery provides a simple configuration
file, where you can create galleries. Core Fast Gallery then scans for new 
images which are the past to the storages engine, that stores images. Same thing
for retrieving images. Core Fast Gallery is being called and then calls the 
storage engine to get the images. So you can implement new sorting orders and
all kinds of fancy stuff in your own storage engine if you would like to do so.

There are only few requirements that need to be met:

* implement hook_fast_gallery_info (see fast_gallery_fast_gallery_info as example)
* implement a storageengine that implements the Istorage interface
  (see default.storage.inc)
* the storage engine MUST have fast_gallery_get_storage() which returns an
  storageengine object