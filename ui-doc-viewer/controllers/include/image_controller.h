//
// Created by Patrik Patera on 28/11/2019.
//

#ifndef STOCHASTIC_IMAGE_DENOISING_IMAGE_CONTROLLER_H
#define STOCHASTIC_IMAGE_DENOISING_IMAGE_CONTROLLER_H

#include <QObject>
#include <QMap>

#include <image_model.h>
#include <image_tab_widget.h>


namespace qcontroller {

    class ImageController : public QObject {
    Q_OBJECT

    private:
        qview::ImageTabViewWidget *m_img_widget;

        QMap<QString, qmodel::ImageModel*> m_map_images;

    public:
        explicit ImageController(qview::ImageTabViewWidget *img_widget);

    public slots:
        void create_new_image(const QString &path);

        void create_new_image(const QImage &image);

        void close_image(const QString &img_uuid);

        void save_current_image(const QString &path);

    signals:
        void notify_new_image_tab(qmodel::ImageModel * img_model);

    };

}
#endif //STOCHASTIC_IMAGE_DENOISING_IMAGE_CONTROLLER_H
