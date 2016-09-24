//knightrider radar(s)
size=5;
clones=0;
led.fade_speed(3);

led.color(255,0,0);
led.fade_mode(2);
led.width(size);
led.clone_offset(size*10);
led.clone_count(clones);

led.step(size);
led.repeat_begin(6);
led.draw();
led.delay(8);
led.repeat_end();

led.step(-size);
led.repeat_begin(6);
led.draw();
led.delay(8);
led.repeat_end();


