#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    file=nullptr;
    lineNumber=0;
    nowLine=0;
    ui->setupUi(this);
    ui->spinBox->setValue(12);
    QFont font("宋体",12);
    ui->textBrowser->setFont(font);
    QFontDatabase database;
    QStringList fontFamilies = database.families();
    for(int i=0;i<fontFamilies.size();i++){
        ui->comboBox->addItem(fontFamilies[i]);
    }
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout *hLayout = new QVBoxLayout;
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->setAlignment(Qt::AlignLeft);
    hlayout1->addWidget(ui->pushButton);
    hlayout1->addWidget(ui->spinBox);
    hlayout1->addWidget(ui->comboBox);
    hlayout1->addWidget(ui->pushButton_2);
    hlayout1->addWidget(ui->pushButton_3);
    hlayout1->addWidget(ui->lineEdit);
    QIntValidator *validator = new QIntValidator();
    ui->lineEdit->setValidator(validator);
    ui->pushButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->spinBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->comboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->pushButton_2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->pushButton_3->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    hLayout->addLayout(hlayout1);
    hLayout->addWidget(ui->textBrowser);
    centralWidget()->setLayout(hLayout);
    ui->textBrowser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->textBrowser->setPlainText("你好，欢迎使用");
    connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(onSpinbox()));
    connect(ui->comboBox,SIGNAL(activated(int)),this,SLOT(onComobox()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(importFile()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(getFileLineNumber()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(jumpToSpecifiedLine()));
    QObject::connect(ui->textBrowser->verticalScrollBar(), &QScrollBar::valueChanged, [this]() {
        if(ui->textBrowser->verticalScrollBar()->value()==ui->textBrowser->verticalScrollBar()->maximum()){
            //QString str=ui->textBrowser->toPlainText();
            nowLine+=1000;
            QString str=readLineOnText(nowLine);
//            if(str.size()>100*1000){
//                str.remove(0,50*1000);//???不该这么写的
//            }
            ui->textBrowser->setPlainText(str);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete file;
}

QString MainWindow::readLineOnText(int line)
{
    QString str;
    file->seek(0);
    while(line){
        file->readLine();
        line--;
    }
    for(int i=0;i<1000&&(!file->atEnd());i++){
        str+=file->readLine();
    }
    if(str.size()==0){
        QMessageBox::information(this,"提示","已到文件末尾");
    }
    return str;
}

void MainWindow::onSpinbox()
{
    QFont font=ui->textBrowser->font();
    font.setPointSize(ui->spinBox->value());
    ui->textBrowser->setFont(font);
}

void MainWindow::onComobox()
{
    QFont font=ui->textBrowser->font();
    font.setFamily(ui->comboBox->currentText());
    ui->textBrowser->setFont(font);
}

void MainWindow::importFile()
{
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Select File", QDir::homePath(), "Text Files (*.txt)");
    if(!filePath.isEmpty()){
        if(file!=nullptr){
            file->close();
            delete file;
        }
        file=new QFile(filePath);
        if(file->open(QIODevice::ReadOnly | QIODevice::Text)){
            QString str=readLineOnText(0);
            ui->textBrowser->setPlainText(str);
            lineNumber=0;
            ui->pushButton_2->setText("查询文件行数");
        }else{
            qDebug()<<"Failed to open the file."<<endl;
        }
    }
}

void MainWindow::getFileLineNumber()
{
    int count=0;
    if(file!=nullptr){
        while(!file->atEnd()){
            file->readLine();
            count++;
        }
    }else{
        QMessageBox::information(this,"注意","文件未打开");
        return;
    }
    lineNumber=count;
    QString str="总计："+QString::number(count)+"行";
    QMessageBox::information(this,"注意",str);
    ui->pushButton_2->setText(str);
}

void MainWindow::jumpToSpecifiedLine()
{
    int lines=ui->lineEdit->text().toInt();
    if(lineNumber!=0&&lines<=lineNumber){
        QString str=readLineOnText(lines);
        ui->textBrowser->setPlainText(str);
    }else if(lineNumber!=0&&lines>lineNumber){
        QMessageBox::information(this,"注意","超出最大行");
    }else{
        QMessageBox::information(this,"注意","请先查询文件行数");
    }
}

void MainWindow::goToNextPage()
{

}

