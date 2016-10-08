//(C)2016 Edwin Eefting - edwin@datux.nl
control={}

control._values={};

//called when something is changed
control._change_timer=undefined;
control._changed=function()
{
    control.keep=true;
    if (!control._change_timer)
    {
        _change_timer = setTimeout(function()
        {
            control._change_timer=undefined;
            //set by controller.js to direct us to compiler
            control.compiler();

        }, 100);
    }
}

control._begin=function()
{
    //remove all controls
    if (!control.keep)
    {
        $("#controls .control:not(.template)").remove();
    }
}

//gets existing control
// control._get=function(name)
// {
//     var element=$('#controls .control[name="'+name+'"]');
//     if (element.length==0)
//     {
//         return(null);
//     }
//     return(element);
// }

control._end=function()
{
    //cleans up unused controls
    // $("#controls .unused").remove();
}

//get stored value for this control
control._get_value=function(pars)
{
    if (!pars.category)
    {
        pars.category="Global control";
    }

    if (! (pars.category  in control._values))
    {
        control._values[pars.category]={};
    }

    if (! (pars.name in control._values[pars.category]))
    {
        control._values[pars.category][pars.name]=pars.default;
    }

    return(control._values[pars.category][pars.name]);
}

control._set_value=function(pars, value)
{
    if (!pars.category)
    {
        pars.category="Global control";
    }

    if (! (pars.category in control._values))
    {
        control._values[pars.category]={}
    }

    control._values[pars.category][pars.name]=value;
    control._changed();
}


control._fill_control=function(pars, context)
{
    $(".name", context).text(pars.name);
    $(".category", context).text(pars.category);
}

control.slider=function(pars)
{
    if (!control.keep)
    {
        var context=clone_template($(".template.slider"));

        var handle = $(".custom-slider-handle", context);

        if (pars.default==undefined)
        {
            pars.default=(pars.min+pars.max)/2;
        }

        $(".widget", context).slider({
            min: pars.min,
            max: pars.max,
            value: control._get_value(pars),
            create: function()
            {
                handle.text( $( this ).slider( "value" ) );
            },
            slide: function( event, ui )
            {
                handle.text( ui.value );
                control._set_value(pars, ui.value);
            }
        });

        control._fill_control(pars, context);

    }

    return(control._get_value(pars));
};
