#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "guihandler.h"

#include <QMainWindow>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void addHistoryEntry(QString);

    void on_quitButton_clicked();

    void on_clearHistoryButton_clicked();

private:
    Ui::MainWindow *ui;

    QStringListModel *m_historyModel;
    GuiHandler m_ghandler;
};

#endif // MAINWINDOW_H
