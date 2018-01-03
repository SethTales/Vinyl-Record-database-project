
#include <QWidget>
#include <QFont>

#include "line_edit.h"

line_edit::line_edit(const QString& text, QWidget *parent)
    : QLineEdit(parent)
{
    setPlaceholderText(text);

    setAlignment(Qt::AlignLeft);
}
