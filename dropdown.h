#ifndef DROPDOWN_H
#define DROPDOWN_H

#include <QComboBox>

class searchFields : public QComboBox
{
    Q_OBJECT

public:
    explicit searchFields(const QStringList& fields, QWidget* parent = 0);

};

#endif // DROPDOWN_H
