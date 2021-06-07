#include <iostream>
#include "graph/graph.h"
#include "graph/solucao.h"
#include <bits/stdc++.h>
#include <cstdlib>
#include "aresta.h"

using namespace std;

vector<int> verticesInfectados;
vector<int> arestasRemovidasInt;

queue<Vertice> filaDeInfectados;
queue<Vertice> copiaFilaDeInfectados;
vector <Aresta> vectorDeArestasRemovidas;

vector<int> porcentagemDeChanceDeSerEscolhido;
vector<Aresta> listaTabu;

int infeccaoOriginal = 0;

int indiceDaArestaEscolhida = 0;

//Transmission rate
float beta = 0.3;

//Recovery rate
float gamma = 0.7;

//Auxilia a função de infectar aleatoriamente
bool existeNoVetor(vector<int>vec, int input)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (input == vec[i]) return true;
    }
    return false;
}

//Infecta os vertices iniciais
void infectaAleatorio(Graph &graph, int porcentagemDeNodosInfectados)
{

    float qtdNodos = ceil((porcentagemDeNodosInfectados/100.0) * (graph.getQtdVertices()));

    for (int i = 0; i < qtdNodos; i++){
        //+1????
        int numero = rand() % graph.getQtdVertices();
        if (existeNoVetor(verticesInfectados, numero))
        {
            i--;
        } else {
            graph.listaVertices[numero].setRotulo(Vertice::infectado);
            filaDeInfectados.push(graph.listaVertices[numero]);
            cout << "Numero do infectado:  " << graph.listaVertices[numero].getId();
            copiaFilaDeInfectados.push(graph.listaVertices[numero]);
            verticesInfectados.push_back(numero);
            //cout << numero << endl;
        }
    }
    cout << "qtd infectados: " << filaDeInfectados.size() << endl;

}

//Verifica se a aresta esta removida, caso sim, a mesma não é utilizada na infecção
bool arestaEstaRemovida(vector <Aresta> vetorSendoTestado , int idDoVizinho, int idDoVerticeInfectado)
{
    for (int i = 0; i < vetorSendoTestado.size(); ++i) {

        if ((vetorSendoTestado[i].v1 == idDoVizinho && vetorSendoTestado[i].v2 == idDoVerticeInfectado)
            || (vetorSendoTestado[i].v1 = idDoVerticeInfectado && vetorSendoTestado[i].v2 == idDoVizinho))
        {
            return true;
        }
    }

    return false;
}

void reinicializarStatusDeInfeccao(Graph &graph){

    for (int j = 0; j < graph.getQtdVertices(); ++j) {

        if (existeNoVetor(verticesInfectados, j)){
            graph.listaVertices[j].setRotulo(Vertice::infectado);

        } else{
            graph.listaVertices[j].setRotulo(Vertice::suscetivel);
        }
    }

    filaDeInfectados = copiaFilaDeInfectados;
}

