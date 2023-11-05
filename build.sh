#!/bin/bash
ALEGRO=$(pkg-config --cflags --libs \
  allegro-5\
  allegro_main-5\
  allegro_acodec-5\
  allegro_audio-5\
  allegro_color-5\
  allegro_dialog-5\
  allegro_font-5\
  allegro_image-5\
  allegro_memfile-5\
  allegro_physfs-5\
  allegro_primitives-5\
  allegro_ttf-5)
gcc -c cena_podio.c -o cena_podio.o $ALEGRO
gcc -c cena_inicio.c -o cena_inicio.o $ALEGRO
gcc -c cena_jogo.c -o cena_jogo.o $ALEGRO
gcc -c jogo_final.c -o jogo_final.o $ALEGRO
gcc -o main cena_inicio.o cena_podio.o jogo_final.o cena_jogo.o $(pkg-config --libs allegro-5 allegro_main-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_memfile-5 allegro_physfs-5 allegro_primitives-5 allegro_ttf-5) -lm

#gcc -lcurl -Wall $ALEGRO -lm cena_inicio.o cena_podio.o jogo_final.o cena_jogo.o -o main
# clang -Wall $ALEGRO test.c -o main
./main
