#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileSystemModel>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QString getSelectedFilePath() const;

private slots:
    void on_dirView_clicked(const QModelIndex &index);
    virtual void done(int);

private:
    Ui::Dialog *ui;
    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;

    bool isValid();

};

#endif // DIALOG_H
