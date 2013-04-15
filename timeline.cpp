#include "timeline.h"

Timeline::Timeline(QWidget *parent)
    : QMainWindow(parent)
{
    id = 0;
    dateFormat = tr("MM/dd/yyyy", "Only use the following expressions: MM (month), dd (day), yy (year with 2 digits) and yyyy (year with 4 digits)");
    setWindowTitle(tr("Timeline"));
    timeline = new Painting;
    timeline->setMap(&dateToName);
    timeline->setHash(&nameToColor);
    timeline->setEnabled(false);
    resize(800,600);
    setCentralWidget(timeline);

    makeDialogs();
    makeMenus();
}

void Timeline::makeMenus()
{
    menus = menuBar();
    file = menus->addMenu(tr("&File"));
    edit = menus->addMenu(tr("&Edit"));
    view = menus->addMenu(tr("&View"));
    help = menus->addMenu(tr("?"));

    const QString iconsPath = qApp->applicationDirPath() + "/icons/";

    QIcon createIcon;
    createIcon.addFile(iconsPath + "document-new.png");
    createIcon.addFile(iconsPath + "document-new-l.png");
    create = new QAction(createIcon, tr("New"), this);

    QIcon openIcon;
    openIcon.addFile(iconsPath + "document-open.png");
    openIcon.addFile(iconsPath + "document-open-l.png");
    open = new QAction(openIcon, tr("Open"), this);

    QIcon saveIcon;
    saveIcon.addFile(iconsPath + "document-save.png");
    saveIcon.addFile(iconsPath + "document-save-l.png");
    save = new QAction(saveIcon, tr("Save"), this);

    QIcon saveAsIcon;
    saveAsIcon.addFile(iconsPath + "document-save-as.png");
    saveAsIcon.addFile(iconsPath + "document-save-as-l.png");
    saveAs = new QAction(saveAsIcon, tr("Save as..."), this);

    QIcon printIcon;
    printIcon.addFile(iconsPath + "document-print.png");
    printIcon.addFile(iconsPath + "document-print-l.png");
    print = new QAction(printIcon, tr("Print"), this);

    QIcon quitIcon;
    quitIcon.addFile(iconsPath + "application-exit.png");
    quitIcon.addFile(iconsPath + "application-exit-l.png");
    quit = new QAction(quitIcon, tr("Quit"), this);

    QIcon addIcon;
    addIcon.addFile(iconsPath + "tag-new.png");
    addIcon.addFile(iconsPath + "tag-new-l.png");
    addEvent = new QAction(addIcon, tr("Add event"), this);

    QIcon editIcon;
    editIcon.addFile(iconsPath + "tag-edit.png");
    editIcon.addFile(iconsPath + "tag-edit-l.png");
    editEvent = new QAction(editIcon, tr("Edit event"), this);

    QIcon removeIcon;
    removeIcon.addFile(iconsPath + "tag-remove.png");
    removeIcon.addFile(iconsPath + "tag-remove-l.png");
    removeEvent = new QAction(removeIcon, tr("Remove event"), this);

    QIcon renameIcon;
    renameIcon.addFile(iconsPath + "document-properties.png");
    renameIcon.addFile(iconsPath + "document-properties-l.png");
    rename = new QAction(QIcon(iconsPath + "document-properties.png"), tr("Rename timeline"), this);

    QIcon zoomInIcon;
    zoomInIcon.addFile(iconsPath + "zoom-in.png");
    zoomInIcon.addFile(iconsPath + "zoom-in-l.png");
    zoomIn = new QAction(zoomInIcon, tr("Zoom in"), this);

    QIcon zoomOutIcon;
    zoomOutIcon.addFile(iconsPath + "zoom-out.png");
    zoomOutIcon.addFile(iconsPath + "zoom-out-l.png");
    zoomOut = new QAction(zoomOutIcon, tr("Zoom out"), this);

    QIcon setPeriodIcon;
    setPeriodIcon.addFile(iconsPath + "zoom-best-fit.png");
    setPeriodIcon.addFile(iconsPath + "zoom-best-fit-l.png");
    setPeriod = new QAction(setPeriodIcon, tr("Set period"), this);

    QIcon aboutIcon;
    aboutIcon.addFile(iconsPath + "help-contents.png");
    aboutIcon.addFile(iconsPath + "help-contents-l.png");
    about = new QAction(aboutIcon, tr("About"), this);


    toolbar = addToolBar(tr("Toolbar"));
    toolbar->addAction(create);
    toolbar->addAction(open);
    toolbar->addAction(save);
    toolbar->addAction(print);
    toolbar->addSeparator();
    toolbar->addAction(addEvent);
    toolbar->addAction(editEvent);
    toolbar->addAction(removeEvent);
    toolbar->addSeparator();
    toolbar->addAction(zoomIn);
    toolbar->addAction(zoomOut);
    toolbar->addAction(setPeriod);

    save->setEnabled(false);
    saveAs->setEnabled(false);
    print->setEnabled(false);

    addEvent->setEnabled(false);
    editEvent->setEnabled(false);
    removeEvent->setEnabled(false);
    rename->setEnabled(false);

    zoomIn->setEnabled(false);
    zoomOut->setEnabled(false);
    setPeriod->setEnabled(false);

    create->setShortcut(QKeySequence::New);
    open->setShortcut(QKeySequence::Open);
    save->setShortcut(QKeySequence::Save);
    saveAs->setShortcut(QKeySequence::SaveAs);
    print->setShortcut(QKeySequence::Print);
    quit->setShortcut(QKeySequence::Quit);

    zoomIn->setShortcut(QKeySequence::ZoomIn);
    zoomOut->setShortcut(QKeySequence::ZoomOut);

    connect(create, SIGNAL(triggered()), this, SLOT(newTimeline()));
    connect(open, SIGNAL(triggered()), this, SLOT(openTimeline()));
    connect(save, SIGNAL(triggered()), this, SLOT(saveTimeline()));
    connect(saveAs, SIGNAL(triggered()), this, SLOT(saveAsTimeline()));
    connect(print, SIGNAL(triggered()), this, SLOT(printTimeline()));
    connect(quit, SIGNAL(triggered()), this, SLOT(close()));

    connect(addEvent, SIGNAL(triggered()), this, SLOT(addFact()));
    connect(editEvent, SIGNAL(triggered()), this, SLOT(editFact()));
    connect(removeEvent, SIGNAL(triggered()), this, SLOT(removeFact()));
    connect(rename, SIGNAL(triggered()), this, SLOT(renameTimeline()));

    connect(zoomIn, SIGNAL(triggered()), timeline, SLOT(zoomIn()));
    connect(zoomOut, SIGNAL(triggered()), timeline, SLOT(zoomOut()));
    connect(setPeriod, SIGNAL(triggered()), periodDialog, SLOT(show()));

    connect(about, SIGNAL(triggered()), this, SLOT(aboutApp()));

    file->addAction(create);
    file->addAction(open);
    file->addAction(save);
    file->addAction(saveAs);
    file->addSeparator();
    file->addAction(print);
    file->addSeparator();
    file->addAction(quit);

    edit->addAction(addEvent);
    edit->addAction(editEvent);
    edit->addAction(removeEvent);
    edit->addSeparator();
    edit->addAction(rename);

    view->addAction(zoomIn);
    view->addAction(zoomOut);
    view->addAction(setPeriod);

    help->addAction(about);

    statusBar()->setVisible(true);
}

