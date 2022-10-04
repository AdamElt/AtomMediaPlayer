#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);

    audioOutput = new QAudioOutput(this);
    vw = new QVideoWidget(this);
    layout = new QGridLayout(this);

    on_volumeSlider_valueChanged(50);

    player->setAudioOutput(audioOutput);


    ui->groupBox->setLayout(layout);

    layout->addWidget(vw);

    player->setVideoOutput(vw);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open a File");

    on_actionstop_triggered();

    player->setSource(QUrl::fromLocalFile(filename));

    on_actionPlay_triggered();

    on_volumeSlider_valueChanged(50);

}


void MainWindow::on_actionPause_triggered()
{
    player->pause();

    ui->statusbar->showMessage("Pausing");
}


void MainWindow::on_actionPlay_triggered()
{
    player->play();

    ui->statusbar->showMessage("Playing Media");
}

void MainWindow::on_actionstop_triggered()
{
    player->stop();

    ui->statusbar->showMessage("Stopping Media");
}

QMediaPlayer::PlaybackState MainWindow::state() const
{
    return playerState;
}

void MainWindow::on_playButton_clicked()
{
      if(player->playbackState() == QMediaPlayer::PausedState){

          player->play();

           ui->playButton->setIcon(QIcon(":/img/Images/pause.png"));
      }
           else if (player->playbackState() == QMediaPlayer::PlayingState){

          player->pause();

           ui->playButton->setIcon(QIcon(":/img/Images/play.png"));
      }
}




void MainWindow::on_muteButton_clicked()
{
    if (ui->volumeSlider->value() == 0) {
            audioOutput->setVolume(20);
            ui->volumeSlider->setSliderPosition(20);
        } else {
            audioOutput->setVolume(0);
            ui->volumeSlider->setSliderPosition(0);
        }
    }

void MainWindow::on_volumeSlider_valueChanged(int value)
{


    qreal linearVolume =  QAudio::convertVolume(ui->volumeSlider->value() / qreal(100),
                                                QAudio::LogarithmicVolumeScale,
                                                QAudio::LinearVolumeScale);
    audioOutput->setVolume(linearVolume);




    if(value == 0){

        ui->muteButton->setIcon(QIcon(":/img/Images/mute.png"));

    } else if(value > 65){

        ui->muteButton->setIcon(QIcon(":/img/Images/max-volume.png"));

    } else if(value <= 65 && value > 35){

        ui->muteButton->setIcon(QIcon(":/img/Images/volume.png"));

    } else if (value < 35){
        ui->muteButton->setIcon(QIcon(":/img/Images/low-volume.png"));
    }


}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Escape || event->key() == Qt::Key_Back) ) {
        if( vw->isFullScreen() != true){
            vw->setFullScreen(true);
        } else(showNormal());

    }
}


 void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
 {
     vw->showMaximized();
    event->accept();
 }
void MainWindow::on_fullScreen_clicked()
{
   vw->setFullScreen(!vw->isFullScreen());

    }


