#ifndef _PROTOTIPOS_H
#define _PROTOTIPOS_H


typedef struct processos processos;
typedef struct no No;
typedef struct fila Fila;

Fila *criar_fila();
int vazio(Fila *);
int tamanho(Fila *);
void empilhar(Fila);
void enfileirar(Fila *, processos);
processos desenfileirar(Fila *);

#endif
