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

int aux = 1, aux2 = 0;

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
                        (e_w_chegada.no_eventos));
    double lambda = e_w_chegada.no_eventos / acadaT;
    printf("%lf,%lf", acadaT, e_n_final);               // Tempo Atual   E[N]
    printf(",%lf", e_w_final);                          // E[W]:
    printf(" ,%.15lf", e_n_final - lambda * e_w_final); // Erro de Little:
    printf(",%lf\n", soma_tempo_servico / acadaT);      // ocupação

    acadaT += tempo;
    aux2 = 0;
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
    acadaT = tempo;

    /**
     Little
    */
    little e_n;
    little e_w_chegada;
    little e_w_saida;

    little e_n_aux, e_w_chegada_aux, e_w_saida_aux;

    inicia_little(&e_n);
    inicia_little(&e_w_chegada);
    inicia_little(&e_w_saida);

    // srand(time(NULL));
    srand(10000);

    tempo_simulacao = 36000;
    intervalo_medio_chegada = 0.2;
    tempo_medio_servico = 0.16; // 80% = 0,16 ; 90% = 0,18 ; 95% = 0,19; 99% = 0,198

    puts("Tempo,E[N],E[W],Erro_Little,Ocupacao");
    // gerando o tempo de chegada da primeira requisicao
    chegada = (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());

    while (tempo_decorrido <= tempo_simulacao)
    {

        tempo_decorrido = !fila ? minimo(chegada, acadaT) : minimo(minimo(chegada, servico), acadaT);

        if (tempo_decorrido == chegada)
        {
            // printf("Chegada em %lf.", tempo_decorrido);
            // printf("   NumeroIf:%d\n",contE);
            // aux += tempo_decorrido;{}
            if (!fila)
            {
                servico = tempo_decorrido + (-1.0 / (1.0 / tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }
            fila++;
            maxFila = maximo(maxFila, fila);
            chegada = tempo_decorrido + (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());
            // little
            // printf("\nTempo DEcorrido en:%lf\n",tempo_decorrido);
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos++;

            e_w_chegada.soma_areas +=
                (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_chegada.no_eventos++;
            // if(tempo_decorrido >= tempo_simulacao)
            // printf("\n\nUltima chegada:%f\n\n",chegada);
        }
        else 
            if (tempo_decorrido == servico)
            {
                //  printf("Saida  em %lf.", tempo_decorrido);
                // printf("  NumeroElse:%d\n",contS);

                fila--;

                if (fila)
                {
                    servico = tempo_decorrido + (-1.0 / (1.0 / tempo_medio_servico)) * log(aleatorio());
                    soma_tempo_servico += servico - tempo_decorrido;
                }
                // little
                // e_n_aux = e_n;

                e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
                e_n.tempo_anterior = tempo_decorrido;
                e_n.no_eventos--;

                // Quando encontramos chegada < acadaT calculamos um novo e_n soma_areas desconsiderando novas chegadas
                // pois e_n_aux = e_n quando chegada < acadaT e só incrementa soma_areas para as saidas
                // if(tempo_decorrido < acada t) faca soma areas


                e_w_saida.soma_areas +=
                    (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;
                e_w_saida.tempo_anterior = tempo_decorrido;
                e_w_saida.no_eventos++;

                // if(tempo_decorrido >= tempo_simulacao)
                // printf("\n\nElse:Ultima chegada:%f\n\n",chegada);

            }
            else if (tempo_decorrido == acadaT)
            {
                if(e_w_chegada.no_eventos == e_w_saida.no_eventos){
                    printf("Saida == Entrada\n");
                    e_n_aux = e_n;
                    //supondo AcadaT = 100, chegada = 95 e saida = 98 temos que e_n foi calculado corretamente para if(95) mas ao fim do calculo de e_n, e_n.no_eventos foi incrementado para a proxima chegada(ex:104), portanto quando o e_n for ser calculado para a saida de 95 (98) o e_n.no_eventos tera 1 unidade a mais do que deveria, portanto estamos retirando essa parcela indevida.
                    e_n_aux.soma_areas = e_n.soma_areas - (((e_n.tempo_anterior - e_w_chegada.tempo_anterior) * (e_n.no_eventos + 1)) - ((e_n.tempo_anterior - e_w_chegada.tempo_anterior) * e_n.no_eventos));
                    coletaDados(e_n_aux,e_w_chegada,e_w_saida,soma_tempo_servico);

                }
                else{
                    printf("Saida != Entrada\n");
                    e_n_aux = e_n;
                    e_n_aux.soma_areas += (acadaT - e_n.tempo_anterior) * (e_n.no_eventos - 1);
                    e_w_chegada_aux = e_w_chegada;
                    e_w_chegada_aux.soma_areas = (acadaT - e_w_chegada.tempo_anterior) * (e_w_chegada.no_eventos - 1);
                    coletaDados(e_n_aux,e_w_chegada_aux,e_w_saida,soma_tempo_servico);
                }
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

    printf("Ocupacao: %lf\n", soma_tempo_servico / maximo(tempo_decorrido, servico)); // lF
    // printf("Max fila: %ld\n", maxFila);
    // printf("Aux m em %d.\n", aux);

    return 0;
}
