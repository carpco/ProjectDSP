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
 * \author Pablo Alvarado/Jose Miguel Barboza
 * \date   2010.12.12/2017.05.26
 *
 * $Id: equalizer.cpp $
 */

#ifndef DSPSYSTEM_H
#define DSPSYSTEM_H

#include "processor.h"
#include "controlvolume.h"

class dspSystem : public processor {
public:

  /**
   * Constructor
   */
  dspSystem();

  /**
   * Destructor
   */
  ~dspSystem();

  /**
   * Initialization function for the current filter plan
   */
  virtual bool init(const int frameRate,const int bufferSize);

  /**
   * Processing function
   */
  virtual bool process(float* in,float* out);

  /**
   * Shutdown the processor
   */
  virtual bool shutdown();

  /**
   * Set buffer size
   */
  virtual int setBufferSize(const int bufferSize);

  /**
   * Set frame rate
   */
  virtual int setSampleRate(const int sampleRate);

  void updateVolume(int value);

  /*
   * Se definen los metodos que actualizan el valor de cada slider.
   */
  void updateG32(int value);
  void updateG64(int value);
  void updateG125(int value);
  void updateG250(int value);
  void updateG500(int value);
  void updateG1k(int value);
  void updateG2k(int value);
  void updateG4k(int value);
  void updateG8k(int value);
  void updateG16k(int value);

  /**
   * Sample rate
   */
  int sampleRate_;

  /**
   * Buffer size
   */
  int bufferSize_;

  /**
   * VolumeGain
   */

  int volumeGain_;

  // Constantes donde se almacena el valor de cada slider.
  int g32_;
  int g64_;
  int g125_;
  int g250_;
  int g500_;
  int g1k_;
  int g2k_;
  int g4k_;
  int g8k_;
  int g16k_;

  /**
   * control Volume
   */
  controlVolume* cv_;


};


#endif // DSPSYSTEM_H
