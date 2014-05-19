#include <QCoreApplication>

#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QTextStream>

void scan_folder(QString path)
{
    qDebug() << "scan_folder" << path;
    QDir dir(path);
    /* Check each file in the path */
    foreach (QString file_name, dir.entryList(QDir::Files
                                             | QDir::NoDotAndDotDot
                                             | QDir::NoSymLinks))
    {
        qDebug () << "scan_folder::checking::" << path + QDir::separator() + file_name;
        if (file_name == "wakeup")
        {
            qDebug () << "scan_folder::found::";
            QFile file(path + QDir::separator() + file_name);
            QTextStream stream(&file);
            file.open(QFile::WriteOnly);
            stream << "enabled";
        }
    }

    /* Recursively look */
    foreach (QString sub_dir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        scan_folder(path + QDir::separator() + sub_dir);
    }

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("USB Wakeup enabler");
    QCoreApplication::setApplicationVersion("1.0");
    QString source_path("/sys/devices/pci0000:00/0000:00:1a.0/usb1");
    QCommandLineParser parser;
    parser.setApplicationDescription("USB Wakeup enabler");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("source",
                                 QCoreApplication::translate("main", "Source folder to scan"));

    const QStringList args = parser.positionalArguments();

    if (args.size() > 0 && !args.at(0).isEmpty())
        source_path = args.at(0);

    if (!source_path.isEmpty())
        scan_folder(source_path);

    return a.exec();
}
