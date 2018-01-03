#ifndef BUTTON_H
#define BUTTON_H

#include <QToolButton>
#include <QLineEdit>

class Button : public QToolButton
{
    Q_OBJECT

public:
    explicit Button(const QString& text, QWidget *parent = 0);

    QSize sizeHint() const override;

private:
    void emitAddNewClicked();

};

#endif // BUTTON_H
