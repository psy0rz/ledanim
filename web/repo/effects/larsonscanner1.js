//larson scanner type 1 (a "kitt scanner" that is fully configurable)
lights=control.slider({
    name:"Number of lights",
    min:3,  
    max:led.leds,
    default: 8
});   

size=control.slider({
    name:"Number of leds per light",
    min:1,  
    max:led.leds/8,
    default: 1
});   

mirror=control.slider({
    name:"Mirror distance",
    min:10,  
    max:led.leds,
    default: 70
});   



delay=control.slider({
    name:"Delay",
    min:0,  
    max:20,
    default: 8
});   


tail=control.slider({
    name:"Dim speed",
    min:1,  
    max:20,
    default: 5
});   


color=control.color({
    name:"Color", 
    default: { r:255, g: 0, b: 0}

});


led.mirror(mirror);
led.fade_speed(tail);

led.color(color.r, color.g, color.b);
led.fade_mode(2);
led.width(size);

led.step(size);
led.repeat_begin(lights-2);
led.draw();
led.delay(delay);
led.repeat_end();

led.step(-size);
led.repeat_begin(lights-2);
led.draw();
led.delay(delay);
led.repeat_end();