void Timeline::makeDialogs()
{
    editDialog =  new QDialog;
    editLayout = new QGridLayout;
    eventLabel = new QLabel(tr("Event"));
    eventEdit = new QLineEdit;
    dateLabel = new QLabel(tr("Date"));
    editButtons = new QHBoxLayout;
    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setDisplayFormat(dateFormat);
    dateEdit->setMinimumDate(QDate(100, 1, 1));
    colorLabel = new QLabel(tr("Color"));
    colorPreview = new QWidget;
    colorChange = new QPushButton(tr("Change"));
    editOk = new QPushButton(tr("Ok"));
    editOk->setDefault(true);
    editCancel = new QPushButton(tr("Cancel"));

    colorPreview->setFixedSize(35, 21);
    colorPreview->setStyleSheet("border:1px solid black; background-color: #" + QString::number(QColor(0, 224, 0).rgb(), 16).right(6) + ";");
    colorDialog = new QColorDialog;

    connect(colorChange, SIGNAL(clicked()), colorDialog, SLOT(show()));
    connect(colorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(changeColor(QColor)));

    editLayout->addWidget(eventLabel, 0, 0);
    editLayout->addWidget(eventEdit, 0, 1, 1, 2);
    editLayout->addWidget(dateLabel, 1, 0);
    editLayout->addWidget(dateEdit, 1, 1, 1, 2);
    editLayout->addWidget(colorLabel, 2, 0);
    editLayout->addWidget(colorPreview, 2, 1);
    editLayout->addWidget(colorChange, 2, 2);
    editLayout->addLayout(editButtons, 3, 0, 1, 3);

    editButtons->addWidget(editOk);
    editButtons->addWidget(editCancel);

    editDialog->setLayout(editLayout);

    connect(editCancel, SIGNAL(clicked()), editDialog, SLOT(close()));

    pickDialog = new QDialog;
    pickLayout = new QGridLayout;
    pickList = new QListWidget;
    searchEdit = new QLineEdit;
    searchEdit->setPlaceholderText(tr("Search"));
    pickOk = new QPushButton(tr("Ok"));
    pickCancel = new QPushButton(tr("Cancel"));
    pickOk->setDefault(true);

    pickLayout->addWidget(pickList, 0, 0, 1, 2);
    pickLayout->addWidget(searchEdit, 1, 0, 1, 2);
    pickLayout->addWidget(pickOk, 2, 0);
    pickLayout->addWidget(pickCancel, 2, 1);

    connect(pickCancel, SIGNAL(clicked()), pickDialog, SLOT(close()));
    connect(searchEdit, SIGNAL(textChanged(QString)), this, SLOT(search(QString)));
    pickDialog->setLayout(pickLayout);

    periodDialog = new QDialog;
    periodLayout = new QGridLayout;
    beginLabel = new QLabel(tr("Begin date:"));
    beginDate = new QDateEdit;
    endLabel = new QLabel(tr("End date:"));
    endDate = new QDateEdit;
    periodButtons = new QHBoxLayout;
    periodOk = new QPushButton(tr("Ok"));
    periodCancel = new QPushButton(tr("Cancel"));

    periodOk->setDefault(true);
    beginDate->setDisplayFormat(dateFormat);
    endDate->setDisplayFormat(dateFormat);

    periodLayout->addWidget(beginLabel, 0, 0);
    periodLayout->addWidget(beginDate, 0, 1);
    periodLayout->addWidget(endLabel, 1, 0);
    periodLayout->addWidget(endDate, 1, 1);
    periodLayout->addLayout(periodButtons, 2, 0, 1, 2);

    periodButtons->addWidget(periodOk);
    periodButtons->addWidget(periodCancel);

    periodDialog->setLayout(periodLayout);

    connect(timeline, SIGNAL(periodChanged(QDate,QDate)), this, SLOT(periodChanged(QDate,QDate)));
    connect(periodOk, SIGNAL(clicked()), this, SLOT(setActivePeriod()));
    connect(periodCancel, SIGNAL(clicked()), periodDialog, SLOT(close()));
}