int modeloSirOriginal(Graph &graph)
{
    //Quantidade de vertices recuperados ao final da epidemia
    int verticesRemovidos = 0;

    //cout << "Quantidade de infectados: " << filaDeInfectados.size() << endl;

    //Enquanto houver vertices infectados:
    while(!filaDeInfectados.empty())
    {
        //vertice atual = primeiro vertice infectado da lista
        Vertice vertice = filaDeInfectados.front();

        //vertice é removido para que o proximo possa infectar seus vizinhos, caso ele não se recupere
        //voltara para o final da fila
        filaDeInfectados.pop();

        //Para cada vizinho do vértice infectado considerando arestas removidas:
        for (Vertice *vizinho : vertice.getListaAdjacencia()) {

            float infectionBeta = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);



            //Atribui à variável o peso da aresta entre estes dois vertices
            float rangeDeInfeccaoDaLigacao = graph.matrizAdj[vertice.getId()][vizinho->getId()];
            float rangeDeInfeccaoNormalizado = (rangeDeInfeccaoDaLigacao - graph.menorPesoDeAresta)/(graph.maiorPesoDeAresta - graph.menorPesoDeAresta);

            //Se o vizinho for suscetível e a probabilidade de infecção seja atendida, o vertice é infectado
            //e adicionado a fila de infectados

            //cout << "Range da aresta: " << rangeDeInfeccaoDaLigacao << endl;
            //cout << endl << "vertice ID: " << vertice.getId() << " Rotulo:  " << vertice.getRotulo();
            //cout << endl << "Vizinho ID: " << vizinho->getId() << " Rotulo:  " << vizinho->getRotulo() << endl;
            //cout << "Numero sorteado pra infeccao:  " << infectionBeta << endl;
            //cout << "Numero da aresta:  " << rangeDeInfeccaoNormalizado << endl;


            //if (vertice.getId() == 7 && vizinho->getId() == 1){
               // infectionBeta = 0.19;
           // }

            if (vizinho->getRotulo() == Vertice::suscetivel && infectionBeta <= rangeDeInfeccaoNormalizado){
                //cout << "INFECTOU"<< endl;

                //cout << "Infectou" << endl;
                vizinho->setRotulo(Vertice::infectado);
                filaDeInfectados.push(*vizinho);
            }

                //float rangeRecovery = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

                //Caso o vertice infectado atual atenda a probabilidade de recuperação, ele se torna removido
//            if (rangeRecovery < gamma){
//                vertice.setRotulo(Vertice::removido);
//            } else{
//                //Caso ele não se recupere, será adicionado de volta ao fim da fila de infectados
//                filaDeInfectados.push(vertice);
//            }

        }

        vertice.setRotulo(Vertice::removido);
        verticesRemovidos++;
    }
    //Retorna a quantidade de vertices que se recuperaram
    //cout <<"Quantidade de vertices recuperados: "<< verticesRemovidos;

    reinicializarStatusDeInfeccao(graph);

    infeccaoOriginal = verticesRemovidos;

    return verticesRemovidos;
}

//void getMenorValorDosPesos(){
    //for (int i = 0; i < ; ++i) {
        
    //}
//}

int modeloSir(Graph &graph, Solucao solucaoTestada)
{
    //Quantidade de vertices recuperados ao final da epidemia
    int verticesRemovidos = 0;

    //Enquanto houver vertices infectados:
    while(!filaDeInfectados.empty())
    {
        //vertice atual = primeiro vertice infectado da lista
       Vertice vertice = filaDeInfectados.front();

       //vertice é removido para que o proximo possa infectar seus vizinhos, caso ele não se recupere
       //voltara para o final da fila
       filaDeInfectados.pop();

       //Para cada vizinho do vértice infectado considerando arestas removidas:
        for (Vertice *vizinho : vertice.getListaAdjacencia()) {


            if (!arestaEstaRemovida(solucaoTestada.arestas , vizinho->getId(), vertice.getId()))
            {
                //Atribui à variável o peso da aresta entre estes dois vertices
                float infectionBeta = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

                float rangeDeInfeccaoDaLigacao = graph.matrizAdj[vertice.getId()][vizinho->getId()];

                //float primeiraPorcentagemNormalizada = porcentagemDaPrimeiraAresta / (porcentagemDaPrimeiraAresta + porcentagemDaSegundaAresta);

//                cout << endl << "RANGE DA LIGACAO: " << rangeDeInfeccaoDaLigacao << endl;
//                cout << endl << "menor peso da aresta: " << graph.menorPesoDeAresta << endl;
//                cout << endl << "maior peso da aresta: " << graph.maiorPesoDeAresta << endl;

                double rangeDeInfeccaoNormalizado = (rangeDeInfeccaoDaLigacao - graph.menorPesoDeAresta)/(graph.maiorPesoDeAresta - graph.menorPesoDeAresta);
                //cout << endl << "Range NORMALIZADO: " << rangeDeInfeccaoNormalizado << endl;
//                cout << endl << "RANGE NORMALIZADO: " << rangeDeInfeccaoNormalizado << endl;

                //float rangeInfection = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                //cout << rangeInfection << endl;

                //Se o vizinho for suscetível e a probabilidade de infecção seja atendida, o vertice é infectado
                //e adicionado a fila de infectados
                if (vizinho->getRotulo() == Vertice::suscetivel && infectionBeta <= rangeDeInfeccaoNormalizado){
                    //cout << endl;

                    //cout << "vertice infectado: " << vertice.getId() << endl;
                    //cout << "vizinho suscetivel id: " << vizinho->getId() << endl;

                    vizinho->setRotulo(Vertice::infectado);
                    filaDeInfectados.push(*vizinho);
                }

                //float rangeRecovery = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

                //Caso o vertice infectado atual atenda a probabilidade de recuperação, ele se torna removido
//            if (rangeRecovery < gamma){
//                vertice.setRotulo(Vertice::removido);
//            } else{
//                //Caso ele não se recupere, será adicionado de volta ao fim da fila de infectados
//                filaDeInfectados.push(vertice);
//            }

            }
        }
        vertice.setRotulo(Vertice::removido);
        verticesRemovidos++;
    }
    //Retorna a quantidade de vertices que se recuperaram
    //cout <<"Quantidade de vertices recuperados: "<< verticesRemovidos;

    reinicializarStatusDeInfeccao(graph);

    return verticesRemovidos;
}

