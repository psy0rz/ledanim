//random disco colors

bpm=control.slider({
    name:"BPM",
    category: "music",
    min:100,  
    max:200,
    default: 160
});   



color=control.color({
    name:"Color", 
    default: { r:255, g: 255, b: 255}

});

led.mirror(30);

led.led_rnd(0, 25);
led.width(5);
led.color_rnd(color.r/2,color.r, color.g/2, color.g, color.b/2,color.b);
led.fade_mode(2);
led.fade_speed(10);
led.draw();


led.interval(60000/bpm);


