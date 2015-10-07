#include <QMessageBox>
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
    ui->drives->addItems(disks->drives());
    ui->blocks->addItems(disks->blockDevices());
}

MainWindow::~MainWindow()
{
    delete disks;
    delete ui;
}

void MainWindow::on_drives_currentTextChanged(const QString &currentText)
{
    ui->driveProps->setText(disks->drive(currentText)->toString());
}

void MainWindow::on_blocks_currentTextChanged(const QString &currentText)
{
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
    ui->blocks->addItem(node);
}

void MainWindow::udisks2_blockRemoved(const QString &node)
{
    ui->msgs->addItem("Disconnected block " + node);
    foreach (auto item, ui->blocks->findItems(node, Qt::MatchExactly))
        delete item;
}

void MainWindow::udisks2_blockChanged(const QString &node)
{
    ui->msgs->addItem("Block " + node + " changed");
    if (ui->blocks->currentItem() && ui->blocks->currentItem()->text() == node) {
        ui->blockProps->setText(disks->blockDevice(node)->toString());
    }
}

void MainWindow::udisks2_driveAdded(const QString &node)
{
    ui->msgs->addItem("Connected drive " + node);
    ui->drives->addItem(node);
}

void MainWindow::udisks2_driveRemoved(const QString &node)
{
    ui->msgs->addItem("Disconnected drive " + node);
    auto items = ui->drives->findItems(node, Qt::MatchExactly);
    foreach (auto item, items)
        delete item;
}

void MainWindow::udisks2_driveChanged(const QString &node)
{
    ui->msgs->addItem("Drive " + node + " changed");
    if (ui->drives->currentItem() && ui->drives->currentItem()->text() == node)
        ui->driveProps->setText(disks->drive(node)->toString());
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
    ui->msgs->addItem("Block " + node + " changed its filesystem");
    if (ui->blocks->currentItem() && ui->blocks->currentItem()->text() == node) {
        ui->mounts->clear();
        ui->mounts->addItems(disks->blockDevice(node)->fileSystem()->mountPoints());
    }
}

void MainWindow::on_mount_clicked()
{
    if (!ui->blocks->currentItem())
        return;
    auto fs = disks->blockDevice(ui->blocks->currentItem()->text())->fileSystem();
    if (fs)
        QMessageBox::information(this, "Mounted directory was", fs->mount());
}

void MainWindow::on_unmount_clicked()
{
    if (!ui->blocks->currentItem())
        return;
    auto fs = disks->blockDevice(ui->blocks->currentItem()->text())->fileSystem();
    if (fs)
        fs->unmount();
}
