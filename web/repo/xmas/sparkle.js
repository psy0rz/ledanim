// sparkly effect for xmas stuff
// a blue icey glow with occasional sparkles

// sparkles
led.led_rnd(0, leds);
led.color(255, 255, 255);
led.fade_mode(2);
led.fade_speed(20);
led.draw();

//blue glow
led.repeat_begin_rnd(0,100);
led.led_rnd(0, leds);
led.color_rnd(0, 0,  0, 0,  100, 250);
led.fade_mode(1);
led.fade_speed(1);
led.draw();
led.delay(0);
led.repeat_end();

        