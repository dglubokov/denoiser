#include "denoisermain.h"
#include "ui_denoisermain.h"
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <opencv2/opencv.hpp>
#include <opencv2/quality.hpp>
#include <opencv2/xphoto.hpp>
#include <chrono>

DenoiserMain::DenoiserMain(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DenoiserMain)
{
    ui->setupUi(this);
}

DenoiserMain::~DenoiserMain()
{
    delete ui;
}


QImage mat_to_qimage_ref(cv::Mat &mat, QImage::Format format)
{
    return QImage(mat.data, mat.cols, mat.rows,
                  static_cast<int>(mat.step), format);
}

/*
    Преобразование cv::Mat в QMatrix
*/
QImage mat_to_qimage(cv::Mat &mat, bool swap)
{
    if(!mat.empty()){
        switch(mat.type()){

        case CV_8UC3 :{
            if(swap){
                return mat_to_qimage_ref(mat, QImage::Format_RGB888).rgbSwapped();
            }else{
                return mat_to_qimage_ref(mat, QImage::Format_RGB888);
            }
        }

        case CV_8U :{
            return mat_to_qimage_ref(mat, QImage::Format_Indexed8);
        }

        case CV_8UC4 :{
           return mat_to_qimage_ref(mat, QImage::Format_ARGB32);
        }

        }
    }

    return {};
}


void DenoiserMain::on_pushButton_clicked()
{
    this->file_name = QFileDialog::getOpenFileName(
        this,
        tr("Open image"),
        "/",
        tr("Image Files (*.png *.jpg *.bmp)")
    );

    QImage image;
    bool valid = image.load(this->file_name);

    if (valid) {
        ui->pic_opened->setPixmap(QPixmap::fromImage(image));
        this->cv_image = cv::imread(this->file_name.toStdString());
        this->noisy_cv_image = cv_image.clone();
    }
}

/*
    Оценка качества изображения.
*/
void DenoiserMain::estimate(cv::Mat &ref, cv::Mat &cmp, int duration){

    // PSNR
    cv::quality::QualityPSNR psnr_est = *cv::quality::QualityPSNR::create(ref);
    QString psnr_est_eval = QString::number(psnr_est.compute(cmp)[0]);
    ui->label_psnr->setText("PSNR: " + psnr_est_eval);

    // SSIM
    cv::quality::QualitySSIM ssim_est = *cv::quality::QualitySSIM::create(ref);
    QString ssim_est_eval = QString::number(ssim_est.compute(cmp)[0]);
    ui->label_ssim->setText("SSIM: " + ssim_est_eval);

    // Speed
    QString speed_eval = QString::number(duration);
    ui->label_speed->setText("Speed: " + speed_eval + "ms");
}

void DenoiserMain::on_comboBox_activated(const QString &arg1)
{
    cv::Mat denoised_image;
    // medianfilter
    if (arg1 == "median filter") {
        auto start = std::chrono::high_resolution_clock::now();
        cv::medianBlur(this->noisy_cv_image, denoised_image, 7);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        this->estimate(this->cv_image, denoised_image, duration.count());
        QImage img  = mat_to_qimage(denoised_image , 1);
        ui->pic_denoised->setPixmap(QPixmap::fromImage(img));
    }
    // bilateral filter
    else if (arg1 == "bilateral filter"){
        auto start = std::chrono::high_resolution_clock::now();

        cv::bilateralFilter(this->noisy_cv_image, denoised_image, 15, 80, 80);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        this->estimate(this->cv_image, denoised_image, duration.count());
        QImage img  = mat_to_qimage(denoised_image , 1);
        ui->pic_denoised->setPixmap(QPixmap::fromImage(img));
    }
    // non-local means filter
    else if (arg1 == "non-local means"){
        auto start = std::chrono::high_resolution_clock::now();

        cv::fastNlMeansDenoising(this->noisy_cv_image, denoised_image, 10);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        this->estimate(this->cv_image, denoised_image, duration.count());
        QImage img  = mat_to_qimage(denoised_image , 1);
        ui->pic_denoised->setPixmap(QPixmap::fromImage(img));
    }
    // BM3D
    else if (arg1 == "BM3D"){
        auto start = std::chrono::high_resolution_clock::now();
        cv::Mat channels[3];
        cv::Mat denoised_channels[3];

        split(this->noisy_cv_image, channels);

        cv::xphoto::bm3dDenoising(channels[0], denoised_channels[0], 10);
        cv::xphoto::bm3dDenoising(channels[1], denoised_channels[1], 10);
        cv::xphoto::bm3dDenoising(channels[2], denoised_channels[2], 10);
        cv::merge(denoised_channels, 3, denoised_image);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        this->estimate(this->cv_image, denoised_image, duration.count());

        QImage img  = mat_to_qimage(denoised_image , 1);
        ui->pic_denoised->setPixmap(QPixmap::fromImage(img));
    }
    // TODO
    // Weiner
    // wavelet
    // TR
    // dct-noise Xphoto
    // ximgproc edgePreservingFilter
}

void DenoiserMain::on_button_add_noise_clicked()
{
    cv::Mat copy_m = this->noisy_cv_image.clone();
    cv::randn(copy_m, 1,50);
    this->noisy_cv_image = this->noisy_cv_image + copy_m;
    QImage img  = mat_to_qimage(this->noisy_cv_image, 1);
    ui->pic_opened->setPixmap(QPixmap::fromImage(img));
}
