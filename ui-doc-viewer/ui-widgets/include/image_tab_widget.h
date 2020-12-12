//
// Created by Patrik Patera on 28/11/2019.
//

#ifndef STOCHASTIC_IMAGE_DENOISING_IMAGE_TAB_WIDGET_H
#define STOCHASTIC_IMAGE_DENOISING_IMAGE_TAB_WIDGET_H

#include <QTabWidget>
#include <QApplication>
#include <QBoxLayout>

#include <image_renderer.h>
#include <image_model.h>

namespace qview {

    // IMAGE TAB VIEW WIDGET CLASS
    class ImageTabViewWidget : public QTabWidget {
    Q_OBJECT

    public:
        explicit ImageTabViewWidget(QWidget *parent = nullptr);

        QImage get_current_image();

        qmodel::ImageModel *get_current_image_model();

    public slots:

        void create_new_tab(qmodel::ImageModel *image_model);
        //TODO: close tab, renedering image, get current tab,

    signals:

        void request_close_image(const QString &uuid);

        void changed_tab();

    };
    // END IMAGE TAB VIEW WIDGET


    // IMAGE TAB WIDGET
    class ImageTabWidget : public QWidget {
    Q_OBJECT

    private:
        QHBoxLayout *vertical_layout, *vertical_layout_content;
        QScrollArea *scroll_area_doc;
        QWidget *scroll_area_doc_content;

        ImageRenderer *image_renderer;
        qmodel::ImageModel *m_image_model;

    public:
        explicit ImageTabWidget(qmodel::ImageModel *image_model, QWidget *parent = nullptr,
                                Qt::WindowFlags f = Qt::WindowFlags());

        ~ImageTabWidget() override;

        inline void save_image(const QString &path) const { m_image_model->save_image(path); }

        inline bool is_path_empty() const { return m_image_model->is_path_empty(); }

        inline QString get_image_uuid() const { return m_image_model->get_internal_id(); }

        inline qmodel::ImageModel *get_image_mode() const { return m_image_model; }

        inline void clear_selection() { if (image_renderer->scene()) image_renderer->scene()->clearSelection(); }
    };

    // END IMAGE TAB WIDGET


}

#endif //STOCHASTIC_IMAGE_DENOISING_IMAGE_TAB_WIDGET_H
