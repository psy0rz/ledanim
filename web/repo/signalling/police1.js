//policy lights (will add more in the future)
width=led.leds/4;


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
led.delay(14);

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
led.delay(14);

led.led(led.leds-width);
led.color(0,0,0);
led.draw();
