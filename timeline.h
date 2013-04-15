#ifndef TIMELINE_H
#define TIMELINE_H

#include <QtWidgets>
#include <QtXml>
#include <QtPrintSupport>

#include "painting.h"

class Timeline : public QMainWindow
{
    Q_OBJECT
    
public:
    Timeline(QWidget *parent = 0);
private:
    void makeMenus();
    void makeDialogs();

    QToolBar* toolbar;
    QMenuBar* menus;

    QMenu* file;
    QMenu* edit;
    QMenu* view;
    QMenu* help;

    QAction* create;
    QAction* open;
    QAction* save;
    QAction* saveAs;
    QAction* print;
    QAction* quit;

    QAction* addEvent;
    QAction* editEvent;
    QAction* removeEvent;
    QAction* rename;

    QAction* zoomIn;
    QAction* zoomOut;
    QAction* setPeriod;

    QAction* about;

    QString dateFormat;
    QDomDocument domDocument;
    QDomElement root;
    QString path;
    QString name;

    Painting* timeline;
    QMap<QDate, QStringList > dateToName;
    QHash<QString, int> nameToId;
    QHash<QString, QColor> nameToColor;

    QDialog* pickDialog;
    QGridLayout* pickLayout;
    QListWidget* pickList;
    QLineEdit* searchEdit;
    QPushButton* pickOk;
    QPushButton* pickCancel;

    QDialog* editDialog;
    QGridLayout* editLayout;
    QLabel* eventLabel;
    QLineEdit* eventEdit;
    QLabel* dateLabel;
    QDateEdit* dateEdit;
    QLabel* colorLabel;
    QWidget* colorPreview;
    QPushButton* colorChange;
    QHBoxLayout* editButtons;
    QPushButton* editOk;
    QPushButton* editCancel;

    QColorDialog* colorDialog;
    QColor currentColor;

    QDialog* periodDialog;
    QGridLayout* periodLayout;
    QLabel* beginLabel;
    QDateEdit* beginDate;
    QLabel* endLabel;
    QDateEdit* endDate;
    QHBoxLayout* periodButtons;
    QPushButton* periodOk;
    QPushButton* periodCancel;

    int id;
    int editingIndex;

private slots:
    void newTimeline();
    void openTimeline();
    void saveTimeline();
    void saveAsTimeline();
    void printTimeline();

    void addFact();
    void editFact();
    void removeFact();
    void renameTimeline();

    void setActivePeriod();

    void aboutApp();

    void confirmAddition();
    void openEditing();
    void confirmEditing();
    void confirmDeletion();

    void search(QString);
    void periodChanged(QDate, QDate);
    void changeColor(QColor);

protected:
    virtual void closeEvent(QCloseEvent *);

};

#endif // TIMELINE_H
