#pragma once

#include "uiinteractor.h"

#include <QMainWindow>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

/**
 * @brief Main GUI window
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    /**
     * @brief Add new entry to history list
     * @param desc raw pointer to shape, function doesn't take ownership over it
     */
    void addShapeToHistory(PShape desc);
    /**
     * @brief Update label with hough matrix image
     * @param image hough parametric field representation
     */
    void updateHoughView(const QImage &image);

    /**
     * @brief Trigerred when quit button pressed.
     */
    void on_quitButton_clicked();
    /**
     * @brief Trgerred when clear history button pressed.
     */
    void on_clearHistoryButton_clicked();

private:

    Ui::MainWindow *ui;
    /// List of strings model to show history information
    QStringListModel *m_historyModel;
    /// Object to interact with recognition library
    UIInteractor m_ghandler;
};
