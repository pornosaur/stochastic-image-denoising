//
// Created by Patrik Patera on 28/11/2019.
//

#include "image_model.h"

#include <utility>
#include <QUuid>
#include <QFileInfo>

cv::Mat qmodel::ImageModel::convert_to_mat(const QImage &img) {
    cv::Mat mat;
    switch (img.format()) {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32_Premultiplied:
            mat = cv::Mat(img.height(), img.width(), CV_8UC4, (void *) img.constBits(), img.bytesPerLine());
            cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
            break;
        case QImage::Format_RGB888:
            mat = cv::Mat(img.height(), img.width(), CV_8UC3, (void *) img.constBits(), img.bytesPerLine());
            cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
            break;
        case QImage::Format_Grayscale8:
            mat = cv::Mat(img.height(), img.width(), CV_8UC1, (void *) img.constBits(), img.bytesPerLine());
            break;
        case QImage::Format_RGB16:
            QImage _img = img.convertToFormat(QImage::Format_RGB888);
            mat = cv::Mat(_img.height(), _img.width(), CV_8UC3, (void *) _img.constBits(), _img.bytesPerLine());
            cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
            break;
    }

    return mat;
}

qmodel::ImageModel::ImageModel(QString img_path) : m_img_path(std::move(img_path)) {
    m_image_internal_uuid = QUuid::createUuid().toString();

    QFileInfo info(m_img_path);
    m_img_name = info.baseName();
    m_img_format = info.suffix();

    m_img.load(m_img_path);
}

bool qmodel::ImageModel::save_image(const QString &img_path) {
    if (img_path.isEmpty() && m_img_path.isEmpty()) return false;

    QString path = img_path.isEmpty() ? m_img_path : img_path;

    QFileInfo in(path);
    if (in.suffix().isEmpty()) path.append(".png");
    m_img.save(path);

    return true;
}

QImage qmodel::ImageModel::mat_to_qimage(const cv::Mat &mat) {
    return QImage((uchar *) mat.data, mat.cols, mat.rows, static_cast<int>(mat.step),
                  QImage::Format_RGB888).rgbSwapped();
}

QImage qmodel::ImageModel::mat_to_qimage2(const cv::Mat &inMat) {
    switch (inMat.type()) {
        // 8-bit, 4 channel
        case CV_8UC4: {
            return QImage(inMat.data,
                          inMat.cols, inMat.rows,
                          static_cast<int>(inMat.step),
                          QImage::Format_ARGB32);

        }

            // 8-bit, 3 channel
        case CV_8UC3: {
            return QImage(inMat.data,
                          inMat.cols, inMat.rows,
                          static_cast<int>(inMat.step),
                          QImage::Format_RGB888);

        }

            // 8-bit, 1 channel
        case CV_8UC1: {
            cv::Mat m = inMat.clone();
            return QImage(reinterpret_cast<const uchar *>(m.data),
                          inMat.cols, inMat.rows,
                          static_cast<int>(inMat.step),
                          QImage::Format_Grayscale8);
        }

        default:
            std::cout << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
    }

    return QImage();
}

qmodel::ImageModel::ImageModel(const QImage &image) : m_img(image) {
    m_image_internal_uuid = QUuid::createUuid().toString();
    m_img_name = "denoised_image";

}
