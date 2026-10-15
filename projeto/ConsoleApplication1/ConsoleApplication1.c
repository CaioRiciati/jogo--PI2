#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#define LARGURA 900
#define ALTURA 500
#define VELOCIDADE 4.0

int main(void) {
    if (!al_init()) {
        printf("Erro ao inicializar Allegro!\n");
        return 1;
    }

    if (!al_install_keyboard()) {
        printf("Erro ao inicializar teclado!\n");
        return 1;
    }

    if (!al_init_image_addon()) {
        printf("Erro ao inicializar imagens!\n");
        return 1;
    }

    ALLEGRO_DISPLAY* janela = al_create_display(LARGURA, ALTURA);
    if (!janela) {
        printf("Erro ao criar janela!\n");
        return 1;
    }

    ALLEGRO_BITMAP* fundo = al_load_bitmap("Imagem_de_fundo.png");
    ALLEGRO_BITMAP* sprite_sheet = al_load_bitmap("skeleton-idle2.png");

    if (!sprite_sheet) {
        printf("Erro ao carregar skeleton-idle2.png\n");
        al_destroy_display(janela);
        return 1;
    }

    float jogador_x = LARGURA / 2;
    float jogador_y = ALTURA / 2; // Centralizado melhor na tela

    int quantidade_frames = 12;
    int largura_sprite = al_get_bitmap_width(sprite_sheet) / quantidade_frames;
    int altura_sprite = al_get_bitmap_height(sprite_sheet);

    // ALTERE AQUI: 0.5f significa 50% do tamanho. Se quiser menor, use 0.3f.
    float escala = 0.6f;
    float largura_final = largura_sprite * escala;
    float altura_final = altura_sprite * escala;

    // VARIÁVEIS DO PULO
    float vel_y = 0;              // Velocidade vertical atual
    float gravidade = 0.6f;       // Força que puxa para baixo
    float forca_pulo = -12.0f;    // Altura/força do pulo (negativo sobe)
    bool pulando = false;         // Controla se o jogador está no ar
    float chao_atual = jogador_y; // Guarda de onde o pulo começou

    int frame = 0;
    float contador_animacao = 0;

    bool teclas[ALLEGRO_KEY_MAX] = { false };
    bool rodando = true;

    // Fila de eventos
    ALLEGRO_EVENT_QUEUE* fila = al_create_event_queue();
    al_register_event_source(fila, al_get_keyboard_event_source());
    al_register_event_source(fila, al_get_display_event_source(janela));

    while (rodando) {
        ALLEGRO_EVENT evento;
        while (al_get_next_event(fila, &evento)) {
            if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                rodando = false;
            }

            if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
                teclas[evento.keyboard.keycode] = true;
                if (evento.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    rodando = false;
                }
            }

            if (evento.type == ALLEGRO_EVENT_KEY_UP) {
                teclas[evento.keyboard.keycode] = false;
            }
        }

        bool andando = false;

        // Só permite andar em W e S se NÃO estiver pulando (evita bugar a referência do chão)
        if (!pulando) {
            if (teclas[ALLEGRO_KEY_W] || teclas[ALLEGRO_KEY_UP]) {
                jogador_y -= VELOCIDADE;
                andando = true;
            }
            if (teclas[ALLEGRO_KEY_S] || teclas[ALLEGRO_KEY_DOWN]) {
                jogador_y += VELOCIDADE;
                andando = true;
            }
        }

        // Movimentação horizontal livre (pode andar enquanto pula)
        if (teclas[ALLEGRO_KEY_A] || teclas[ALLEGRO_KEY_LEFT]) {
            jogador_x -= VELOCIDADE;
            andando = true;
        }
        if (teclas[ALLEGRO_KEY_D] || teclas[ALLEGRO_KEY_RIGHT]) {
            jogador_x += VELOCIDADE;
            andando = true;
        }

        // COMANDO DE PULO (Barra de Espaço)
        if (teclas[ALLEGRO_KEY_SPACE] && !pulando) {
            chao_atual = jogador_y; // Salva o y de onde ele pulou
            vel_y = forca_pulo;     // Aplica o impulso para cima
            pulando = true;
        }

        // FÍSICA DO PULO
        if (pulando) {
            vel_y += gravidade;     // Gravidade puxando para baixo continuamente
            jogador_y += vel_y;     // Atualiza a posição com a velocidade

            // Se o jogador caiu de volta ou abaixo do chão de origem
            if (jogador_y >= chao_atual) {
                jogador_y = chao_atual; // Prende ele no chão exato
                vel_y = 0;
                pulando = false;        // Pulo terminou
            }
        }

        // Limitador de tela perfeito baseado na escala nova
        if (jogador_x < 0) jogador_x = 0;
        if (jogador_y < 0) jogador_y = 0;
        if (jogador_x > LARGURA - largura_final) jogador_x = LARGURA - largura_final;
        if (jogador_y > ALTURA - altura_final) jogador_y = ALTURA - altura_final;

        if (andando || pulando) { // Continua animando se estiver pulando
            contador_animacao += 0.2;
            if (contador_animacao >= 1) {
                frame++;
                if (frame >= quantidade_frames) {
                    frame = 0;
                }
                contador_animacao = 0;
            }
        }
        else {
            frame = 0;
        }

        al_clear_to_color(al_map_rgb(30, 30, 30));

        // Desenha o fundo
        al_draw_scaled_bitmap(
            fundo,
            0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo),
            0, 0, LARGURA, ALTURA,
            0
        );

        int origem_x = frame * largura_sprite;

        // 1. Criamos uma sub-imagem contendo APENAS o frame atual da animação (sem corte)
        ALLEGRO_BITMAP* frame_atual = al_create_sub_bitmap(sprite_sheet, origem_x, 0, largura_sprite, altura_sprite);

        // 2. Desenhamos essa sub-imagem aplicando a redução de escala perfeitamente
        if (frame_atual) {
            al_draw_scaled_rotated_bitmap(
                frame_atual,
                0, 0,               // Ponto de pivô dentro do sprite (0,0 = topo esquerdo)
                jogador_x, jogador_y, // Onde ele vai aparecer na tela
                escala, escala,     // O quanto ele vai diminuir (X e Y)
                0,                  // Ângulo de rotação (0 = em pé)
                0                   // Flags
            );

            // 3. Importante: Destruir a sub-imagem temporária para não travar a memória do PC
            al_destroy_bitmap(frame_atual);
        }

        al_flip_display();
        al_rest(0.01);
    }

    al_destroy_bitmap(fundo);
    al_destroy_bitmap(sprite_sheet);
    al_destroy_event_queue(fila);
    al_destroy_display(janela);
    al_shutdown_image_addon();

    return 0;
}
