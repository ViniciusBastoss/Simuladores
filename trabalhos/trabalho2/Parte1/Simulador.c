#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct little_
{
    unsigned long int no_eventos;
    double tempo_anterior;
    double soma_areas;
} little;

double aleatorio()
{
    double u = rand() / ((double)RAND_MAX + 1);
    // limitando entre (0,1]
    u = 1.0 - u;
    return (u);
}

double minimo(double num1, double num2)
{
    if (num1 < num2)
    {
        return num1;
    }
    return num2;
}

double maximo(double num1, double num2)
{
    if (num1 < num2)
    {
        return num2;
    }
    return num1;
}

void inicia_little(little *l)
{
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}

double acadaT , tempo = 100;

void coletaDados(little e_n, little e_w_chegada, little e_w_saida, double soma_tempo_servico)
{
    double e_n_final = (e_n.soma_areas) / acadaT;
    double e_w_final = ((e_w_chegada.soma_areas -
                         (e_w_saida.soma_areas)) /
                        (abs(e_w_chegada.no_eventos)));
    double lambda = e_w_chegada.no_eventos / acadaT;

    printf("%lf,%lf,", acadaT, e_n_final);               // Tempo Atual   E[N]
    printf("%lf", e_w_final);                          // E[W]:
    printf(",%.15lf", e_n_final - lambda * e_w_final); // Erro de Little:
    printf(",%lf\n", soma_tempo_servico / acadaT);      // ocupação

    acadaT += tempo;
}

int main()
{
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
    double TAM_pacote;
    double soma_total_pacot = 0;
    acadaT = tempo;
    double TAM_max_pacote;
    double link_capacidade;
    /**
     Little
    */
    little e_n;
    little e_w_chegada;
    little e_w_saida;

    inicia_little(&e_n);
    inicia_little(&e_w_chegada);
    inicia_little(&e_w_saida);

    // srand(time(NULL));
    srand(10000);
    TAM_max_pacote = 1500;
    link_capacidade = 2500*100;
    tempo_simulacao = 36000;
    intervalo_medio_chegada = 0.01;
    //tempo_medio_servico = 0.16; // 80% = 0,16 ; 90% = 0,18 ; 95% = 0,19; 99% = 0,198

    puts("Tempo,E[N],E[W],Erro_Little,Ocupacao");
    // gerando o tempo de chegada da primeira requisicao
    chegada = (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());

    while (tempo_decorrido <= tempo_simulacao)
    {

        tempo_decorrido = !fila ? minimo(chegada, acadaT) : minimo(minimo(chegada, servico), acadaT);

        if (tempo_decorrido == chegada)
        {
            if (!fila)
            {
                TAM_pacote = (-1.0 / (1.0 / TAM_max_pacote)) * log(aleatorio());
                servico = tempo_decorrido + (-1.0 / (1.0 / (TAM_pacote/link_capacidade))) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
                soma_total_pacot += TAM_pacote;
            }
            fila++;
            maxFila = maximo(maxFila, fila);
            chegada = tempo_decorrido + (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());
            // little
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos++;

            e_w_chegada.soma_areas +=
                (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_chegada.no_eventos++;
        }
        else 
            if (tempo_decorrido == servico)
            {
                fila--;

                if (fila)
                {
                    TAM_pacote = (-1.0 / (1.0 / TAM_max_pacote)) * log(aleatorio());
                    servico = tempo_decorrido + (-1.0 / (1.0 / (TAM_pacote/link_capacidade))) * log(aleatorio());
                    soma_tempo_servico += servico - tempo_decorrido;
                    soma_total_pacot += TAM_pacote;
                }
                // little
                e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
                e_n.tempo_anterior = tempo_decorrido;
                e_n.no_eventos--;


                e_w_saida.soma_areas +=
                    (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;
                e_w_saida.tempo_anterior = tempo_decorrido;
                e_w_saida.no_eventos++;


            }
            else if (tempo_decorrido == acadaT)
            {
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

            e_w_saida.tempo_anterior = tempo_decorrido;
            e_n.tempo_anterior = tempo_decorrido;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            coletaDados(e_n,e_w_chegada,e_w_saida,soma_tempo_servico);
            }


    }
    // printf("\nConte:%d\n",contE);

    e_w_chegada.soma_areas +=
        (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;

    e_n.soma_areas +=
        (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;

    e_w_saida.soma_areas +=
        (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

    double e_n_final = e_n.soma_areas / tempo_decorrido;
    double e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;

    double lambda = e_w_chegada.no_eventos / tempo_decorrido;

    printf("\nE[N]: %lf\n", e_n_final); // lF
    printf("E[W]: %lf\n", e_w_final);   // lF
    // printf("lambda: %lf\n\n", lambda);  // lF

    printf("Erro de Little: %.20lf\n", e_n_final - lambda * e_w_final); // lF

    printf("Ocupacao: %lf\n", (soma_tempo_servico / maximo(tempo_decorrido, servico))); // lF
    // printf("Max fila: %ld\n", maxFila);
    // printf("Aux m em %d.\n", aux);
    double totalpact = 100 * maximo(tempo_decorrido,servico);
    //double somar = totalpact * 0.3
   // printf("totalpac:%d\n", totalpact);
    printf("somaTotalpacotes:%f\n",soma_total_pacot);
    printf("550:%f", (totalpact * 0 * 550 + totalpact * 0 * 40 + totalpact * 1 * 1500));
    

    return 0;
}
