#include "spectrogramWorker.h"
#include "QThread"//
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <sndfile.h>//

#include <thirdparty/kissfft/kiss_fft.h>

SpectrogramWorker::SpectrogramWorker(std::string value, QObject *parent)
            : QObject(parent), fileToRender(value) {}

void SpectrogramWorker::calculeSpectrogram() {
        SNDFILE *file;
        SF_INFO sfInfo;
        char* cFile = const_cast<char*>(this->fileToRender.c_str());
        file = sf_open(cFile, SFM_READ, &sfInfo);
        if (!file) {
            // Gérer l'erreur
            qDebug() << "Eror reading file";
        }

        // 2. Transformation de Fourier avec Kiss FFT
        const int N = 1024;
        const int overlap = N / 2;
        int num_chunks = (sfInfo.frames - N) / overlap + 1;

        std::vector<float> audio_data(sfInfo.frames);
        sf_readf_float(file, &audio_data[0], sfInfo.frames);
        sf_close(file);

        cv::Mat spectrogram(N/2, num_chunks, CV_32F); // N/2 because we're only interested in positive frequencies

        kiss_fft_cfg cfg = kiss_fft_alloc(N, 0, NULL, NULL);

        for (int k = 0; k < num_chunks; k++) {
            kiss_fft_cpx in[N], out[N];

            for (int i = 0; i < N; i++) {
                in[i].r = audio_data[k*overlap + i];
                in[i].i = 0;
            }

            kiss_fft(cfg, in, out);

            for (int i = 0; i < N/2; i++) {
                float magnitude = sqrt(out[i].r * out[i].r + out[i].i * out[i].i);
                //spectrogram.at<float>(i, k) = magnitude;
                spectrogram.at<float>(N/2-i-1, num_chunks-k-1) = magnitude;

            }
            emit progressChanged(((k+1) * 90) / (num_chunks));
        }
        audio_data.clear();
        free(cfg);

        // Normalize and visualize
        cv::Mat normalized, grayscale;
        const int targetWidth = 1900;   // Desired width
        const int targetHeight = 900;  // Desired height

        cv::Mat resizedSpectrogram;
        cv::resize(spectrogram, resizedSpectrogram, cv::Size(targetWidth, targetHeight));

        cv::normalize(resizedSpectrogram, normalized, 0.0, 1.0, cv::NORM_MINMAX, CV_32F);
        normalized.convertTo(grayscale, CV_8U, 255);
        cv::Mat coloredSpectrogram;
        cv::applyColorMap(grayscale, coloredSpectrogram, cv::COLORMAP_JET);
        emit progressChanged(100);
        emit resultReady(coloredSpectrogram);
        emit finished();
}
