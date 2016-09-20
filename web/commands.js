anim={}

anim.clear_commmands=function()
{
    if (this.commands)
        this.commands.delete();

    this.commands=new Module.commands_t();

};

anim.verify_number=function(value, min, max)
{
    if (value > max)
    {
        return ("value too big");
    }

    if (value < min)
    {
        return ("value too small");
    }
}

anim.add=function(nr)
{
    this.commands.push_back(nr);
}

anim.add_uint8=function(value, min,max)
{
    value=Number(value);

    if (max==undefined)
        max=255;
    if (min==undefined)
        min=0;

    check=this.verify_number(value, min, max);
    if (check)
        return(check);

    this.add(value);
}

anim.add_int8=function(value, min, max)
{
    value=Number(value);

    if (max==undefined)
        max=255;
    if (min==undefined)
        min=-127;

    check=this.verify_number(value, min, max);
    if (check)
        return(check);

    this.add(value);
}

anim.add_uint16=function(value, min, max)
{

    value=Number(value);

    if (max==undefined)
        max=65535;
    if (min==undefined)
        min=0;

    check=this.verify_number(value, min, max);
    if (check)
        return(check);

    this.add(value >> 8);
    this.add(value & 0xff);
}

// anim.convert_enum(value, command_def, commands)
// {
//
//     value=Number(value);
//
//     if (value<0 || value>=command_def.enum.length)
//         return("Invalid choice");
//
//     commands.push_back(value);
// }


anim.eof=function()
{
    this.add(0);
}
anim.eof.desc="End of program";

anim.update=function()
{
    this.add(1);

}
anim.update.desc="Update led strip with current values";


anim.delay=function(steps)
{
    this.add(3);
    this.add_uint16(steps);

}
anim.delay.desc="Delay program and update led strip and delay for this many steps.";


anim.repeat_begin=function(times)
{
    this.add(4);
    this.add_uint16(steps);
}
anim.repeat_begin.desc="Repeat this block of commands.";

anim.repeat_begin_rnd=function(min, max)
{
    this.add(5);
    this.add_uint16(min);
    this.add_uint16(max);
}
anim.repeat_begin_rnd.desc="Repeat this block of commands a random number of times.";


anim.repeat_end=function()
{
    this.add(6);
}
anim.repeat_end.desc="End repeat block";


anim.led=function(nr)
{
    this.add(9);
    this.add_uint16(nr);
}
anim.led.desc="Change current lednr";


anim.led_rnd=function(min, max)
{
    this.add(10);
    this.add_uint16(min);
    this.add_uint16(max);
}

anim.led_rnd.desc="Change current led nr to random led.";


anim.led_set_next=function(r,g,b)
{
    this.add(11);
    this.add_uint8(r);
    this.add_uint8(g);
    this.add_uint8(b);
}
anim.led_set_next.desc="Set current led to specified color, without effects. And go to next led. (determined by pen_step)";


anim.color=function(r,g,b)
{
    this.add(12);
    this.add_uint8(r);
    this.add_uint8(g);
    this.add_uint8(b);
}
anim.color.desc="Set pen color";


anim.color_rnd=function(r_min, r_max, g_min, g_max, b_min, b_max)
{
    this.add(13);
    this.add_uint8(r_min);
    this.add_uint8(r_max);
    this.add_uint8(g_min);
    this.add_uint8(g_max);
    this.add_uint8(b_min);
    this.add_uint8(b_max);
}
anim.color.desc="Set pen color randomly";


anim.step=function(offset)
{
    this.add(14);
    this.add_int8(offset);
}
anim.step.desc="Stepsize taken after drawing a pixel with the pen.";



anim.width=function(width)
{
    this.add(15);
    this.add_uint8(width);
}
anim.width.desc="Width of one pixel of the pen";


anim.width_rnd=function(min, max)
{
    this.add(16);
    this.add_uint8(min);
    this.add_uint8(max);
}
anim.width.desc="Set width of pen randomly";


anim.draw=function()
{
    this.add(17);
}
anim.width.desc="Draw one 'pixel' with current pen settings.";


anim.fade_mode=function(mode)
{
    this.add(18);
    this.add_uint8(mode);
}
anim.fade_mode.desc="Set fade mode for next pixels.";


anim.fade_speed=function(levels)
{
    this.add(19);
    this.add_uint8(levels);
}
anim.fade_speed.desc="Set fade speed, in color-levels per step.";


anim.fade_speed_rnd=function(min, max)
{
    this.add(20);
    this.add_uint8(min);
    this.add_uint8(max);
}
anim.fade_speed.desc="Set fade speed randomly";


anim.clone_count=function(count)
{
    this.add(21);
    this.add_uint16(count);
}
anim.clone_count.desc="Clone pen this number of times by clone offset.";


anim.clone_offset=function(offset)
{
    this.add(22);
    this.add_uint16(offset);
}
anim.clone_offset.desc="Offset between clones";





function assemble_commands(lines, commands)
{
    // var lines=input_text.split("\n");

    for (line_nr in lines)
    {
        line_nr=Number(line_nr);

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
            return([ line_nr+1, 0, "Command '"+ command_name +"' on line " + (line_nr+1) + " not found."]);
        }
        commands.push_back(command_def.nr);

        for (par_nr in command_def.pars)
        {
            par_nr=Number(par_nr);
            var error="";

            if (words[par_nr] === undefined)
            {
                error="Missing parameter "+(par_nr+1)+": "+command_def.pars[par_nr].desc;
            }
            else
            {
                error=command_def.pars[par_nr].convert(words[par_nr], command_def.pars[par_nr], commands);
            }

            if (error)
                return([ line_nr+1, par_nr+1, error ]);
        }
    }
}
