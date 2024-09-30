#ifndef CHM_CPP
#define CHM_CPP

#include <thread>
#include <iostream>
#include <fstream>
#include <cctype>

#include "HashMapConcurrente.hpp"

HashMapConcurrente::HashMapConcurrente() {
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
    }
}

unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(tolower(clave[0]) - 'a');         // Agregamos tolower
}

void HashMapConcurrente::incrementar(std::string clave) {
    int letra = hashIndex(clave);
    for (hashMapPair &par : *tabla[letra]) {
        if (par.first == clave) {
            par.second++;
            return;
        }
    }
    tabla[letra]->insertar({clave, 1});
}

std::vector<std::string> HashMapConcurrente::claves() {
    std::vector<std::string> res;
    for (int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        for (hashMapPair par : *tabla[i]) {
            res.push_back(par.first);
        }
    }
    return res;
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    int letra = hashIndex(clave);
    for (hashMapPair par : *tabla[letra]) {
        if (par.first == clave) {
            return par.second;
        }
    }
    return 0;
}

float HashMapConcurrente::promedio() {

    float sum = 0.0;
    unsigned int count = 0;
    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        for (const auto& p : *tabla[index]) {
            sum += p.second;
            count++;
        }
    }
    if (count > 0) {
        return sum / count;
    }
    return 0;        
}



#endif
