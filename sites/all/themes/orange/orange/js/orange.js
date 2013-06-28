(function ($) {
Drupal.behaviors.orange = {
  attach: function(context) {
    $('#nav ul li.expanded', context).hover(function() {
      $(this).toggleClass('hover');
    }, function() {
      $(this).toggleClass('hover');
    });
  }
  
};

})(jQuery);