#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uiinteractor.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    m_historyModel = new QStringListModel(this);

    ui->gesturesHistory->setModel(m_historyModel);
    ui->gesturesHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QObject::connect(ui->drawArea, SIGNAL(gestureCaptured(QVector<QPoint>)),
                     &m_ghandler, SLOT(processGesture(QVector<QPoint>)));
    QObject::connect(&m_ghandler, SIGNAL(newGestureRegistered(PShape)),
                     this, SLOT(addShapeToHistory(PShape)));
    QObject::connect(&m_ghandler, SIGNAL(newGestureRegistered(PShape)),
                     ui->drawArea, SLOT(setShape(PShape)));
    QObject::connect(&m_ghandler, SIGNAL(updateHoughTransformView(QImage)),
                     this, SLOT(updateHoughView(QImage)));
}

MainWindow::~MainWindow() {
    delete ui;
    delete m_historyModel;
}

void MainWindow::addShapeToHistory(PShape desc) {
    m_historyModel->insertRow(m_historyModel->rowCount());
    QModelIndex index = m_historyModel->index(m_historyModel->rowCount() - 1);
    m_historyModel->setData(index, desc->toString());
}

void MainWindow::updateHoughView(const QImage &image) {
    ui->houghView->setPixmap(QPixmap::fromImage(image));
    ui->houghView->update();
}

void MainWindow::on_quitButton_clicked() {
    QApplication::quit();
}

void MainWindow::on_clearHistoryButton_clicked() {
    m_historyModel->setStringList(QStringList());
}
