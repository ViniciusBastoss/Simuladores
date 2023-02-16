#include "prototipos.h"

typedef struct no{
	double inicio;
	double fim;
	double prox_pacote;
	NO *proximo;
}NO;

NO *inserirInicio(NO *lista, double v, double f){	
	//printf("\nValor inserido:%lf",v);		//FEITA
	NO *novo;
	novo = (NO*) malloc(sizeof(NO));
	novo->inicio = v;
	novo->fim = f;
	novo->prox_pacote = v;
	novo->proximo = lista;
	return novo;
}

NO *inserirFinal(NO *lista, double v, double f){				//FEITA
	NO *novo, *aux2;
	aux2 = lista;
	novo = (NO*) malloc(sizeof(NO));
	novo->inicio = v;
	novo->fim = f;
	novo->prox_pacote = v;

	if( lista == NULL){
		novo->proximo = NULL;
		return novo;
	}else{
		NO *aux;	
		for(aux = lista; aux->proximo != NULL; aux = aux->proximo){
		}
		novo->proximo = NULL;
		aux->proximo = novo;
		return aux2;
	}
}

NO *inserirOrdenado(NO *lista, double v, double f){			//FEITA
	NO *novo;
	novo = (NO*) malloc(sizeof(NO));
	novo->inicio = v;
	novo->fim = f;
	novo->prox_pacote = v;
	if( lista == NULL){
		novo->proximo  = lista;
		return novo;
	}else{
		//INICIO
		if( lista->inicio > novo->inicio){
			novo->proximo = lista;
			return novo;
		}
		
		NO *aux;
		for(aux=lista; aux->proximo != NULL; aux= aux->proximo){
			//MEIO	
			if( aux->proximo->inicio > novo->inicio){
				novo->proximo = aux->proximo;
				aux->proximo = novo;
				return lista;
			}
		}
		novo->proximo = NULL;
		aux->proximo = novo;
		return lista;
	}
}

NO *criarLista(){								//FEITA
	return NULL;
	//linhavazia
}

NO *copiarLista(NO *lista){						//FEITA
	NO *aux = criarLista();
	for(; lista != NULL; lista= lista->proximo){
		aux = inserirFinal(aux, lista->inicio, lista->fim);
	}
	return aux;
}

void mostrarLista(NO *lista){					//FEITA
	if( lista == NULL){
		printf("Lista Vazia.\n");
	}else{
		printf("LISTA: ");
		for(NO *aux=lista; aux != NULL; aux= aux->proximo){
			printf("\n%lf %lf",aux->inicio,aux->fim);
		}
		printf("\n");
	}
}

void mostrarInverso(NO *lista){					//FEITA
	if( lista == NULL){
		printf("Lista Vazia.\n");
	}else{
		if(lista->proximo == NULL){
			printf("%lf ", lista->inicio);
			return ;
		}
		mostrarInverso(lista->proximo);
		printf("%lf ", lista->inicio);
	}
}

NO *buscar(NO *lista, double v){					//FEITA
	if( lista == NULL){
		printf("Lista Vazia.\n");
		return criarLista();
	}else{
		for(NO *aux=lista; aux->proximo != NULL; aux= aux->proximo){
			if( aux->inicio == v){
				printf("Valor encontrado.\n");
				return aux;
			}
		}
		printf("Valor não encontrado.\n");
		return criarLista();
	}
}

int qtdElementos(NO *lista){					//FEITA
	int qtd = 0;
	for(NO *aux=lista; aux != NULL; aux= aux->proximo){
		qtd++;
	}
	return qtd;
}

NO* removerElemento(NO *lista, double valor){		//FEITA
    NO *aux;
	if( lista == NULL){
		//printf("Lista Vazia.\n");
		return lista;
	}else{

		NO *aux;
		//INICIO
		if(lista->inicio == valor){	
			//printf("Valor %lf encontrado.\n", lista->fim);
			aux = lista->proximo;
			return aux;
		}
		for(aux=lista; aux->proximo != NULL; aux= aux->proximo){
			if( (aux->proximo)->inicio == valor){
				NO *local;
				local = (aux->proximo)->proximo;
				aux->proximo = local;
				//printf("Valor %lf encontrado.\n", aux->fim);
				return lista;
			}
		}
		printf("Valor %lf não encontrado.\n", valor);
		return lista;
	}
}

void liberar(NO *lista){						//FEITA
	if( lista->proximo != NULL){
		liberar(lista->proximo);
	}
	printf("Limpando elemento: %lf\n", lista->inicio);
	free(lista);
}


NO* trata_pacote_transmissao(NO *lista_chamadas, double tempo_decorrido, double *chegada_chamada){
   NO *aux, *transmissao_atual;
   double menor_tempo = 50000000000;
   //printf("eieie");
   if(lista_chamadas == NULL){
	return lista_chamadas;
   }
	  if(lista_chamadas->proximo == NULL){
		menor_tempo = lista_chamadas->prox_pacote;
		transmissao_atual = lista_chamadas;
		//printf("\n Inicio:%lf    fim:%lf   proxpacote:%lf", lista_chamadas->inicio, lista_chamadas->fim, lista_chamadas->prox_pacote);
	  }
	for(aux = lista_chamadas; aux->proximo != NULL; aux = aux->proximo){
		if(aux->fim < tempo_decorrido){
			//printf("\nTempo_Atual: %lf", tempo_decorrido);
			//printf("\nRemovido: Inicio:%lf    fim:%lf   proxpacote:%lf", aux->inicio, aux->fim, aux->prox_pacote);
			//mostrarLista(lista_chamadas);
			lista_chamadas = removerElemento(lista_chamadas, aux->inicio);
			//mostrarLista(lista_chamadas);

		}
		else
		   if(aux->prox_pacote < menor_tempo){
            menor_tempo = aux->prox_pacote;
			transmissao_atual = aux;
		}
	}
	transmissao_atual->prox_pacote += 0.02;
			//printf("\n Inicio:%lf    fim:%lf   proxpacote:%lf", lista_chamadas->inicio, lista_chamadas->fim, lista_chamadas->prox_pacote);
		//exit(10);
	//printf("kkkk");
	//printf("\nTempo_decorrido: %lf", tempo_decorrido);
	//mostrarLista(lista_chamadas);
	*chegada_chamada = menor_tempo;
	return lista_chamadas;

	
    
	//for(aux = lista_chamadas; aux->proximo != NULL; aux = aux->proximo){
	



}
