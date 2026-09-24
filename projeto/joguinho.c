
#include<stdlib.h>
#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_image.h>
#include<allegro5/mouse.h>


int main() {
	al_init();
	al_init_image_addon();
	al_init_primitives_addon();
	int larguraTela = 1280;
	ALLEGRO_DISPLAY* display = al_create_display(larguraTela, 720);
	bool estaJogando = true;

	float velocidade = 0.04f;
	float raio = 70.0f;
	float gravidade = 0.04f;
	ALLEGRO_COLOR corVermelho = al_map_rgb(255, 0, 0);
	float chao = 560.0f;


	float x = 640.0;
	float y = chao;
	while (estaJogando) {
		//game loop
		al_clear_to_color(al_map_rgb(0, 200, 255));
		//mudar a tela sem deixar rastro
		al_draw_filled_circle(x, y, raio, corVermelho);
		//circulo

		al_draw_filled_rectangle(0, chao, 1280, 720, al_map_rgb(120, 30, 0));

		al_flip_display();

		if () {

			y -= 200;
		}


		y += gravidade;
		if (y + raio >= chao) {
			y -= gravidade;


		}




	}
	al_destroy_display(display);
	return 0;
}