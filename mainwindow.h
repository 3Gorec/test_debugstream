#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "debugstream.h"
#include <QMainWindow>

namespace Ui {

void OutputToTextArea(const char *s, unsigned int n);

class MainWindow;



}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void TextAreaInputString(const char *s, unsigned int n);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    void TestDebugStream();
    friend void OutputToTextArea(const char *s, unsigned int n);
    DebugStreambuf debug_output;
    DebugStream debug;
};

extern Ui::MainWindow *global_ui;

#endif // MAINWINDOW_H
