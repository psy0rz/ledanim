//policy lights (will add more in the future)
delay=control.slider({
    name:"Delay (1/60s seconds)",
    min:1,  
    max:60,
    default: 6
});   

color=control.color({
    name:"color", 
    default: { r:255, g:255, b:255}    

});    


led.width(5);
led.mirror(20);

//flash
led.led(0);
led.color(color.r, color.g, color.b);
led.draw();
led.delay(0);


led.led(0);
led.color(0,0,0);
led.draw();
led.delay(delay-1);
