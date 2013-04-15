#include "painting.h"

Painting::Painting(QWidget *parent) :
    QWidget(parent)
{
    dateToName = NULL;
    nameToColor = NULL;

    beginDate = QDate(1900, 1, 1);
    endDate = QDate::currentDate();
    diffDays = beginDate.daysTo(endDate);
    moving = false;
}

void Painting::setMap(QMap<QDate, QStringList> *map)
{
    dateToName = map;
}

void Painting::setHash(QHash<QString, QColor> *hash)
{
    nameToColor = hash;
}

void Painting::paintEvent(QPaintEvent *event)
{
    const int mWidth = width();
    const int mHeight = height();
    QPainter painter(this);
    QFont font("sans");
    font.setPointSize(7);
    painter.setFont(font);

    painter.setPen(Qt::gray);
    painter.drawLine(15, mHeight - 35, mWidth - 15, mHeight - 35);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setPen(QColor(64,64,64));
    painter.drawLine(20, mHeight - 40, 20, mHeight - 30);
    painter.drawText(QRect(0, height()-30, 40, 30), Qt::AlignCenter, beginDate.toString(tr("MMM, d\nyyyy")));

    painter.setPen(Qt::black);

    if(dateToName && !dateToName->isEmpty())
    {
        const int length = mWidth - 40;
        const double angle = 25*(3.14159265358979323846/180);
        const double labelLength = 150;
        const double labelHeight = 30;
        const double space = 10;
        const double dx = space*qSin(angle);
        const double dy = space*qCos(angle);
        const double labelX = (labelHeight+space)*qSin(angle);
        const double labelY = (labelHeight+space)*qCos(angle);
        const double labelSideX = labelLength*qCos(angle);
        const double labelSideY = labelLength*qSin(angle);
        const double vOffset = (labelSideY+dy-labelY)*qCos(angle);

        double offset = 0;

        const QMap<QDate,QStringList>::Iterator begin = dateToName->lowerBound(beginDate);
        const QMap<QDate, QStringList>::Iterator end = dateToName->upperBound(endDate);

        for(QMap<QDate, QStringList>::Iterator it = end; it != begin;)
        {
            it--;
            double current = 20+(length*beginDate.daysTo(it.key()))/diffDays;

            painter.setBrush(QBrush(Qt::white));
            painter.setPen(Qt::black);

            painter.drawEllipse(QPointF(current, mHeight-35), 3, 3);
            painter.drawText(QRect(current-20, mHeight-30, 40, 30), Qt::AlignCenter, it.key().toString(tr("MMM, d\nyyyy")));

            if(current-dx+labelSideX+1 >= mWidth)
                continue;

            if(it != end-1)
            {
                const double previous = 20+(length*beginDate.daysTo((it+1).key()))/diffDays;
                const double difference = previous-current;
                if(previous-dx+labelSideX+1 >= mWidth || (it+1).value().size()*labelX <= difference - (labelSideX-dx))
                    offset = 0;
                else
                    offset += vOffset - (labelSideY+dy-(it+1).value().size()*labelY - ((it+1).value().size()*labelX+labelSideX-dx-difference)*qTan(angle))*qCos(angle);
            }
            else
                offset = 0;

            painter.drawLine(QLineF(current, mHeight-60, current, mHeight-38));

            QStringList list = it.value();

            QTransform transform;
            transform.translate(current, mHeight-60);
            transform.rotateRadians(-angle, Qt::ZAxis);

            painter.setTransform(transform);
            font.setPointSize(10);
            painter.setFont(font);

            painter.drawLine(0., 0., 0., -offset);

            for(int i=0; i<list.size(); i++)
            {
                painter.setPen(Qt::black);
                const double spacing = (labelHeight+space)*i;

                QColor currentColor = nameToColor->value(list.at(i));
                painter.setBrush(QBrush(currentColor));

                painter.drawLine(0., -spacing-offset-space, 0., -spacing+space-offset-space);
                QRectF rect(0., -labelHeight-spacing-offset-space, labelLength, labelHeight);
                painter.drawRect(rect);

                if(currentColor.valueF() < 0.5)
                    painter.setPen(Qt::white);
                else
                    painter.setPen(Qt::black);

                painter.drawText(rect, Qt::AlignVCenter, " " + list.at(i));
            }

            painter.setTransform(QTransform());
            font.setPointSize(7);
            painter.setFont(font);
        }
    }

    painter.setPen(QColor(192,0,0));
    painter.drawLine(mWidth - 20, mHeight - 40, mWidth - 20, mHeight - 30);

    painter.drawText(QRect(mWidth-40, mHeight-30, 40, 30), Qt::AlignCenter, endDate.toString(tr("MMM, d\nyyyy")));
    event->accept();
}

void Painting::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        moving = true;
        movingReference = beginDate;
        dayLength = ((double)diffDays)/(width()-40);
        mouseX = event->x();
    }
    event->accept();
}

void Painting::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        moving = false;
    event->accept();
}

void Painting::mouseMoveEvent(QMouseEvent *event)
{
    if(!moving)
    {
        event->accept();
        return;
    }
    lastX = event->x();
    beginDate = movingReference.addDays(qRound(dayLength*(mouseX - lastX)));
    endDate = beginDate.addDays(diffDays);
    emit periodChanged(beginDate, endDate);
    repaint();
}

void Painting::zoomIn()
{
    const int variation = beginDate.daysTo(endDate)/20;
    beginDate = beginDate.addDays(variation);
    endDate = endDate.addDays(-variation);
    diffDays -= 2*variation;
    emit periodChanged(beginDate, endDate);
    repaint();
}

void Painting::zoomOut()
{
     const int variation = beginDate.daysTo(endDate)/18;
     beginDate = beginDate.addDays(-variation);
     endDate = endDate.addDays(variation);
     diffDays += 2*variation;
     emit periodChanged(beginDate, endDate);
     repaint();
}

void Painting::setPeriod(QDate begin, QDate end)
{
    beginDate = begin;
    endDate = end;
    diffDays = beginDate.daysTo(endDate);
    if(moving)
    {
        movingReference = beginDate;
        mouseX = lastX;
        dayLength = ((double)diffDays)/(width()-40);
    }
    repaint();
}

