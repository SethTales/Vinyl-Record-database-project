#ifndef CLICKABLEBUTTON_H
#define CLICKABLEBUTTON_H

#include <QObject>
#include <QPushButton>

class clickablePushButton : public QPushButton
{
    Q_OBJECT

public:
    clickablePushButton(const QString& text, QWidget* parent = 0);
};

#endif // CLICKABLEBUTTON_H
