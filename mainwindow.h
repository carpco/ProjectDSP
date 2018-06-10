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
 * \file   mainwindow.h
 *         Implements the equalizer H(w) computation
 * \author Pablo Alvarado\Jose Miguel Barboza\ Luis Alfredo Piedra Esquivel\ Jose Pablo Delgado Martinez
 * \date   2010.12.12\2017.05.26\2017.02.12
 *
 * $Id: mainwindow.h $
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QtGui>
#include <QtCore>

#include "dspsystem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

    void paintEvent(QPaintEvent *e);

private:
    Ui::MainWindow *ui;

    int volumeGain;
    /**
      *Tolerance value
      */
    static const float Epsilon;

     /**
      * Verbose flag
      */
     bool verbose_;

     /**
      * Timer used to recompute the filter once the user changes the
      * values
      */
     QTimer *timer_;

     /**
      * List of selected files so far
      */
     QStringList selectedFiles_;

     /**
      * Pointer to an inherited class of processor, which does
      * all the real work.
      */
     dspSystem* dsp_;

     /**
      *DSP change
      */
     bool dspChanged_;

   private slots:
     void on_fileEdit_returnPressed();
     void on_fileButton_clicked();
     void on_volumeSlider_valueChanged(int value);


     void update();
     void on_f1kSlider_valueChanged(int value);
     void on_f32Slider_valueChanged(int value);
     void on_f64Slider_valueChanged(int value);
     void on_f125Slider_valueChanged(int value);
     void on_f250Slider_valueChanged(int value);
     void on_f500Slider_valueChanged(int value);
     void on_f2kSlider_valueChanged(int value);
     void on_f4kSlider_valueChanged(int value);
     void on_f8kSlider_valueChanged(int value);
     void on_f16kSlider_valueChanged(int value);
     void on_actionClassical_triggered();
     void on_actionClub_triggered();
     void on_actionDance_triggered();
     void on_actionFull_Bass_Treble_triggered();
     void on_actionFull_Treble_triggered();
     void on_actionPop_triggered();
     void on_actionReggae_triggered();
     void on_actionRock_triggered();
     void on_actionTechno_triggered();
     void on_actionOpen_wav_triggered();
     void on_actionFlat_triggered();
     void on_actionZero_triggered();
};




#endif // MAINWINDOW_H
