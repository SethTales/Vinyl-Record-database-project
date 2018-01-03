#ifndef LINE_EDIT_H
#define LINE_EDIT_H

#include <QLineEdit>
#include <QObject>

class line_edit : public QLineEdit
{
    Q_OBJECT

public:
    explicit line_edit(const QString& text, QWidget *parent = 0);


};

#endif // LINE_EDIT_H
