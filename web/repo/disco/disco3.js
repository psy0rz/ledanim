//regea disco

bpm=control.slider({
    name:"BPM",
    category: "music",
    min:100,  
    max:200,
    default: 160
});   

colors=[

    control.color({
        name:"Color 1", 
        default: {r: 255, g:0, b:0 }
    
    }),

    control.color({
        name:"Color 2", 
        default: {r: 127, g:127, b:0 }
    
    }),

    control.color({
        name:"Color 3", 
        default: {r: 0, g:255, b:0 }
    
    })
]

colors[3]=colors[1];

//Red, blue, green
//  black, yellow, red, yellow,     black, yellow, red, yellow

// led.fade_mode(2);
// led.fade_speed(5);
led.mirror(40);
led.step(10);
led.width(10);

for (s=0; s<4; s++)
{
    led.led(0);
    for (i=0; i<4; i++ )
    {
    
        led.color(colors[i].r, colors[i].g, colors[i].b);
        led.draw();
    }
    colors.push(colors.shift());

    led.interval(60000/bpm);
}

