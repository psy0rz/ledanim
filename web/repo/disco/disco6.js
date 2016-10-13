//vu meter disco

bpm=control.slider({
    name:"BPM",
    category: "music",
    min:100,  
    max:200,
    default: 160
});   



color1=control.color({
    name:"Color 1", 
    default: { r: 0, g:255, b:0 }

});

color2=control.color({
    name:"Color 2", 
    default: { r: 255, g:255, b:0 }

});

color3=control.color({
    name:"Color 3", 
    default: { r: 255, g:0, b:0 }

});


led.led(0);
led.fade_mode(2);
led.mirror(50);

width=20;


led.led(width);
led.step(-1);
for (i=1;i<width;i=i+1)
{ 
    led.fade_speed(i*2);
    if ( i > (width*0.8) )
        led.color(
            color3.r,
            color3.g , 
            color3.b 
        );
    else if ( i > (width*0.6) )
        led.color(
            color2.r,
            color2.g , 
            color2.b 
        );
    else
        led.color(
            color1.r,
            color1.g , 
            color1.b 
        );
    
    
    led.draw();
}

led.led(width);
led.step(1);
for (i=1;i<width;i=i+1)
{ 
    led.fade_speed(i*2);
    if ( i > (width*0.8) )
        led.color(
            color3.r,
            color3.g , 
            color3.b 
        );
    else if ( i > (width*0.6) )
        led.color(
            color2.r,
            color2.g , 
            color2.b 
        );
    else
        led.color(
            color1.r,
            color1.g , 
            color1.b 
        );
    
    
    led.draw();
}



// bpm=bpm*4;
led.delay(   (60/(bpm/60)) -1);
 

