// The Matrix rain code.

tail=30;
mirror=60;

led.mirror(mirror);
led.fade_mode(1);
led.step(0);

for(i=0;i<mirror; i++)
{
    //white dot
    led.led(i);
    led.set(255,255,255);

    //fade to green fast
    led.color_rnd(0,0, 50,180, 0,0);
    led.fade_speed_rnd(10,25);
    led.draw();

    //fade to black slower at a later point
    if (i>=tail)
    {
        led.led(i-tail);
    }
    else
    {
        led.led(i-tail+mirror);
        
    }
    led.color(0,0,0);
    led.fade_speed_rnd(2,3);
    led.draw();

    
    
    led.delay(4);    
    
}

