#include <stdio.h>
#include <stdlib.h>
#define MAX 100

typedef struct processos {
    int id;

    int temp_CPU;
    int temp_CPU_rest;

    int temp_disc;
    int temp_disc_rest;

    int rodadas_total;
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
processos desenfileirar () {
    processos p = fila[inicio];
    inicio = (inicio + 1) % MAX;
    return(p);
}
int vazio() {
    return(inicio == fim);
}

void enfileirar_disc(processos p) {
    fila_disc[fim_disc] = p;
    fim_disc = (fim_disc + 1) % MAX;
}
processos desenfileirar_disc() {
    processos p = fila_disc[inicio_disc];
    inicio_disc = (inicio_disc + 1) % MAX;
    return(p);
}
int vazio_disc() {
    return(inicio_disc == fim_disc);
}