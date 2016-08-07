#pragma once

#include "uiinteractor.h"

#include <QMainWindow>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void addShapeToHistory(PShape desc);

    void updateHoughView(const QImage &image);

    void on_quitButton_clicked();

    void on_clearHistoryButton_clicked();


private:
    Ui::MainWindow *ui;

    QStringListModel *m_historyModel;
    UIInteractor m_ghandler;
};
