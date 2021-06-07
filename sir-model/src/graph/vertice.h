#ifndef VERTICE_H
#define VERTICE_H

#include <vector>
#include <iostream>
#include "aresta.h"

using namespace std;

class Vertice
{
public:
    
    Vertice(unsigned id);
    void addVerticeAdjacente(Vertice *vertice);
    void addVerticeAdjacenteEntrada(Vertice *vertice);
    void addVerticeAdjacenteSaida(Vertice *vertice);
    void addArestaPropria(int indice);
    void setId(int id){this->id = id;}
    void setRotulo(char rotulo){this->rotulo = rotulo;}
    void setForca(float forca){this->forca = forca;}
    void setForcaEntrada(float forca_entrada){this->forca_entrada = forca_entrada;}
    void setForcaSaida(float forca_saida){this->forca_saida = forca_saida;}
    unsigned getId() {return this->id;}
    unsigned getGrau(){return this->grau;}
    unsigned getGrauEntrada(){return this->grau_entrada;}
    unsigned getGrauSaida(){return this->grau_saida;}
    float getForca(){return this->forca;}
    float getForcaEntrada(){return this->forca_entrada;}
    float getForcaSaida(){return this->forca_saida;}
    char getRotulo(){return this->rotulo;}
    vector<Vertice*> getListaAdjacencia(){return this->lista_adjacencia;}
    vector<Vertice*> getListaAdjacenciaEntrada(){return this->lista_adjacencia_entrada;}
    vector<Vertice*> getListaAdjacenciaSaida(){return this->lista_adjacencia_saida;}
    vector<int> getListaArestasProprias(){return this->lista_arestas_proprias;}
    vector<int> lista_arestas_proprias;
    vector<Vertice*> lista_adjacencia;
    vector<Vertice*> lista_adjacencia_entrada; //contém os vértices adjacentes que apontam para este
    vector<Vertice*> lista_adjacencia_saida; //contém os vértices adjacentes que são apontados por este
    static char const suscetivel = 's';
    static char const infectado = 'i';
    static char const removido = 'r';

private:
    unsigned id;
    unsigned grau;
    unsigned grau_entrada;
    unsigned grau_saida;
    float forca;
    float forca_entrada;
    float forca_saida;
    char rotulo;
};

//#include "vertice.cpp"

#endif // VERTICE_H
