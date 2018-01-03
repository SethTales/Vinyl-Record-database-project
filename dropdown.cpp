
#include <QtWidgets>

#include "dropdown.h"

searchFields::searchFields(const QStringList& fields, QWidget* parent)
    : QComboBox(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    addItems(fields);
}

