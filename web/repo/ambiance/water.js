// effect that should look like water with waves
wildness=control.slider({
    name:"Wavyness",
    min:1,  
    max:100,
    default: 10
});   



color=control.color({
    name:"Color", 
    default: { r:0, g: 0, b: 255}

});

led.fade_mode(1);
led.fade_speed(wildness);

led.repeat_begin(20);
led.led_rnd(0, led.leds);
led.color_rnd(0, color.r,  0, color.g,  0, color.b);
led.draw();
led.repeat_end();
