<?php

/**
 * @file orange.skinr.inc
 * Implementation of a Skinr plugin.
 */
 
/**
 * Implements hook_skinr_group_info().
 *
 * Optional. Defines group(s) that will contain skins. Groups are form element
 * containers used to organize skins categorically using vertical_tabs. If you
 * don't define a group, your skins will appear in Skinr's default group, which
 * is labeled "General."
 *
 * Group name(s):
 * Each group name must be unique. It is recommended to prefix the name of each
 * group with the name of the theme or module name implementing it, followed by
 * the name of the group. Note that you cannot define 2 groups with the same
 * the same name, even if they are in different plugins.
 *
 * Group properties:
 * - title (required): Brief title of the tab.
 * - description (optional): Description of the tab group.
 * - weight (discouraged): Weight of the tab group; NULL by default.
 *
 * The "hook" prefix is substituted with the name of the module or theme that
 * implements it, followed by the plugin name, e.g.
 * THEMENAME_PLUGINNAME_skinr_group_info(), or
 * MODULENAME_PLUGINNAME_skinr_group_info().
 */
function orange_skinr_group_info() {
  $groups['orange_links'] = array(
    'title' => t('Links'),
    'description' => t('Choose one or more styles to apply to links and menus.'),
  );
  return $groups;
}

/**
 * Implements hook_skinr_skin_info().
 *
 * Required. Define the skin(s) for this Skinr plugin. Each skin definition
 * consists of properties that define its form element and settings that are
 * needed to make it work, such as the class(es) Skinr should apply, which files
 * it should load, whether or not it should be disabled by default and which
 * theme hook(s) it was designed to work with.
 *
 * Skin name(s):
 * Each skin name must be unique. It is recommended to prefix the name of each
 * skin with the name of the theme or module name implementing it, followed by
 * the name of the skin. Note that you cannot define 2 skins with the same
 * the same name, even if they are in different plugins.
 *
 * Skin settings:
 * - title (required): Title of the skin form element.
 * - description (optional): Description of the skin form element.
 * - group (optional): The group the skin is attached to; defaults to a Skinr
 *   provided group labeled "General."
 * - type (optional): The type of form element. Allowed values:
 *   - checkboxes (default): Useful when single or multiple options can be
 *     chosen.
 *     Does not need to be set manually, as Skinr will apply this by default.
 *   - select: Useful when a single option can be chosen, but many exist.
 *   - radios: Useful when a single option can be chosen and only a few options
 *     exist.
 * - weight (discouraged): Sets the weight of the skin inside the group; NULL
 *   by default. weight should not be set on each individual skin. Instead 
 * - attached (optional): A array containing information about CSS and
 *   JavaScript files the skin requires. Each entry is an array keyed by type:
 *   - css (optional): Maps to the functionality of drupal_add_css() with one
 *     exception: paths are automatically assumed relative to the plugin
 *     directory, unless external. Examples:
 *     - Simple:
 *       'css' => array('css/skin-name.css')
 *     - Advanced:
 *       'css' => array(
 *         array('css/skin-name-ie.css', array(
 *           'media' => 'screen',
 *           'browsers' => array('IE' => 'lte IE 8'),
 *         ),
 *       )
 *   - js (optional): Maps to the functionality of drupal_add_js() with one
 *     exception: paths are automatically assumed as relative to the plugin
 *     directory, unless external. Examples:
 *     - Simple:
 *       'js' => array('js/skin-name.js')
 *     - Advanced:
 *       'js' => array(
 *         array(
 *           'js/skin-name-advanced.js',
 *           array(
 *             'scope' => 'footer',
 *             'group' => JS_THEME,
 *         ),
 *       )
 * - options (required): An array containing one or more options (also arrays)
 *   for the user to choose from when applying skins. Each option key should be
 *   a machine name describing the option. An option should including the
 *   following keys:
 *   - label (required): The option label.
 *   - class (required): An array containing one or more classes the skin
 *     should apply. All classes should be entered as an array: For example:
 *       'class' => array('class-b', 'class-b')
 *   - attached (optional): Same syntax as above, but applies to a specific
 *     option only.
 * - theme hooks (optional): An array containing certain allowed hooks, which
 *   allow you to limit where the skin can be used. Allowed options include:
 *   block, block__MODULE, comment, comment__NODETYPE, comment_wrapper,
 *   comment__wrapper_NODETYPE, node, node__NODETYPE, region,
 *   region__REGIONNAME, panels_display, panels_region, panels_pane, views_view,
 *   views_view__STYLENAME, views_view__DISPLAY_NAME, views_view__VIEWNAME, and
 *   views_view__VIEWNAME_DISPLAYNAME.
 * - default_status (optional): Skins are disabled by default to keep UI
 *   clutter to a minimum. In some cases, like contrib themes, it makes sense to
 *   enable skins which are required to make the theme work properly by default.
 *   Setting this property to 1, will cause it to be enabled it by default for
 *   all installed themes.
 *
 * The "hook" prefix is substituted with the name of the module or theme
 * implementing it, followed by the plugin name, e.g.
 * THEMENAME_PLUGINNAME_skinr_skin_info(), or
 * MODULENAME_PLUGINNAME_skinr_skin_info().
 * 
 */
