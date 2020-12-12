//
// Created by Patrik Patera on 28/11/2019.
//

#include <QStyle>

#include <image_renderer.h>

qview::ImageRenderer::ImageRenderer(QWidget *parent) : QGraphicsView(parent) {
    setMouseTracking(true);
    setStyleSheet("background-color: rgba(255, 255, 255, 0.1);");

    m_scene = new QGraphicsScene(this);
    setScene(m_scene);

    m_doc_pixmap = new QGraphicsPixmapItem();
    m_scene->addItem(m_doc_pixmap);

    m_uuid = ""; //TODO: Generate internal ID! DELETE IT
}

void qview::ImageRenderer::set_image_pixmap(const QImage &image) {
    m_doc_pixmap->setPixmap(QPixmap::fromImage(image));

    setGeometry(0, 0, image.width(), image.height());
    m_scene->setSceneRect(0, 0, image.width(), image.height());
    setFrameStyle(QFrame::NoFrame);

    setMaximumWidth(image.width() + parentWidget()->style()->pixelMetric(QStyle::PM_ScrollBarExtent));
    setMinimumWidth(0);
}

