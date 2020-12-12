//
// Created by pornosaur on 1.4.19.
//

#include <iostream>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);


    progress_bar = new QProgressBar(this);
    progress_bar->setMaximumSize(150, 20);

    progress_info = new QLabel(this);

    ui->statusbar_info->addWidget(progress_bar);
    ui->statusbar_info->addWidget(progress_info);

    toolbox_controller = new qcontroller::ToolBoxController(ui->button_denoise, ui->tab_widget_img,
                                                            ui->slider_dev, ui->slider_samples, progress_bar,
                                                            progress_info);
    image_controller = new qcontroller::ImageController(ui->tab_widget_img);


    connect(ui->action_load, &QAction::triggered, this, &MainWindow::load_window);
    connect(ui->action_save_as, &QAction::triggered, this, &MainWindow::save_image);
    connect(toolbox_controller, &qcontroller::ToolBoxController::notify_new_image, image_controller,
            QOverload<const QImage &>::of(&qcontroller::ImageController::create_new_image));

}

MainWindow::~MainWindow() {
    delete toolbox_controller;
    delete image_controller;
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QMainWindow::keyPressEvent(event);
}

void MainWindow::load_window(bool checked) {
    const QString file_path = QFileDialog::getOpenFileName(this, tr("Choose image file"), "/home/patpa/Pictures");
    if (file_path.isEmpty()) return;

    image_controller->create_new_image(file_path);
}

void MainWindow::save_image() {
    const QString path = QFileDialog::getSaveFileName(this, tr("Save image"));
    image_controller->save_current_image(path);
}
