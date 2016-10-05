//(C)2016 Edwin Eefting - edwin@datux.nl
control={}

control._begin=function()
{
    //marks all controls as unused
    $("#controls.control:not(.template)").addClass("unused");
}

//gets existing control and remove unused label
control._get=function(name)
{
    var element=$('#controls .control[name="'+name+'"]');
    console.log("element", element);
    if (element.length==0)
        return(null);

    element.removeClass("unused");
    return(element);
}

control._end=function()
{
    //cleans up unused controls
    $("#controls .unused").remove();
}

control.slider=function(pars)
{
    var context=this._get(pars.name);

    if (!context)
    {
        context=clone_template($(".template.slider"));
        context.attr("name", pars.name);
        $("label", context).text(pars.name);

        $(".widget", context).slider({
            min: pars.min,
            max: pars.max,
            value: pars.default
        });

    }

    return($(".widget", context).slider("value"));
};
