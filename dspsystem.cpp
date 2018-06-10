/*
 * DSP Example is part of the DSP Lecture at TEC-Costa Rica
 * Copyright (C) 2010  Pablo Alvarado
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
 * \file   freqFilter.h
 *         Implements filtering in the frequency domain
 * \author Pablo Alvarado/Jose Miguel Barboza/Luis Alfredo Piedra Esquivel Jose Pablo Delgado Martinez.
 * \date   2010.12.12/2017.05.26/2017.2.12
 *
 * $Id: equalizer.cpp $
 */

#include "dspsystem.h"
#include <cstring>

#undef _DSP_DEBUG
#define _DSP_DEBUG

#ifdef _DSP_DEBUG
#define _debug(x) std::cerr << x
#include <iostream>
#else
#define _debug(x)
#endif


dspSystem::dspSystem()
  :sampleRate_(0),bufferSize_(0),cv_(0){
}

dspSystem::~dspSystem() {
    delete cv_;
    cv_;
}


void dspSystem::updateVolume(int value){
   /*
    * Updating volume value
    */
   volumeGain_=value;

}
/**
 * @brief dspSystem::updateG32 Metodo que escala la salida del filtro de 32Hz
 * @param value numero entero que representa la posicion del slider.
 */
void dspSystem::updateG32(int value){

    g32_ = value;
}
/**
 * @brief dspSystem::updateG64 Metodo que escala la salida del filtro de 64Hz
 * @param value numero entero que representa la posicion del slider.
 */
void dspSystem::updateG64(int value){

    g64_ = value;
}
/**
 * @brief dspSystem::updateG125 Metodo que escala la salida del filtro de 125Hz
 * @param value numero entero que representa la posicion del slider.
 */
void dspSystem::updateG125(int value){

    g125_ = value;
}
/**
 * @brief dspSystem::updateG250 Metodo que escala la salida del filtro de 250Hz
 * @param value numero entero que representa la posicion del slider.
 */
void dspSystem::updateG250(int value){

    g250_ = value;
}
/**
 * @brief dspSystem::updateG500 Metodo que escala la salida del filtro de 500Hz
 * @param value numero entero que representa la posicion del slider.
 */
void dspSystem::updateG500(int value){

    g500_ = value;
}
/**
 * @brief dspSystem::updateG1k Metodo que escala la salida del filtro de 1kHz
 * @param value numero entero que representa la posicion del slider.
 */
void dspSystem::updateG1k(int value){

    g1k_ = value;
}
/**
 * @brief dspSystem::updateG2k Metodo que escala la salida del filtro de 2kHz
 * @param value numero entero que representa la posicion del slider.
 */
void dspSystem::updateG2k(int value){

    g2k_ = value;
}
/**
 * @brief dspSystem::updateG4k Metodo que escala la salida del filtro de 4kHz
 * @param value numero entero que representa la posicion del slider.
 */
void dspSystem::updateG4k(int value){

    g4k_ = value;
}
/**
 * @brief dspSystem::updateG8k Metodo que escala la salida del filtro de 8kHz
 * @param value numero entero que representa la posicion del slider.
 */
void dspSystem::updateG8k(int value){

    g8k_ = value;
}
/**
 * @brief dspSystem::updateG16k Metodo que escala la salida del filtro de 16kHz
 * @param value numero entero que representa la posicion del slider.
 */
void dspSystem::updateG16k(int value){

    g16k_ = value;
}

/**
 * Initialization function for the current filter plan
 */
bool dspSystem::init(const int sampleRate,const int bufferSize) {
  _debug("dspSystem::init()" << std::endl);

  sampleRate_ = sampleRate;
  bufferSize_ = bufferSize;
  volumeGain_ = 0;
  g32_ = 25;
  g64_ = 25;
  g125_ = 25;
  g250_ = 25;
  g500_ = 25;
  g1k_ = 25;
  g2k_ = 25;
  g4k_ = 25;
  g8k_ = 25;
  g16k_ = 25;

  delete cv_;
  cv_=new controlVolume();

  return true;
}


/**
 * Processing function
 */
bool dspSystem::process(float* in,float* out) {

  float* tmpIn = in;
  float* tmpOut = out;

  cv_->filter(bufferSize_,volumeGain_,g32_,g64_,g125_,g250_,g500_,g1k_,g2k_,g4k_,g8k_,g16k_,tmpIn,tmpOut);

  return true;
}

/**
 * Shutdown the processor
 */
bool dspSystem::shutdown() {
  return true;
}

/**
 * Set buffer size (call-back)
 */
int dspSystem::setBufferSize(const int bufferSize) {
  bufferSize_=bufferSize;
  return 1;
}

/**
 * Set sample rate (call-back)
 */
int dspSystem::setSampleRate(const int sampleRate) {
  sampleRate_=sampleRate;
  return 1;
}
