/*
 * DSP Example is part of the DSP Lecture at TEC-Costa Rica
 * Copyright (C) 2017  Pablo Alvarado
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

#include "controlvolume.h"
#include <cmath>
#include <iostream>
using namespace std;

#define REAL 0
#define IMAG 1
#define PI 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481

/*
 * Constructor
 */
controlVolume::controlVolume(){

    //Inicializacion de los punteros de tipo double[2048][2].
    f32 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2048);
    f64 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2048);
    f125 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)* 2048);
    f250 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)* 2048);
    f500 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)* 2048);
    f1k = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2048);
    f2k = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2048);
    f4k = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2048);
    f8k = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * 2048);
    f16k = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)* 2048);

    //valor booleano que indica el inicio de una cancion.
    inicio = true;

    // Arreglos donde se almacenan los M-1 valores de la salida que se generan al aplicar el metodo de solapamiento y suma.
    datos32 = new float[1024];
    datos64 = new float[1024];
    datos125 = new float[1024];
    datos250 = new float[1024];
    datos500 = new float[1024];
    datos1k = new float[1024];
    datos2k = new float[1024];
    datos4k = new float[1024];
    datos8k = new float[1024];
    datos16k = new float[1024];

    tmpOut = new float[1024];

    // Se inicializa el valor de la salida como 0
    lastOut = new float[MAX_D];
    lastReverb = new float[MAX_D];

    for(int i = 0;i < MAX_D; i++){
        lastOut[i] = 0.0;
        lastReverb[i] = 0.0;
    }

    //Inicializacion de los valores en los punteros de tipo double[2048][2]
    inicializarH32();
    inicializarH64();
    inicializarH125();
    inicializarH250();
    inicializarH500();
    inicializarH1k();
    inicializarH2k();
    inicializarH4k();
    inicializarH8k();
    inicializarH16k();

}
/*
 * Destructor
 */
