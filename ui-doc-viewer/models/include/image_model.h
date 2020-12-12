//
// Created by Patrik Patera on 28/11/2019.
//

#ifndef STOCHASTIC_IMAGE_DENOISING_IMAGE_MODEL_H
#define STOCHASTIC_IMAGE_DENOISING_IMAGE_MODEL_H

#include <QObject>
#include <QImage>
#include <image_denoising.h>
#include <QtCore/QMap>

namespace qmodel {

    class ImageModel : public QObject {
    Q_OBJECT

    private:
        QString m_image_internal_uuid;
        QString m_img_path, m_img_name, m_img_format;

        QImage m_img;



    public:
        explicit ImageModel(QString img_path);
        ImageModel(const QImage &image);

        /**
         * Saving an image to a file.
         * @param img_path path to an image.
         * @return true - image was saved, otherwise not.
         */
        bool save_image(const QString &img_path = "");

        inline QString get_internal_id() const { return m_image_internal_uuid; }

        inline bool is_loaded() const { return !m_img.isNull(); }

        inline QString get_name() const { return m_img_name; }

        inline QImage &get_image() { return m_img; }

        inline bool is_path_empty() const {return m_img_path.isEmpty(); }

        static cv::Mat convert_to_mat(const QImage &img);

        static QImage mat_to_qimage(const cv::Mat &mat);

         static QImage mat_to_qimage2(const cv::Mat &inMat);
    };


    class ImageModelList {
    private:
        ImageModel *m_actual_model;

        QMap<QString, ImageModel> m_image_map;

    public:
    };

}

#endif //STOCHASTIC_IMAGE_DENOISING_IMAGE_MODEL_H
