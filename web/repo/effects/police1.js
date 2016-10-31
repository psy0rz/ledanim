//police lights 
bpm=control.slider({
    name:"BPM",
    category: "music",
    min:100,  
    max:200,
    default: 160
});   

width=led.leds/2;


led.width(width);
//left red
led.led(0);
led.color(255,0,0);
led.draw();
led.delay(4);

led.led(0);
led.color(0,0,0);
led.draw();
led.delay(2);

led.led(0);
led.color(255,0,0);
led.draw();
led.interval(60000/bpm);


led.led(0);
led.color(0,0,0);
led.draw();

//right blue
led.led(led.leds-width);
led.color(0,0,255);
led.draw();
led.delay(4);

led.led(led.leds-width);
led.color(0,0,0);
led.draw();
led.delay(2);

led.led(led.leds-width);
led.color(0,0,255);
led.draw();
led.interval(60000/bpm);

led.led(led.leds-width);
led.color(0,0,0);
led.draw();