controlVolume::~controlVolume(){

}
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
void controlVolume::inicializarHK(fftw_complex *puntero, double G, double a_0, double b_0, double c_0, double e_0, double f_0, double g_0, double b_1, double c_1, double d_1, double e_1, double f_1, double g_1){

    int N = 2048; //Largo del enventanado.

    fftw_complex *h = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    //Asumiendo que x(n) = d(n) Impulso unitario.
    //calculo de h(0).
    h[0][REAL] = G * a_0;
    //calculo de h(1).
    h[1][REAL] = G * b_0 + b_1 * h[0][REAL];
    //calculo de h(2).
    h[2][REAL] = G * c_0 + h[1][REAL] * b_1 + c_1 * h[0][REAL];
    //calculo de h(3).
    h[3][REAL] = h[2][REAL] * b_1 + c_1 * h[1][REAL] + d_1 * h[0][REAL];
    //calculo de h(4).
    h[4][REAL] = G * e_0 + b_1 * h[3][REAL] + c_1 * h[2][REAL] + d_1 * h[1][REAL] + e_1 * h[0][REAL];
    //calculo de h(5).
    h[5][REAL] = G * f_0 + b_1 * h[4][REAL] + c_1 * h[3][REAL] + d_1 * h[2][REAL] + e_1 * h[1][REAL] + f_1 * h[0][REAL];
    //calculo de h(6).
    h[6][REAL] = G * g_0 + b_1 * h[5][REAL] + c_1 * h[4][REAL] + d_1 * h[3][REAL] + e_1 * h[2][REAL] + f_1 * h[1][REAL] + g_1 * h[0][REAL];

    //De h(7) en adelante solo depende de las salidas anteriores. Por lo que recursivamente se calculan los demas valores.
    for(int i = 7; i<1025; i++){


        h[i][REAL] = b_1 * h[i-1][REAL] + c_1 * h[i-2][REAL] + d_1 * h[i-3][REAL] + e_1 * h[i-4][REAL] + f_1 * h[i-5][REAL] + g_1 * h[i-6][REAL];

    }

    //Se agregan ceros hasta que el largo de h(n) sea igual a L+M-1 = 2048.
    for(int i = 1025;i<N;i++){

        h[i][REAL] = 0.0;

    }

    //Se inicializan la parte imaginaria en cero debido a que la respuesta al impulso es real.
    for(int i =0;i<N;i++){

        h[i][IMAG] = 0.0;

    }

    //Se aplica la DFT.
    fftw_plan plan = fftw_plan_dft_1d(N,h,puntero,FFTW_FORWARD,FFTW_ESTIMATE);
    fftw_execute(plan);

    //Se libera la memoria.
    fftw_destroy_plan(plan);
    fftw_free(h);
    fftw_cleanup();

}
void controlVolume::inicializarH32(){

    //Coeficientes obtenidos al generar la funcion de transferencia y ecuacion de diferencias de los filtros de orden 6.
    double G = 0.0000051726440015777536390439301;
    double a_0 = 1.0;
    double b_0 = -3.9983645237106202330323867;
    double c_0 = 4.996729047853150973423908;
    double e_0 = -4.996729047853150973423908;
    double f_0 = 3.9983645237106202330323867;
    double g_0 = -1.0;

    double b_1 = 5.9967453132568113716160951;
    double c_1 = -14.983801901853976090706055;
    double d_1 = 19.9677543230578251609586;
    double e_1 = -14.967904692182449011284007;
    double f_1 = 5.9840274563218738279601894;
    double g_1 = -0.99682049860009280806139031;

    //Se almacenan en el arreglo asociado a el filtro de 32Hz los valores de su H(k).
    inicializarHK(f32,G,a_0,b_0,c_0,e_0,f_0,g_0,b_1,c_1,d_1,e_1,f_1,g_1);

}
void controlVolume::inicializarH64(){

    //Coeficientes obtenidos al generar la funcion de transferencia y ecuacion de diferencias de los filtros de orden 6.
    double G = 0.000010341142425214124028623811;
    double a_0 = 1.0;
    double b_0 = -3.9934658733542631736668227;
    double c_0 = 4.9869317536107633515030102;
    double e_0 = -4.9869317536107633515030102;
    double f_0 = 3.9934658733542631736668227;
    double g_0 = -1.0;

    double b_1 = 5.9933505401304589810251855;
    double c_1 = -14.967054483387533636573608;
    double d_1 = 19.934711294803655334817449;
    double e_1 = -14.935312410443541963900316;
    double f_1 = 5.9679561693283540435572831;
    double g_1 = -0.99365111043196252538223234;

    //Se almacenan en el arreglo asociado a el filtro de 64Hz los valores de su H(k).
    inicializarHK(f64,G,a_0,b_0,c_0,e_0,f_0,g_0,b_1,c_1,d_1,e_1,f_1,g_1);
}
void controlVolume::inicializarH125(){

    //Coeficientes obtenidos al generar la funcion de transferencia y ecuacion de diferencias de los filtros de orden 6.
    double G = 0.000019738188870821139823908894;
    double a_0 = 1.0;
    double b_0 = -3.9763100468322947200761064;
    double c_0 = 4.9526201985423163876021135;
    double e_0 = -4.9526201985423163876021135;
    double f_0 = 3.9763100468322947200761064;
    double g_0 = -1.0;

    double b_1 = 5.9867734651203612372682983;
    double c_1 = -14.935035264418475620118443;
    double d_1 = 19.872396905624650997879144;
    double e_1 = -14.874714159791597722914958;
    double f_1 = 5.9385113334062298307003402;
    double g_1 = -0.98793227997522281569331426;

    //Se almacenan en el arreglo asociado a el filtro de 125Hz los valores de su H(k).
    inicializarHK(f125,G,a_0,b_0,c_0,e_0,f_0,g_0,b_1,c_1,d_1,e_1,f_1,g_1);
}
void controlVolume::inicializarH250(){

    //Coeficientes obtenidos al generar la funcion de transferencia y ecuacion de diferencias de los filtros de orden 6.
    double G = 0.000040929054711206818079668318;
    double a_0 = 1.0;
    double b_0 = -3.902501155624368944785374;
    double c_0 = 4.8050038824718228624988114;
    double e_0 = -4.8050038824718228624988114;
    double f_0 = 3.902501155624368944785374;
    double g_0 = -1.0;

    double b_1 = 5.970858033075922222110421;
    double c_1 = -14.858935846164756711118571;
    double d_1 = 19.72708134746538988224529;
    double e_1 = -14.736215153719255610553773;
    double f_1 = 5.8726394387280063114076256;
    double g_1 = -0.97542782140164918658342685;

    //Se almacenan en el arreglo asociado a el filtro de 250Hz los valores de su H(k).
    inicializarHK(f250,G,a_0,b_0,c_0,e_0,f_0,g_0,b_1,c_1,d_1,e_1,f_1,g_1);

}
void controlVolume::inicializarH500(){

    //Coeficientes obtenidos al generar la funcion de transferencia y ecuacion de diferencias de los filtros de orden 6.
    double G = 0.000086593064180069152921057074;
    double a_0 = 1.0;
    double b_0 = -3.6358483439687612204238576;
    double c_0 = 4.2717201469857393192341988;
    double e_0 = -4.2717201469857393192341988;
    double f_0 = 3.6358483439687612204238576;
    double g_0 = -1.0;

    double b_1 = 5.9332745075537252077424455;
    double c_1 = -14.685204622617673564377583;
    double d_1 = 19.407335230768644862564543;
    double e_1 = -14.443617387617392111565096;
    double f_1 = 5.7396734044529704732440223;
    double g_1 = -0.95146125988497265435483996;

    //Se almacenan en el arreglo asociado a el filtro de 500Hz los valores de su H(k).
    inicializarHK(f500,G,a_0,b_0,c_0,e_0,f_0,g_0,b_1,c_1,d_1,e_1,f_1,g_1);
}
void controlVolume::inicializarH1k(){

    //Coeficientes obtenidos al generar la funcion de transferencia y ecuacion de diferencias de los filtros de orden 6.
    double G = 0.000213804126957497;
    double a_0 = 1.0;
    double b_0 = -2.8486080392648727155;
    double c_0 = 2.6975120363417355307;
    double e_0 = -2.6975120363417355307;
    double f_0 = 2.8486080392648727155;
    double g_0 = -1.0;

    double b_1 = 5.8333303089089971749;
    double c_1 = -14.244364370759852179;
    double d_1 = 18.636922806015558507;
    double e_1 = -13.779420871523798908;
    double f_1 = 5.4588165829462678147;
    double g_1 = -0.90529237899539838352;

    //Se almacenan en el arreglo asociado a el filtro de 1kHz los valores de su H(k).
    inicializarHK(f1k,G,a_0,b_0,c_0,e_0,f_0,g_0,b_1,c_1,d_1,e_1,f_1,g_1);
}
void controlVolume::inicializarH2k(){

    //Coeficientes obtenidos al generar la funcion de transferencia y ecuacion de diferencias de los filtros de orden 6.
    double G = 0.00074926873545703097899417511;
    double a_0 = 1.0;
    double b_0 = -1.4947375892812146958021913;
    double c_0 = -0.0079745402897966055633105498;
    double e_0 = 0.0079745402897966055633105498;
    double f_0 = 1.4947375892812146958021913;
    double g_0 = -1.0;

    double b_1 = 5.5387754475048005531334638;
    double c_1 = -13.029374739156493134828452;
    double d_1 = 16.651554296506425600910006;
    double e_1 = -12.191866778059177178761274;
    double f_1 = 4.8500890150686037927130201;
    double g_1 = -0.81965335930735705449734496;

    //Se almacenan en el arreglo asociado a el filtro de 2kHz los valores de su H(k).
    inicializarHK(f2k,G,a_0,b_0,c_0,e_0,f_0,g_0,b_1,c_1,d_1,e_1,f_1,g_1);

}
void controlVolume::inicializarH4k(){

    //Coeficientes obtenidos al generar la funcion de transferencia y ecuacion de diferencias de los filtros de orden 6.
    double G = 0.0038620598365113022187866676;
    double a_0 = 1.0;
    double b_0 = -0.4538632308826702210780013;
    double c_0 = -2.0784018774208239399570175;
    double e_0 = 2.0784018774208239399570175;
    double f_0 = 0.4538632308826702210780013;
    double g_0 = -1.0;

    double b_1 = 4.6144062374193630660101917;
    double c_1 = -9.6764649201924619603687461;
    double d_1 = 11.642260519904482407582691;
    double e_1 = -8.4704561841444778735876753;
    double f_1 = 3.5365447408459269595937258;
    double g_1 = -0.67244749821832905389840107;

    //Se almacenan en el arreglo asociado a el filtro de 4kHz los valores de su H(k).
    inicializarHK(f4k,G,a_0,b_0,c_0,e_0,f_0,g_0,b_1,c_1,d_1,e_1,f_1,g_1);
}
void controlVolume::inicializarH8k(){

    //Coeficientes obtenidos al generar la funcion de transferencia y ecuacion de diferencias de los filtros de orden 6.
    double G = 0.023485908459061365094466822;
    double a_0 = 1.0;
    double b_0 = -0.053294510377427606329092669;
    double c_0 = -2.8410023434563282940246154;
    double e_0 = 2.8410023434563282940246154;
    double f_0 = 0.053294510377427606329092669;
    double g_0 = -1.0;

    double b_1 = 1.8584813304477576512141468;
    double c_1 = -3.0324829142244507096393136;
    double d_1 = 2.8064965808216433273969415;
    double e_1 = -2.3723134094008937466924181;
    double f_1 = 1.0888038141957738780263298;
    double g_1 = -0.4546435756375884484903338;

    //Se almacenan en el arreglo asociado a el filtro de 8kHz los valores de su H(k).
    inicializarHK(f8k,G,a_0,b_0,c_0,e_0,f_0,g_0,b_1,c_1,d_1,e_1,f_1,g_1);

}
void controlVolume::inicializarH16k(){

    //Coeficientes obtenidos al generar la funcion de transferencia y ecuacion de diferencias de los filtros de orden 6.
    double G = 0.10997904749987028050206561;
    double a_0 = 1.0;
    double b_0 = 0.030250891271815127936406498;
    double c_0 = -2.9393565409747353101010958;
    double e_0 = 2.9393565409747353101010958;
    double f_0 = -0.030250891271815127936406498;
    double g_0 = -1.0;

    double b_1 = -3.3343994032731112397982542;
    double c_1 = -4.7727334282323425895810942;
    double d_1 = -4.3522525193891041794813646;
    double e_1 = -2.9690684783118825862402446;
    double f_1 = -1.2885809650908786050393928;
    double g_1 = -0.23414890805816163110719685;

    //Se almacenan en el arreglo asociado a el filtro de 16kHz los valores de su H(k).
    inicializarHK(f16k,G,a_0,b_0,c_0,e_0,f_0,g_0,b_1,c_1,d_1,e_1,f_1,g_1);
}

