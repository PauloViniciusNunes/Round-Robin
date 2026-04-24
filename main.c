#include <stdio.h>
#include "funcoes.c"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

// ---------------- DESENHO ----------------

void desenhar_fila(Fila *fila, int y, ALLEGRO_FONT *font, char *titulo) {
    int x = 50;

    al_draw_text(font, al_map_rgb(255,255,255), x, y - 30, 0, titulo);

    int tam = tamanho(fila);

    for (int i = 0; i < tam; i++) {
        processos p = desenfileirar(fila);

        al_draw_filled_rectangle(x, y, x+60, y+40, al_map_rgb(100,100,255));

        char txt[10];
        sprintf(txt, "P%d", p.id);
        al_draw_text(font, al_map_rgb(255,255,255), x+10, y+10, 0, txt);

        enfileirar(fila, p);

        x += 70;
    }
}

// ---------------- MAIN ----------------

int main() {

    processos processo[MAX];

    Fila *fila_cpu = criar_fila();
    Fila *fila_disc = criar_fila();

    int N, quantum, tempo_total;

    int tempo = 0;
    int cpu_ocupada = 0;
    int processo_finalizados = 0;

    printf("Quantidade de processos: ");
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        processo[i].id = i;

        printf("\nProcesso %d\n", i);

        printf("Tempo de CPU: ");
        scanf("%d", &processo[i].temp_CPU);

        printf("Tempo de Disco: ");
        scanf("%d", &processo[i].temp_disc);

        printf("Rodadas: ");
        scanf("%d", &processo[i].rodadas);

        processo[i].temp_CPU_rest = processo[i].temp_CPU;
        processo[i].temp_disc_rest = 0;
        processo[i].rodadas_rest = processo[i].rodadas;

        processo[i].temp_espera = 0;
        processo[i].temp_resposta = 0;
        processo[i].respondeu = 0;

        processo[i].em_disco = 0;
        processo[i].finalizado = 0;

        enfileirar(fila_cpu, processo[i]);
    }

    printf("Quantum: ");
    scanf("%d", &quantum);

    printf("Tempo total: ");
    scanf("%d", &tempo_total);

    // -------- INIT ALLEGRO --------

    al_init();
    al_init_primitives_addon();
    al_init_font_addon();

    ALLEGRO_DISPLAY *display = al_create_display(1200, 800);
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    ALLEGRO_TIMER *timer = al_create_timer(0.5); // velocidade da simulação

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    ALLEGRO_FONT *font = al_create_builtin_font();

    al_start_timer(timer);

    int rodando = 1;

    processos atual;
    int executando = 0;
    int quantum_rest = quantum;

    // -------- LOOP --------

    while (rodando) {

        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER) {

            // ----- DISCO -----
            int tamanho_io = tamanho(fila_disc);

            for (int i = 0; i < tamanho_io; i++) {
                processos p = desenfileirar(fila_disc);

                p.temp_disc_rest--;

                if (p.temp_disc_rest <= 0) {
                    p.em_disco = 0;
                    enfileirar(fila_cpu, p);
                } else {
                    enfileirar(fila_disc, p);
                }
            }

            // ----- CPU -----
            if (!executando && !vazio(fila_cpu)) {
                atual = desenfileirar(fila_cpu);
                executando = 1;
                quantum_rest = quantum;

                if (!atual.respondeu) {
                    atual.temp_resposta = tempo;
                    atual.respondeu = 1;
                }
            }

            if (executando) {
                atual.temp_CPU_rest--;
                quantum_rest--;
                cpu_ocupada++;

                if (atual.temp_CPU_rest <= 0) {
                    atual.rodadas_rest--;

                    if (atual.rodadas_rest <= 0) {
                        atual.finalizado = 1;
                        processo_finalizados++;
                    } else {
                        atual.temp_CPU_rest = atual.temp_CPU;
                        atual.temp_disc_rest = atual.temp_disc;
                        enfileirar(fila_disc, atual);
                    }

                    executando = 0;
                }
                else if (quantum_rest <= 0) {
                    enfileirar(fila_cpu, atual);
                    executando = 0;
                }
            }

            // ----- ESPERA -----
            int tam = tamanho(fila_cpu);
            for (int i = 0; i < tam; i++) {
                processos p = desenfileirar(fila_cpu);
                p.temp_espera++;
                enfileirar(fila_cpu, p);
            }

            tempo++;

            // ----- RENDER -----
            al_clear_to_color(al_map_rgb(0,0,0));

            desenhar_fila(fila_cpu, 150, font, "CPU");
            desenhar_fila(fila_disc, 300, font, "DISCO");

            // processo atual
            if (executando) {
                al_draw_filled_rectangle(350, 50, 450, 100, al_map_rgb(255,100,100));

                char txt[20];
                sprintf(txt, "P%d", atual.id);
                al_draw_text(font, al_map_rgb(255,255,255), 370, 65, 0, txt);
            }

            // tempo
            char tempo_txt[50];
            sprintf(tempo_txt, "Tempo: %d", tempo);
            al_draw_text(font, al_map_rgb(255,255,255), 10, 10, 0, tempo_txt);

            al_flip_display();

            if (tempo >= tempo_total || processo_finalizados == N)
                rodando = 0;
        }

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            rodando = 0;
        }
    }

    printf("\nSimulacao finalizada.\n");

    return 0;
}