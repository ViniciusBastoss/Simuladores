#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct little_{
    unsigned long int no_eventos;
    double tempo_anterior;
    double soma_areas;
}little;

double aleatorio(){
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
    if(num1 < num2){
        return num2;
    }
    return num1;
}

void inicia_little(little *l){
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}

int main(){
    double chegada;
    double servico;

    double soma_tempo_servico = 0.0;

    double tempo_simulacao;
    double tempo_decorrido = 0.0;

    double intervalo_medio_chegada;
    double tempo_medio_servico;

    unsigned long int fila = 0;
    unsigned long int maxFila = 0;

    /**
     Little
    */
   little e_n;
   little e_w_chegada;
   little e_w_saida;

   inicia_little(&e_n);
   inicia_little(&e_w_chegada);
   inicia_little(&e_w_saida);

    srand(time(NULL));

    printf("Informe o tempo de simulacao (segundos):");
    scanf("%lF",&tempo_simulacao);

    printf("Informe o intervalo medio entre chegadas (segundos):");
    scanf("%lF",&intervalo_medio_chegada);

    printf("Informe o tempo medio de serviço (segundos):");
    scanf("%lF",&tempo_medio_servico);

    //gerando o tempo de chegada da primeira requisicao
    chegada = (-1.0 / (1.0/intervalo_medio_chegada)) * log(aleatorio());

    while(tempo_decorrido <= tempo_simulacao){
        tempo_decorrido = !fila ? chegada : minimo(chegada, servico);

        if(tempo_decorrido == chegada){
            printf("Chegada em %lF.\n", tempo_decorrido);
            if(!fila){
                servico = tempo_decorrido + (-1.0 / (1.0/tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
                
            }
            fila++;
            maxFila = maximo(maxFila,fila);
            chegada = tempo_decorrido + (-1.0 / (1.0/intervalo_medio_chegada)) * log(aleatorio());
            //little
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior)*e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos++;

            e_w_chegada.soma_areas +=
            (tempo_decorrido - e_w_chegada.tempo_anterior)*e_w_chegada.no_eventos;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_chegada.no_eventos++;


        }else{
            printf("Saida  em %lF.\n", tempo_decorrido);
            fila--;

            if(fila){
                servico = tempo_decorrido + (-1.0 / (1.0/tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }
            //little
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior)*e_n.no_eventos;
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

    e_n.soma_areas +=
    (tempo_decorrido - e_n.tempo_anterior)*e_n.no_eventos;

    e_w_saida.soma_areas +=
    (tempo_decorrido - e_w_saida.tempo_anterior)*e_n.no_eventos;

    double e_n_final = e_n.soma_areas / tempo_decorrido;
    double e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas)/e_w_chegada.no_eventos;
    double lambda = 
    printf("lambda: %F\n\n", lambda);
    
    printf("Erro de Little: %.20lF\n\n", e_n_final - lambda * e_w_final);

    printf("Ocupacao: %lF\n", soma_tempo_servico/maximo(tempo_decorrido, servico));
    printf("Max fila: %ld\n", maxFila);
    return 0;

}