//Guarda as arestas da rede
Solucao melhorSolucao;

//Guarda o indice ORIGINAL (das arestas da rede complexa) sobre as arestas que se encontram na melhor solucao
vector <int> indiceDasArestasNaMelhorSolucao;

Solucao melhorCandidato;
Solucao antigoMelhorCandidato;


//Monta a solução inicial (Arestas a serem removidas)
void removerArestasAleatorias(Graph &graph, int porcentagemDeArestasQueSeraoRemovidas)
{
    //getQtdLigações() + 1    ??????????? perguntar para o israel
    float qtdArestas = ceil((porcentagemDeArestasQueSeraoRemovidas/100.0) * (graph.getQtdLigacoes()));
    srand(2);

    //cout << endl << "Quantidade de arestas: " << graph.getQtdLigacoes();

    //cout << endl << "Quantidade de arestas a serem removidas:  " << qtdArestas << endl;
    //Estava ++i.....
    for (int i = 0; i < qtdArestas; i++) {
        int numero = rand() % graph.getQtdLigacoes();
        //numero--;
        //cout << "Numero de areta sorteada para solucao inicial: " << numero << endl;

        if (existeNoVetor(arestasRemovidasInt, numero))
        {
            i--;
        } else {
            //Salva a aresta da rede na melhor solucao
            melhorSolucao.arestas.push_back(graph.listaArestas[numero]);

            //Salva o indice original de cada aresta encontrada na melhor solucao
            indiceDasArestasNaMelhorSolucao.push_back(numero);
            arestasRemovidasInt.push_back(numero);
        }
    }


    for (int i = 0; i < indiceDasArestasNaMelhorSolucao.size(); i++) {
        cout << "Indices da melhor solucao:  " << indiceDasArestasNaMelhorSolucao[i] << endl;
    }
}

//EM TESE N PRECISA SER ALTERADO
//Inicializa um vetor de porcentagens com a mesma quantidade de casas do vetor de soluções
void inicializarVetorDePorcentagem(){

    srand(time(nullptr));

    if (!porcentagemDeChanceDeSerEscolhido.empty()){
        for (int i = 0; i < melhorSolucao.arestas.size(); ++i) {
            porcentagemDeChanceDeSerEscolhido.pop_back();
        }
    }

    for (int i = 0; i < melhorSolucao.arestas.size(); i++) {
        //sorteia de 0 a 8
        int porcentagem = rand() %  99;

        //Soma 1 pra sortear de 1 a 99
        porcentagem++;

        porcentagemDeChanceDeSerEscolhido.push_back(porcentagem);
        //vetorDePorcentagem.push_back(porcentagem);
        //cout <<  "porcentagem sorteada:  " << porcentagem << endl;
    }
}

bool existeNoVetorTabu(vector <Vertice> tabu, Vertice &vertice)
{
    for (int i = 0; i < tabu.size(); i++)
    {
        if (vertice.getId() == tabu[i].getId()) return true;
    }

    return false;
}


bool porcentagemJaSorteada(vector<int> sorteados, int sorteado){

    for (int i = 0; i < sorteados.size(); i++) {
        if (porcentagemDeChanceDeSerEscolhido[sorteados[i]] == porcentagemDeChanceDeSerEscolhido[sorteado]){
            return true;
        }
    }

    return false;
}

int escolherArestaASerRemovidaAleatoriamente(Graph &graph, Solucao solucao){
    srand (time(nullptr));

    int indiceDaArestaEscolhidaParaTroca  = rand() % solucao.arestas.size();

    //cout << endl << "Indice sorteado pra troca: " << indiceDaArestaEscolhidaParaTroca << endl;
    return solucao.arestas[indiceDaArestaEscolhidaParaTroca].id;
}

