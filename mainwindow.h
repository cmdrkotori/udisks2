#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "udisks2.h"

namespace Ui {
class MainWindow;
}

class UDisks2;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_drives_currentTextChanged(const QString &currentText);
    void on_blocks_currentTextChanged(const QString &currentText);
    void udisks2_blockAdded(const QString &node);
    void udisks2_blockRemoved(const QString &node);
    void udisks2_blockChanged(const QString &node);
    void udisks2_driveAdded(const QString &node);
    void udisks2_driveRemoved(const QString &node);
    void udisks2_driveChanged(const QString &node);
    void udisks2_filesystemAdded(const QString &node);
    void udisks2_filesystemRemoved(const QString &node);
    void udisks2_filesystemChanged(const QString &node);

    void on_mount_clicked();

    void on_unmount_clicked();

private:
    Ui::MainWindow *ui;
    UDisks2 *disks;
};




#endif // MAINWINDOW_H
