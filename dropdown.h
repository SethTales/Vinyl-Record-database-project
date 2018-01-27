#ifndef DROPDOWN_H
#define DROPDOWN_H

#include <QComboBox>

class dropDownMenu : public QComboBox
{
    Q_OBJECT

public:
    explicit dropDownMenu(const QStringList& fields, QWidget* parent = 0);

};

#endif // DROPDOWN_H