int escolheArestaQueSeraRemovidaDaSolucaoETrocadaPorOutraArestaVizinha(Graph &graph, Solucao solucao){
    srand(time(nullptr));

    inicializarVetorDePorcentagem();
    //int qtdDeArestasParaPrimeiraEtapaDeSelecao = (20/100) * (melhorSolucao.size()); 20% de arestas

    //Escolhe 2 arestas para a primeira etapa
    int qtdDeArestasParaPrimeiraEtapaDeSelecao = 2;

    //for (int i = 0; i < porcentagemDeChanceDeSerEscolhido.size(); i++) {
        //cout << endl << "Porcentagem escolhida:  " << porcentagemDeChanceDeSerEscolhido[i];
    //}

    //cout << endl;

    vector<int> indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao;

    //Esolhe 2 arestas da solução para participar da primeira etapa de seleção da aresta que sera escolhida para ser trocada na solução

////    cout << "Melhor solucao size: " << melhorSolucao.arestas.size() << endl;
    //cout << "Melhor solucao size: " << solucao.arestas.size() << endl;

    srand(time(nullptr));

    for (int i = 0; i < qtdDeArestasParaPrimeiraEtapaDeSelecao; i++) {

        //Numero aleatorio dentro do intervalo da quantidade de arestas sorteadas
        ////int indiceDasArestaEscolhidasNaMelhorSolucao  = rand() % melhorSolucao.arestas.size();
        int indiceDasArestaEscolhidasNaMelhorSolucao  = rand() % solucao.arestas.size();

        //cout << endl << "aqui aqui ja";


        //ATESTA QUE FUNCIONAAAAAAAAA
        //out << "Indice das arestas escolhidas na melhor solucao:" << indiceDasArestaEscolhidasNaMelhorSolucao << "id dela (indice original): " << melhorSolucao.arestas[indiceDasArestaEscolhidasNaMelhorSolucao].id << endl;

        //Diminui 1 pois melhorSolucao é um vetor, então deve considerar o 0, caso tenha sorteado 1 (por ex);
        //indiceDasArestaEscolhidasNaMelhorSolucao--;

        if (porcentagemJaSorteada(indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao, indiceDasArestaEscolhidasNaMelhorSolucao)){
            i--;
        } else{
            indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao.push_back(indiceDasArestaEscolhidasNaMelhorSolucao);
        }
    }

    float porcentagemDaPrimeiraAresta = porcentagemDeChanceDeSerEscolhido[indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao[0]];
    float porcentagemDaSegundaAresta = porcentagemDeChanceDeSerEscolhido[indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao[1]];

    //Normaliza o valor das duas porcentagens para o intervalo de 0 a 1
    float primeiraPorcentagemNormalizada = porcentagemDaPrimeiraAresta / (porcentagemDaPrimeiraAresta + porcentagemDaSegundaAresta);
    float segundaPorcentagemNormalizada = porcentagemDaSegundaAresta / (porcentagemDaPrimeiraAresta + porcentagemDaSegundaAresta);

    bool primeiraEhAMaisAlta = false;

    if(primeiraPorcentagemNormalizada > segundaPorcentagemNormalizada){
        primeiraEhAMaisAlta = true;
    }

    //Sorteio entre as duas primeiras selecionadas
    float sorteioMutacao = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    //Caso a pimeira do array seja a mais alta
    if (primeiraEhAMaisAlta){
        //Se o numero sorteado for maior que a segunda porcentagem (q é a mais baixa)
        if (sorteioMutacao > segundaPorcentagemNormalizada){
            //Retorna o que é o mais alto
            indiceDaArestaEscolhida = indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao[0];
            // Essa variavel está errada...Agr é o indice da aresta escolhida neste solução
            return solucao.arestas[indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao[0]].id;
        } else{
            //Se não, retorna oq é mais baixo
            indiceDaArestaEscolhida = indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao[1];
            return solucao.arestas[indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao[1]].id;
        }
    } else{
        //Se o numero sorteado for maior que a primeira porcentagem (q é a mais baixa)
        if (sorteioMutacao > primeiraPorcentagemNormalizada){
            //retorna o que oe mais alto
            indiceDaArestaEscolhida = indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao[1];
            return solucao.arestas[indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao[1]].id;
            ///return indiceDasArestasNaMelhorSolucao[indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao[1]];
        } else{
            //retorna oq é mais baixo
            indiceDaArestaEscolhida = indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao[0];
            return solucao.arestas[indiceDasArestasSorteadasParaAPrimeiraEtapaDeSelecao[0]].id;
        }
    }
}

