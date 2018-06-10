/*
 * DSP Example is part of the DSP Lecture at TEC-Costa Rica
 * Copyright (C) 2017  Jose Miguel Barboza
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file   controlVolume.cpp
 *         Implements a equalizer using filters IIR with central frequencies of 32Hz,64Hz,64Hz,125Hz,250Hz,500Hz,1kHz,2kHz,4kHz,8kHz and 16kHz.
 * \author Jose Miguel Barboza \ Luis Alfredo Piedra Esquivel \ Jose Pablo Delgado Martinez
 * \date   2017.31.05 \ 2017.1.12
 *
 * $Id: controlVolume.cpp $
 */


#ifndef CONTROLVOLUME_H
#define CONTROLVOLUME_H
#include <fftw3.h>

/**
 * Control Volume class
 *
 * This filter has control in the volume gain of the signal.
 *
 * The filter follows the difference equation
 * \f[
 * y(n)=\cvGain x(n)
 * \f]
 */

class controlVolume {
public:

    // Puntero de tipo double[][2] que almacena H(k) dividiendo cada termino en parte real y parte imaginaria.
    fftw_complex *f32;
    fftw_complex *f64;
    fftw_complex *f125;
    fftw_complex *f250;
    fftw_complex *f500;
    fftw_complex *f1k;
    fftw_complex *f2k;
    fftw_complex *f4k;
    fftw_complex *f8k;
    fftw_complex *f16k;

    //Arreglos donde se almacenan las ultimas M-1 muestras del la salida anterior calculada. (Solapamiento y suma).
    float* datos32;
    float* datos64;
    float* datos125;
    float* datos250;
    float* datos500;
    float* datos1k;
    float* datos2k;
    float* datos4k;
    float* datos8k;
    float* datos16k;
    bool inicio;

    /**
     * Constructor
     */
    controlVolume();

    /**
     * Destructor
     */
    ~controlVolume();

    /**
    * @brief filter Funcion encargada de filtrar la entrada de datos pasandola por distintos filtros y luego sumando la salida de cada uno.
    * @param blockSize cantidad de muestras que contiene la entrada.
    * @param volumeGain valor entero que representa la posicion del slider y escala el valor final de la salida controlando asi el volumen.
    * @param g32 valor entero que representa la posicion del slider asociado al filtro de 32Hz y escala el valor de la salida especifica de dicho filtro.
    * @param g64 valor entero que representa la posicion del slider asociado al filtro de 64Hz y escala el valor de la salida especifica de dicho filtro.
    * @param g125 valor entero que representa la posicion del slider asociado al filtro de 125Hz y escala el valor de la salida especifica de dicho filtro.
    * @param g250 valor entero que representa la posicion del slider asociado al filtro de 250Hz y escala el valor de la salida especifica de dicho filtro.
    * @param g500 valor entero que representa la posicion del slider asociado al filtro de 500Hz y escala el valor de la salida especifica de dicho filtro.
    * @param g1k valor entero que representa la posicion del slider asociado al filtro de 1kHz y escala el valor de la salida especifica de dicho filtro.
    * @param g2k valor entero que representa la posicion del slider asociado al filtro de 2kHz y escala el valor de la salida especifica de dicho filtro.
    * @param g4k valor entero que representa la posicion del slider asociado al filtro de 4kHz y escala el valor de la salida especifica de dicho filtro.
    * @param g8k valor entero que representa la posicion del slider asociado al filtro de 8kHz y escala el valor de la salida especifica de dicho filtro.
    * @param g16k valor entero que representa la posicion del slider asociado al filtro de 16kHz y escala el valor de la salida especifica de dicho filtro.
    * @param in puntero al arreglo de valores de tipo float que conforman la entrada del sistema.
    * @param out puntero a un arreglo de valores tipo float que conforman la salida del ecualizador y son enviados a la tarjeta de audio a reproducirse.
    */
   void filter(int blockSize,
               int volumeGain,
               int g32,
               int g64,
               int g125,
               int g250,
               int g500,
               int g1k,
               int g2k,
               int g4k,
               int g8k,
               int g16k,
               float* in,
               float* out);

   /**
    * @brief filtroGeneral Funcion encargada aplicar el filtrado por DFT a una arreglo de muestras de tipo float.
    * @param blockSize numero de elementos que contiene la entrada, son valores de tipo float.
    * @param volumeGain numero entero que representa el escalado que se aplica a cada valor de la salida calculada.
    * @param in puntero al arreglo de valores de tipo float que conforman la entrada del sistema.
    * @param out puntero a un arreglo de valores tipo float que conforman la salida del ecualizador y son enviados a la tarjeta de audio a reproducirse.
    * @param hk puntero a un arreglo que contiene los valores complejos de H(k) de un filtro especifico.
    * @param temporal puntero al arreglo donde se almacenan los valores de la salida anterior que no se utilizaron y se guardaran los M-1 datos que sobren al filtrar.
    */
   void filtroGeneral(int blockSize,int volumeGain, float* in, float* out,fftw_complex *hk,float* temporal);

private:

   /**
    * @brief inicializarHK Funcion encargada de generar H(k) utilizando la DFT para un filtro especifico.
    * @param puntero puntero a un arreglo donde se almacenaran los valores de H(k).
    * @param G valor que representa la ganancia total de la ecuacion de diferencias de grado 6.
    * @param a_0 valor decimal que representa coeficiente que multiplica a x(n).
    * @param b_0 valor decimal que representa coeficiente que multiplica a x(n-1).
    * @param c_0 valor decimal que representa coeficiente que multiplica a x(n-2).
    * @param e_0 valor decimal que representa coeficiente que multiplica a x(n-4).
    * @param f_0 valor decimal que representa coeficiente que multiplica a x(n-5).
    * @param g_0 valor decimal que representa coeficiente que multiplica a x(n-6).
    * @param b_1 valor decimal que representa coeficiente que multiplica a y(n-1).
    * @param c_1 valor decimal que representa coeficiente que multiplica a y(n-2).
    * @param d_1 valor decimal que representa coeficiente que multiplica a y(n-3).
    * @param e_1 valor decimal que representa coeficiente que multiplica a y(n-4).
    * @param f_1 valor decimal que representa coeficiente que multiplica a y(n-5).
    * @param g_1 valor decimal que representa coeficiente que multiplica a y(n-6).
    */
   void inicializarHK(fftw_complex* puntero,double G,double a_0,double b_0,
                                      double c_0,double e_0,double f_0,double g_0,double b_1,
                                      double c_1,double d_1,double e_1,double f_1,double g_1);

   //Metodos que llaman a inicializarHk() con los coeficientes especificos de cada filtro
   void inicializarH32();
   void inicializarH64();
   void inicializarH125();
   void inicializarH250();
   void inicializarH500();
   void inicializarH1k();
   void inicializarH2k();
   void inicializarH4k();
   void inicializarH8k();
   void inicializarH16k();

};


#endif // CONTROLVOLUME_H
