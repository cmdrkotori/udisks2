#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    disks = new UDisks2(this);
    connect(disks, &UDisks2::filesystemAdded, this, &MainWindow::udisks2_filesystemAdded);
    connect(disks, &UDisks2::filesystemRemoved, this, &MainWindow::udisks2_filesystemRemoved);
    connect(disks, &UDisks2::driveAdded, this, &MainWindow::udisks2_driveAdded);
    connect(disks, &UDisks2::driveRemoved, this, &MainWindow::udisks2_driveRemoved);
    connect(disks, &UDisks2::blockDeviceAdded, this, &MainWindow::udisks2_blockAdded);
    connect(disks, &UDisks2::blockDeviceRemoved, this, &MainWindow::udisks2_blockRemoved);
    ui->listWidget->addItems(disks->drives());
}

MainWindow::~MainWindow()
{
    delete disks;
    delete ui;
}

void MainWindow::on_listWidget_currentTextChanged(const QString &currentText)
{
    ui->textEdit->setText(disks->driveInfo(currentText).toString());
}

void MainWindow::udisks2_blockAdded(const QString &node)
{
    ui->msgs->addItem("Connected block " + node);
}

void MainWindow::udisks2_blockRemoved(const QString &node)
{
    ui->msgs->addItem("Disconnected block " + node);
}

void MainWindow::udisks2_driveAdded(const QString &node)
{
    ui->msgs->addItem("Connected drive " + node);
    ui->listWidget->addItem(node);
}

void MainWindow::udisks2_driveRemoved(const QString &node)
{
    ui->msgs->addItem("Disconnected drive " + node);
    auto items = ui->listWidget->findItems(node, Qt::MatchExactly);
    foreach (auto item, items)
        delete item;
}

void MainWindow::udisks2_filesystemAdded(const QString &node)
{
    ui->msgs->addItem("Block " + node + " got a filesystem");
}

void MainWindow::udisks2_filesystemRemoved(const QString &node)
{
    ui->msgs->addItem("Block " + node + " lost a filesystem");
}

