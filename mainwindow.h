#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString path;
    void open(QString filename);

private slots:
    void on_action_open_triggered();
    void on_action_save_triggered();
    void on_action_saveAs_triggered();
    void on_action_about_triggered();
};

#endif // MAINWINDOW_H