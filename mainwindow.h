#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qprogressbar.h>
#include <opencv2/opencv.hpp>
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

private:
    Ui::MainWindow *ui;
    QProgressBar *progressBar;
    QLabel * imageLabel;

public slots:
    void startBackgroundTask(std::string FileToRender);
    void onWorkerFinished(cv::Mat spectrogramOutput);
private slots:
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
