#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QTableWidget>

class recordTable : public QTableWidget
{
    Q_OBJECT

public:
    explicit recordTable(int rows, int columns, QWidget* parent = 0);

};

#endif // TABLEWIDGET_H
