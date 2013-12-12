#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QWidget>
#include <QFileSystemModel>


namespace Ui {
class FileExplorer;
}

class FileExplorer : public QWidget
{
    Q_OBJECT
public:
    explicit FileExplorer(QWidget *parent = 0);
    ~FileExplorer();

private:
    Ui::FileExplorer *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
signals:

public slots:

};

#endif // FILEEXPLORER_H
