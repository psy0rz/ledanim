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
}

control.has_controls=function()
{
    return ($("#controls .control:not(.template)").length!=0);
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

        //restore defaults
        $(".on-click-restore-default", context).off().on("click", function()
        {
            $(".widget", context).slider("value", pars.default);
        });


        control._fill_basics(pars, context);

    }

    return(control._get_value(pars));
};


control.color=function(pars)
{
    if (!control.keep)
    {
        var context=clone_template($(".template.color-picker"));

        if (pars.default==undefined)
        {
            pars.default={ r: 255, g: 255, b: 255 };
        }


        function changed( event, ui)
        {
            var rgb=ui.color.toRgb();
            control._set_value(pars, rgb);
            $(".color-value", context).text(rgb.r+", "+rgb.b+", "+ rgb.g);
        }

        $(".widget", context).iris({
            hide: false,
            mode: 'hsv',
            controls: {
                    horiz: 'h', // horizontal defaults to saturation
                    vert: 's', // vertical defaults to lightness
                    strip: 'v' // right strip defaults to hue
                },
            width: 300,
            palettes: true,
            change: changed,
            color: control._get_value(pars),
            border:false
        })

        $(".on-click-restore-default", context).off().on("click", function()
        {
            console.log(pars.default);
            console.log(Color(pars.default));
            $(".widget", context).iris("color", Color(pars.default));
        });

        control._fill_basics(pars, context);

    }

    return(control._get_value(pars));
};
