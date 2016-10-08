//simple spots

width=control.slider({
    name:"Spot width", 
    min:1,  
    max:50,
    default: 3
});   

offset=control.slider({
    name:"Offset between spots", 
    min:1,  
    max:50,
    default: 10
});   


led.width(width);
led.mirror(offset);


led.led(0);
led.draw();



