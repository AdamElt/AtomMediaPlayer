#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>
#include <QProgressBar>
#include <QSlider>
#include <QBoxLayout>
#include <QLayout>
#include <QGroupBox>
#include <QAudio>
#include <QAudioOutput>
#include <QComboBox>
#include <QKeyEvent>
#include <QLabel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QMediaPlayer::PlaybackState state() const;
    float volume() const;

signals:
   void changeVolume(float volume);
private slots:
    void on_actionOpen_triggered();

    void on_actionPause_triggered();

    void on_actionPlay_triggered();

    void on_actionstop_triggered();

    void on_playButton_clicked();

    void on_muteButton_clicked();

    void on_volumeSlider_valueChanged(int value);

    void mouseDoubleClickEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    
    void updateDurationInfo(qint64 duration);

private:
    QMediaPlayer::PlaybackState playerState = QMediaPlayer::StoppedState;
    QAudioOutput *audioOutput = 0;
    Ui::MainWindow *ui;
    QMediaPlayer* player;
    QVideoWidget* vw;
    QProgressBar* bar;
    QSlider* slider;
    QGridLayout* layout;
    QMainWindow* newWindow;
    QGroupBox* box;
    QHBoxLayout* hLayout;
    QHBoxLayout* controlLayout;
    QLabel* label;
    qint64 duration;
};
#endif // MAINWINDOW_H
