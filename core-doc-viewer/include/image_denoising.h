//
// Created by Patrik Patera on 28/11/2019.
//

#ifndef STOCHASTIC_IMAGE_DENOISING_IMAGE_DENOISING_H
#define STOCHASTIC_IMAGE_DENOISING_IMAGE_DENOISING_H

#include <opencv2/opencv.hpp>

/** 2-dimensional vector */
using vec2d = std::array<double, 2>;

/** Prototype of callback function */
using callback_progess = std::function<void(double p)>;

/** Library class for image denoising */
class ImageDenoising {
private:
    /** Threshold values for weights (probability) */
    static const double RW_THRESHOLD, RW_THRESHOLD_LOG;

    /** Pre-defined 3x3 kernel with position values */
    static const int KERNEL_POS_X[8], KERNEL_POS_Y[8];

    /**
     * Process of random walk.
     *
     * @param x0 - start pixel.
     * @param path - the output sequence of the walk.
     * @param M - matrix of weights.
     * @param image - the input image.
     * @param K - number of samples in walk.
     * @param dev - deviation constant.
     * @return cumulative weight for appropriate walk.
     */
    static double
    random_walk(const cv::Point &x0, std::vector<cv::Point> &path, cv::Mat &M, const cv::Mat &image, double K,
                double dev);

    /**
     * Compute transition probabilities in neighbourhood using kernel.
     *
     * @param x_0 - start pixel.
     * @param x_i - actual pixel.
     * @param image - the input image.
     * @param K - number of samples in walk.
     * @param dev - deviation constant.
     * @return probabilities for nearby pixels in kernel.
     */
    static std::vector<vec2d>
    compute_kernel_prob(const cv::Point &x_0, const cv::Point &x_i, const cv::Mat &image, double K, double dev);

    /**
     * Choosing next pixel in random walk according to transition probabilities.
     *
     * @param x_i - next pixel.
     * @param prob - kernel with transition probabilities.
     * @return index of next pixel in the kernel.
     */
    static int next_walk_step(cv::Point &x_i, const std::vector<vec2d> &prob);

    /**
     * Computing transition probability between two pixels with the start one.
     *
     * @param x_0 - start pixel of the walk.
     * @param x_j - actual pixel.
     * @param x_j_1 - nearby pixel to the actual one.
     * @param image - the input image.
     * @param K - normalization constant.
     * @param dev - deviation constant.
     * @return transition probability between i and j pixel; [1] = log-prob.
     */
    static vec2d probability(const cv::Point &x_0, const cv::Point &x_j, const cv::Point &x_j_1, const cv::Mat &image,
                             double K, double dev);

    /**
     * Run stochastic image denoising.
     *
     * @param image - the input image.
     * @param result_image - a denoised image.
     * @param K - normalization constant.
     * @param dev - deviation constant.
     * @param samples - number of samples in a walk.
     * @param progress - callback function.
     */
    static void _run_denoising(const cv::Mat &image, cv::Mat &result_image, double K, double dev, int samples,
                               callback_progess &progress);

    /**
     * Computing Euclidean distance between two pixels.
     *
     * @param x1 - first pixel.
     * @param x2 - second pixel.
     * @param image - the input image.
     * @return value of distance.
     */
    static inline double distance(const cv::Point &x1, const cv::Point &x2, const cv::Mat &image);


public:
    ImageDenoising() = delete;

    /**
     * Run stochastic image denoising.
     *
     * @param image - the input image.
     * @param dev - deviation input.
     * @param samples - number of samples in a walk.
     * @param progress - callback function.
     * @return the final denoised image.
     */
    static cv::Mat run_denoising(const cv::Mat &image, double dev, int samples, callback_progess progress);
};

#endif //STOCHASTIC_IMAGE_DENOISING_IMAGE_DENOISING_H
