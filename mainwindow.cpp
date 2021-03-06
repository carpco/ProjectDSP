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
 * \file   mainwindow.cpp
 *         Implements the equalizer H(w) computation
 * \author Pablo Alvarado\ Luis Alfredo Piedra Esquivel Jose Pablo Delgado Martinez
 * \date   2010.12.12 \ 2017.2.12
 *
 * $Id: equalizer.cpp $
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "jack.h"
#include <string>
#include <QPalette>

#undef _DSP_DEBUG
#define _DSP_DEBUG

#ifdef _DSP_DEBUG
#define _debug(x) std::cerr << x
#include <iostream>
#else
#define _debug(x)
#endif


/**
 * Precision used by trimming
 */
const float MainWindow::Epsilon = 0.001;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    verbose_(false),
    dspChanged_(true)
{
    ui->setupUi(this);
    ui->fileEdit->setVisible(false);
    ui->fileButton->setVisible(false);
    this->setStyleSheet("QLabel {color:lime} QStatusBar {background: dimgray; color: lime} QToolBar { background: dimgray} QToolButton:hover {background-color:lime}");
    ui->mainToolBar->layout()->setSpacing(15);
    /*
     * Set up a timer 4 times in a second to check if the user
     * changed the equalizer values, and if so, then create a new
     * filter response
     */
    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(update()));
    timer_->start(250);

    timer_ = new QTimer(this);
    connect(timer_, SIGNAL(timeout()), this, SLOT(drawSpectral()));
    // Estaba en 500
    timer_->start(50);

    dsp_ = new dspSystem;
    jack::init(dsp_);

    // parse some command line arguments
    QStringList argv(QCoreApplication::arguments());

    QStringList::const_iterator it(argv.begin());
    while(it!=argv.end()) {
      if ((*it)=="-v" || (*it)=="--verbose") {
        verbose_=true;
      } else if ((*it).indexOf(".wav",0,Qt::CaseInsensitive)>0) {
        ui->fileEdit->setText(*it);
        std::string tmp(qPrintable(*it));
        jack::playAlso(tmp.c_str());
      }
      ++it;
    }
}

MainWindow::~MainWindow()
{
    jack::close();
    delete timer_;
    delete ui;
    delete dsp_;
}

void MainWindow::update() {
    if(dspChanged_){
        _debug("Updating" << std::endl);

        dspChanged_=false;
    }

}

