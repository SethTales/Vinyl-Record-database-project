
#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <QWidget>
#include <QObject>
#include <QVector>
#include <string>
#include <vector>

class line_edit;
class Button;
class recordTable;
class searchFields;
class QStringList;

class recordManager : public QWidget
{
    Q_OBJECT

public:
    recordManager(QWidget *parent = 0);

signals:
    void addNewClickedSignal(QString newRecordData[]);

public slots:

private slots:
    void addNewClicked();
    void saveChangesClicked();
    void deleteSelectionClicked();
    void searchClicked();
    void editSelectionClicked();

    void addNewRecordToTable();

private:
    Button *createButton(const QString& text);
    recordTable *createTable(const char* member, Button *button);
    searchFields *createDropdown();
    line_edit *createDisplay(const QString& text, const char *member, Button* button);

    QString display[5] = { "Band Name", "Album Title", "Genre", "Year Released", "Record Label" };
    static QStringList searchFieldsList;

    QString recordData[5];

    QVector<line_edit*> userInputPointers;
    recordTable *pointerToTable;
    int editRow;

};


#endif // RECORDMANAGER_H
