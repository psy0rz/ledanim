wildness=1;

led.fade_mode(1);
led.fade_speed(wildness);

led.repeat_begin(20);
led.led_rnd(0, led.leds);
led.color_rnd(0, 0,  0, 0,  100, 255);
led.draw();
led.repeat_end();

