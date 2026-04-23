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

typedef struct no {
    processos p;
    struct no *proximo;
}No;

typedef struct {
    No *inicio;
    No *fim;
}Fila;

Fila *criar_fila() {
    Fila *f=(Fila *)malloc(sizeof(Fila));
    f->inicio=f->fim=NULL;
    return f;
}

int vazio(Fila *pfila) {
    if(pfila->inicio==NULL) {
        return 1;
    } else {
        return 0;
    }
}

void enfileirar(Fila *pfila, processos proc) {
    No *novo=(No *)malloc(sizeof(No));
    if(novo) {
        novo->p=proc;
        if(vazio(pfila)) { //fila vazia (inserir o primeiro elemento)
            pfila->inicio=pfila->fim=novo;
        } else {
            pfila->fim->proximo=novo;
            pfila->fim=novo;
        }
        novo->proximo=pfila->inicio;
    }
}

processos desenfileirar(Fila *pfila) {
    processos proc;
    No *remove=NULL;
    if(!vazio(pfila)) {
        remove=pfila->inicio;
        if(remove=pfila->fim) { //nó único
            pfila->inicio=pfila->fim=NULL;
        } else {
            pfila->inicio=remove->proximo;
            pfila->fim=pfila->inicio;
        }
        proc=remove->p;
    }
    free(remove);
    return proc;
}