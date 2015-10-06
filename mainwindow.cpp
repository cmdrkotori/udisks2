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
    connect(disks, &UDisks2::filesystemChanged, this, &MainWindow::udisks2_filesystemChanged);
    connect(disks, &UDisks2::driveAdded, this, &MainWindow::udisks2_driveAdded);
    connect(disks, &UDisks2::driveRemoved, this, &MainWindow::udisks2_driveRemoved);
    connect(disks, &UDisks2::driveChanged, this, &MainWindow::udisks2_driveChanged);
    connect(disks, &UDisks2::blockDeviceAdded, this, &MainWindow::udisks2_blockAdded);
    connect(disks, &UDisks2::blockDeviceRemoved, this, &MainWindow::udisks2_blockRemoved);
    connect(disks, &UDisks2::blockDeviceChanged, this, &MainWindow::udisks2_blockChanged);
    ui->listWidget->addItems(disks->drives());
    ui->blocks->addItems(disks->blockDevices());
}

MainWindow::~MainWindow()
{
    delete disks;
    delete ui;
}

void MainWindow::on_listWidget_currentTextChanged(const QString &currentText)
{
    ui->textEdit->setText(disks->drive(currentText)->toString());
}

void MainWindow::on_blocks_currentTextChanged(const QString &currentText)
{
    qDebug() << currentText;
    auto block = disks->blockDevice(currentText);
    ui->blockProps->setText(block->toString());
    auto fs = block->fileSystem();
    ui->mounts->clear();
    if (fs)
        ui->mounts->addItems(fs->mountPoints());
}

void MainWindow::udisks2_blockAdded(const QString &node)
{
    ui->msgs->addItem("Connected block " + node);
}

void MainWindow::udisks2_blockRemoved(const QString &node)
{
    ui->msgs->addItem("Disconnected block " + node);
}

void MainWindow::udisks2_blockChanged(const QString &node)
{
    if (ui->blocks->currentItem() && ui->blocks->currentItem()->text() == node) {
        ui->blockProps->setText(disks->blockDevice(node)->toString());
    }
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

void MainWindow::udisks2_driveChanged(const QString &node)
{
    if (ui->listWidget->currentItem() && ui->listWidget->currentItem()->text() == node)
        ui->textEdit->setText(disks->drive(node)->toString());
}

void MainWindow::udisks2_filesystemAdded(const QString &node)
{
    ui->msgs->addItem("Block " + node + " got a filesystem");
    if (ui->blocks->currentItem() && ui->blocks->currentItem()->text() == node) {
        auto block = disks->blockDevice(node);
        ui->mounts->clear();
        ui->mounts->addItems(block->fileSystem()->mountPoints());
    }
}

void MainWindow::udisks2_filesystemRemoved(const QString &node)
{
    ui->msgs->addItem("Block " + node + " lost a filesystem");
    if (ui->blocks->currentItem()
            && ui->blocks->currentItem()->text() == node) {
        ui->mounts->clear();
    }
}

void MainWindow::udisks2_filesystemChanged(const QString &node)
{
    if (ui->blocks->currentItem() && ui->blocks->currentItem()->text() == node) {
        ui->mounts->clear();
        ui->mounts->addItems(disks->blockDevice(node)->fileSystem()->mountPoints());
    }
}