//Get Soluções Vizinhas
//Vizinhos não tabu

bool arestaJaExisteNaSolucao(int idDaAresta, Solucao solucao)
{
    for (int i = 0; i < solucao.arestas.size(); i++) {
        if (solucao.arestas[i].id == idDaAresta){
            return true;
        }
    }

    return false;
}

//bool arestaNaoEhTabu(int idDaAresta)
//{

//}

vector<Solucao> getVizinhancaDaSolucao(Graph &graph, Solucao solucao){

    //vector<vector<Aresta>> solucoesVizinhas;

    //cout << "SIZE ANTES DE ENTRAR NA ESCOLHA :" << solucao.arestas.size() << endl;

    int indiceDaArestaEscolhidaParaTroca = escolheArestaQueSeraRemovidaDaSolucaoETrocadaPorOutraArestaVizinha(graph, solucao);
    //int indiceDaArestaEscolhidaParaTroca = escolherArestaASerRemovidaAleatoriamente(graph, solucao);

    int primeiroVertice = graph.listaArestas[indiceDaArestaEscolhidaParaTroca].v1;
    int segundoVertice = graph.listaArestas[indiceDaArestaEscolhidaParaTroca].v2;

    //cout << "Indice da aresta escolhida na melhor solucao:  " << indiceDaArestaEscolhidaParaTroca << endl;
    //cout << "ID da aresta escolhida na melhor solucao:  " << graph.listaArestas[indiceDaArestaEscolhidaParaTroca].id;
    //cout << endl << "Primeiro vertice da aresta:  " << primeiroVertice << " Qtd de arestas: " << graph.listaVertices[primeiroVertice].getListaArestasProprias().size() << endl;
    //cout << "Segundo vertice da aresta:  " << segundoVertice << " Qtd de arestas: " << graph.listaVertices[segundoVertice].getListaArestasProprias().size() << endl;

    //cout << "antes" << endl;

    //Pega as arestas que compoem o primeiro vertice
    vector <int> arestasDoPrimeiroVertice = graph.listaVertices[primeiroVertice].getListaArestasProprias();

    //for (int i = 0; i < graph.listaVertices[primeiroVertice].getListaArestasProprias().size(); ++i) {
       // cout << "Arestas1:  " << arestasDoPrimeiroVertice[i] << endl;
    //}

    //Pega as arestas que compoem o segundo vertice
    vector <int> arestasDoSegundoVertice = graph.listaVertices[segundoVertice].getListaArestasProprias();

    //for (int i = 0; i < graph.listaVertices[segundoVertice].getListaArestasProprias().size(); ++i) {
       // cout << "Arestas2:  " << arestasDoSegundoVertice[i] << endl;
    //}

    //Vizinhança da solução atual
    vector <Solucao> vizinhancaDaSolucao;

    int iteradorDoVetorDeVizinhanca = 0;

    for (int i = 0; i < graph.listaVertices[primeiroVertice].getListaArestasProprias().size(); ++i) {
        //Aresta *aresta : arestasDoPrimeiroVertice

        //Caso um desses vertices seja o segundo vertice, n deve entrar no if (Oq significaria que é a propria aresta que estamos querendo trocar)
        // AND if NÃO É TABU
        int indice  = arestasDoPrimeiroVertice[i];

        Aresta arestaTestada = graph.listaArestas[indice];
        //cout << "depois: " << arestaTestada.id << endl;

        //AND esta aresta n ta na solução atual AND n é aresta tabu
        if (arestaTestada.v1 != segundoVertice && arestaTestada.v2 != segundoVertice && !arestaJaExisteNaSolucao(arestaTestada.id, solucao)){
            vizinhancaDaSolucao.push_back(solucao);

            //Encontra a aresta da solucao que deve ser trocada
            for (int j = 0; j < solucao.arestas.size(); j++) {

                //Caso o id da aresta na solucao seja igual ao id da aresta escolhida
                if (vizinhancaDaSolucao[iteradorDoVetorDeVizinhanca].arestas[j].id == graph.listaArestas[indiceDaArestaEscolhidaParaTroca].id){
                    //Troca essa aresta pela nova (nova escolhida)

                    //Substitui a aresta pela nova
                    vizinhancaDaSolucao[iteradorDoVetorDeVizinhanca].arestas[j] = arestaTestada;
                    //Troca o indice da aresta antiga pela nova
                    //////SERA QUE PRECISA DISSO AQ? indiceDasArestasNaMelhorSolucao[j] = arestaTestada.id;

                    break;
                }
            }

            iteradorDoVetorDeVizinhanca++;
            //deve gerar uma solução com esta aresta, substituindo a original assim como deve trocar o indice dela dentro do vetor de indices da melhor solução
            //vizinhancaDaSolucao.push_back(  SOLUCAO{MESMA SOLUCAO ATUAL PORÉM COM A ARESTA TROCADA POR ESSA NOVA}  )

           //cout << "entrou" << endl;
        }
    }

    for (int i = 0; i < graph.listaVertices[segundoVertice].getListaArestasProprias().size(); ++i) {
        //Caso um desses vertices seja o segundo vertice, n deve entrar no if (Oq significaria que é a propria aresta que estamos querendo trocar)
        // AND if NAO EXISTE JA NA MELHOR SOLUCAO and NÃO É TABU

        int indice  = arestasDoSegundoVertice[i];

        Aresta arestaTestada = graph.listaArestas[indice];

        if (arestaTestada.v1 != primeiroVertice && arestaTestada.v2 != primeiroVertice && !arestaJaExisteNaSolucao(arestaTestada.id, solucao)){
            vizinhancaDaSolucao.push_back(solucao);

            //Encontra a aresta da solucao que deve ser trocada
            for (int i = 0; i < solucao.arestas.size(); i++) {

                //Caso o id da resta na solucao seja igual ao id da aresta escolhida
                if (vizinhancaDaSolucao[iteradorDoVetorDeVizinhanca].arestas[i].id == graph.listaArestas[indiceDaArestaEscolhidaParaTroca].id){
                    //Troca essa aresta pela nova (nova escolhida)

                    //Substitui a aresta pela nova
                    vizinhancaDaSolucao[iteradorDoVetorDeVizinhanca].arestas[i] = arestaTestada;
                    //Troca o indice da aresta antiga pela nova
                    ////indiceDasArestasNaMelhorSolucao[i] = arestaTestada.id;

                    break;
                }
            }

            iteradorDoVetorDeVizinhanca++;
        }
    }

   // for (int i = 0; i < vizinhancaDaSolucao.size(); ++i) {
       // cout << endl << "solucao "<< i <<":  " << endl;
     //   for (int j = 0; j < vizinhancaDaSolucao[i].arestas.size(); ++j) {
          //  cout << "Aresta " << j << ": "  << vizinhancaDaSolucao[i].arestas[j].id << endl;
       // }
  //  }

  //  cout << "Vizinhanca SIZE: " << vizinhancaDaSolucao.size();

    return vizinhancaDaSolucao;
}

