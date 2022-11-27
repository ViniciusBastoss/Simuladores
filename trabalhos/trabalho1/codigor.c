#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct little_{
    unsigned long int no_eventos;
    double tempo_anterior;
    double soma_areas;
}little;
int aux = 1,aux2 = 0;
double vetcheg[15], vetsaid[15];
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

int acadaT = 100;
void coletaDados(little e_n,little e_w_chegada,little e_w_saida,int a){
    double aux,complementoEn, complementoEw_saida = 0,complementoEw_entrada = 0;
    if(a == 1){
        complementoEw_saida = (acadaT - e_w_saida.tempo_anterior)*(e_w_saida.no_eventos);
        complementoEw_entrada = (acadaT - e_w_chegada.tempo_anterior)*(e_w_chegada.no_eventos);
    }

    printf("\nTempo Atual: %d(s)",acadaT);
    double e_n_final = (e_n.soma_areas + (acadaT - e_n.tempo_anterior)*(e_n.no_eventos)) / acadaT;
    printf("\nE[N]: %lf\n", e_n_final);//lF
    
   // aux = (e_w_chegada.tempo_anterior > acada100)? acada100 : e_w_chegada.tempo_anterior;

    double e_w_final = ((e_w_chegada.soma_areas + (complementoEw_entrada) -
    (e_w_saida.soma_areas + complementoEw_saida))/(e_w_chegada.no_eventos));

    if(acadaT == 200){
        printf("e_w_chegada.soma_areas:%lf,complementoEw_entrada:%lf,e_w_saida.soma_areas:%lf,complementoEw_saida:%lf,e_w_chegada.no_eventos:%d", 
        e_w_chegada.soma_areas,complementoEw_entrada,e_w_saida.soma_areas,
        complementoEw_saida,e_w_chegada.no_eventos);
    }
    printf("\nE[W]: %lf\n", e_w_final);//lF
    /*printf("Erro de Little: %.20lf\n\n", e_n_final - lambda * e_w_final);//lF
    printf("Ocupacao: %lf\n", soma_tempo_servico/maximo(tempo_decorrido, servico));//lF
    */

 
   acadaT += 100;
   aux = 1;

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
    double GuardSaida;
    int contS = 0, contE = 0,contE2;

    /**
     Little
    */
   little e_n;
   little e_w_chegada;
   little e_w_saida;

   little e_n_aux,e_w_chegada_aux,e_w_saida_aux;

   inicia_little(&e_n);
   inicia_little(&e_w_chegada);
   inicia_little(&e_w_saida);

    //srand(time(NULL));
    srand(10000);
    printf("Tempo de simulacao: 36.000 (segundos).\n");
    //scanf("%lF",&tempo_simulacao);
    tempo_simulacao = 400;

    printf("Intervalo medio entre chegadas: 0.2 (segundos).\n");
    //scanf("%lF",&intervalo_medio_chegada);
    intervalo_medio_chegada = 10;

    printf("Informe o tempo medio de serviço (segundos):\n");
    //scanf("%lF",&tempo_medio_servico);
    tempo_medio_servico = 5;

    //gerando o tempo de chegada da primeira requisicao
    chegada = (-1.0 / (1.0/intervalo_medio_chegada)) * log(aleatorio());

    while(tempo_decorrido <= tempo_simulacao){

        tempo_decorrido = !fila ? chegada : minimo(chegada, servico);

        if(chegada >= acadaT && aux){
            contE2 = contE;
            e_w_chegada_aux = e_w_chegada;
            e_n_aux = e_n;
            aux = 0;
        }

        if(contE2 == contS && chegada >= acadaT){
            if(GuardSaida >= acadaT){
                coletaDados(e_n_aux,e_w_chegada_aux,e_w_saida_aux,1);
            }
            else{
                coletaDados(e_n_aux,e_w_chegada_aux,e_w_saida_aux,0);
            }
            printf("\nGurad:%lf\n",GuardSaida);
        }

        if(tempo_decorrido == chegada){
            contE++;
            printf("Chegada em %lf.", tempo_decorrido);
            printf("   NumeroIf:%d\n",contE);
            if(tempo_decorrido <= 200){
                vetcheg[aux] = tempo_decorrido;
                aux++;
            }
            //aux += tempo_decorrido;{}
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
            contS++;
            GuardSaida = servico;
            printf("Saida  em %lf.", tempo_decorrido);
            printf("  NumeroElse:%d\n",contS);
                 if(tempo_decorrido <= 200){
                vetsaid[aux2] = tempo_decorrido;    
                aux2++;
            }
            fila--;

            if(fila){
                servico = tempo_decorrido + (-1.0 / (1.0/tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }
            //little
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior)*e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos--;
             
            e_w_saida_aux = e_w_saida;
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
    (tempo_decorrido - e_w_saida.tempo_anterior)*e_w_saida.no_eventos;

    double e_n_final = e_n.soma_areas / tempo_decorrido;
    double e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas)/e_w_chegada.no_eventos;
    
    double lambda = e_w_chegada.no_eventos / tempo_decorrido;

    printf("\nE[N]: %lf\n", e_n_final);//lF
    printf("E[W]: %lf\n", e_w_final);//lF
    printf("lambda: %lf\n\n", lambda);//lF
    
    printf("Erro de Little: %.20lf\n\n", e_n_final - lambda * e_w_final);//lF

    printf("Ocupacao: %lf\n", soma_tempo_servico/maximo(tempo_decorrido, servico));//lF
    printf("Max fila: %ld\n", maxFila);
    printf("Aux m em %d.\n", aux);
    return 0;

}