void MainWindow::drawSpectral(){
    struct Spectral spectral = this->dsp_->spectral_;

    const int min = 0;
    const int max = 100;
    const int scale = 1000;

    if(spectral.main < min){spectral.main = spectral.main * -1;}
    if(spectral.f32 < min){spectral.f32 = spectral.f32 * -1;}
    if(spectral.f64 < min){spectral.f64 = spectral.f64 * -1;}
    if(spectral.f125 < min){spectral.f125 = spectral.f125 * -1;}
    if(spectral.f250 < min){spectral.f250 = spectral.f250 * -1;}
    if(spectral.f500 < min){spectral.f500 = spectral.f500 * -1;}
    if(spectral.f1k < min){spectral.f1k = spectral.f1k * -1;}
    if(spectral.f2k < min){spectral.f2k = spectral.f2k * -1;}
    if(spectral.f4k < min){spectral.f4k = spectral.f4k * -1;}
    if(spectral.f8k < min){spectral.f8k = spectral.f8k * -1;}
    if(spectral.f16k < min){spectral.f16k = spectral.f16k * -1;}

    spectral.main = spectral.main * scale;
    spectral.f32 = spectral.f32 * scale;
    spectral.f64 = spectral.f64 * scale;
    spectral.f125 = spectral.f125 * scale;
    spectral.f250 = spectral.f250 * scale;
    spectral.f500 = spectral.f500 * scale;
    spectral.f1k = spectral.f1k * scale;
    spectral.f2k = spectral.f2k * scale;
    spectral.f4k = spectral.f4k * scale;
    spectral.f8k = spectral.f8k * scale;
    spectral.f16k = spectral.f16k * scale;

    if(spectral.main >= max){spectral.main = max;}
    if(spectral.f32 >= max){spectral.f32 = max;}
    if(spectral.f64 >= max){spectral.f64 = max;}
    if(spectral.f125 >= max){spectral.f125 = max;}
    if(spectral.f250 >= max){spectral.f250 = max;}
    if(spectral.f500 >= max){spectral.f500 = max;}
    if(spectral.f1k >= max){spectral.f1k = max;}
    if(spectral.f2k >= max){spectral.f2k = max;}
    if(spectral.f4k >= max){spectral.f4k = max;}
    if(spectral.f8k >= max){spectral.f8k = max;}
    if(spectral.f16k >= max){spectral.f16k = max;}

    QPropertyAnimation *animationmain = new QPropertyAnimation(ui->progress_general, "value");
    animationmain->setDuration(50);
    animationmain->setStartValue(ui->progress_general->value());
    animationmain->setEndValue(spectral.main);
    animationmain->start();

    QPropertyAnimation *animation32 = new QPropertyAnimation(ui->progress_32, "value");
    animation32->setDuration(50);
    animation32->setStartValue(ui->progress_32->value());
    animation32->setEndValue(spectral.f32);
    animation32->start();

    QPropertyAnimation *animation64 = new QPropertyAnimation(ui->progress_64, "value");
    animation64->setDuration(50);
    animation64->setStartValue(ui->progress_64->value());
    animation64->setEndValue(spectral.f64);
    animation64->start();

    QPropertyAnimation *animation125 = new QPropertyAnimation(ui->progress_125, "value");
    animation125->setDuration(50);
    animation125->setStartValue(ui->progress_125->value());
    animation125->setEndValue(spectral.f125);
    animation125->start();

    QPropertyAnimation *animation250 = new QPropertyAnimation(ui->progress_250, "value");
    animation250->setDuration(50);
    animation250->setStartValue(ui->progress_250->value());
    animation250->setEndValue(spectral.f250);
    animation250->start();

    QPropertyAnimation *animation500 = new QPropertyAnimation(ui->progress_500, "value");
    animation500->setDuration(50);
    animation500->setStartValue(ui->progress_500->value());
    animation500->setEndValue(spectral.f500);
    animation500->start();

    QPropertyAnimation *animation1k = new QPropertyAnimation(ui->progress_1k, "value");
    animation1k->setDuration(50);
    animation1k->setStartValue(ui->progress_1k->value());
    animation1k->setEndValue(spectral.f1k);
    animation1k->start();

    QPropertyAnimation *animation2k = new QPropertyAnimation(ui->progress_2k, "value");
    animation2k->setDuration(50);
    animation2k->setStartValue(ui->progress_2k->value());
    animation2k->setEndValue(spectral.f2k);
    animation2k->start();

    QPropertyAnimation *animation4k = new QPropertyAnimation(ui->progress_4k, "value");
    animation4k->setDuration(50);
    animation4k->setStartValue(ui->progress_4k->value());
    animation4k->setEndValue(spectral.f4k);
    animation4k->start();

    QPropertyAnimation *animation8k = new QPropertyAnimation(ui->progress_8k, "value");
    animation8k->setDuration(50);
    animation8k->setStartValue(ui->progress_8k->value());
    animation8k->setEndValue(spectral.f8k);
    animation8k->start();

    QPropertyAnimation *animation16k = new QPropertyAnimation(ui->progress_16k, "value");
    animation16k->setDuration(50);
    animation16k->setStartValue(ui->progress_16k->value());
    animation16k->setEndValue(spectral.f16k);
    animation16k->start();
}

void MainWindow::on_volumeSlider_valueChanged(int value){
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateVolume(value);
    ;
}

void MainWindow::on_fileButton_clicked() {
  selectedFiles_ =
      QFileDialog::getOpenFileNames(this,
                                   "Select one or more audio files to open",
                                   ui->fileEdit->text(),
                                   "WAV Files (*.wav)");

  if (!selectedFiles_.empty()) {
    ui->fileEdit->setText(*selectedFiles_.begin());

    dsp_->cv_->inicio = true;
    jack::stopFiles();
    QStringList::iterator it;
    for (it=selectedFiles_.begin();it!=selectedFiles_.end();++it) {
      ui->statusBar->showMessage("Reproduciendo: "+*it);
      std::string tmp(qPrintable(*it));
      jack::playAlso(tmp.c_str());
    }
  }
}


