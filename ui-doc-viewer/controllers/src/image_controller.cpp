//
// Created by Patrik Patera on 28/11/2019.
//

#include <image_controller.h>

qcontroller::ImageController::ImageController(qview::ImageTabViewWidget *img_widget) : m_img_widget(img_widget) {
    connect(this, &ImageController::notify_new_image_tab, img_widget, &qview::ImageTabViewWidget::create_new_tab);
    connect(m_img_widget, &qview::ImageTabViewWidget::request_close_image, this, &ImageController::close_image);
}

void qcontroller::ImageController::create_new_image(const QString &path) {
    auto *image_model = new qmodel::ImageModel(path);

    if (image_model->is_loaded()) {
        m_map_images.insert(image_model->get_internal_id(), image_model);
        emit notify_new_image_tab(image_model);
    } else {
        delete image_model;
    }
}

void qcontroller::ImageController::create_new_image(const QImage &image) {
    auto *image_model = new qmodel::ImageModel(image);

    if (image_model->is_loaded()) {
        m_map_images.insert(image_model->get_internal_id(), image_model);
        emit notify_new_image_tab(image_model);
    } else {
        delete image_model;
    }
}

void qcontroller::ImageController::close_image(const QString &img_uuid) {
    auto *image_model = m_map_images.take(img_uuid);
    delete image_model;
}

void qcontroller::ImageController::save_current_image(const QString &path) {
   auto *image_model = m_img_widget->get_current_image_model();

   if (image_model) image_model->save_image(path);
}