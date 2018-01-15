
#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <QWidget>
#include <QObject>
#include <QVector>
#include <string>
#include <vector>
#include <QTableWidgetSelectionRange>

#include "recordentryfordb.h"
#include "dbmanager.h"

class line_edit;
class Button;
class recordTable;
class searchFields;
class QStringList;
class databaseService;

class recordManager : public QWidget
{
    Q_OBJECT

public:
    recordManager(QWidget *parent = 0);
    ~recordManager();

    //only instance of databaseService class
    databaseService *dbService;


    //recordManager *pointerToRecMgr;

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
    //application widget declarations
    Button *createButton(const QString& text);
    recordTable *createTable(const char* member, Button *button);
    searchFields *createDropdown();
    line_edit *createDisplay(const QString& text, const char *member, Button* button);

    //provides string values for input line edits and search field drop-down
    QString display[5] = { "Band Name", "Album Title", "Genre", "Year Released", "Record Label" };
    static QStringList searchFieldsList;

    //provides a way for captured record data to be accessed throughout the class
    QString recordData[5];

    //allows input line edit objects and table object to be directly accessed within the class
    QVector<line_edit*> userInputPointers;
    recordTable *pointerToTable;
    int editRow;

    //function to remove entries from the table
    void deleteBottomRow(QList <QTableWidgetSelectionRange>);

    //variables and functions to access dbmanager class
    record recordEntry;
    QList <record> recordEntries;

    void addNewRecordToList();
    void fillTable();

};


#endif // RECORDMANAGER_H