void MainWindow::on_actionOpen_wav_triggered()
{
    selectedFiles_ =
        QFileDialog::getOpenFileNames(this,
                                     "Select one or more audio files to open",
                                     ui->fileEdit->text(),
                                     "WAV Files (*.wav)");

    if (!selectedFiles_.empty()) {
      ui->fileEdit->setText(*selectedFiles_.begin());

      dsp_->cv_->inicio = true;
      jack::stopFiles();
      QStringList::iterator it;
      for (it=selectedFiles_.begin();it!=selectedFiles_.end();++it) {
        ui->statusBar->showMessage("Reproduciendo: "+*it);
        std::string tmp(qPrintable(*it));
        jack::playAlso(tmp.c_str());
      }
    }
}

void MainWindow::on_fileEdit_returnPressed() {
  jack::stopFiles();
  std::string tmp(qPrintable(ui->fileEdit->text()));
  if (!tmp.empty()) {
    jack::playAlso(tmp.c_str());
  }
}
/**
 * @brief MainWindow::on_f32Slider_valueChanged Metodo que indica cuando se cambio el valor de un slider en la interfaz grafica.
 * @param value numero entero que representa la posicion del slider.
 */
void MainWindow::on_f32Slider_valueChanged(int value)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateG32(value);
    this->repaint();
}

/**
 * @brief MainWindow::on_f64Slider_valueChanged Metodo que indica cuando se cambio el valor de un slider en la interfaz grafica.
 * @param value numero entero que representa la posicion del slider.
 */
void MainWindow::on_f64Slider_valueChanged(int value)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateG64(value);
    this->repaint();
}

/**
 * @brief MainWindow::on_f125Slider_valueChanged Metodo que indica cuando se cambio el valor de un slider en la interfaz grafica.
 * @param value numero entero que representa la posicion del slider.
 */
void MainWindow::on_f125Slider_valueChanged(int value)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateG125(value);
    this->repaint();
}

/**
 * @brief MainWindow::on_f250Slider_valueChanged Metodo que indica cuando se cambio el valor de un slider en la interfaz grafica.
 * @param value numero entero que representa la posicion del slider.
 */
void MainWindow::on_f250Slider_valueChanged(int value)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateG250(value);
    this->repaint();
}

/**
 * @brief MainWindow::on_f500Slider_valueChanged Metodo que indica cuando se cambio el valor de un slider en la interfaz grafica.
 * @param value numero entero que representa la posicion del slider.
 */
void MainWindow::on_f500Slider_valueChanged(int value)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateG500(value);
    this->repaint();
}

/**
 * @brief MainWindow::on_f1kSlider_valueChanged Metodo que indica cuando se cambio el valor de un slider en la interfaz grafica.
 * @param value numero entero que representa la posicion del slider.
 */
void MainWindow::on_f1kSlider_valueChanged(int value)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateG1k(value);
    this->repaint();
}

/**
 * @brief MainWindow::on_f2kSlider_valueChanged Metodo que indica cuando se cambio el valor de un slider en la interfaz grafica.
 * @param value numero entero que representa la posicion del slider.
 */
void MainWindow::on_f2kSlider_valueChanged(int value)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateG2k(value);
    this->repaint();
}

/**
 * @brief MainWindow::on_f4kSlider_valueChanged Metodo que indica cuando se cambio el valor de un slider en la interfaz grafica.
 * @param value numero entero que representa la posicion del slider.
 */
void MainWindow::on_f4kSlider_valueChanged(int value)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateG4k(value);
    this->repaint();
}

/**
 * @brief MainWindow::on_f8kSlider_valueChanged Metodo que indica cuando se cambio el valor de un slider en la interfaz grafica.
 * @param value numero entero que representa la posicion del slider.
 */
void MainWindow::on_f8kSlider_valueChanged(int value)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateG8k(value);
    this->repaint();
}

/**
 * @brief MainWindow::on_f16kSlider_valueChanged Metodo que indica cuando se cambio el valor de un slider en la interfaz grafica.
 * @param value numero entero que representa la posicion del slider.
 */
void MainWindow::on_f16kSlider_valueChanged(int value)
{
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateG16k(value);
    this->repaint();
}

/**
 * @brief MainWindow::on_actionClassical_triggered Preset Classical que coloca modifica los valores de los slider.
 */
void MainWindow::on_actionClassical_triggered()
{
    ui->f32Slider->setValue(25);
    ui->f64Slider->setValue(25);
    ui->f125Slider->setValue(25);
    ui->f250Slider->setValue(25);
    ui->f500Slider->setValue(25);
    ui->f1kSlider->setValue(25);
    ui->f2kSlider->setValue(16);
    ui->f4kSlider->setValue(16);
    ui->f8kSlider->setValue(16);
    ui->f16kSlider->setValue(13);
}

