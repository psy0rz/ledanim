//(C)2016 Edwin Eefting - edwin@datux.nl
control={}

control.values={};

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

control.slider=function(pars)
{
    if (!control.keep)
    {
        var context=clone_template($(".template.slider"));

        $(".name", context).text(pars.name);

        var handle = $(".custom-slider-handle", context);

        //not set?
        if (!name in control.values)
        {
            if (pars.default===undefined)
            {
                control.values[pars.name]=(pars.min+pars.max)/2;
            }
            else
            {
                control.values[pars.name]=pars.default;
            }
        }

        $(".widget", context).slider({
            min: pars.min,
            max: pars.max,
            value: control.values[pars.name],
            create: function()
            {
                handle.text( $( this ).slider( "value" ) );
            },
            slide: function( event, ui )
            {
                control.values[pars.name]=ui.value;
                handle.text( ui.value );
                control._changed();
            }
        });
    }

    return(control.values[pars.name]);
};
