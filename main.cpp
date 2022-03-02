#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "data/systemdata.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "WordTranslateTool_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.setTitleBase("Word Translate Tool");
    //init
    QString path = SystemData::Instance()->Init();

    // 打开上一次打开过的工程
    if(path.length()){
        w.loadFile(path);
    }
    w.show();
    return a.exec();
}
