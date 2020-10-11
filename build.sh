#!/bin/bash
ALEGRO=$(pkg-config --cflags --libs allegro-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5)
# clang -c cena_podio.c -o cena_podio.o 
# clang -c jogo_final.c -o jogo_final.o
# clang -lcurl -Wall $ALEGRO cena_podio.o jogo_final.o -o main 
clang -Wall $ALEGRO cena_inicio.c -o main 
./main