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
    }
}





function compile_command_string(input_text, commands)
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
