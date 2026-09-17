#include<stdlib.h>
#include<stdio.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_primitives.h>
#include<allegro5/allegro_image.h>

int main() {
	al_init();
	al_init_image_addon();
	al_init_primitives_addon();
	int larguraTela = 1280;
	ALLEGRO_DISPLAY* display = al_create_display(larguraTela, 720);
	bool estaJogando = true;
	ALLEGRO_BITMAP* image = al_load_bitmap("white-foam-bubbles-texture-background-image-png.webp");

	float velocidade = 0.04f;
	float raio = 100;
	ALLEGRO_COLOR corVermelho = al_map_rgb(255, 0, 0);


	float x = 640.0;
	while (estaJogando) {
		//game loop
		al_clear_to_color(al_map_rgb(255, 255, 255));
		//mudar a tela sem deixar rastro
		al_draw_filled_circle(x, 360, raio, corVermelho);
		//circulo

		al_draw_bitmap(image, 100, 100, 0);

		al_flip_display();
		x += velocidade;
		if (x + raio >= larguraTela) {
			velocidade = velocidade * -1;
		}
		if (x + raio == raio) {
			velocidade = velocidade * -1;
		}




	}
	al_destroy_display(display);
	return 0;
}