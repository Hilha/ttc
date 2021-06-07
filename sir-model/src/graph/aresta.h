//
// Created by Hilha on 05/04/2021.
//

#ifndef SIR_MODEL_ARESTA_H
#define SIR_MODEL_ARESTA_H

struct Aresta{
    unsigned v1;
    unsigned v2;
    unsigned id;
    Aresta(unsigned x, unsigned y, unsigned id): v1(x), v2(y), id(id){}
};

#endif //SIR_MODEL_ARESTA_H


