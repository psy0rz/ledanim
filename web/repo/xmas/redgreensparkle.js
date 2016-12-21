// red green xmas sparkles

led.step(0)

delay=control.slider({
    name:"delay",
    min:0,  
    max:100,
    default: 20
});   

flashcolor=control.color({
    name:"Flash color", 
    default: { r:255, g: 255, b: 255}
});


color1=control.color({
    name:"Color 1", 
    default: { r:255, g: 0, b: 0}
});


color2=control.color({
    name:"Color 2", 
    default: { r:0, g: 255, b: 0}
});

/// flash first led color 
led.led_rnd(0, led.leds);
led.set(color1.r, color1.g, color1.b)
led.color(flashcolor.r, flashcolor.g, flashcolor.b);
led.fade_mode(2);
led.fade_speed(20);
led.draw();

led.repeat_begin_rnd(0,delay)
led.delay(1)
led.repeat_end();


/// flash second led color
led.led_rnd(0, led.leds);
led.set(color2.r, color2.g, color2.b)
led.color(flashcolor.r, flashcolor.g, flashcolor.b);
led.fade_mode(2);
led.fade_speed(20);
led.draw();

led.repeat_begin_rnd(0,delay)
led.delay(1)
led.repeat_end();


/// turn a led off
led.led_rnd(0, led.leds);
led.color(0,0,0);
led.fade_mode(1);
led.fade_speed(1);
led.draw();

led.repeat_begin_rnd(0,delay)
led.delay(1)
led.repeat_end();



        