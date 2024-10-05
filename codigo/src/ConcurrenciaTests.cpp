#include <vector>
#include <cmath>
#include "lib/littletest.hpp"

#include "../src/ListaAtomica.hpp"
#include "../src/HashMapConcurrente.hpp"
#include "../src/CargarArchivos.hpp"

LT_BEGIN_SUITE(TestsDeConcurrencia)

HashMapConcurrente hM;

void set_up()
{
}

void tear_down()
{
}

LT_END_SUITE(TestsDeConcurrencia)

LT_BEGIN_TEST(TestsDeConcurrencia, muchasPalabras)
    cargarArchivo(hM, "data/corpus");
    LT_CHECK_EQ(hM.valor("Freidora"), 91);
    LT_CHECK_EQ(hM.valor("Cargador"), 210);
    LT_CHECK_EQ(std::round(hM.promedio()), std::round(120.252));
LT_END_TEST(muchasPalabras)

LT_BEGIN_TEST(TestsDeConcurrencia, muchasPalabrasMuchasVeces)
    std::vector<std::string> corpuses(10000, "data/corpus");
    cargarMultiplesArchivos(hM, 100, corpuses);
    LT_CHECK_EQ(hM.valor("Freidora"), 910000);
    LT_CHECK_EQ(hM.valor("Cargador"), 2100000);
    LT_CHECK_EQ(std::round(hM.promedio()), std::round(1202524.0));
    LT_CHECK_EQ(hM.promedio(), hM.promedioParalelo(10));
LT_END_TEST(muchasPalabrasMuchasVeces)

LT_BEGIN_AUTO_TEST_ENV()
    AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()


//std::cout << hM.promedio() << std::endl;