bool arestaEhTabu(vector<int> listaTabu, Solucao solucaoTestada){
    for (int i = 0; i < listaTabu.size(); i++) {
        if (listaTabu[i] == solucaoTestada.arestas[i].id){
            return true;
        }
    }

    return false;
}

Solucao heuristicaTabu(Graph &graph, int porcentagemTamanhoDaListaTabu)
{
    auto inicioDaExecucao = std::chrono::high_resolution_clock::now();

    //Iteração mais recente que forneceu a melhor solução de ARESTAS
    int melhorIteracao = 0;

    //Contador de iterações
    int iter = 0;

    //Numero maximo de iterações sem melhora
    int totalDetentativas = 1500;

    int maxTabuSize = ceil((porcentagemTamanhoDaListaTabu/100.0) * (graph.getQtdLigacoes()));

   // int maxTabuSize = melhorSolucao.arestas.size();
    //int maxTabuSize = 6;

    //Lista de arestas tabu guarda o indice da aresta tabu
    vector <int> listaDeArestasTabu;

    melhorCandidato = melhorSolucao;

    while (iter <= totalDetentativas){

        vector <Solucao> vizinhancaDaSolucao = getVizinhancaDaSolucao(graph, melhorCandidato);

        int btMax = 0;

        if(vizinhancaDaSolucao.empty()){
            bool vazio = true;

            melhorCandidato = antigoMelhorCandidato;

            while (vazio || btMax < 20){
                if (antigoMelhorCandidato.arestas.size() == 0){
                    break;
                }

                cout << endl << "SIZE:  "<< antigoMelhorCandidato.arestas.size();

                vizinhancaDaSolucao = getVizinhancaDaSolucao(graph, melhorCandidato);
                btMax ++;

                if (!vizinhancaDaSolucao.empty()){
                    vazio = false;
                }
            }
        }

        if (btMax >= 20 || melhorCandidato.arestas.size() == 0 || vizinhancaDaSolucao.size() <= 0){
            break;
        }

       // cout << endl << "SIZE ANTIGO:  "<< antigoMelhorCandidato.arestas.size();
       // cout << endl << "SIZE NEW:  "<< melhorCandidato.arestas.size();
        //cout << endl << "ultimo do ULTIMO";
        //cout << endl << "ANTES DO MELHOR CANDIDATO";
       // cout << endl << "SIZE da vizinhanca: " << vizinhancaDaSolucao.size();
        melhorCandidato  = vizinhancaDaSolucao[0];

       // cout << endl << "FIM";

        //Para cada solução vizinha:
        for (Solucao candidato : vizinhancaDaSolucao) {
            if (!arestaEhTabu(listaDeArestasTabu, candidato) && modeloSir(graph, candidato) < modeloSir(graph, melhorCandidato)){
                antigoMelhorCandidato = melhorCandidato;
                melhorCandidato = candidato;
            }
        }

        if (modeloSir(graph, melhorCandidato) < modeloSir(graph, melhorSolucao)){
            melhorSolucao = melhorCandidato;
            melhorIteracao = iter;
        }

        listaDeArestasTabu.push_back(melhorCandidato.arestas[indiceDaArestaEscolhida].id);

        if (listaDeArestasTabu.size() > maxTabuSize){
            listaDeArestasTabu.erase(listaDeArestasTabu.begin());
        }

        iter = iter + 1;
    }


    //Retorna a melhor solucao encontrada

   // for (int i = 0; i < melhorSolucao.arestas.size(); ++i) {
     //   cout << endl << "Melhores arestas ID: " << melhorSolucao.arestas[i].id << endl;
   // }

    if (infeccaoOriginal < modeloSir(graph, melhorSolucao)){
        cout << endl << "NENHUM RESULTADO SATISFATORIO" << endl;
    }
    cout << endl << "Melhor iteracao:  " << melhorIteracao << endl;

    auto tempoDeExecucao = std::chrono::high_resolution_clock::now() - inicioDaExecucao;

    long long segundosDeExecucao = std::chrono::duration_cast<std::chrono::milliseconds>(tempoDeExecucao).count();

    cout << "Tempo de execucao:  " << segundosDeExecucao << endl;
    return melhorSolucao;
}

