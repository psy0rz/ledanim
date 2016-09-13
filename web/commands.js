function verify_number(value, command_def)
{
    if (value > command_def.max)
    {
        return ("value too big");
    }

    if (value < command_def.min)
    {
        return ("value too small");
    }
}

function convert_uint8(value, command_def, commands)
{
    value=Number(value);

    if (command_def.max==undefined)
        command_def.max=255;
    if (command_def.min==undefined)
        command_def.min=0;

    check=verify_number(value, command_def);
    if (check)
        return(check);

    commands.push_back(value);
}

function convert_int8(value, command_def, commands)
{
    value=Number(value);

    if (command_def.max==undefined)
        command_def.max=127;
    if (command_def.min==undefined)
        command_def.min=-127;

    check=verify_number(value, command_def);
    if (check)
        return(check);

    commands.push_back(value);
}

function convert_uint16(value, command_def, commands)
{
    value=Number(value);

    if (command_def.max==undefined)
        command_def.max=65535;
    if (command_def.min==undefined)
        command_def.min=0;

    check=verify_number(value, command_def);
    if (check)
        return(check);

    commands.push_back(value >> 8);
    commands.push_back(value & 0xff);
}


command_defs=
{
    "eof": {
        "desc"  :"End of program",
        "nr"   : 0,
        "pars"  : []
    },
    "update": {
        "desc"  : "Update led strip with current values",
        "nr"   : 1,
        "pars"  : []
    },
    "delay": {
        "desc"  : "Delay program and update led strip and delay for this many steps.",
        "nr"   : 3,
        "pars"  : [
            {
                "desc": "delay in steps (60 steps/s)",
                "convert": convert_uint16,
            },
        ],
    },

    "repeat_begin": {
        "desc"  : "Repeat this block of commands.",
        "nr"   : 4,
        "pars"  : [
            {
                "desc": "Times to repeat",
                "convert": convert_uint16,
            },
        ],
    },
    "repeat_begin": {
        "desc"  : "Repeat this block of commands a random number of times.",
        "nr"   : 5,
        "pars"  : [
            {
                "desc": "Minimum times to repeat",
                "convert": convert_uint16,
            },
        ],
        "pars"  : [
            {
                "desc": "Maximum times to repeat",
                "convert": convert_uint16,
            },
        ],
    },
    "repeat_end": {
        "desc"  : "End of repeat block.",
        "nr"   : 6,
        "pars"  : [ ],
    },


    "led_nr": {
        "desc"  : "Change current lednr",
        "nr"   : 9,
        "pars"  : [
            {
                "desc": "Led nr",
                "convert": convert_uint16,
            },
        ],
    },
    "led_nr_rnd": {
        "desc"  : "Change current led nr to random led.",
        "nr"   : 10,
        "pars"  : [
            {
                "desc": "Minimum lednr to choose",
                "convert": convert_uint16,
            },
        ],
        "pars"  : [
            {
                "desc": "Maximum led nr to choose",
                "convert": convert_uint16,
            },
        ],
    },
    "led_set_next": {
        "desc"  : "Set current led to specified color, without effects. And go to next led. (determined by pen_step)",
        "nr"   : 11,
        "pars"  : [
            {
                "desc": "red",
                "convert": convert_uint8,
            },
        ],
        "pars"  : [
            {
                "desc": "green",
                "convert": convert_uint8,
            },
        ],
        "pars"  : [
            {
                "desc": "blue",
                "convert": convert_uint8,
            },
        ],
    },

    "pen_color": {
        "desc"  : "Set pen color",
        "nr"   : 12,
        "pars"  : [
            {
                "desc": "red",
                "convert": convert_uint8,
            },
        ],
        "pars"  : [
            {
                "desc": "green",
                "convert": convert_uint8,
            },
        ],
        "pars"  : [
            {
                "desc": "blue",
                "convert": convert_uint8,
            },
        ],
    },


    "pen_color_rnd": {
        "desc"  : "Set pen color randomly",
        "nr"   : 13,
        "pars"  : [
            {
                "desc": "red min",
                "convert": convert_uint8,
            },
        ],
        "pars"  : [
            {
                "desc": "red max",
                "convert": convert_uint8,
            },
        ],
        "pars"  : [
            {
                "desc": "green min",
                "convert": convert_uint8,
            },
        ],
        "pars"  : [
            {
                "desc": "green max",
                "convert": convert_uint8,
            },
        ],
        "pars"  : [
            {
                "desc": "blue min",
                "convert": convert_uint8,
            },
        ],
        "pars"  : [
            {
                "desc": "blue max",
                "convert": convert_uint8,
            },
        ],
    },

    "pen_step": {
        "desc"  : "Stepsize taken after drawing a pixel with the pen.",
        "nr"   : 14,
        "pars"  : [
            {
                "desc": "Step size (can be negative to step to the left)",
                "convert": convert_int8,
            },
        ],
    },

    "pen_width": {
        "desc"  : "Width of one pixel of the pen.",
        "nr"   : 15,
        "pars"  : [
            {
                "desc": "Width",
                "convert": convert_uint8,
            },
        ],
    },

    "pen_width_rnd": {
        "desc"  : "Set width of pen randomly",
        "nr"   : 16,
        "pars"  : [
            {
                "desc": "Min width",
                "convert": convert_uint8,
            },
            {
                "desc": "Max width",
                "convert": convert_uint8,
            },
        ],
    },

    "pen_fade_mode": {
        "desc"  : "Fade mode of pixels that are drawn by pen.",
        "nr"   : 18,
        "pars"  : [
            {
                "desc": "Fade mode",
                "convert": convert_uint8,
            },
        ],
    },

}

// CMD_PEN_FADE_MODE=18,
// CMD_PEN_FADE_SPEED=19,
// CMD_PEN_FADE_SPEED_RND=20,
// CMD_PEN_CLONE_COUNT=21,
// CMD_PEN_CLONE_OFFSET=22,
// CMD_PEN_DRAW=17,





function assemble_commands(input_text, commands)
{
    var lines=input_text.split("\n");
    for (line_nr in lines)
    {
        //cleanup line
        var line=lines[line_nr];
        line=line.trim()
        line=line.replace(/[ ]+/g," ");

        //ignore comments and empty lines
        if (line[0]=="#" || line=="")
            continue;


        var words=line.split(" ");
        var command_name=words.shift();
        var command_def=command_defs[command_name];
        if (!command_def)
        {
            return("Command '"+ command_name +"' on line " + line_nr + " not found.");
        }
        commands.push_back(command_def.nr);

        for (par_nr in command_def.pars)
        {
            var error=command_def.pars[par_nr].convert(words[par_nr], command_def, commands);
            if (error)
                return("Error in line " + line_nr +": "+ error);
        }
    }
}
