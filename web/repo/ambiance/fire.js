// fire effect


//glowing red
led.fade_mode(1);
led.fade_speed_rnd(3,5);

led.repeat_begin_rnd(20,100);
led.led_rnd(0, led.leds);
led.color_rnd(100, 200,  0, 0,  0,0);
led.draw();
led.repeat_end();

//yellow flames
led.fade_mode(1);
led.fade_speed_rnd(20,255);
led.led_rnd(0, led.leds);
led.color_rnd(255, 255,  255, 255,  0,0);
led.draw();