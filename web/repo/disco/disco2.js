//flame colored blobs


bpm=control.slider({
    name:"BPM",
    category: "music",
    min:100,  
    max:200,
    default: 160
});   

split=control.slider({
    name:"Split",
    min:1,  
    max:led.leds/40,
    default: 2
});   


color=control.color({
    name:"Color", 
    default: { r:255, g:161, b:0}


});

led.mirror(led.leds/split);

led.led_rnd(0, (led.leds/split)-40);
led.fade_mode(2);
led.fade_speed(8);

//fadein 
width=15;
fade_speed=20;

for (i=1;i<width;i=i+1)
{ 
    led.color(
        color.r / (fade_speed/i),
        color.g / (fade_speed/i), 
        color.b / (fade_speed/i)
    );
    led.draw();
}

for (i=width;i>1;i=i-1)
{ 
    led.color(
        color.r / (fade_speed/i),
        color.g / (fade_speed/i), 
        color.b / (fade_speed/i)
    );
    led.draw();
}



led.interval(60000/bpm);


