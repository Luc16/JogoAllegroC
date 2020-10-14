#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

enum KEYS{ UP, DOWN, LEFT, RIGHT};

int main()
{
	int largura = 640;
	int altura = 480;
    char local[500];
	int i;


	bool sair = false;
	int pos_x = largura / 2;
	int pos_y = altura / 2;

	const int max_desenho_parado = 12;
	const int max_desenho_pulando = 21;
	const int max_desenho_andando = 8;
	int desenho_atual = 0;
	int i_desenho = 0;
    int espera = 5;
    //int espera_pulando = 10;

	bool keys[4] = {false, false, false, false};

    ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_BITMAP *pato_parado[max_desenho_parado], *pato_pulando[max_desenho_pulando], *pato_andando[max_desenho_andando];

	if(!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(largura, altura);			//create our display object

	if(!display)										//test display object
		return -1;

	al_init_primitives_addon();
	al_install_keyboard();
	al_init_image_addon();


	for(i=0; i<max_desenho_parado; i++){
        sprintf(local, "Images/pato/pato_idle/pato_idle-%d.png.png", i+1);
        pato_parado[i] = al_load_bitmap(local);
        if(!pato_parado[i]){
          printf("erro");
        }
    }
    for(i=0; i<max_desenho_pulando; i++){
        sprintf(local, "Images/pato/pato_jump/pato_jump-%d.png.png", i+1);
        pato_pulando[i] = al_load_bitmap(local);
        if(!pato_pulando[i]){
          printf("erro");
        }
    }
    for(i=0; i<max_desenho_andando; i++){
        sprintf(local, "Images/pato/pato_walk/pato_walk-%d.png.png", i+1);
        pato_andando[i] = al_load_bitmap(local);
        if(!pato_andando[i]){
          printf("erro");
        }
    }

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0/60);

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_start_timer(timer);

	while(!sair)
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
					sair = true;
					break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_TIMER){
            if(keys[UP]){
               pos_y -= keys[UP] * 10;
               if(++i_desenho >= espera)
               {
                    if(++desenho_atual >= max_desenho_pulando){
                         desenho_atual = 0;
                    }
                    i_desenho = 0;
               }
            }
            else if(keys[RIGHT]){
               pos_x += keys[RIGHT] * 10;
               if(++i_desenho >= espera)
               {
                    if(++desenho_atual >= max_desenho_andando){
                         desenho_atual = 0;
                    }
                    i_desenho = 0;
               }
            }
            else if(keys[LEFT]){
               pos_x -= keys[LEFT] * 10;
               if(++i_desenho >= espera)
               {
                    if(++desenho_atual >= max_desenho_andando){
                         desenho_atual = 0;
                    }
                    i_desenho = 0;
               }
            }
            else{
                if(++i_desenho >= espera){
                    if(++desenho_atual >= max_desenho_parado)
                    {
                         desenho_atual = 0;
                    }
                i_desenho = 0;
                }
            }

		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			sair = true;
		}

		pos_y += keys[DOWN] * 10;

		// al_draw_filled_circle(pos_x + 30, pos_y + 30, 20, al_map_rgb(255, 255, 255));
        if(keys[UP]){
            al_draw_bitmap(pato_pulando[desenho_atual], pos_x, pos_y, 0);
        }
        else if(keys[RIGHT]){
            al_draw_bitmap(pato_andando[desenho_atual], pos_x, pos_y, 0);
        }
        else if(keys[LEFT]){
            al_draw_bitmap(pato_andando[desenho_atual], pos_x, pos_y, ALLEGRO_FLIP_HORIZONTAL);
        }
		else{
            al_draw_bitmap(pato_parado[desenho_atual], pos_x, pos_y, 0);
		}
		al_flip_display();
		al_clear_to_color(al_map_rgb(0,0,0));
	}
	for(i=0; i<max_desenho_parado; i++){
        al_destroy_bitmap(pato_parado[i]);
	}
	for(i=0; i<max_desenho_pulando; i++){
        al_destroy_bitmap(pato_pulando[i]);
	}

	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						//destroy our display object

	return 0;
}
