#include "prototipos.h"

struct no{
	double inicio;
	double fim;
	NO *proximo;
};

NO *inserirInicio(NO *lista, double v, double f){	
	//printf("\nValor inserido:%lf",v);		//FEITA
	NO *novo;
	novo = (NO*) malloc(sizeof(NO));
	novo->inicio = v;
	novo->fim = f;
	novo->proximo = lista;
	return novo;
}

NO *inserirFinal(NO *lista, double v, double f){				//FEITA
	NO *novo, *aux2;
	aux2 = lista;
	novo = (NO*) malloc(sizeof(NO));
	novo->inicio = v;
	novo->inicio = f;

	if( lista == NULL){
		novo->proximo = lista;
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
			printf("%lf ",aux->inicio);
		}
		printf("\n");
	}
}

void mostrarInverso(NO *lista){					//FEITA
	if( lista == NULL){
		printf("Lista Vazia.\n");
	}else{
		if(lista->proximo == NULL){
			printf("%d ", lista->inicio);
			return ;
		}
		mostrarInverso(lista->proximo);
		printf("%d ", lista->inicio);
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
	if( lista == NULL){
		printf("Lista Vazia.\n");
		return lista;
	}else{

		NO *aux;
		//INICIO
		if(lista->inicio == valor){	
			printf("Valor %d encontrado.\n", valor);
			return lista->proximo;
		}
		for(aux=lista; aux->proximo != NULL; aux= aux->proximo){
			if( (aux->proximo)->inicio == valor){
				NO *local;
				local = (aux->proximo)->proximo;
				aux->proximo = local;
				printf("Valor %d encontrado.\n", valor);
				return lista;
			}
		}
		printf("Valor %d não encontrado.\n", valor);
		return lista;
	}
}

void liberar(NO *lista){						//FEITA
	if( lista->proximo != NULL){
		liberar(lista->proximo);
	}
	printf("Limpando elemento: %d\n", lista->inicio);
	free(lista);
}
