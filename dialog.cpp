#include "dialog.h"
#include "ui_dialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    dirModel->setRootPath(QDir::currentPath());

    ui->dirView->setModel(dirModel);

    ui->dirView->hideColumn(1);
    ui->dirView->hideColumn(2);
    ui->dirView->hideColumn(3);
    ui->dirView->hideColumn(4);

    fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(QDir::currentPath());
    fileModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    ui->fileView->setModel(fileModel);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_dirView_clicked(const QModelIndex &index)
{
    QString path;

    path = dirModel->fileInfo(index).absoluteFilePath();

    ui->fileView->setRootIndex(fileModel->setRootPath(path));
}


bool Dialog::isValid()
{
    QModelIndexList indexes = ui->fileView->selectionModel()->selectedIndexes();

    if(indexes.length() > 0)
        return true;

    return false;
}

void Dialog::done(int r)
{

    if(!isValid() && r == QDialog::Accepted)
    {
        QMessageBox::warning(this, tr("Selecione um arquivo"),
                             tr("Voce precisa selecionar um arquivo descritor antes de continuar."),
                             QMessageBox::Ok);
    }
    else
    {
        QDialog::done(r);
    }

}
QString Dialog::getSelectedFilePath() const
{

    QString path;

    QModelIndexList indexes = ui->fileView->selectionModel()->selectedIndexes();

    path = fileModel->fileInfo(indexes.first()).absoluteFilePath();
    return path;
}
