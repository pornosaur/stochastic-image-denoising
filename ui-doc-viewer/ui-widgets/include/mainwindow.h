//
// Created by pornosaur on 1.4.19.
//

#ifndef DOC_VIEWER_MAINWINDOW_H
#define DOC_VIEWER_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDataWidgetMapper>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QProgressBar>
#include <QLabel>

#include <controllers/include/toolbox_controller.h>
#include <controllers/include/image_controller.h>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow() override;


private:
    Ui::MainWindow *ui;

    /* Controllers */
    qcontroller::ToolBoxController *toolbox_controller;
    qcontroller::ImageController *image_controller;

    QProgressBar *progress_bar;
    QLabel *progress_info;

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void load_window(bool checked);

    void save_image();

};

#endif //DOC_VIEWER_MAINWINDOW_H
