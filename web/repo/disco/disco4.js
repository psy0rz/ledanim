//lego disco

bpm=control.slider({
    name:"BPM",
    category: "music",
    min:100,  
    max:200,
    default: 140
});   

fade=control.slider({
    name:"fade",
    min:0,  
    max:200,
    default: 0
});   

colors=[

    control.color({
        name:"Color gap", 
        default: { r:0, g:0, b:0 }
    
    }),

    control.color({
        name:"Color edge", 
        default: { r:0, g:0, b:132 }
    
    }),

    control.color({
        name:"Color 1", 
        default: { r:0, g:255, b:0 }
    
    })
    
]

colors[3]=colors[1];
colors[4]=colors[0];
colors[5]=colors[1];
colors[6]=control.color({
        name:"Color 2",
        default: { r:255, g:0, b:0 }
});
colors[7]=colors[1];

//Red, blue, green
//  black, yellow, red, yellow, black, yellow, blue, yellow, 

led.fade_mode(2);
led.fade_speed(fade);
led.mirror(80);
led.step(10);
led.width(10);

for (s=0; s<8; s++)
{
    led.led(0);
    led.color(colors[0].r, colors[0].g, colors[0].b);
    led.draw();
    led.color(colors[1].r, colors[1].g, colors[1].b);
    led.draw();
    led.color(colors[2].r, colors[2].g, colors[2].b);
    led.draw();
    led.color(colors[3].r, colors[3].g, colors[3].b);
    led.draw();
    led.color(colors[4].r, colors[4].g, colors[4].b);
    led.draw();
    led.color(colors[5].r, colors[5].g, colors[5].b);
    led.draw();
    led.color(colors[6].r, colors[6].g, colors[6].b);
    led.draw();
    led.color(colors[7].r, colors[7].g, colors[7].b);
    led.draw();

    colors.push(colors.shift());

    led.delay((60/(bpm/60)) -1);
}

