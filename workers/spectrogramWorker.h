#ifndef SPECTROGRAMWORKER_H
#define SPECTROGRAMWORKER_H

#include <QObject>
#include <opencv2/opencv.hpp>
class SpectrogramWorker : public QObject
{
    Q_OBJECT
public:
    explicit SpectrogramWorker(std::string value, QObject *parent = nullptr);

public slots:
    void calculeSpectrogram();
signals:
    void finished();
    void progressChanged(int value);
    void resultReady(cv::Mat spectrogramOutput);

private:
    std::string fileToRender;
};

#endif // SPECTROGRAMWORKER_H