/**
 * @brief MainWindow::on_actionClub_triggered Preset Club que coloca modifica los valores de los slider.
 */
void MainWindow::on_actionClub_triggered()
{

    ui->f32Slider->setValue(25);
    ui->f64Slider->setValue(25);
    ui->f125Slider->setValue(35);
    ui->f250Slider->setValue(32);
    ui->f500Slider->setValue(32);
    ui->f1kSlider->setValue(32);
    ui->f2kSlider->setValue(29);
    ui->f4kSlider->setValue(25);
    ui->f8kSlider->setValue(25);
    ui->f16kSlider->setValue(25);

}

/**
 * @brief MainWindow::on_actionDance_triggered Preset Dance que coloca modifica los valores de los slider.
 */
void MainWindow::on_actionDance_triggered()
{
    ui->f32Slider->setValue(37);
    ui->f64Slider->setValue(34);
    ui->f125Slider->setValue(28);
    ui->f250Slider->setValue(25);
    ui->f500Slider->setValue(25);
    ui->f1kSlider->setValue(18);
    ui->f2kSlider->setValue(16);
    ui->f4kSlider->setValue(16);
    ui->f8kSlider->setValue(25);
    ui->f16kSlider->setValue(25);

}

/**
 * @brief MainWindow::on_actionFull_Bass_Treble_triggered Preset Full Bass & Treble que coloca modifica los valores de los slider.
 */
void MainWindow::on_actionFull_Bass_Treble_triggered()
{
    ui->f32Slider->setValue(34);
    ui->f64Slider->setValue(32);
    ui->f125Slider->setValue(25);
    ui->f250Slider->setValue(16);
    ui->f500Slider->setValue(19);
    ui->f1kSlider->setValue(27);
    ui->f2kSlider->setValue(35);
    ui->f4kSlider->setValue(39);
    ui->f8kSlider->setValue(40);
    ui->f16kSlider->setValue(40);
}

/**
 * @brief MainWindow::on_actionFull_Treble_triggered Preset Full Treble que coloca modifica los valores de los slider.
 */
void MainWindow::on_actionFull_Treble_triggered()
{
    ui->f32Slider->setValue(13);
    ui->f64Slider->setValue(13);
    ui->f125Slider->setValue(13);
    ui->f250Slider->setValue(21);
    ui->f500Slider->setValue(29);
    ui->f1kSlider->setValue(39);
    ui->f2kSlider->setValue(43);
    ui->f4kSlider->setValue(43);
    ui->f8kSlider->setValue(43);
    ui->f16kSlider->setValue(45);
}

/**
 * @brief MainWindow::on_actionPop_triggered Preset Pop que coloca modifica los valores de los slider.
 */
void MainWindow::on_actionPop_triggered()
{
    ui->f32Slider->setValue(27);
    ui->f64Slider->setValue(31);
    ui->f125Slider->setValue(34);
    ui->f250Slider->setValue(35);
    ui->f500Slider->setValue(32);
    ui->f1kSlider->setValue(25);
    ui->f2kSlider->setValue(22);
    ui->f4kSlider->setValue(22);
    ui->f8kSlider->setValue(27);
    ui->f16kSlider->setValue(27);
}

/**
 * @brief MainWindow::on_actionReggae_triggered Preset Reggae que coloca modifica los valores de los slider.
 */
void MainWindow::on_actionReggae_triggered()
{
    ui->f32Slider->setValue(25);
    ui->f64Slider->setValue(25);
    ui->f125Slider->setValue(25);
    ui->f250Slider->setValue(18);
    ui->f500Slider->setValue(25);
    ui->f1kSlider->setValue(33);
    ui->f2kSlider->setValue(33);
    ui->f4kSlider->setValue(25);
    ui->f8kSlider->setValue(25);
    ui->f16kSlider->setValue(25);
}

/**
 * @brief MainWindow::on_actionRock_triggered Preset Rock que coloca modifica los valores de los slider.
 */
void MainWindow::on_actionRock_triggered()
{
    ui->f32Slider->setValue(35);
    ui->f64Slider->setValue(31);
    ui->f125Slider->setValue(18);
    ui->f250Slider->setValue(15);
    ui->f500Slider->setValue(21);
    ui->f1kSlider->setValue(30);
    ui->f2kSlider->setValue(36);
    ui->f4kSlider->setValue(39);
    ui->f8kSlider->setValue(39);
    ui->f16kSlider->setValue(39);
}

