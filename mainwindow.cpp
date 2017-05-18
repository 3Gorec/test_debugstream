#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>

Ui::MainWindow *global_ui;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    debug(&debug_output)
{
    ui->setupUi(this);
    global_ui=ui;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(){
    TestDebugStream();
}

void MainWindow::TextAreaInputString(const char *s, unsigned int n){
    std::string str(s,n);
    global_ui->textBrowser_2->insertPlainText(QString(str.data()));
}



void MainWindow::TestDebugStream(){
    debug_output.RegisterStream(&std::cout);
    std::fstream file;
    file.open("output.txt", std::ios::out );
    if(file.is_open()){
           debug_output.RegisterStream(&file);
    }
    else{
        std::cout<<"Error!: Fail open file\n";
    }
    debug_output.RegisterCallback(Ui::OutputToTextArea);
    connect(&debug_output,SIGNAL(OutputString(const char*,uint)),this,SLOT(TextAreaInputString(const char*,uint)));

    debug<<"Test output\n";
    debug<<"All targets "<<123<<std::endl;

    debug_output.UnRegisterStream(&std::cout);
    debug_output.UnRegisterCallback(Ui::OutputToTextArea);
    disconnect(&debug_output,SIGNAL(OutputString(const char*,uint)),this,SLOT(TextAreaInputString(const char*,uint)));
    debug<<"This string only for file"<<std::endl;

    debug_output.UnRegisterStream(&file);
    debug_output.RegisterStream(&std::cout);
    debug<<"This string only for console"<<std::endl;

    debug_output.UnRegisterStream(&std::cout);
    debug_output.RegisterCallback(Ui::OutputToTextArea);
    debug<<"This string only for Text area 1"<<std::endl;

    debug_output.UnRegisterCallback(Ui::OutputToTextArea);
    connect(&debug_output,SIGNAL(OutputString(const char*,uint)),this,SLOT(TextAreaInputString(const char*,uint)));
    debug<<"This string only for Text area 2"<<std::endl;

    debug_output.UnRegisterStream(&std::cout);
    debug_output.UnRegisterStream(&file);
    debug_output.UnRegisterCallback(Ui::OutputToTextArea);
    disconnect(&debug_output,SIGNAL(OutputString(const char*,uint)),this,SLOT(TextAreaInputString(const char*,uint)));
    file.close();
}

void Ui::OutputToTextArea(const char *s, unsigned int n){
    std::string str(s,n);
    global_ui->textBrowser->insertPlainText(QString(str.data()));
}

