#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "prototipos.h"

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
int cont550 = 0, cont40 = 0, cont1500 = 0;
double acadaT;

void coletaDados(little e_n, little e_w_chegada, little e_w_saida, double soma_tempo_servico)
{
    double e_n_final = (e_n.soma_areas) / acadaT;
    double e_w_final = ((e_w_chegada.soma_areas -
                         (e_w_saida.soma_areas)) /
                        (abs(e_w_chegada.no_eventos)));
    double lambda = e_w_chegada.no_eventos / acadaT;

    printf("%lf,%lf,", acadaT, e_n_final);             // Tempo Atual   E[N]
    printf("%lf", e_w_final);                          // E[W]:
    printf(",%.15lf", e_n_final - lambda * e_w_final); // Erro de Little:
    printf(",%lf\n", soma_tempo_servico / acadaT);     // ocupação

    acadaT += 100;
}

double gerapacote()
{
    double porcentagem = rand() % 100;
    if (porcentagem < 50)
    {
        cont550++;
        return 550;
    }
    if (porcentagem < 90)
    {
        cont40++;
        return 40;
    }
    if (porcentagem >= 90)
    {
        cont1500++;
        return 1500;
    }
}

int main()
{
    double chegada;
    double servico;
    double chamada;
    double duracao_chamada;

    double soma_tempo_servico = 0.0;

    double tempo_simulacao;
    double tempo_decorrido = 0.0;

    double intervalo_medio_chegada;
    double inter_medio_chegada_chamada;
    double duracao_media_chamada;
    double chegada_transmissao;
    // double tempo_medio_servico;

    unsigned long int fila = 0;
    unsigned long int maxFila = 0;
    double pacote;
    acadaT = 100;
    double link_capacidade;
    NO *lista_chamadas;
    lista_chamadas = criarLista();

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
    srand(4400);
    link_capacidade = 10000 * 100; // 60% = 735 80% = 551,25   95% = 464,21 99% = 445,45
    tempo_simulacao = 360000;
    intervalo_medio_chegada = 0.01;
    inter_medio_chegada_chamada = 5;
    duracao_media_chamada = 80;
    chegada_transmissao = tempo_simulacao * 2;
    puts("Tempo,E[N],E[W],Erro_Little,Ocupacao");
    // gerando o tempo de chegada da primeira requisicao
    chegada = (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());
    chamada = (-1.0 / (1.0 / inter_medio_chegada_chamada)) * log(aleatorio());

    duracao_chamada = tempo_decorrido + (-1.0 / (1.0 / duracao_media_chamada)) * log(aleatorio());
    lista_chamadas = inserirFinal(lista_chamadas, chamada, duracao_chamada);
    chamada = tempo_decorrido + (-1.0 / (1.0 / inter_medio_chegada_chamada)) * log(aleatorio());


    lista_chamadas = trata_pacote_transmissao(lista_chamadas, tempo_decorrido,&chegada_transmissao);

    while (tempo_decorrido <= tempo_simulacao)
    {

        tempo_decorrido = !fila ? minimo(minimo(minimo(chegada, acadaT), chamada), chegada_transmissao) : minimo(minimo(minimo(minimo(chegada, servico), acadaT), chamada), chegada_transmissao);

        if (tempo_decorrido == chegada)
        {
            if (!fila)
            {
                pacote = gerapacote();
                servico = tempo_decorrido + pacote / link_capacidade;
                soma_tempo_servico += servico - tempo_decorrido;
            }
            fila++;
            maxFila = maximo(maxFila, fila);
            chegada = tempo_decorrido + (-1.0 / (1.0 / intervalo_medio_chegada)) * log(aleatorio());
            // printf("\nChegada:%lf",chegada);
            //  little
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos++;

            e_w_chegada.soma_areas +=
                (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_chegada.no_eventos++;
        }
        else if (tempo_decorrido == servico)
        {
            fila--;

            if (fila)
            {
                pacote = gerapacote();
                servico = tempo_decorrido + pacote / link_capacidade;
                soma_tempo_servico += servico - tempo_decorrido;
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
            coletaDados(e_n, e_w_chegada, e_w_saida, soma_tempo_servico);
        }
        else if (tempo_decorrido == chamada)
        {
            duracao_chamada = tempo_decorrido + (-1.0 / (1.0 / duracao_media_chamada)) * log(aleatorio());
            lista_chamadas = inserirFinal(lista_chamadas, chamada, duracao_chamada);
            chamada = tempo_decorrido + (-1.0 / (1.0 / inter_medio_chegada_chamada)) * log(aleatorio());
        }
        else if (tempo_decorrido == chegada_transmissao)
        {
            if (!fila)
            {
                pacote = 1280;
                servico = tempo_decorrido + pacote / link_capacidade;
                soma_tempo_servico += servico - tempo_decorrido;
            }
            fila++;
            maxFila = maximo(maxFila, fila);
            // printf("\nChegada:%lf",chegada);
            //  little
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos++;

            e_w_chegada.soma_areas +=
                (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_chegada.no_eventos++;

             lista_chamadas = trata_pacote_transmissao(lista_chamadas, tempo_decorrido, &chegada_transmissao);
            //mostrarLista(lista_chamadas);
        }
    }
    e_w_chegada.soma_areas +=
        (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;

    e_n.soma_areas +=
        (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;

    e_w_saida.soma_areas +=
        (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

    double e_n_final = e_n.soma_areas / tempo_decorrido;
    double e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;

    double lambda = e_w_chegada.no_eventos / tempo_decorrido;
    /*
        printf("\nE[N]: %lf\n", e_n_final); // lF
        printf("E[W]: %lf\n", e_w_final);   // lF
        // printf("lambda: %lf\n\n", lambda);  // lF

        printf("Erro de Little: %.20lf\n", e_n_final - lambda * e_w_final); // lF

        printf("Ocupacao: %lf\n", (soma_tempo_servico / maximo(tempo_decorrido, servico))); // lF
        // printf("Max fila: %ld\n", maxFila);
        // printf("Aux m em %d.\n", aux);
        double totalpact = 100 * maximo(tempo_decorrido,servico);
        //printf("totalpac:%f\n", totalpact);
        printf("\n550:%f,  40:%f 1500:%f\n", cont550/totalpact,cont40/totalpact,cont1500/totalpact);
        */
       
    printf("\nElementos na lista:%d", qtdElementos(lista_chamadas));
    mostrarLista(lista_chamadas);
    // printf("\nElementos na lista:%lf", lista_chamadas->inicio);
    return 0;
}
