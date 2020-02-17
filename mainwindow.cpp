#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QScrollBar>
#include <QStringList>
//#include <stdio.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);//Windows is Only Readable (Not Editable)
    ui->textEdit->setTextInteractionFlags(Qt::NoTextInteraction);
    ui->textEdit->clear();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::readNrOfLines()
{
    self = ui->textEdit->toPlainText();
    numberOfLines = 0;
    lines = self.split("\n", QString::SkipEmptyParts);
    foreach(QString line, lines)
    {
        //do somthing with the line
        numberOfLines++;
    }
}

void MainWindow::on_pushButton_clicked()
{
    //Load Test report and put the text into the Test report widget
    ui->textEdit->clear();//Clear all text in test report widget

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open text file"), "",
                                                    tr("text(*.txt);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }

        QTextStream in(&file);
        ui->textEdit->insertPlainText(in.readAll());
        self = ui->textEdit->toPlainText();
        numberOfLines = 0;
        ui->textEdit->clear();//Clear all text in test report widget
        ui->textEdit->insertPlainText(self);
        readNrOfLines();

        ui->label_5->setNum(numberOfLines);

        //QTextEdit *edit; // assuming this is your text edit
        QScrollBar *sb = ui->textEdit->verticalScrollBar();//Autoscroll down
        sb->setValue(sb->maximum());

        file.close();
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);

    }

}

void MainWindow::on_pushButton_3_clicked()
{
    //Save Test Report
    QString self = ui->textEdit->toPlainText();
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Text"),  "Text.txt",
                                                    tr("Test (*.txt);;All Files (*)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QTextStream out(&file);
        out << self;
      //  printf("Test Report saved\n");
        file.close();

    }

}

void MainWindow::on_spinBox_3_valueChanged(int arg1)
{
    startAtLine = arg1;
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    removeLines = arg1;
}

void MainWindow::on_spinBox_2_valueChanged(int arg1)
{
    jumpOverLines = arg1;
}

void MainWindow::on_pushButton_2_clicked()
{
    //Modify bottom
    self = ui->textEdit->toPlainText();
    numberOfLines = 0;
    ui->textEdit->clear();//Clear all text in test report widget
    //lines = self.split("\n", QString::SkipEmptyParts);
    lines = self.split("\n");
    int rollerCoutner = 0;
    int maxRollerCnt = removeLines + jumpOverLines;
    foreach(QString line, lines)
    {
        if (numberOfLines+2 > startAtLine)
        {
            //beginRemoveLines = true;
            //Now it's time to remove lines;
            if(rollerCoutner < removeLines)
            {
                //Erase line
                if (line == "")
                {

                }
                else
                {
                    line = "";
                }
            }
            else
            {
            //    Not remove line
                if (line == "")
                {
                    line = "\n";
                    printf("line == ''\n");
                }
                else
                {
                    line = line + "\n";
                }
            }
            rollerCoutner++;
            if(rollerCoutner >= maxRollerCnt)
            {
                rollerCoutner = 0;
            }
        }
        else
        {
            //Don't start remove lines yet wait until
            line = line + "\n";
        }
        ui->textEdit->insertPlainText(line);//Insert line in the text view
        numberOfLines++;
    }
    numberOfLines = 0;
    readNrOfLines();
    ui->label_5->setNum(numberOfLines);
}
