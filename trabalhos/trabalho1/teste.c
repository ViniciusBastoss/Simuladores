#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

typedef struct little_{
    unsigned long int no_eventos;
    double tempo_anterior;
    double soma_areas;
} little;

double aleatorio() {
	double u = rand() / ((double) RAND_MAX + 1);
	//limitando entre (0,1]
	u = 1.0 - u;

	return (u);
}

double minimo(double num1, double num2){
    if(num1 < num2){
        return num1;
    }
    return num2;
}

double maximo(double num1, double num2){
    if(num1 > num2){
        return num1;
    }
    return num2;
}

void inicia_little(little * l){
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}

int main(){
    double tempo_simulacao;
    double tempo_decorrido = 0.0;

    double intervalo_medio_chegada;
    double tempo_medio_servico;

    double chegada;
    double servico;

    double soma_tempo_servico = 0.0;

    unsigned long int fila = 0;
    unsigned long int max_fila = 0;

    /**
    Little
    */
    little e_n;
    little e_w_chegada;
    little e_w_saida;

    inicia_little(&e_n);
    inicia_little(&e_w_chegada);
    inicia_little(&e_w_saida);
    /**
    Little -- fim
    */

    //srand(time(NULL));
    srand(10000);
    printf("Tempo de simulacao: 36.000 (segundos).\n");
    //scanf("%lF",&tempo_simulacao);
    tempo_simulacao = 400;

    printf("Intervalo medio entre chegadas: 0.2 (segundos).\n");
    //scanf("%lF",&intervalo_medio_chegada);
    intervalo_medio_chegada = 40;

    printf("Informe o tempo medio de serviço (segundos):");
    //scanf("%lF",&tempo_medio_servico);
    tempo_medio_servico = 5;

    //gerando o tempo de chegada da primeira requisicao.
    chegada = (-1.0 / (1.0/intervalo_medio_chegada)) * log(aleatorio());

    while(tempo_decorrido <= tempo_simulacao){
        tempo_decorrido = !fila ? chegada : minimo(chegada, servico);

        //chegada
        if(tempo_decorrido == chegada){
            printf("Chegada em %lf.\n", tempo_decorrido);
            if(!fila){
                servico = tempo_decorrido + (-1.0 / (1.0/tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }
            fila++;
            max_fila = fila > max_fila? fila: max_fila;

            chegada = tempo_decorrido + (-1.0 / (1.0/intervalo_medio_chegada)) * log(aleatorio());

            //little
            e_n.soma_areas += 
            (tempo_decorrido - e_n.tempo_anterior)*e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos++;

            e_w_chegada.soma_areas +=
            (tempo_decorrido - e_w_chegada.tempo_anterior)*e_w_chegada.no_eventos;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_chegada.no_eventos++;

        }else{//saida
            //printf("Saida em %lF.\n", tempo_decorrido);
            fila--;

            if(fila){
                servico = tempo_decorrido + (-1.0 / (1.0/tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }

            //little
            e_n.soma_areas += 
            (tempo_decorrido - e_n.tempo_anterior)*e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos--;

            e_w_saida.soma_areas +=
            (tempo_decorrido - e_w_saida.tempo_anterior)*e_w_saida.no_eventos;
            e_w_saida.tempo_anterior = tempo_decorrido;
            e_w_saida.no_eventos++;
        }
    }
    e_w_chegada.soma_areas +=
    (tempo_decorrido - e_w_chegada.tempo_anterior)*e_w_chegada.no_eventos;
    
    e_w_saida.soma_areas +=
    (tempo_decorrido - e_w_saida.tempo_anterior)*e_w_saida.no_eventos;

    double e_n_final = e_n.soma_areas / tempo_decorrido;
    double e_w_final =
    (e_w_chegada.soma_areas - e_w_saida.soma_areas)/e_w_chegada.no_eventos;
    double lambda = e_w_chegada.no_eventos / tempo_decorrido;

    printf("E[N]: %lf\n", e_n_final);
    printf("E[W]: %lf\n", e_w_final);
    printf("lambda: %lf\n\n", lambda);

    printf("Erro de Little: %.20lf\n\n", e_n_final - lambda * e_w_final);

    printf("Ocupacao: %lf.\n", soma_tempo_servico/maximo(tempo_decorrido, servico));
    printf("Max fila: %d.\n", max_fila);

    return 0;
}