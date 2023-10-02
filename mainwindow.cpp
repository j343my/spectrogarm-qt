#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QThread>
#include <QLabel>
#include <workers/spectrogramWorker.h>
#include <opencv2/opencv.hpp>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    progressBar = new QProgressBar(this);
    imageLabel = new QLabel(this);
    ui->verticalLayout->addWidget(progressBar);
    ui->verticalLayout->addWidget(imageLabel);
    imageLabel->hide();
    progressBar->hide();
    QWidget* containerWidget = ui->verticalLayout->parentWidget();
    if(containerWidget) {
        int height = containerWidget->height();
        int width = containerWidget->width();
        qDebug() << "Height:" << height << "Width:" << width;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startBackgroundTask(std::string FileToRender) {
    progressBar->show();
    SpectrogramWorker* spectrogramWorker = new SpectrogramWorker(FileToRender);
    QThread* thread = new QThread(this);

    spectrogramWorker->moveToThread(thread);

    connect(thread, &QThread::started, spectrogramWorker, &SpectrogramWorker::calculeSpectrogram);
    connect(spectrogramWorker, &SpectrogramWorker::finished, thread, &QThread::quit);
    connect(spectrogramWorker, &SpectrogramWorker::progressChanged, progressBar, &QProgressBar::setValue);
    connect(spectrogramWorker, &SpectrogramWorker::finished, spectrogramWorker, &SpectrogramWorker::deleteLater);
    connect(spectrogramWorker, &SpectrogramWorker::resultReady, this, &MainWindow::onWorkerFinished); // Custom slot for the result

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}

void MainWindow::onWorkerFinished(cv::Mat spectrogramOutput) {
        QImage img = QImage((uchar*)spectrogramOutput.data, spectrogramOutput.cols, spectrogramOutput.rows, spectrogramOutput.step, QImage::Format_RGB888);

        QPixmap pixmap = QPixmap::fromImage(img);

        //imageLabel->clear();
        imageLabel->setPixmap(pixmap);


        progressBar->hide();
        imageLabel->show();
}

void MainWindow::on_pushButton_clicked()
{
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("WAV Files (*.wav);;All Files (*)"));
        if (!fileName.isEmpty()) {
            ui->label->setText(fileName);
            this->startBackgroundTask(fileName.toStdString());
        }

}