/**
 * @brief MainWindow::on_actionTechno_triggered  Preset Techno que coloca modifica los valores de los slider.
 */
void MainWindow::on_actionTechno_triggered()
{
    ui->f32Slider->setValue(35);
    ui->f64Slider->setValue(32);
    ui->f125Slider->setValue(25);
    ui->f250Slider->setValue(18);
    ui->f500Slider->setValue(19);
    ui->f1kSlider->setValue(25);
    ui->f2kSlider->setValue(35);
    ui->f4kSlider->setValue(37);
    ui->f8kSlider->setValue(37);
    ui->f16kSlider->setValue(36);

}

/**
 * @brief MainWindow::on_actionFlat_triggered  Preset Flat que coloca modifica los valores de los slider.
 */
void MainWindow::on_actionFlat_triggered()
{
    ui->f32Slider->setValue(25);
    ui->f64Slider->setValue(25);
    ui->f125Slider->setValue(25);
    ui->f250Slider->setValue(25);
    ui->f500Slider->setValue(25);
    ui->f1kSlider->setValue(25);
    ui->f2kSlider->setValue(25);
    ui->f4kSlider->setValue(25);
    ui->f8kSlider->setValue(25);
    ui->f16kSlider->setValue(25);

}

/**
 * @brief MainWindow::on_actionZero_triggered Preset Zero que coloca modifica los valores de los slider.
 */
void MainWindow::on_actionZero_triggered()
{
    ui->f32Slider->setValue(0);
    ui->f64Slider->setValue(0);
    ui->f125Slider->setValue(0);
    ui->f250Slider->setValue(0);
    ui->f500Slider->setValue(0);
    ui->f1kSlider->setValue(0);
    ui->f2kSlider->setValue(0);
    ui->f4kSlider->setValue(0);
    ui->f8kSlider->setValue(0);
    ui->f16kSlider->setValue(0);

}

/**
 * @brief MainWindow::on_aSlider_valueChanged
 * @param value
 */
void MainWindow::on_aSlider_valueChanged(int value){
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateReverbA(value);
    this->repaint();
}

/**
 * @brief MainWindow::on_dSlider_valueChanged
 * @param value
 */
void MainWindow::on_dSlider_valueChanged(int value){
    if (!dspChanged_){
        dspChanged_=true;
    }
    dsp_->updateReverbD(value);
    this->repaint();
}

/**
 * @brief MainWindow::on_reverberatorCheckBox_stateChanged
 * @param enabled
 */
void MainWindow::on_reverberatorCheckBox_stateChanged(int value){

    _debug("Reverberation Changed" << std::endl);

    dsp_->updateReverbEnabled(ui->reverberatorCheckBox->isChecked());
    this->repaint();
}

void MainWindow::on_reverbComboBox_currentIndexChanged(int value){

    _debug("Reverberation Type: " << value << std::endl);

    dsp_->updateReverbType(value);
}
/**
 * @brief MainWindow::paintEvent  Metodo que dibuja en la interfaz las lines y curvas que representan la magnitud de los filtros.
 * @param e
 */