/**
 * @brief filtroGeneral Funcion encargada aplicar el filtrado por DFT a una arreglo de muestras de tipo float.
 * @param blockSize numero de elementos que contiene la entrada, son valores de tipo float.
 * @param volumeGain numero entero que representa el escalado que se aplica a cada valor de la salida calculada.
 * @param in puntero al arreglo de valores de tipo float que conforman la entrada del sistema.
 * @param out puntero a un arreglo de valores tipo float que conforman la salida del ecualizador y son enviados a la tarjeta de audio a reproducirse.
 * @param hk puntero a un arreglo que contiene los valores complejos de H(k) de un filtro especifico.
 * @param temporal puntero al arreglo donde se almacenan los valores de la salida anterior que no se utilizaron y se guardaran los M-1 datos que sobren al filtrar.
 */
void controlVolume::filtroGeneral(int blockSize, int volumeGain, float *in, float *out, fftw_complex *hk, float *temporal){

    int dobleBloque = 2 * blockSize;

   //Inicializan los arreglos que almacenaran a x(n), X(k), y(n), Y(k).
    fftw_complex *x = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * dobleBloque);
    fftw_complex *X = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * dobleBloque);
    fftw_complex *Y = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * dobleBloque);
    fftw_complex *y = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * dobleBloque);

    // CAMBIO
    // Se agregan los valores que se van a utilizar en el bloque
    if(inicio){
        for(int i = 0; i < blockSize; i++){

            // Si es el inicio, el bloque inicia en 0s
            x[i][REAL] = 0.0;
            x[i][IMAG] = 0.0;

        }
        for(int i = blockSize; i< dobleBloque; i++){

            // Se guarda el valor actual en temporal, para ser utilizado en el siguiente ciclo
            // y se agregan los valores de entrada
            temporal[i-blockSize] = in[i-blockSize];
            x[i][REAL] = in[i-blockSize];
            x[i][IMAG] = 0.0;

        }
    } else {
        for(int i = 0; i < blockSize; i++){

            // Se utilizan los valores almacenados previamente en temporal
            x[i][REAL] = temporal[i];
            x[i][IMAG] = 0.0;

        }
        for(int i = blockSize; i< dobleBloque; i++){

            // Se guarda el valor actual en temporal, para ser utilizado en el siguiente ciclo
            // y se agregan los valores de entrada
            temporal[i-blockSize] = in[i-blockSize];
            x[i][REAL] = in[i-blockSize];
            x[i][IMAG] = 0.0;

        }
    }

    //Se aplica la DFT a x(n) para obtener X(k).
    fftw_plan dft = fftw_plan_dft_1d(dobleBloque,x,X,FFTW_FORWARD,FFTW_ESTIMATE);
    fftw_execute(dft);

    /*Se realiza la multiplicacion de los valores complejos de X(k)H(k) = Y(k)
      A ser valores complejos dados en parte real e imaginaria se utiliza:

        Re{Y(k)} = Re{X(k)}*Re{H(k)} - Im{X(k)}*Im{H(k)}
        Im{Y(k)} = Re{X(k)}*Im{H(k)} + Re{H(k)}*Im{X(k)} */

    for(int i = 0;i<dobleBloque;i++){

        Y[i][REAL] = (X[i][REAL]*hk[i][REAL]) - (X[i][IMAG]*hk[i][IMAG]);
        Y[i][IMAG] = (X[i][IMAG]*hk[i][REAL]) + (X[i][REAL]*hk[i][IMAG]);

    }

    //Se aplica la IDFT a Y(k) para obtener y(n).
    fftw_plan idft = fftw_plan_dft_1d(dobleBloque,Y,y,FFTW_BACKWARD,FFTW_ESTIMATE);
    fftw_execute(idft);

    double Div = static_cast<double>(dobleBloque);

    // CAMBIO
    // Se almacenan los valores actuales en la salida (a partir de M-1), utilizando la ganancia del filtro
    for(int i=0; i<blockSize;i++){
       //out[i] = static_cast<float>(0.02 * (volumeGain)* (y[blockSize+i][REAL]/Div));
       out[i] = static_cast<float>(0.02 * (volumeGain)* (y[blockSize+i][REAL]/Div));

    }

    //Se libera la memoria.
    fftw_destroy_plan(dft);
    fftw_destroy_plan(idft);
    fftw_free(x);
    fftw_free(X);
    fftw_free(y);
    fftw_free(Y);
    fftw_cleanup();
}

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
void controlVolume::filter(int blockSize, int volumeGain,int g32,int g64,int g125,int g250,int g500,int g1k,int g2k,int g4k,int g8k,int g16k, float *in, float *out, int aReverb, int dReverb, bool enabledReverb, int typeReverb){

    //Se inicializan los punteros que almacenaran la salida de cada filtro.
    float* pf32 = new float[blockSize];
    float* pf64 = new float[blockSize];
    float* pf125 = new float[blockSize];
    float* pf250 = new float[blockSize];
    float* pf500 = new float[blockSize];
    float* pf1k = new float[blockSize];
    float* pf2k = new float[blockSize];
    float* pf4k = new float[blockSize];
    float* pf8k = new float[blockSize];
    float* pf16k = new float[blockSize];

    //Se llama la funcion que realiza el filtrado para cada uno de los filtros.
    filtroGeneral(blockSize,g32,in,pf32,f32,datos32);
    filtroGeneral(blockSize,g64,in,pf64,f64,datos64);
    filtroGeneral(blockSize,g125,in,pf125,f125,datos125);
    filtroGeneral(blockSize,g250,in,pf250,f250,datos250);
    filtroGeneral(blockSize,g500,in,pf500,f500,datos500);
    filtroGeneral(blockSize,g1k,in,pf1k,f1k,datos1k);
    filtroGeneral(blockSize,g2k,in,pf2k,f2k,datos2k);
    filtroGeneral(blockSize,g4k,in,pf4k,f4k,datos4k);
    filtroGeneral(blockSize,g8k,in,pf8k,f8k,datos8k);
    filtroGeneral(blockSize,g16k,in,pf16k,f16k,datos16k);

    // Se va a implementar el siguiente efecto de reverberacion
    // y(n) = a * y(n - D) - a * x(n) + x(n - D)

    // Se define cada elemento de la salida como la suma de las salidas de los filtros para un n, escalado por una constante.
    for (int n=0; n<blockSize;++n){

        tmpOut[n] = 0.02 * (volumeGain)*(pf32[n]+pf64[n]+pf125[n]+pf250[n]+pf500[n]+pf1k[n]+pf2k[n]+pf4k[n]+pf8k[n]+pf16k[n]);

        // Reverberacion
        if(enabledReverb){

            float y_nD = (n < MAX_D)? lastReverb[n]:out[n-dReverb];
            float x_nD = (n < MAX_D)? lastOut[n]:tmpOut[n-dReverb];

            switch (typeReverb) {
            case 0: // y(n) = a * y(n - D) - a * x(n) + x(n - D)
                out[n] = 0.01 * aReverb * y_nD - 0.01 * aReverb * tmpOut[n] + x_nD;
                break;
            case 1: // y(n) = x(n) + a y(n - D)
                out[n] = tmpOut[n] +  0.01 * aReverb * y_nD;
            default:
                out[n] = 0.01 * aReverb * y_nD - 0.01 * aReverb * tmpOut[n] + x_nD;
                break;
            }

        } else {
            out[n] = tmpOut[n];
        }

        // Almacena los ultimos valores de salida
        if(n >= blockSize - MAX_D - 1){
            lastOut[n + MAX_D - blockSize] = tmpOut[n];
            lastReverb[n + MAX_D - blockSize] = out[n];
        }
    }

    //Al realizar el procedimiento una vez se define que ya no es el inicio de la cancion.
    if(inicio){
        inicio = false;
    }

    //Se libera la memoria solicitada.
    delete pf32;
    delete pf64;
    delete pf125;
    delete pf250;
    delete pf500;
    delete pf1k;
    delete pf2k;
    delete pf4k;
    delete pf8k;
    delete pf16k;
}
