#ifndef UDISKS2_H
#define UDISKS2_H

#include <QObject>
#include <QtDBus>

class UDisks2Block;
class UDisks2Drive;
class UDisks2Filesystem;

class UDisks2 : public QObject {
    Q_OBJECT
public:
    class BlockInfo {
    public:
        BlockInfo() {}
        QString name;
        QString dev;
        QString id;
        QString drive;
        qulonglong size;
        bool readonly;
        QString usage;
        QString type;
        QString toString();
    };
    class DriveInfo {
    public:
        DriveInfo() {}
        QString name;
        qulonglong size;
        QString vendor;
        QString model;
        QString serial;
        QString id;
        QString media;
        bool optical;
        bool removable;
        bool available;
        QString toString();
    };
    explicit UDisks2(QObject *parent = NULL);
    ~UDisks2();

    QStringList blockDevices();
    BlockInfo blockDeviceInfo(const QString &node);
    UDisks2Block *blockDevice(const QString &node);

    QStringList drives();
    DriveInfo driveInfo(const QString &node);
    UDisks2Drive *drive(const QString &node);

signals:
    void deviceInformationChanged(QString node, QVariantMap info);
    void driveAdded(const QString& node);
    void driveRemoved(const QString& node);
    void blockDeviceAdded(const QString& node);
    void blockDeviceRemoved(const QString &node);
    void filesystemAdded(const QString& node);
    void filesystemRemoved(const QString &node);

private:
    void addDrive(const QString &node);
    void addBlock(const QString &node);
    void removeDrive(const QString &node);
    void removeBlock(const QString &node);

private slots:
    void dbus_interfaceAdded(const QDBusObjectPath &path, const QMap<QString, QVariant> &interfaces);
    void dbus_interfaceRemoved(const QDBusObjectPath &path, const QStringList &interfaces);

private:
    QMap<QString,UDisks2Drive*> drives_;
    QMap<QString,UDisks2Block*> blocks_;
};



class UDisks2Block : public QObject {
    Q_OBJECT
public:
    explicit UDisks2Block(const QString &node, QObject *parent = NULL);

public:
    QString name;
    QString dev;
    QString id;
    QString drive;
    qulonglong size;
    bool readonly;
    QString usage;
    QString type;
    QString toString();

    void update();
    void updateFilesystem();
    void addFilesystem();
    void removeFilesystem();
    UDisks2Filesystem *fileSystem();

signals:
    void filesystemAdded(const QString& node);
    void filesystemRemoved(const QString &node);

private slots:
    void self_propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated);

private:
    QDBusInterface *dbus;
    UDisks2Filesystem* fs;
};

class UDisks2Filesystem : public QObject {
    Q_OBJECT
public:
    UDisks2Filesystem(const QString &node, QObject *parent = NULL);
    QStringList mountPoints() const;
    void update();
    bool isValid();

    QString name;

private:
    QDBusInterface *dbus;
    QStringList mountPoints_;
};

class UDisks2Drive : public QObject {
    Q_OBJECT
public:
    explicit UDisks2Drive(const QString &node, QObject *parent = NULL);

    QString name;
    qulonglong size;
    QString vendor;
    QString model;
    QString serial;
    QString id;
    QString media;
    bool optical;
    bool removable;
    bool available;
    QString toString();

    void update();

private slots:
    void self_propertiesChanged(const QString &interface, const QVariantMap &changed, const QStringList &invalidated);

private:
    QDBusInterface *dbus;
};


#endif // UDISKS2_H