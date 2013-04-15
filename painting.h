#ifndef PAINTING_H
#define PAINTING_H

#include <QtWidgets>

class Painting : public QWidget
{
    Q_OBJECT
public:
    explicit Painting(QWidget *parent = 0);
    void setMap(QMap<QDate, QStringList>* map);
    void setHash(QHash<QString, QColor>* hash);

public slots:
    void zoomIn();
    void zoomOut();
    void setPeriod(QDate begin, QDate end);

signals:
    void periodChanged(QDate, QDate);

private:
    QMap<QDate, QStringList>* dateToName;
    QHash<QString, QColor>* nameToColor;
    QDate beginDate;
    QDate endDate;

    int diffDays;
    bool moving;
    int mouseX;
    int lastX;
    QDate movingReference;
    double dayLength;

protected:
    virtual void paintEvent(QPaintEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

};

#endif // PAINTING_H
