//
// Created by Patrik Patera on 28/11/2019.
//

#ifndef STOCHASTIC_IMAGE_DENOISING_IMAGE_RENDERER_H
#define STOCHASTIC_IMAGE_DENOISING_IMAGE_RENDERER_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QUuid>

namespace qview {

    class ImageRenderer : public QGraphicsView {
        Q_OBJECT

    private:
        QString m_uuid;

        QGraphicsPixmapItem *m_doc_pixmap;
        QGraphicsScene *m_scene;

    public:
        explicit ImageRenderer(QWidget *parent = nullptr);

    public slots:
        void set_image_pixmap(const QImage &image);
    };

}

#endif //STOCHASTIC_IMAGE_DENOISING_IMAGE_RENDERER_H