void MainWindow::paintEvent(QPaintEvent *e)//funcion encargada de graficar el nivel de ganancia
{
    // Se definen los puntos donde estaran el centro de cada banda.
    QPoint centro;
    QPoint centro2;

    QPoint pI1;
    QPoint pf1;

    QPoint pI2;
    QPoint pf2;

    QPoint pI3;
    QPoint pf3;

    QPoint pI4;
    QPoint pf4;

    QPoint pI5;
    QPoint pf5;

    QPoint pI6;
    QPoint pf6;

    QPoint pI7;
    QPoint pf7;

    QPoint pI8;
    QPoint pf8;

    QPoint pI9;
    QPoint pf9;

    QPoint pI10;
    QPoint pf10;

    QPainterPath myPath;

    //Se define el color y grosor de la linea.
    QPen myPen;
    myPen.setColor(QColor(0, 255, 0));
    myPen.setWidth(2);

    QPainter painter(this);
    painter.setPen(myPen);

    // Se definen las posiciones X e Y de cada linea.
    pI1.setX(120);
    pI1.setY(175+(-2*ui->f32Slider->value()+50));

    pf1.setX(145);
    pf1.setY(175+(-2*ui->f32Slider->value()+50));

    pI2.setX(185);
    pI2.setY(175+(-2*ui->f64Slider->value()+50));

    pf2.setX(210);
    pf2.setY(175+(-2*ui->f64Slider->value()+50));

    pI3.setX(250);
    pI3.setY(175+(-2*ui->f125Slider->value()+50));

    pf3.setX(275);
    pf3.setY(175+(-2*ui->f125Slider->value()+50));

    pI4.setX(315);
    pI4.setY(175+(-2*ui->f250Slider->value()+50));

    pf4.setX(340);
    pf4.setY(175+(-2*ui->f250Slider->value()+50));

    pI5.setX(380);
    pI5.setY(175+(-2*ui->f500Slider->value()+50));

    pf5.setX(405);
    pf5.setY(175+(-2*ui->f500Slider->value()+50));

    pI6.setX(445);
    pI6.setY(175+(-2*ui->f1kSlider->value()+50));

    pf6.setX(470);
    pf6.setY(175+(-2*ui->f1kSlider->value()+50));

    pI7.setX(510);
    pI7.setY(175+(-2*ui->f2kSlider->value()+50));

    pf7.setX(535);
    pf7.setY(175+(-2*ui->f2kSlider->value()+50));

    pI8.setX(575);
    pI8.setY(175+(-2*ui->f4kSlider->value()+50));

    pf8.setX(600);
    pf8.setY(175+(-2*ui->f4kSlider->value()+50));

    pI9.setX(640);
    pI9.setY(175+(-2*ui->f8kSlider->value()+50));

    pf9.setX(665);
    pf9.setY(175+(-2*ui->f8kSlider->value()+50));

    pI10.setX(705);
    pI10.setY(175+(-2*ui->f16kSlider->value()+50));

    pf10.setX(730);
    pf10.setY(175+(-2*ui->f16kSlider->value()+50));

    centro.setX(100);
    centro.setY(175);

    centro2.setX(102);
    centro2.setY(175);

    //Se dibuja la linea punteada.
    for(int i=0;i<130;i++){

        myPath.moveTo(centro);
        myPath.lineTo(centro2);
        painter.drawPath(myPath);
        centro.setX(centro.x()+5);
        centro2.setX(centro2.x()+5);

    }

    // Se dibujan las lineas centrales.
    myPath.moveTo(pI1);
    myPath.lineTo(pf1);
    painter.drawPath(myPath);

    myPath.moveTo(pI2);
    myPath.lineTo(pf2);
    painter.drawPath(myPath);

    myPath.moveTo(pI3);
    myPath.lineTo(pf3);
    painter.drawPath(myPath);

    myPath.moveTo(pI4);
    myPath.lineTo(pf4);
    painter.drawPath(myPath);

    myPath.moveTo(pI5);
    myPath.lineTo(pf5);
    painter.drawPath(myPath);

    myPath.moveTo(pI6);
    myPath.lineTo(pf6);
    painter.drawPath(myPath);

    myPath.moveTo(pI7);
    myPath.lineTo(pf7);
    painter.drawPath(myPath);

    myPath.moveTo(pI8);
    myPath.lineTo(pf8);
    painter.drawPath(myPath);

    myPath.moveTo(pI9);
    myPath.lineTo(pf9);
    painter.drawPath(myPath);

    myPath.moveTo(pI10);
    myPath.lineTo(pf10);
    painter.drawPath(myPath);

    // Se unen los inicios de cada linea con el final de la siguiente.
    myPath.moveTo(pf1);
    myPath.lineTo(pI2);
    painter.drawPath(myPath);

    myPath.moveTo(pf2);
    myPath.lineTo(pI3);
    painter.drawPath(myPath);

    myPath.moveTo(pf3);
    myPath.lineTo(pI4);
    painter.drawPath(myPath);

    myPath.moveTo(pf4);
    myPath.lineTo(pI5);
    painter.drawPath(myPath);

    myPath.moveTo(pf5);
    myPath.lineTo(pI6);
    painter.drawPath(myPath);

    myPath.moveTo(pf6);
    myPath.lineTo(pI7);
    painter.drawPath(myPath);

    myPath.moveTo(pf7);
    myPath.lineTo(pI8);
    painter.drawPath(myPath);

    myPath.moveTo(pf8);
    myPath.lineTo(pI9);
    painter.drawPath(myPath);

    myPath.moveTo(pf9);
    myPath.lineTo(pI10);
    painter.drawPath(myPath);

}
