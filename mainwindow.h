#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QDebug>
#include <QFontDatabase>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QScrollBar>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QString readLineOnText(int line);
private slots:
    void onSpinbox();
    void onComobox();
    void importFile();
    void getFileLineNumber();
    void jumpToSpecifiedLine();
    void goToNextPage();
private:
    Ui::MainWindow *ui;
    QFile* file;
    int lineNumber;
    int nowLine;
};
#endif // MAINWINDOW_H