void Timeline::newTimeline()
{
    if(save->isEnabled())
    {
        int answer = QMessageBox::question(this, tr("Timeline"), tr("Do you want to save the timeline %1?") .arg(name), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
        if(answer == QMessageBox::Save)
            saveTimeline();
        else if(answer == QMessageBox::Cancel)
            return;
    }

    id = 0;
    nameToId.clear();
    nameToColor.clear();
    dateToName.clear();
    timeline->repaint();
    name = tr("Untitled");
    setWindowTitle(tr("Timeline") + " - " + name);

    domDocument = QDomDocument("Timeline");
    root = domDocument.createElement("Timeline");
    root.setAttribute("Name", name);
    root.setAttribute("Version", qApp->applicationVersion());
    domDocument.appendChild(root);

    path = "";

    timeline->setEnabled(true);

    print->setEnabled(true);
    save->setEnabled(false);
    saveAs->setEnabled(true);

    addEvent->setEnabled(true);
    editEvent->setEnabled(false);
    removeEvent->setEnabled(false);
    rename->setEnabled(true);

    zoomIn->setEnabled(true);
    zoomOut->setEnabled(true);
    setPeriod->setEnabled(true);
}
void Timeline::openTimeline()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open timeline"), QDir::currentPath(), tr("Timeline files (*.time)"));
    if(fileName.isEmpty())
        return;
    QFile* file = new QFile(fileName);
    if(!file->open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Timeline"), tr("Cannot read file %1:\n%2.") .arg(fileName) .arg(file->errorString()));
        return;
    }

    if(save->isEnabled())
    {
        int answer = QMessageBox::question(this, tr("Timeline"), tr("Do you want to save the timeline %1?") .arg(name), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
        if(answer == QMessageBox::Save)
            saveTimeline();
        else if(answer == QMessageBox::Cancel)
            return;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(file, true, &errorStr, &errorLine, &errorColumn)) {
        QMessageBox::critical(window(), tr("Timeline"), tr("Parse error at line %1, column %2:\n%3") .arg(errorLine) .arg(errorColumn) .arg(errorStr));
        file->close();
        return;
    }

    file->close();

    root = domDocument.documentElement();
    if(root.tagName() != "Timeline" || !root.hasAttribute("Version"))
    {
        QMessageBox::information(this, tr("Timeline"), tr("This is not a valid timeline file."));
        return;
    }

    name = root.attribute("Name");
    id = 0;
    nameToId.clear();
    nameToColor.clear();
    dateToName.clear();

    QDomNode curr = root.firstChild();
    while(!curr.isNull())
    {
        QDomElement e = curr.toElement();
        if(!e.isNull())
        {
            nameToId.insert(e.attribute("Name"), id);
            bool ok;
            nameToColor.insert(e.attribute("Name"), QColor(QString("ff" + e.attribute("Color", "ffffff")).toUInt(&ok, 16)));
            dateToName[QDate::fromString(e.attribute("Date"), Qt::ISODate)] << e.attribute("Name");
            id++;
        }
        curr = curr.nextSibling();
    }
    path = fileName;
    setWindowTitle(tr("Timeline") + " - " + root.attribute("Name"));

    timeline->setEnabled(true);

    print->setEnabled(true);
    saveAs->setEnabled(true);

    addEvent->setEnabled(true);
    editEvent->setEnabled(!nameToId.isEmpty());
    removeEvent->setEnabled(!nameToId.isEmpty());

    zoomIn->setEnabled(true);
    zoomOut->setEnabled(true);
    setPeriod->setEnabled(true);
    rename->setEnabled(true);

    statusBar()->showMessage(tr("File loaded"), 3000);
}
void Timeline::saveTimeline()
{
    if(path.isEmpty())
        path = QFileDialog::getSaveFileName(this, tr("Save timeline"), QDir::currentPath(), tr("Timeline files (*.time)"));
    if(path.isEmpty())
        return;
    QFile* file = new QFile(path);
    if(!file->open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Timeline"), tr("Cannot write file %1:\n%2.") .arg(path) .arg(file->errorString()));
        return;
    }

    QTextStream out(file);
    domDocument.save(out, 4);
    save->setEnabled(false);
}
void Timeline::saveAsTimeline()
{
    QString newPath = QFileDialog::getSaveFileName(this, tr("Save timeline as..."), QDir::currentPath(), tr("Timeline files (*.time)"));
    if(newPath.isEmpty())
        return;
    path = newPath;
    QFile* file = new QFile(path);
    if(!file->open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Timeline"), tr("Cannot write file %1:\n%2.") .arg(path) .arg(file->errorString()));
        return;
    }

    QTextStream out(file);
    domDocument.save(out, 4);
    save->setEnabled(false);
}
void Timeline::printTimeline()
{
    QPrinter* printer = new QPrinter;
    printer->setOrientation(QPrinter::Landscape);
    QPrintDialog dialog(printer, this);
    if(dialog.exec() == QDialog::Accepted)
    {
        //printing process
    }
}

void Timeline::addFact()
{
    dateEdit->setDate(QDate::currentDate());
    eventEdit->setText("");
    editOk->disconnect();
    currentColor = Qt::white;
    colorPreview->setStyleSheet("border:1px solid black; background-color: #" + QString::number(currentColor.rgb(), 16).right(6) + ";");
    colorDialog->setCurrentColor(currentColor);
    connect(editOk, SIGNAL(clicked()), this, SLOT(confirmAddition()));
    eventEdit->setFocus();
    editDialog->show();
}
void Timeline::editFact()
{
    searchEdit->clear();
    pickList->clear();
    search("");
    pickOk->disconnect();
    connect(pickOk, SIGNAL(clicked()), this, SLOT(openEditing()));
    pickDialog->show();
}
void Timeline::removeFact()
{
    searchEdit->clear();
    pickList->clear();
    search("");
    pickOk->disconnect();
    connect(pickOk, SIGNAL(clicked()), this, SLOT(confirmDeletion()));
    pickDialog->show();
}
void Timeline::renameTimeline()
{
    bool ok;
    QString newName = QInputDialog::getText(this, tr("Timeline"), tr("New name:"), QLineEdit::Normal, name, &ok);
    if(ok && !newName.trimmed().isEmpty())
    {
        name = newName;
        setWindowTitle(tr("Timeline") + " - " + name);
        root.setAttribute("Name", name);
        save->setEnabled(true);
    }
}


void Timeline::setActivePeriod()
{
    if(beginDate->date().daysTo(endDate->date()) <= 0)
    {
        QMessageBox::warning(periodDialog, tr("Timeline"), tr("Please give an ending date after the begining one."));
        return;
    }
    periodDialog->close();
    timeline->setPeriod(beginDate->date(), endDate->date());
}

void Timeline::aboutApp()
{
    qApp->aboutQt();
}

void Timeline::closeEvent(QCloseEvent* event)
{
    if(save->isEnabled())
    {
        int answer = QMessageBox::question(this, tr("Timeline"), tr("Do you want to save the timeline %1?") .arg(name), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
        if(answer == QMessageBox::Save)
        {
            saveTimeline();
            event->accept();
            return;
        }
        if(answer == QMessageBox::Discard)
        {
            event->accept();
            return;
        }
        else if(answer == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
    }
    event->accept();
}

void Timeline::confirmAddition()
{
    QString tmpName = eventEdit->text().trimmed();
    QDate tmpDate = dateEdit->date();
    if(tmpName.isEmpty())
    {
        QMessageBox::warning(editDialog, tr("Timeline"), tr("Please enter a name."));
        return;
    }
    if(nameToId.count(tmpName) != 0)
    {
        QMessageBox::warning(editDialog, tr("Timeline"), tr("An event with this name already exists."));
        return;
    }
    editDialog->close();
    nameToId.insert(tmpName, id);
    dateToName[tmpDate] << tmpName;
    nameToColor.insert(tmpName, currentColor);
    QDomElement elt = domDocument.createElement("Event" + QString::number(id));
    elt.setAttribute("Name", tmpName);
    elt.setAttribute("Date", dateEdit->date().toString(Qt::ISODate));
    elt.setAttribute("Color", QString::number(currentColor.rgb(), 16));
    root.appendChild(elt);

    id++;

    save->setEnabled(true);
    editEvent->setEnabled(true);
    removeEvent->setEnabled(true);
    timeline->repaint();
}

void Timeline::openEditing()
{
    if(!pickList->currentItem()->isSelected())
    {
        QMessageBox::warning(pickDialog, tr("Timeline"), tr("Please chose an event to edit."));
        return;
    }
    QString selected = pickList->currentItem()->text();
    selected = selected.left(selected.size()-dateFormat.length()-1);
    editingIndex = nameToId[selected];
    pickDialog->close();

    QDate previous = QDate::fromString(pickList->currentItem()->text().right(dateFormat.length()), dateFormat);
    dateToName[previous].removeAll(selected);
    nameToId.remove(selected);
    currentColor = nameToColor[selected];
    colorPreview->setStyleSheet("border:1px solid black; background-color: #" + QString::number(currentColor.rgb(), 16).right(6) + ";");
    colorDialog->setCurrentColor(currentColor);

    dateEdit->setDate(previous);
    eventEdit->setText(selected);
    editOk->disconnect();
    connect(editOk, SIGNAL(clicked()), this, SLOT(confirmEditing()));
    eventEdit->setFocus();
    editDialog->show();
}

void Timeline::confirmEditing()
{
    QString tmpName = eventEdit->text().trimmed();
    QDate tmpDate = dateEdit->date();
    if(tmpName.isEmpty())
    {
        QMessageBox::warning(editDialog, tr("Timeline"), tr("Please enter a name."));
        return;
    }
    editDialog->close();

    QDomElement old = domDocument.elementsByTagName("Event" + QString::number(editingIndex)).at(0).toElement();

    QDate currentDate = QDate::fromString(old.attribute("Date"), Qt::ISODate);
    QString currentName = old.attribute("Name");

    QDomElement elt = old;
    elt.setAttribute("Name", tmpName);
    elt.setAttribute("Date", tmpDate.toString(Qt::ISODate));
    elt.setAttribute("Color", QString::number(currentColor.rgb(), 16).right(6));
    root.replaceChild(elt, old);

    dateToName[currentDate].removeAll(currentName);
    if(dateToName[currentDate].isEmpty())
        dateToName.remove(currentDate);
    dateToName[tmpDate] << tmpName;

    nameToId.remove(currentName);
    nameToId.insert(tmpName, id);

    nameToColor.remove(currentName);
    nameToColor.insert(tmpName, currentColor);
    save->setEnabled(true);
    timeline->repaint();
}

void Timeline::confirmDeletion()
{
    if(!pickList->currentItem()->isSelected())
    {
        QMessageBox::warning(pickDialog, tr("Timeline"), tr("Please chose an event to remove."));
        return;
    }
    QString selected = pickList->currentItem()->text();
    selected = selected.left(selected.size()-dateFormat.length()-1);
    QString selectedDate = pickList->currentItem()->text().left(dateFormat.length());
    pickDialog->close();
    int answer = QMessageBox::question(this, tr("Timeline"), tr("Do you want to remove the event \"%1\" (%2)?") .arg(selected) .arg(selectedDate));
    if(answer == QMessageBox::Yes)
    {
        editingIndex = nameToId[selected];
        QDomElement old = domDocument.elementsByTagName("Event" + QString::number(editingIndex)).at(0).toElement();
        root.removeChild(old);
        QDate current = QDate::fromString(selectedDate, dateFormat);
        dateToName[current].removeAll(selected);
        if(dateToName[current].isEmpty())
            dateToName.remove(current);
        nameToId.remove(selected);
        save->setEnabled(true);

        editEvent->setEnabled(!nameToId.isEmpty());
        removeEvent->setEnabled(!nameToId.isEmpty());
        return;
    }
    timeline->repaint();
}

void Timeline::search(QString string)
{
    pickList->clear();
    QMap<QDate, QStringList>::const_iterator end = dateToName.constEnd();
    for(QMap<QDate, QStringList>::Iterator it = dateToName.begin(); it != end; it++)
    {
        QStringList current = it.value().filter(string, Qt::CaseInsensitive);
        current.sort(Qt::CaseInsensitive);
        for(int i=0; i<current.size(); i++)
        {
            QListWidgetItem* item = new QListWidgetItem;
            item->setText(current[i] + " " + it.key().toString(dateFormat));
            item->setBackgroundColor(nameToColor[current[i]]);
            if(nameToColor[current[i]].valueF() < 0.5)
                item->setTextColor(Qt::white);
            else
                item->setTextColor(Qt::black);
            pickList->addItem(item);
        }
    }
}

void Timeline::periodChanged(QDate begin, QDate end)
{
    beginDate->setDate(begin);
    endDate->setDate(end);
}

void Timeline::changeColor(QColor color)
{
    currentColor = color;
    colorPreview->setStyleSheet("border:1px solid black; background-color: #" + QString::number(color.rgb(), 16).right(6) + ";");
}
