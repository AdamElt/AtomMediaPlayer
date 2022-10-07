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

    player->setAudioOutput(audioOutput);

    ui->groupBox->setLayout(layout);

    layout->addWidget(vw);

    player->setVideoOutput(vw);
    
    connect(player, &QMediaPlayer::durationChanged, ui->durationSlider, &QSlider::setMaximum);

    connect(player, &QMediaPlayer::positionChanged, ui->durationSlider, &QSlider::setValue);

    connect(ui->durationSlider, &QSlider::sliderMoved, player, &QMediaPlayer::setPosition);
    
    this->setWindowTitle("Atom Media");
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
    
    setWindowTitle(filename);
    
    ui->statusbar->showMessage("Playing " + filename);
    
    ui->volumeSlider->setSliderPosition(100);

    on_actionPlay_triggered();

    on_volumeSlider_valueChanged(50);
    
    connect(player, &QMediaPlayer::durationChanged, this, [&]() {
     
        updateDurationInfo(player->duration());
           
    });

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

void MainWindow::updateDurationInfo(qint64 duration)
 {

     int seconds = (duration/1000) % 60;
     int minutes = (duration/60000) % 60;
     int hours = (duration/3600000) % 24;

     QTime time(hours, minutes,seconds);

     ui->label->setText(time.toString());

 }