int main(){
    auto *graph = new Graph("../src/redes/quantificadas/email.net");

    //Infecta sempre os mesmos (Inicialmente infectados) OK 1,5,10
    infectaAleatorio(*graph, 10);

    //srand(time(nullptr));

    cout << "Rede: Airports" << endl << "Quantidade de vertices da rede: " << graph->listaVertices.size() << endl;

    //Passar porcentagem de arestas removidas 1% ; 5% ; 10% remove sempre as mesmas arestas  OK

    int infeccaoCrua = modeloSirOriginal(*graph);
    cout << endl;

     removerArestasAleatorias(*graph, 15);

    //falta desconsiderar as arestas removidas
    //modeloSir(*graph, melhorSolucao.arestas);
    //cout << endl << graph->listaArestas.size();

    //getVizinhancaDaSolucao(*graph, melhorSolucao);

    heuristicaTabu(*graph, 10);

    int infeccaoTabu = modeloSir(*graph, melhorSolucao);

    cout << endl << endl << "Infeccao inicial: " << infeccaoCrua << endl;
    //cout << endl << endl << "Menor valor: " << graph->getMenorPesoDasArestas() << endl;
    //cout << endl << endl << "Maior valor: " << graph->getMaiorPesoDasArestas() << endl;
    cout << "Infeccao apos interferencia da heuristica tabu: " << infeccaoTabu;

    // OK
    //cout <<  "Indice original da aresta escolhida para remocao: " << escolheArestaQueSeraRemovidaDaSolucaoETrocadaPorOutraArestaVizinha(*graph);

    //cout << endl << "qtd:  " << indiceDasArestasNaMelhorSolucao.size();
//    for (int i = 0; i < indiceDasArestasNaMelhorSolucao.size(); i++) {
//        cout << "Indice original da aresta na melhor solucao:  " << indiceDasArestasNaMelhorSolucao[i] << endl;
//    }

    return 0;
}