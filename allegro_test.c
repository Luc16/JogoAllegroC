#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

enum KEYS{ UP, DOWN, LEFT, RIGHT};

int main(void)
{
int width = 640;
int height = 480;
    char local[500];
int i;


bool done = false;
int pos_x = width / 2;
int pos_y = height / 2;

const int maxFrame = 12;
int curFrame = 0;
int frameCount = 0;
    int frameDelay = 5;

bool keys[4] = {false, false, false, false};

    ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer;
ALLEGRO_BITMAP *pato[maxFrame];

if(!al_init()) //initialize Allegro
return -1;

display = al_create_display(width, height); //create our display object

if(!display) //test display object
return -1;

al_init_primitives_addon();
al_install_keyboard();

// for(i=0; i<maxFrame; i++){

        sprintf(local, "Images/pato/pato_idle/pato_idle-%d.png.png", i+1);
        pato[0] = al_load_bitmap("Images/pato/pato_idle/pato_idle-%d.png.png");
        if (!pato[0]){
          printf("erro com o pato");
        }

// }
// for(i=0; i<maxFrame; i++){
//         al_convert_mask_to_alpha(pato[i], al_map_rgb(255, 255, 255));
// }

event_queue = al_create_event_queue();
timer = al_create_timer(1.0/60);

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
al_register_event_source(event_queue, al_get_keyboard_event_source());
al_register_event_source(event_queue, al_get_display_event_source(display));

while(!done)
{
ALLEGRO_EVENT ev;
al_wait_for_event(event_queue, &ev);

if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
{
switch(ev.keyboard.keycode)
{
case ALLEGRO_KEY_UP:
keys[UP] = true;
break;
case ALLEGRO_KEY_DOWN:
keys[DOWN] = true;
break;
case ALLEGRO_KEY_RIGHT:
keys[RIGHT] = true;
break;
case ALLEGRO_KEY_LEFT:
keys[LEFT] = true;
break;
}
}
else if(ev.type == ALLEGRO_EVENT_KEY_UP)
{
switch(ev.keyboard.keycode)
{
case ALLEGRO_KEY_UP:
keys[UP] = false;
break;
case ALLEGRO_KEY_DOWN:
keys[DOWN] = false;
break;
case ALLEGRO_KEY_RIGHT:
keys[RIGHT] = false;
break;
case ALLEGRO_KEY_LEFT:
keys[LEFT] = false;
break;
case ALLEGRO_KEY_ESCAPE:
done = true;
break;
}
}
else if(ev.type == ALLEGRO_EVENT_TIMER){
            if(++frameCount >= frameDelay)
            {
                if(++curFrame >= maxFrame)
                {
                    curFrame = 0;
                }
                frameCount = 0;
            }

}
else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
{
done = true;
}

pos_y -= keys[UP] * 10;
pos_y += keys[DOWN] * 10;
pos_x -= keys[LEFT] * 10;
pos_x += keys[RIGHT] * 10;

al_draw_bitmap(pato[curFrame], pos_x, pos_y, 0);

al_flip_display();
al_clear_to_color(al_map_rgb(0,0,0));
}

for(i=0; i<maxFrame; i++){
        al_destroy_bitmap(pato[i]);
}

al_destroy_event_queue(event_queue);
al_destroy_display(display); //destroy our display object

return 0;
}
