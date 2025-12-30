#pragma once
#include <math.h>

/**
 * Inicjalizacja magistrali 1-Wire
 */
void ds18b20_init();

/**
 * @return temperatura w °C
 *         NAN jeśli błąd (brak czujnika)
 */
float ds18b20_read();