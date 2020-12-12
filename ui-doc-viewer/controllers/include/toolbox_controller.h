//
// Created by pornosaur on 3.8.19.
//

#ifndef DOC_VIEWER_TOOLBOX_CONTROLLER_H
#define DOC_VIEWER_TOOLBOX_CONTROLLER_H

#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QProgressBar>
#include <QLabel>

#include <image_tab_widget.h>

namespace qcontroller {

    class ToolBoxController : public QObject {
    Q_OBJECT

    private:
        QPushButton *m_button_denoise;

        qview::ImageTabViewWidget *m_image_tab_view;
        QSlider *m_slider_sigma, *m_slider_samples;
        QProgressBar *m_progress;
        QLabel *m_info;


    public:
        explicit ToolBoxController(QPushButton *button_denoise, qview::ImageTabViewWidget *img_widget,
                                   QSlider *slider_sigma, QSlider *slider_samples, QProgressBar *bar, QLabel *info);

    public slots:

        void button_denoise_event(bool checked = false);

        void set_new_value(double p);

    signals:

        void notify_new_image(const QImage &image);
        void update_progress(int p);
    };

}

#endif //DOC_VIEWER_TOOLBOX_CONTROLLER_H
