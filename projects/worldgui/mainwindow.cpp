#include <iostream>

#include <QtGui/QScreen>
#include <Qt3DExtras/qt3dwindow.h>
#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DExtras/qorbitcameracontroller.h>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/qphongmaterial.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qcamera.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "panelterrain.h"
#include "panelworldmap.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    swapGeneratePanel(new PanelWorldMap());
    ui->_3DTab->layout()->replaceWidget(ui->_3DPanel, _3DPanel.getWidget());

    QObject::connect(ui->terrainButton, SIGNAL(clicked(bool)), this,
                     SLOT(changeGeneratePanel()));
    QObject::connect(ui->mapButton, SIGNAL(clicked(bool)), this,
                     SLOT(changeGeneratePanel()));
    // Géneration d'objets
    QObject::connect(ui->generateButton, SIGNAL(clicked(bool)), this,
                     SLOT(generate()));
}

MainWindow::~MainWindow() {
    delete generatePanel;
    delete ui;
}

void MainWindow::setScene(const world::Scene *objects) {
    _3DPanel.setScene(objects);
}

void MainWindow::setImage(const QImage *image) { // TODO check s'il y a pas
                                                 // memory leak sur cette QImage
    if (this->ui->imagePanel->scene() == nullptr) {
        this->ui->imagePanel->setScene(new QGraphicsScene(this));
    }

    QGraphicsScene *scene = this->ui->imagePanel->scene();
    scene->clear();
    scene->addPixmap(QPixmap::fromImage(*image));
    scene->setSceneRect(image->rect());
}

void MainWindow::generate() { generatePanel->generate(); }

void MainWindow::changeGeneratePanel() {
    auto sender = QObject::sender();

    if (sender == this->ui->terrainButton) {
        swapGeneratePanel(new PanelTerrain());
    } else if (sender == this->ui->mapButton) {
        swapGeneratePanel(new PanelWorldMap());
    }
}

void MainWindow::swapGeneratePanel(GeneratePanel *newPanel) {
    // Remplacement
    if (generatePanel != nullptr) {
        if (newPanel != nullptr) {
            ui->generateTab->layout()->replaceWidget(generatePanel, newPanel);
        } else {
            ui->generateTab->layout()->replaceWidget(generatePanel,
                                                     ui->generatePanel);
        }

        generatePanel->disconnect();
        delete generatePanel;
    } else if (newPanel != nullptr) {
        ui->generateTab->layout()->replaceWidget(ui->generatePanel, newPanel);
    }

    generatePanel = newPanel;

    if (newPanel != nullptr) {
        // Connexion signal / slot
        // Changement d'objets 3D
        QObject::connect(newPanel, SIGNAL(meshesChanged(const world::Scene *)),
                         this, SLOT(setScene(const world::Scene *)));
        // Changement d'image
        QObject::connect(newPanel, SIGNAL(imageChanged(const QImage *)), this,
                         SLOT(setImage(const QImage *)));
    }
}
