#include <stdio.h>
#include <stdlib.h>

typedef struct no NO;

NO *criarLista();

NO *inserirInicio(NO *lista, double v, double f);

NO *inserirFinal(NO *lista, double v, double f);

NO *inserirOrdenado(NO *lista, double v, double f);

void mostrarLista(NO *lista);

void mostrarInverso(NO *lista);

NO *copiarLista(NO *lista);

NO *buscar(NO *lista, double v);

int qtdElementos(NO *lista);

NO* removerElemento(NO *lista, double valor);

void liberar(NO *lista);

NO* trata_pacote_transmissao(NO *lista_chamadas, double tempo_decorrido, double *chegada_chamada);