#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_historyModel = new QStringListModel(this);
//    m_historyModel->setStringList(some);

    ui->gesturesHistory->setModel(m_historyModel);
    ui->gesturesHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QObject::connect(ui->drawArea, SIGNAL(gestureCaptured(QVector<QPoint>)),
                     &m_ghandler, SLOT(processGesture(QVector<QPoint>)));
    QObject::connect(&m_ghandler, SIGNAL(newGestureRegistered(QString)),
                     this, SLOT(addHistoryEntry(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_historyModel;
}

void MainWindow::addHistoryEntry(QString entry)
{
    m_historyModel->insertRow(m_historyModel->rowCount());
    QModelIndex index = m_historyModel->index(m_historyModel->rowCount()-1);
    m_historyModel->setData(index, entry);
}

void MainWindow::on_quitButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_clearHistoryButton_clicked()
{
    m_historyModel->setStringList(QStringList());
}
