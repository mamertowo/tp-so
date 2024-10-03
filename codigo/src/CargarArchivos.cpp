#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>
// Tuvimos que agregar thread, lo pide la consigna
#include <thread>

#include "CargarArchivos.hpp"

int cargarArchivo(
    HashMapConcurrente &hashMap,
    std::string filePath
) {
    std::fstream file;
    int cant = 0;
    std::string palabraActual;

    // Abro el archivo.
    file.open(filePath, file.in);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo '" << filePath << "'" << std::endl;
        return -1;
    }
    while (file >> palabraActual) {
        hashMap.incrementar(palabraActual);
        cant++;
    }
    // Cierro el archivo.
    if (!file.eof()) {
        std::cerr << "Error al leer el archivo" << std::endl;
        file.close();
        return -1;
    }
    file.close();
    return cant;
}


void cargarMultiplesArchivos(
    HashMapConcurrente &hashMap,
    unsigned int cantThreads,
    std::vector<std::string> filePaths
) {
    std::atomic_int nextArchivo(0);

    std::thread threads[cantThreads];
    for (unsigned int i = 0; i < cantThreads; i++) {
        threads[i] = std::thread([&nextArchivo, &filePaths, &hashMap] () {
            int archivo;
            while ((archivo = (nextArchivo++)) < (int)filePaths.size()) {
                cargarArchivo(hashMap, filePaths[archivo]);
            }
        });
    }
    for (unsigned int i = 0; i < cantThreads; i++) threads[i].join();
}

#endif
