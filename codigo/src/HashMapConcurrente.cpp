#ifndef CHM_CPP
#define CHM_CPP

#include <thread>
#include <iostream>
#include <fstream>
#include <cctype> // tolower

#include "HashMapConcurrente.hpp"

HashMapConcurrente::HashMapConcurrente() {
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
        bucketMtx[i] = new std::mutex();
    }
}

unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(tolower(clave[0]) - 'a');         // Agregamos tolower
}

void HashMapConcurrente::incrementar(std::string clave) {
    int letra = hashIndex(clave);
    for (hashMapPair &par : *tabla[letra]) {        // No se rompen los iteradores
        if (par.first == clave) {
            bucketMtx[letra]->lock();
            par.second++;
            bucketMtx[letra]->unlock();
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

    for (int i = 0; i < HashMapConcurrente::cantLetras; i++) bucketMtx[i]->lock();

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        for (const auto& p : *tabla[index]) {
            sum += p.second;
            count++;
        }
        bucketMtx[index]->unlock();
    }
    if (count > 0) {
        return sum / count;
    }
    return 0;        
}

float HashMapConcurrente::promedioParalelo(unsigned int cantThreads) {
    float sum(0.0);
    std::mutex mtxSum;
    std::atomic_int count(0);
    std::atomic_int nextLetra(0);
    std::thread threads[cantThreads];

    for (int i = 0; i < HashMapConcurrente::cantLetras; i++) bucketMtx[i]->lock();

    for (unsigned int i = 0; i < cantThreads; i++) {
        threads[i] = std::thread([&sum, &mtxSum, &count, &nextLetra, *this] () {
            int letra;
            while ((letra = (nextLetra++)) < HashMapConcurrente::cantLetras) {
                for (const hashMapPair& par : *tabla[letra]) {
                    mtxSum.lock();
                    sum += par.second;
                    mtxSum.unlock();
                    count++;
                }
                bucketMtx[letra]->unlock();
            }
        });
    }

    for (unsigned int i = 0; i < cantThreads; i++) threads[i].join();
    if (count > 0) return sum / count;
    return 0;
}


#endif
