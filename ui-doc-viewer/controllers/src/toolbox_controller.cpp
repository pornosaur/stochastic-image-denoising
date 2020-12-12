//
// Created by Patrik Patera on 28/11/2019.
//

#include <toolbox_controller.h>
#include <thread>

qcontroller::ToolBoxController::ToolBoxController(QPushButton *button_denoise, qview::ImageTabViewWidget *img_widget,
                                                  QSlider *slider_sigma, QSlider *slider_samples, QProgressBar *bar,
                                                  QLabel *info)
        : m_button_denoise(button_denoise), m_image_tab_view(img_widget), m_slider_sigma(slider_sigma),
          m_slider_samples(slider_samples), m_progress(bar), m_info(info) {

    connect(m_button_denoise, &QPushButton::clicked, this, &ToolBoxController::button_denoise_event);
    connect(this, &ToolBoxController::update_progress, m_progress, &QProgressBar::setValue);
}

void qcontroller::ToolBoxController::set_new_value(double p) {
    emit update_progress((int) p);
}

void qcontroller::ToolBoxController::button_denoise_event(bool checked) {
    const QImage img = m_image_tab_view->get_current_image();
    if (img.isNull()) return;

    m_button_denoise->setEnabled(false);

    const double dev = m_slider_sigma->value() / 100.0;
    const int samples = m_slider_samples->value();

    m_progress->setRange(0, 100);
    m_progress->setMaximum(100);
    m_progress->setValue(50);
    m_info->setText("Denoising in progress...");

    auto denoising_thread = std::thread([=]() {
        cv::Mat mat = qmodel::ImageModel::convert_to_mat(img);

        cv::Mat denoised_img = ImageDenoising::run_denoising(mat, dev, samples, [this](double p) {
           emit update_progress((int) p);
        });
        QImage output = qmodel::ImageModel::mat_to_qimage2(denoised_img);

        if (output.isNull()) {
            m_button_denoise->setEnabled(true);
            m_info->setText("Problem while denoising!");
            return;
        }

        emit notify_new_image(output);
        m_button_denoise->setEnabled(true);
        m_info->setText("Denoising was finished successfully!");
    });

    denoising_thread.detach();
};