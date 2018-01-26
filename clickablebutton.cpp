
#include "clickablebutton.h"

clickablePushButton::clickablePushButton(const QString &text, QWidget *parent)
    : QPushButton(parent)
{
    setText(text);
    setFlat(true);
}
