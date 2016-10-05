//(C)2016 Edwin Eefting - edwin@datux.nl
led={}

led.clear_commmands=function()
{
    if (this.commands)
        this.commands.delete();

    this.commands=new Module.commands_t();

};

led.verify_number=function(value, min, max)
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

led.add=function(nr)
{
    this.commands.push_back(nr);
}

led.add_uint8=function(value, min,max)
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

led.add_int8=function(value, min, max)
{
    value=Number(value);

    if (max==undefined)
        max=255;
    if (min==undefined)
        min=-127;

    check=this.verify_number(value, min, max);
    if (check)
        return(check);

    if (value>=0)
        this.add(value);
    else {
        this.add(value+256); //abuse uints for int
    }
}

led.add_uint16=function(value, min, max)
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

// led.convert_enum(value, command_def, commands)
// {
//
//     value=Number(value);
//
//     if (value<0 || value>=command_def.enum.length)
//         return("Invalid choice");
//
//     commands.push_back(value);
// }


led.eof=function()
{
    this.add(0);
}
led.eof.desc="End of program";

led.update=function()
{
    this.add(1);

}
led.update.desc="Update led strip with current values";


led.delay=function(steps)
{
    this.add(3);
    this.add_uint16(steps);

}
led.delay.desc="Delay program and update led strip and delay for this many steps.";


led.repeat_begin=function(steps)
{
    this.add(4);
    this.add_uint16(steps);
}
led.repeat_begin.desc="Repeat this block of commands.";

led.repeat_begin_rnd=function(min, max)
{
    this.add(5);
    this.add_uint16(min);
    this.add_uint16(max);
}
led.repeat_begin_rnd.desc="Repeat this block of commands a random number of times.";


led.repeat_end=function()
{
    this.add(6);
}
led.repeat_end.desc="End repeat block";


led.led=function(nr)
{
    this.add(9);
    this.add_uint16(nr);
}
led.led.desc="Change current lednr";


led.led_rnd=function(min, max)
{
    this.add(10);
    this.add_uint16(min);
    this.add_uint16(max);
}

led.led_rnd.desc="Change current led nr to random led.";


led.set=function(r,g,b)
{
    this.add(11);
    this.add_uint8(r);
    this.add_uint8(g);
    this.add_uint8(b);
}
led.set.desc="Set current led to specified color, without effects. And go to next led. (determined by pen_step)";


led.color=function(r,g,b)
{
    this.add(12);
    this.add_uint8(r);
    this.add_uint8(g);
    this.add_uint8(b);
}
led.color.desc="Set pen color";


led.color_rnd=function(r_min, r_max, g_min, g_max, b_min, b_max)
{
    this.add(13);
    this.add_uint8(r_min);
    this.add_uint8(r_max);
    this.add_uint8(g_min);
    this.add_uint8(g_max);
    this.add_uint8(b_min);
    this.add_uint8(b_max);
}
led.color.desc="Set pen color randomly";


led.step=function(offset)
{
    this.add(14);
    this.add_int8(offset);
}
led.step.desc="Stepsize taken after drawing a pixel with the led.";



led.width=function(width)
{
    this.add(15);
    this.add_uint8(width);
}
led.width.desc="Width of one pixel of the pen";


led.width_rnd=function(min, max)
{
    this.add(16);
    this.add_uint8(min);
    this.add_uint8(max);
}
led.width.desc="Set width of pen randomly";


led.draw=function()
{
    this.add(17);
}
led.width.desc="Draw one 'pixel' with current pen settings.";


led.fade_mode=function(mode)
{
    this.add(18);
    this.add_uint8(mode);
}
led.fade_mode.desc="Set fade mode for next pixels.";


led.fade_speed=function(levels)
{
    this.add(19);
    this.add_uint8(levels);
}
led.fade_speed.desc="Set fade speed, in color-levels per step.";


led.fade_speed_rnd=function(min, max)
{
    this.add(20);
    this.add_uint8(min);
    this.add_uint8(max);
}
led.fade_speed.desc="Set fade speed randomly";


led.clone_count=function(count)
{
    this.add(21);
    this.add_uint16(count);
}
led.clone_count.desc="Clone pen this number of times by clone offset.";


led.clone_offset=function(offset)
{
    this.add(22);
    this.add_uint16(offset);
}
led.clone_offset.desc="Offset between clones";

led.mirror=function(offset)
{
    this.add(23);
    this.add_uint16(offset);
}
led.clone_offset.desc="Mirror the first X leds over the entire strip.";
