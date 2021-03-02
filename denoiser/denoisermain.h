#ifndef DENOISERMAIN_H
#define DENOISERMAIN_H

#include <QMainWindow>
#include <QMatrix>
#include <opencv2/core.hpp>
#include <chrono>

QT_BEGIN_NAMESPACE
namespace Ui { class DenoiserMain; }
QT_END_NAMESPACE

class DenoiserMain : public QMainWindow
{
    Q_OBJECT

public:
    QString file_name;
    cv::Mat cv_image;
    cv::Mat noisy_cv_image;
    void estimate(cv::Mat &ref, cv::Mat &cmp, int duration);
    DenoiserMain(QWidget *parent = nullptr);
    ~DenoiserMain();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_button_add_noise_clicked();

private:
    Ui::DenoiserMain *ui;
};
#endif // DENOISERMAIN_H
