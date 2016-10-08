//(C)2016 Edwin Eefting - edwin@datux.nl
control={}


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

control._begin=function(animation_name)
{
    control._animation_name=animation_name;

    if (! ("controls" in settings))
    {
        settings.controls={};
    }

    //remove all controls
    if (!control.keep)
    {
        $("#controls .control:not(.template)").remove();
    }
}


control._end=function()
{
    if ($("#controls .control:not(.template)").length)
    {
        $(".highlight-when-control").addClass("highlight");
    }
    else {
        $(".highlight-when-control").removeClass("highlight");

    }
}

//get stored value for this control
control._get_value=function(pars)
{
    if (!pars.category)
    {
        pars.category=control._animation_name;
    }

    if (! (pars.category  in settings.controls))
    {
        settings.controls[pars.category]={};
    }

    if (! (pars.name in settings.controls[pars.category]))
    {
        settings.controls[pars.category][pars.name]=pars.default;
    }


    return(settings.controls[pars.category][pars.name]);
}

control._set_value=function(pars, value)
{
    if (!pars.category)
    {
        pars.category=control._animation_name;
    }

    if (! (pars.category in settings.controls))
    {
        settings.controls[pars.category]={}
    }

    settings.controls[pars.category][pars.name]=value;
    control._changed();
}



control._fill_basics=function(pars, context)
{
    $(".name", context).text(pars.name);
    $(".category", context).text(pars.category);
    // $(context).data("default", pars.default);

    //restore defaults
    $(".on-click-restore-default", context).off().on("click", function()
    {
        $(".widget", context).slider("value", pars.default);
    });

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


        function changed( event, ui)
        {
            handle.text( ui.value );
            control._set_value(pars, ui.value);
        }

        $(".widget", context).slider({
            min: pars.min,
            max: pars.max,
            value: control._get_value(pars),
            create: function()
            {
                handle.text( $( this ).slider( "value" ) );
            },
            slide: changed,
            change: changed,
        });

        $(".widget", context).on("slide", function()
        {
            console.log("JA");
        });

        control._fill_basics(pars, context);

    }

    return(control._get_value(pars));
};
