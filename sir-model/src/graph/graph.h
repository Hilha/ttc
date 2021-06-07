#ifndef Graph_H
#define Graph_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include "vertice.h"
#include <string.h>
#include "../utils/utils.h"
#include "aresta.h"

using namespace std;

class Graph
{
public:
    Graph(string caminho);

    void lerArquivo(string caminho);
    vector<Vertice>  getListaVertices(){return this->listaVertices;}
    vector<Vertice>  &getReferenciaListaVertice(){return this->listaVertices;}
    vector<Aresta> getListaArestas(){ return this->listaArestas; }
    int getQtdVertices(){return this->qtd_vertices;}
    int getQtdLigacoes(){return this->qtd_ligacoes;}
    int getMaiorPesoDasArestas() {return this->maiorPesoDeAresta;}
    int getMenorPesoDasArestas() {return this->menorPesoDeAresta;}

    bool dirigido;
    bool ponderado;
    int qtd_vertices;
    int qtd_ligacoes;
    double menorPesoDeAresta;
    double maiorPesoDeAresta;
    float **matrizAdj; //matriz de adjacência
    float somatorio_pesos;
    vector<Vertice> listaVertices;
    vector<Aresta> listaArestas;
};

//#include "graph.cpp"

#endif // Graph_H
