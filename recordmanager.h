
#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <QWidget>
#include <QObject>
#include <QVector>
#include <string>
#include <vector>
#include <QTableWidgetSelectionRange>
#include <QDialog>

#include "recordentryfordb.h"
#include "dbmanager.h"

class Button;
class line_edit;
class recordTable;
class dropDownMenu;
class databaseService;

class recordManager : public QDialog
{
    Q_OBJECT

public:
    //recordManager(QWidget *parent = 0);
    recordManager(databaseService&, QDialog *parent = 0);
    ~recordManager();

    databaseService& _refToDBServInRecMgr;

signals:
    void addNewClickedSignal(QString newRecordData[]);

public slots:

private slots:
    void addNewClicked();
    void saveChangesClicked();
    void deleteSelectionClicked();
    void searchClicked();
    void editSelectionClicked();
    void clearEntriesClicked();
    void clearSearchResultsClicked();
    void addNewRecordToTable();
    void logoutClicked();
    void changeLibraryClicked();


private:
    //application widget declarations
    Button *addNewRecord, *saveChanges, *deleteRecords, *search, *editSelection, *clearEntries, *clearSearchResults,
           *logoutButton, *changeLibraryButton;
    Button *createButton(const QString& text);
    recordTable *createTable(const char* member, const char* member2, Button *button, Button *button2);
    dropDownMenu *createDropdown();
    line_edit *createDisplay(const QString& text, const char *member, Button* button);
    bool editSelectionLastClicked = false;
    QVector<record> searchResultsList;
    //int tableCount;

    //provides string values for input line edits and search field drop-down
    QString display[5] = { "Band Name", "Album Title", "Genre", "Year Released", "Record Label" };
    static QStringList searchFieldsList;

    //provides a way for captured record data to be accessed throughout the class
    QString recordData[5];

    //allows input line edit objects and table object to be directly accessed within the class
    QVector<line_edit*> userInputPointers;
    recordTable *pointerToTable;
    line_edit* searchTermPointer;
    dropDownMenu* searchMenuPointer;
    int editRow;

    //function to remove entries from the table
    void deleteBottomRow(QList <QTableWidgetSelectionRange>);

    //variables and functions to access dbmanager class
    record recordEntry;
    QList <record> recordEntries;

    void addNewRecordToList();
    void fillTable();
    void displaySearchResults();
    //bool isTableEmpty();

};


#endif // RECORDMANAGER_H
