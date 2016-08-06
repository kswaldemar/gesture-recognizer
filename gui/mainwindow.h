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
    /**
     * @brief Add new entry to gesture history list
     * @param desc gesture description
     */
    void addHistoryEntry(QString desc);

    void on_quitButton_clicked();

    void on_clearHistoryButton_clicked();

private:
    Ui::MainWindow *ui;

    QStringListModel *m_historyModel;
    UIInteractor m_ghandler;
};
