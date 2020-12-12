//
// Created by Patrik Patera on 28/11/2019.
//

#include "image_denoising.h"

#include <utility>
#include <random>
#include <tbb/tbb.h>

const double ImageDenoising::RW_THRESHOLD = 1e-6f;

const double ImageDenoising::RW_THRESHOLD_LOG = std::log(RW_THRESHOLD);

const int ImageDenoising::KERNEL_POS_X[8] = {-1, 0, 1, 1, 1, 0, -1, -1};

const int ImageDenoising::KERNEL_POS_Y[8] = {-1, -1, -1, 0, 1, 1, 1, 0};


cv::Mat ImageDenoising::run_denoising(const cv::Mat &image, double dev, int samples, callback_progess progress) {
    cv::Mat denoised_image;
    image.copyTo(denoised_image);

    double bit_depth = 8;
    switch (image.depth()) {
        case CV_8U:
            bit_depth = 8;
            break;
        case CV_16U:
            bit_depth = 16;
            break;
        case CV_32F:
            bit_depth = 32;
            break;
        case CV_64F:
            bit_depth = 64;
            break;
    }

    // Compute normalization constant.
    double K = 1. / ((std::pow(2, bit_depth) - 1) * (std::pow(2, bit_depth) - 1));

    dev = 2.0 * dev * dev;
    _run_denoising(image, denoised_image, K, dev, samples, progress);

    return denoised_image;
}

void ImageDenoising::_run_denoising(const cv::Mat &image, cv::Mat &result_image, double K, double dev, int samples,
                                    callback_progess &progress) {
    int final = 0;
    cv::Mat M = cv::Mat(image.rows, image.cols, CV_64FC1);
    uchar *input = (uchar *) (image.data);

    tbb::parallel_for(size_t(0), (size_t) image.rows, [&](size_t i){
            final++;
            if (progress) progress((double) final / image.rows * 100);

            for (int j = 0; j < image.cols; ++j) {
                int center_x = j, center_y = i;

                cv::Point x_0(center_x, center_y);
                std::vector<cv::Point> walks_path;

                double cumulative_weights = 0.;
                for (int i1 = 0; i1 < samples; ++i1)
                    cumulative_weights += random_walk(x_0, walks_path, M, image, K, dev);

                double px[3] = {0., 0., 0.};
                for (auto &e : walks_path) {
                    double *m_inp = M.ptr<double>(e.y);
                    if (image.channels() == 1) {
                        px[0] += (double) m_inp[e.x] * ((int) input[image.step * e.y + e.x * image.channels()])
                                 / cumulative_weights;
                    } else if (image.channels() == 3) {
                        px[0] += (double) m_inp[e.x] * ((int) input[image.step * e.y + e.x * image.channels()])
                                 / cumulative_weights;
                        px[1] += (double) m_inp[e.x] * ((int) input[image.step * e.y + e.x * image.channels() + 1])
                                 / cumulative_weights;
                        px[2] += (double) m_inp[e.x] * ((int) input[image.step * e.y + e.x * image.channels() + 2])
                                 / cumulative_weights;
                    }

                    m_inp[e.x] = 0.;
                }

                if (image.channels() == 1) {
                    result_image.at<uchar>(x_0) = px[0];
                } else if (image.channels() == 3) {
                    result_image.at<cv::Vec3b>(x_0)[0] = px[0];
                    result_image.at<cv::Vec3b>(x_0)[1] = px[1];
                    result_image.at<cv::Vec3b>(x_0)[2] = px[2];
                }
            }

    });
}

double ImageDenoising::random_walk(const cv::Point &x_0, std::vector<cv::Point> &path, cv::Mat &M, const cv::Mat &image,
                                   double K, double dev) {

    double actual_prob_val = 0.;
    double cumulative_weight = 0.;
    cv::Point x_i = x_0;

    for (size_t i = 0; i < size_t(10e3); ++i) {     // This MAX value prevents an infinity loop.
        auto prob = compute_kernel_prob(x_0, x_i, image, K, dev);  // Compute probabilities by a kernel.
        int next_i = next_walk_step(x_i, prob); // Coordinates of new position in an image on the walk path.
        actual_prob_val += prob[next_i][1];

        if (actual_prob_val < RW_THRESHOLD_LOG) break;

        double w_i = std::exp(actual_prob_val / i); // Using geometric assumption.
        cumulative_weight += w_i;

        M.at<double>(x_i) += w_i;

        path.emplace_back(x_i);
    }

    return cumulative_weight;
}

std::vector<vec2d>
ImageDenoising::compute_kernel_prob(const cv::Point &x_0, const cv::Point &x_i, const cv::Mat &image, double K,
                                    double dev) {
    std::vector<vec2d> prob(8, {0., 0.});

    double cumulative_prob = 0.;
    for (size_t i = 0; i < 8; ++i) {
        int pos_x = x_i.x + KERNEL_POS_X[i], pos_y = x_i.y + KERNEL_POS_Y[i];

        if (pos_x >= 0 && pos_x < image.cols && pos_y >= 0 && pos_y < image.rows)
            prob[i] = probability(x_0, x_i, {pos_x, pos_y}, image, K, dev);

        cumulative_prob += prob[i][0];
    }

    double log_cumulative_prob = std::log(cumulative_prob);
    for (size_t i = 0; i < 8; ++i) {
        prob[i][0] /= cumulative_prob;
        prob[i][1] -= log_cumulative_prob;
    }

    return std::move(prob);
}

int ImageDenoising::next_walk_step(cv::Point &x_i, const std::vector<vec2d> &prob) {
    static std::mt19937_64 mt;
    static std::uniform_real_distribution urd(0., 1.);

    double rand = urd(mt);
    double cumulative_prob = 0.;

    // Choosing next pixel in the walk.
    size_t i = 0, j = 0;
    for (; i < prob.size(); ++i) {
        cumulative_prob += prob[i][0];
        if (prob[i][0] > 0.) j = i;
        if (rand <= cumulative_prob) break;
    }

    if (i == prob.size()) i = j;

    x_i.x = x_i.x + KERNEL_POS_X[i];
    x_i.y = x_i.y + KERNEL_POS_Y[i];

    return i;
}

vec2d
ImageDenoising::probability(const cv::Point &x_0, const cv::Point &x_j, const cv::Point &x_j_1, const cv::Mat &image,
                            double K, double dev) {
    const double exp1_frac = (-distance(x_0, x_j_1, image)) / (dev) * K;
    const double exp1 = std::exp(exp1_frac);
    const double exp2_frac = (-distance(x_j, x_j_1, image)) / (dev) * K;
    const double exp2 = std::exp(exp2_frac);

    return {exp1 * exp2, exp1_frac + exp2_frac};
}

double ImageDenoising::distance(const cv::Point &x1, const cv::Point &x2, const cv::Mat &image) {
    double distance = 0.;

    // Compute Euclidean distance according to image channels - colorful or grayscale.
    if (image.channels() == 1) {
        distance = ((int) image.at<uchar>(x2) - (int) image.at<uchar>(x1)) *
                   ((int) image.at<uchar>(x2) - (int) image.at<uchar>(x1));
    } else if (image.channels() == 3) {
        distance = cv::norm(image.at<cv::Vec3b>(x1), image.at<cv::Vec3b>(x2), cv::NORM_L2SQR);
    }

    return distance;
}