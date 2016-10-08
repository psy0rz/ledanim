//simple spots

width=control.slider({
    name:"Spot width", 
    min:1,  
    max:50,
    default: 3
});   

distance=control.slider({
    name:"Distance between spots", 
    min:1,  
    max:50,
    default: 20
});   


offset=control.slider({
    name:"Starting offset", 
    min:0,  
    max:50,
    default: 0
});   



color=control.color({
    name:"color", 
    min:0,  
    max:255,

});    


led.color(color.r, color.g, color.b);
led.width(width);
led.mirror(distance);


led.led(offset);
led.draw();



