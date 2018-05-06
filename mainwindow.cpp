#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    move((QApplication::desktop()->width()-width())/2, (QApplication::desktop()->height()-height())/2);
    connect(ui->action_quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->textEdit,SIGNAL(selectionChanged()),this,SLOT(selectionChange()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_open_triggered()
{
    if (path == "") {
        path = QFileDialog::getOpenFileName(this, "打开文本", ".");
    } else {
        path = QFileDialog::getOpenFileName(this, "打开文本", path);
    }
    if (!path.isEmpty()) {
        open(path);
    }
}

void MainWindow::open(QString filename)
{
    qDebug() << "open" << filename;
    path = filename;
    ui->statusBar->showMessage("打开 " + filename);
    setWindowTitle("HTYHex - " + QFileInfo(filename).fileName());
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QString s = "";
    while (!file.atEnd()) {
        s = s + file.read(1).toHex().toUpper() + " ";
        //ui->statusBar->showMessage("读取 "+ QString::number(file.pos()) + "/" + QString::number(file.size()));
    }
    file.close();
    ui->textEdit->setText(s);
    ui->statusBar->showMessage("读取完毕");
}

void MainWindow::on_action_save_triggered()
{
    if (path == "") {
        on_action_saveAs_triggered();
    } else {
        QFile file(path);
        if (file.open(QFile::WriteOnly)) {
            QStringList SL = ui->textEdit->toPlainText().trimmed().split(" ");
            QByteArray BA = "";
            bool ok;
            for(qint64 i=0; i<SL.size(); i++){
                BA += SL.at(i).toInt(&ok,16);
            }
            file.write(BA);
            file.close();
            setWindowTitle("HTYHex - " + QFileInfo(path).fileName() + "[*]");
            setWindowModified(false);
            ui->statusBar->showMessage("保存 " + path);
        }else{
            QMessageBox::warning(this,"错误", QString(" %1:\n%2").arg(path).arg(file.errorString()));
        }
    }
}

void MainWindow::on_action_saveAs_triggered()
{
    if (path == "") {
        path = QFileDialog::getSaveFileName(this, "保存", "./未命名");
    } else {
        path = QFileDialog::getSaveFileName(this, "保存", path);
    }
    if (!path.isEmpty()) {
        on_action_save_triggered();
    }
}

void MainWindow::on_action_about_triggered()
{
    QMessageBox aboutMB(QMessageBox::NoIcon, "关于", "海天鹰16进制编辑器 1.0\n\n一款基于 Qt 的 16进制编辑程序。\n作者：黄颖\nE-mail: sonichy@163.com\n主页：https://github.com/sonichy");
    aboutMB.setIconPixmap(QPixmap(":/icon.png"));
    aboutMB.setWindowIcon(QIcon(":/icon.png"));
    aboutMB.exec();
}

void MainWindow::on_actionReload_triggered()
{
    if (path != "") {
        open(path);
    }
}

void MainWindow::selectionChange()
{
    QString s = ui->textEdit->textCursor().selectedText().trimmed();
    if (s != "") {
        QStringList SL = s.split(" ");
        ui->statusBar->showMessage("选中 " + QString::number(SL.size()) + "个字节");
    }
}