function orange_skinr_skin_info() {
  $skins['orange_sidebar'] = array(
    'title' => t('Sidebar Position'),
    'group' => 'layout',
    'default status' => '1',
    'theme hooks' => array('page'),
    'options' => array(
      'right' => array(
        'title' => 'Position to the right of content area',
        'class' => array('right'),
      ),
    ),
  );
  $skins['orange_align'] = array(
    'title' => t('Content alignment'),
    'type' => 'select',
    'group' => 'layout',
    'default status' => '1',
    'options' => array(
      'left' => array(
        'title' => 'Left',
        'class' => array('left'),
      ),
      'center' => array(
        'title' => 'Center',
        'class' => array('center'),
      ),
      'right' => array(
        'title' => 'Right',
        'class' => array('right'),
      ),
    ),
  );
  $skins['orange_widths'] = array(
    'title' => t('Object width'),
    'description' => t('This option allows you to select a width percentage for blocks, views, panels and nodes.  NOTE that it also floats the content left.'),
    'type' => 'select',
    'group' => 'layout',
    'default status' => '1',
    'theme hooks' => array('block', 'panels_pane', 'views_view', 'node', 'region'),
    'options' => array(
      'width-100' => array(
        'title' => t('100% wide'),
        'class' => array('width-100'),
      ),
      'width-75' => array(
        'title' => t('75% wide'),
        'class' => array('width-75'),
      ),
      'width-50' => array(
        'title' => t('50% wide'),
        'class' => array('width-50'),
      ),
      'width-33' => array(
        'title' => t('33% wide'),
        'class' => array('width-33'),
      ),
      'width-25' => array(
        'title' => t('25% wide'),
        'class' => array('width-25'),
      ),
    ),
  );
  $skins['orange_margin'] = array(
    'title' => t('Margins'),
    'group' => 'layout',
    'default status' => '1',
    'theme hooks' => array('block', 'panels_pane', 'views_view', 'node', 'region'),
    'options' => array(
      'top-10' => array(
        'title' => t('Add 10px to top'),
        'class' => array('top-10'),
      ),
      'right-10' => array(
        'title' => t('Add 10px to right'),
        'class' => array('right-10'),
      ),
      'bottom-10' => array(
        'title' => t('Add 10px to bottom'),
        'class' => array('bottom-10'),
      ),
      'left-10' => array(
        'title' => t('Add 10px to left'),
        'class' => array('left-10'),
      ),
    ),
  );
  $skins['orange_image'] = array(
    'title' => t('Float image fields'),
    'type' => 'select',
    'group' => 'layout',
    'default status' => '1',
    'theme hooks' => array('block', 'panels_pane', 'views_view', 'node', 'region'),
    'options' => array(
      'img-right' => array(
        'title' => t('Float Right'),
        'class' => array('img-right'),
      ),
      'img-left' => array(
        'title' => t('Float Left'),
        'class' => array('img-left'),
      ),
    ),
  );
  $skins['orange_fontcase'] = array(
    'title' => t('Alter text case'),
    'type' => 'select',
    'group' => 'typography',
    'default status' => '1',
    'options' => array(
      'uppercase' => array(
        'title' => t('Uppercase'),
        'class' => array('uppercase'),
      ),
      'lowercase' => array(
        'title' => t('Lowercase'),
        'class' => array('lowercase'),
      ),
      'capitalize' => array(
        'title' => t('Capitalize'),
        'class' => array('capitalize'),
      ),
    ),
  );
  $skins['orange_fontsize'] = array(
    'title' => t('Page font size'),
    'type' => 'select',
    'group' => 'typography',
    'default status' => '1',
    'options' => array(
      'sm-font' => array(
        'title' => t('Small'),
        'class' => array('sm-font'),
      ),
      'md-font' => array(
        'title' => t('Medium'),
        'class' => array('md-font'),
      ),
      'lg-font' => array(
        'title' => t('Large'),
        'class' => array('lg-font'),
      ),
      'ex-font' => array(
        'title' => t('Extra Large'),
        'class' => array('ex-font'),
      ),
      'giant-font' => array(
        'title' => t('Giant (use with caution)'),
        'class' => array('giant-font'),
      ),
    ),
  );
  $skins['orange_headings'] = array(
    'title' => t('Headings'),
    'description' => t('This option will only change the heading font on this element.'),
    'type' => 'select',
    'group' => 'typography',
    'default status' => '1',
    'options' => array(
      'serif-heading' => array(
        'title' => t('Serif headings'),
        'class' => array('serif-heading'),
      ),
      'lucida-heading' => array(
        'title' => t('Lucida sans headings'),
        'class' => array('lucida-heading'),
      ),
      'mono-heading' => array(
        'title' => t('Monospace headings'),
        'class' => array('mono-heading'),
      ),
      'verdana-heading' => array(
        'title' => t('Verdana headings'),
        'class' => array('verdana-heading'),
      ),
    ),
  );
  $skins['orange_fontstack'] = array(
    'title' => t('Font stacks'),
    'description' => t('This option will change the font stack for this element.'),
    'type' => 'select',
    'group' => 'typography',
    'default status' => '1',
    'options' => array(
      'serif-all' => array(
        'title' => t('Big Caslon, Book Antiqua, Palatino Linotype, Georgia, serif'),
        'class' => array('serif-all'),
      ),
      'lucida-all' => array(
        'title' => t('Lucida Grande, Lucida Sans Unicode, Lucida Sans, Geneva, Verdana, sans-serif'),
        'class' => array('lucida-all'),
      ),
      'mono-all' => array(
        'title' => t('Consolas, Courier New, Courier, mono'),
        'class' => array('mono-all'),
      ),
      'verdana-heading' => array(
        'title' => t('Verdana, Geneva, sans-serif'),
        'class' => array('verdana-all'),
      ),
    ),
  );
  $skins['orange_lists'] = array(
    'title' => t('List styles'),
    'type' => 'select',
    'group' => 'orange_links',
    'default status' => '1',
    'theme hooks' => array('block', 'panels_pane', 'views_view', 'node', 'region'),
    'options' => array(
      'img' => array(
        'title' => t('List style with image'),
        'class' => array('img'),
      ),
      'blocky' => array(
        'title' => t('Blocky list with orange hover'),
        'class' => array('blist'),
      ),
      'grey' => array(
        'title' => t('Blocky list with grey hover'),
        'class' => array('blist', 'grey'),
      ),
    ),
  );
  $skins['orange_menus'] = array(
    'title' => t('Menu styles'),
    'type' => 'select',
    'group' => 'orange_links',
    'default status' => '1',
    'theme hooks' => array('block', 'panels_pane', 'views_view', 'node', 'region'),
    'options' => array(
      'inline-links' => array(
        'title' => t('One level links with separators'),
        'class' => array('inline-links'),
      ),
      'column-links' => array(
        'title' => t('Links as columns with sub-options below'),
        'class' => array('column-links'),
      ),
    ),
  );
  
  return $skins;
}
