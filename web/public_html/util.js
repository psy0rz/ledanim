//clones specified jquery element template
function clone_template(element)
{
    var clone=element.clone();
    clone.removeClass("template");
    clone.insertBefore(element);
    return(clone);
}
