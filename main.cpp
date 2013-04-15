#include "timeline.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("CamelCorp");
    a.setApplicationName("Timeline");
    a.setApplicationVersion("1.0");

    QTranslator translator;
    translator.load(a.applicationDirPath() + "/timeline_" + QLocale::system().name().left(2));
    bool locale = a.installTranslator(&translator);

    QTranslator qtTranslator;
    if(locale)
        qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    else
        qtTranslator.load("qt_" + QLocale(QLocale::English).name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    Timeline w;
    w.show();
    
    return a.exec();
}
