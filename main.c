#include <stdio.h>
#include "funcoes.c"

int main() {
    processos processo[MAX];
    Fila *fila_cpu=criar_fila();
    Fila *fila_disc=criar_fila();

    int N;
    int quantum;
    int tempo_total;

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

        printf("Tempo de Disco (E/S): ");
        scanf("%d", &processo[i].temp_disc);

        printf("Numero de rodadas: ");
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

    printf("\nQuantum: ");
    scanf("%d", &quantum);

    printf("Tempo total de simulacao: ");
    scanf("%d", &tempo_total);

    printf("\n--- INICIO DA SIMULACAO ---\n");

    while (tempo < tempo_total) {
        printf("\nTempo %d\n", tempo);

        //int tamanho_io = (fim_disc - inicio_disc + MAX) % MAX;

        while(!vazio(fila_disc)) {
            processos p = desenfileirar(fila_disc);

            p.temp_disc_rest--;

            if (p.temp_disc_rest <= 0) {
                p.em_disco = 0;
                enfileirar(fila_cpu, p);
            } else {
                enfileirar_disc(fila_disc, p);
            }
        }

        if (!vazio(fila_cpu)) {
            processos p = desenfileirar(fila_cpu);

            if (!p.respondeu) {
                p.temp_resposta = tempo;
                p.respondeu = 1;
            }

            int exec = quantum;

            if (p.temp_CPU_rest < quantum)
                exec = p.temp_CPU_rest;

            printf("Executando Processo %d por %d unidades\n", p.id, exec);

            cpu_ocupada += exec;
            tempo += exec;

            p.temp_CPU_rest -= exec;

            if (p.temp_CPU_rest <= 0) {
                p.rodadas_rest--;

                if (p.rodadas_rest <= 0) {
                    p.finalizado = 1;
                    processo_finalizados++;

                    printf("Processo %d FINALIZADO\n", p.id);
                } else {
                    p.temp_CPU_rest = p.temp_CPU;
                    p.temp_disc_rest = p.temp_disc;
                    p.em_disco = 1;

                    enfileirar(fila_disc, p);

                    printf("Processo %d foi para E/S\n", p.id);
                }
            } else {
                enfileirar(fila_cpu, p);
            }
        } else {
            printf("CPU ociosa\n");
            tempo++;
        }

        //int tamanho = (fim - inicio + MAX) % MAX;

        while(!vazia(fila_cpu)) {
            processos p = desenfileirar(fila_cpu);

            p.temp_espera++;

            enfileirar(fila_cpu, p);
        }

        if (processo_finalizados == N)
            break;
    }

    printf("\n--- RESULTADOS ---\n");

    int soma_espera = 0;

    for (int i = 0; i < N; i++) {
        printf("\nProcesso %d\n", i);

        printf("Tempo de resposta: %d\n", processo[i].temp_resposta);
        printf("Tempo de espera: %d\n", processo[i].temp_espera);

        soma_espera += processo[i].temp_espera;
    }

    float uso_cpu = (cpu_ocupada * 100.0) / tempo;

    printf("\nUso da CPU: %.2f%%\n", uso_cpu);
    printf("Processos finalizados: %d\n", processo_finalizados);

    printf("Tempo medio de espera: %.2f\n",(float)soma_espera / N);

    return 0;
}