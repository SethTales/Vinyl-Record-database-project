#include <QWidget>

#include "tablewidget.h"

recordTable::recordTable(int rows, int columns, QWidget* parent)
    : QTableWidget(parent)
{
    setRowCount(rows);
    setColumnCount(columns);
    setColumnWidth(0, 444);
    setColumnWidth(1, 444);
    setColumnWidth(2, 208);
    setColumnWidth(3, 207);
    setColumnWidth(4, 207);
    setColumnWidth(5, 100);
}
