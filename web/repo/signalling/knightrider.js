//knightrider radar(s)

led.color(255,0,0);
led.fade_mode(2);
led.fade_speed(5);
led.clone_count(5);
led.clone_offset(20);

led.step(1);
led.repeat_begin(6);
led.draw();
led.delay(8);
led.repeat_end();

led.step(-1);
led.repeat_begin(6);
led.draw();
led.delay(8);
led.repeat_end();


