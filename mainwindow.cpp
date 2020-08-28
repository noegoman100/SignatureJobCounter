#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDirIterator>
#include <QFileDialog>
#include <QDateTime>
#include <QRegularExpression>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonPickFolder_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    ui->lineEditFolderPath->setText(dir);
}


void MainWindow::on_buttonStartNew_clicked()
{
    //Load an array[EnteredPath\ZDone][AllMonths][AllDays] with the known folders
    //A:\Broomfield\ZDone
    //A:\OB - Tupelo\ZDone
    //A:\Phoenix\ZDone
    //A:\Hattiesburg\ZDone
    //A:\Didde\zDone
    //
    //Example full path: A:\Phoenix\ZDone\2020-06-June\Z06-30-20Done\_PROOF_AZCAPELECTIONGUIDE_0629_ESJC
    //Load array with names of all subfolders
    QString dir = ui->lineEditFolderPath->text();

    QVector<QString> initialPath;
    initialPath.push_back(dir); //Initialize the first dimension of the vector (just the source directory entered in UI)
    QVector<QString> months;
    QVector<QString> days;
    QVector<QString> jobs;
    //QDirIterator it(initialPath[0], QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it(initialPath[0], QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);


    while (it.hasNext()){
        //fill a vector with all of the Months directories
        months.push_back(it.next()); //Path stored as: E:/deleteMeTestFolders/ZDone/2020-05-May
    }
    qDebug() << "Size of Months: " << months.size();
    for (int i = 0; i < months.size(); i++){
        QString newDir(months.at(i));
        //qDebug() << "newDir: " << newDir;
        QDirIterator it(newDir, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        while (it.hasNext()){
            //fill a vector with all of the days directories
            days.push_back(it.next());
        }
    }
    qDebug() << "Size of Days: " << days.size();

    for (int j = 0; j < days.size(); j++){
        QString newDir(days.at(j));
        QDirIterator it(newDir, QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
        //qDebug() << "newDir: " << newDir;
        while (it.hasNext()){
            //fill a vector with all of the jobs directories
            jobs.push_back(it.next());
        }
    }


    //Parse the intials names
    QString rawList(ui->lineEditNameSubstring->text());
    QStringList listInitials(rawList.split(", "));
    for (int i = 0; i < listInitials.size(); i++){
        qDebug() << "Initials: " << listInitials.at(i);
    }


    //Loop through the jobs list with each initial-regex string
    //Go through and count the initials in the jobs vector.
    QRegularExpressionMatch match;
    //setup and initialize an array to count all of the matches for each initial.
    int matchesPer[listInitials.size()];
    for (int i = 0; i < listInitials.size(); i++){
        matchesPer[i] = 0;
    }
    //for each intial in the provided list...
    for (int i = 0; i < listInitials.size(); i++){
        QRegularExpression RE("(?i)(" + listInitials.at(i) + "|" + listInitials.at(i) + "..|" + listInitials.at(i) + "....)$");
        //check for a match in each job name with a regular expression
        for (int j = 0; j < jobs.size(); j++){
            match = RE.match(jobs.at(j));
            if (match.hasMatch()){
                qDebug() << "Match! " << match.captured(0);
                matchesPer[i]++;
            }
        }//End nested for loop
    }//End for loop

    ui->textEdit->append("Results for " + initialPath.at(0) + ": \n __________________");
    //Display the results in the UI
    for (int i = 0; i < listInitials.size(); i++){
        qDebug() << "Matches for " << listInitials.at(i) << ": " << matchesPer[i];
        ui->textEdit->append("Matches for " + listInitials.at(i) + ": " + QString::number(matchesPer[i]));
    }







}
