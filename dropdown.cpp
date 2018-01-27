
#include <QtWidgets>

#include "dropdown.h"

dropDownMenu::dropDownMenu(const QStringList& fields, QWidget* parent)
    : QComboBox(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    addItems(fields);
}

