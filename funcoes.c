#include <stdio.h>
#include <stdlib.h>
#define MAX 100

typedef struct processos {
    int id;

    int temp_CPU;
    int temp_CPU_rest;

    int temp_disc;
    int temp_disc_rest;

    int rodadas;
    int rodadas_rest;

    int temp_espera;
    int temp_resposta;
    int respondeu;

    int em_disco;
    int finalizado;

}processos;

processos fila[MAX];
int inicio = 0;
int fim = 0;

processos fila_disc[MAX];
int inicio_disc = 0;
int fim_disc = 0;

void enfileirar(processos p) {
    fila[fim] = p;
    fim = (fim + 1) % MAX;
}

void enfileirar_disc(processos p) {
    fila_disc[fim_disc] = p;
    fim_disc = (fim_disc + 1) % MAX;
}