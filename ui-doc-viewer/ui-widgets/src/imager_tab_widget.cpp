//
// Created by Patrik Patera on 28/11/2019.
//

#include <image_tab_widget.h>
#include <QMessageBox>
#include <iostream>
#include <QtWidgets/QFileDialog>

qview::ImageTabViewWidget::ImageTabViewWidget(QWidget *parent) : QTabWidget(parent) {
    setUsesScrollButtons(true);
    setTabsClosable(true);
    setMovable(true);
    setDocumentMode(true);

    connect(this, &ImageTabViewWidget::tabCloseRequested, this, [this](int ix) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Quit", "Saving image while closing tab?",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        auto *tab = (ImageTabWidget *) widget(ix);
        if (tab) {
            if (reply == QMessageBox::Yes) {
                QString path = "";
                if (tab->is_path_empty()) {
                    path = QFileDialog::getSaveFileName(this, tr("Save image"));
                }

                tab->save_image(path);

                emit request_close_image(tab->get_image_uuid());
                removeTab(ix);
            }
            else if (reply == QMessageBox::No) {
                emit request_close_image(tab->get_image_uuid());
                removeTab(ix);
            }
        }
    });
}

QImage qview::ImageTabViewWidget::get_current_image() {
    auto *tab = (ImageTabWidget *) currentWidget();
    if (tab) return tab->get_image_mode()->get_image();

    return QImage();
}

qmodel::ImageModel* qview::ImageTabViewWidget::get_current_image_model() {
    auto *tab = (ImageTabWidget *) currentWidget();
    if (tab) return tab->get_image_mode();

    return nullptr;
}

void qview::ImageTabViewWidget::create_new_tab(qmodel::ImageModel *image_model) {
    auto *tab = (ImageTabWidget *) currentWidget();
    if (tab) tab->clear_selection();

    auto *new_tab = new qview::ImageTabWidget(image_model, this);
    int idx = addTab(new_tab, image_model->get_name());

    setCurrentIndex(idx);
    setTabEnabled(idx, true);
}
//------------------------------------------------------------

//-----------------IMAGE TAB WIDGET---------------------------
qview::ImageTabWidget::ImageTabWidget(qmodel::ImageModel *image_model, QWidget *parent, Qt::WindowFlags f)
        : QWidget(parent, f), m_image_model(image_model) {

    setObjectName(QStringLiteral("image_tab"));

    vertical_layout = new QHBoxLayout(this);
    vertical_layout->setSpacing(0);
    vertical_layout->setObjectName(QStringLiteral("vertical_layout_tab"));
    vertical_layout->setContentsMargins(0, 0, 0, 0);


    scroll_area_doc = new QScrollArea(this);
    scroll_area_doc->setObjectName(QStringLiteral("scroll_area_doc"));
    scroll_area_doc->setWidgetResizable(true);

    scroll_area_doc_content = new QWidget(scroll_area_doc);
    scroll_area_doc_content->setObjectName(QStringLiteral("scroll_area_doc_content"));

    vertical_layout_content = new QHBoxLayout(scroll_area_doc_content);
    vertical_layout_content->setSpacing(0);
    vertical_layout_content->setObjectName(QStringLiteral("vertical_layout_content"));
    vertical_layout_content->setContentsMargins(0, 0, 0, 0);

    image_renderer = new ImageRenderer(scroll_area_doc_content);
    image_renderer->setObjectName(QStringLiteral("graphics_view_image_renderer"));
    image_renderer->set_image_pixmap(m_image_model->get_image());


    vertical_layout_content->addWidget(image_renderer, Qt::AlignHCenter);

    scroll_area_doc->setWidget(scroll_area_doc_content);
    vertical_layout->addWidget(scroll_area_doc, Qt::AlignHCenter);
}

qview::ImageTabWidget::~ImageTabWidget() {
    //TODO: DISCONNECT SLOTS_SIGMALS HERE

    delete image_renderer;
}
