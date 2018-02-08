#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <iostream>
#include <string>
#include <vector>
#include <QVariant>
#include <QMessageBox>

#include "recordmanager.h"
#include "button.h"
#include "dropdown.h"
#include "line_edit.h"
#include "tablewidget.h"
#include "recordentryfordb.h"

recordManager::recordManager(databaseService& _refToDBServInConst, QWidget *parent)
    :_refToDBServInRecMgr(_refToDBServInConst), QWidget(parent)
{
    QWidget::showMaximized(); //launch the app maximized
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

    dropDownMenu *searchFieldsDropdown = createDropdown();
    searchMenuPointer = searchFieldsDropdown;

    Button *addNewRecord = createButton("Add New");
    addNewRecord->setToolTip("Adds a new record to the bottom row of the table.");
    Button *saveChanges = createButton("Save Changes");
    saveChanges->setToolTip("Saves changes to an edited record.");
    Button *deleteRecords = createButton("Delete Selection");
    deleteRecords->setToolTip("Deletes one or more selected records.");
    Button *search = createButton("Search");
    Button *editSelection = createButton("Edit Selection");
    editSelection->setToolTip("Allows editing of one selected record.");
    Button *clearEntries = createButton("Clear Input");
    clearEntries->setToolTip("Clears input boxes");
    Button *clearSearchResults = createButton ("Clear Search Results");
    clearSearchResults->setToolTip("Clears results of search and returns to main library");

    line_edit *bandName = createDisplay("Band Name", SLOT(addNewClicked()), addNewRecord);
    userInputPointers.append(bandName);
    line_edit *albumTitle = createDisplay("Album Title", SLOT(saveChangesClicked()), saveChanges);
    userInputPointers.append(albumTitle);
    line_edit *genre = createDisplay("Genre", SLOT(deleteSelectionClicked()), deleteRecords);
    userInputPointers.append(genre);
    line_edit *yearReleased = createDisplay("Year Released", SLOT(clearEntriesClicked()), clearEntries);
    userInputPointers.append(yearReleased);
    line_edit *recordLabel = createDisplay("Record Label", SLOT(addNewRecordToTable()), addNewRecord);
    userInputPointers.append(recordLabel);
    line_edit *searchTerm = createDisplay("Search", SLOT(searchClicked()), search);
    searchTermPointer = searchTerm;

    recordTable *newTable = createTable(SLOT(editSelectionClicked()), SLOT(clearSearchResultsClicked()),
                                        editSelection, clearSearchResults);
    newTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //make table read-only
    pointerToTable = newTable;

    //add line edit objects
    mainLayout->addWidget(bandName, 0, 0, 1, 4);
    mainLayout->addWidget(albumTitle, 0, 4, 1, 4);
    mainLayout->addWidget(genre, 0, 8, 1, 4);
    mainLayout->addWidget(yearReleased, 0, 12, 1, 4);
    mainLayout->addWidget(recordLabel, 0, 16, 1, 4);
    mainLayout->addWidget(searchTerm, 1, 2, 1, 6);

    //add buttons
    mainLayout->addWidget(addNewRecord, 0, 20, 1, 4);
    mainLayout->addWidget(saveChanges, 1, 20, 1, 4);
    mainLayout->addWidget(deleteRecords, 4, 20, 1, 4);
    mainLayout->addWidget(search, 1, 8, 1, 4);
    mainLayout->addWidget(clearSearchResults, 1, 12, 1, 4);
    mainLayout->addWidget(editSelection, 2, 0, 1, 4);
    mainLayout->addWidget(clearEntries, 2, 20, 1, 4);

    //add drop down menu
    mainLayout->addWidget(searchFieldsDropdown, 1, 0, 1, 2);

    //add table widget
    mainLayout->addWidget(newTable, 3, 0, 20, 20);

    setLayout(mainLayout);

    bool isEmpty;
    isEmpty = _refToDBServInRecMgr.isTableEmpty();
    if (isEmpty == false)
    {
        recordEntries = _refToDBServInRecMgr.readRecordsFromDB();
        fillTable();
    }

}

recordManager::~recordManager()
{

}

Button *recordManager::createButton(const QString &text)
{
    Button *button = new Button(text);
    return button;
}

recordTable *recordManager::createTable(const char *member, const char *member2, Button *button, Button *button2)
{
    int rows = 0;
    int columns = 6;
    QStringList columnHeaders;
    columnHeaders << "Artist Name" << "Album Title" << "Genre" << "Year Released" << "Label" << "ID";
    recordTable *table = new recordTable(rows, columns);
    QObject::connect(button, SIGNAL(clicked()), this, member);
    QObject::connect(button2, SIGNAL(clicked()), this, member2);
    table->setHorizontalHeaderLabels(columnHeaders);
    table->setSortingEnabled(false);
    table->setColumnHidden(5, true);
    return table;

}

dropDownMenu *recordManager::createDropdown()
{
    QStringList searchFieldsList =(QStringList() << "Select Column" << "Artist Name" << "Album Title" << "Genre" << "Year Released" << "Label");
    dropDownMenu *dropDown = new dropDownMenu(searchFieldsList);
    dropDown->setFixedHeight(25);

    //makes first item in drop down "Select Column" unselectable, i.e. just a display name
    QStandardItemModel *dropdownModel = qobject_cast<QStandardItemModel*>(dropDown->model());
    QModelIndex firstIndex = dropdownModel->index(0, dropDown->modelColumn(), dropDown->rootModelIndex());
    QStandardItem *firstItem = dropdownModel->itemFromIndex(firstIndex);
    firstItem->setSelectable(false);

    return dropDown;
}

line_edit *recordManager::createDisplay(const QString &text, const char *member, Button *button)
{
    line_edit *newLine = new line_edit(text);
    QObject::connect(button, SIGNAL(clicked()), this, member);
    QFont font = newLine->font();
    font.setPointSize(font.pointSize() + 4);
    newLine->setFont(font);
    return newLine;
}

void recordManager::fillTable()
{
    for (int i = 0; i < recordEntries.length(); ++i) //rows
    {
        pointerToTable->insertRow(pointerToTable->rowCount());
        pointerToTable->setItem(i, 0, new QTableWidgetItem(recordEntries.at(i).bName));
        pointerToTable->setItem(i, 1, new QTableWidgetItem(recordEntries.at(i).aTitle));
        pointerToTable->setItem(i, 2, new QTableWidgetItem(recordEntries.at(i).genre));
        pointerToTable->setItem(i, 3, new QTableWidgetItem(recordEntries.at(i).year));
        pointerToTable->setItem(i, 4, new QTableWidgetItem(recordEntries.at(i).recLabel));
        pointerToTable->setItem(i, 5, new QTableWidgetItem(QString::number(recordEntries.at(i).ID)));
    }

    pointerToTable->setSortingEnabled(true);
}

//slot and function to add new record to the table - triggered by clicked() slot from Add New button
void recordManager::addNewClicked()
{
    line_edit *capture;

    for (int i = 0; i < 5; i++)
    {
        capture = userInputPointers[i];
        recordData[i] = capture->text();
    }
}

void recordManager::addNewRecordToTable()
{
    bool noText = true;
    bool isEmpty = false;
    if (recordEntries.length() == 0)
    {
        isEmpty = _refToDBServInRecMgr.isTableEmpty();
    }

    if (editSelectionLastClicked == true)
    {
        QMessageBox messageBox;
        messageBox.setText("You are about to add a record you are editing. Are you sure you want to continue?");
        messageBox.setInformativeText("Clicking OK will add what is in the input boxes to your library");
        messageBox.setWindowTitle("Caution - Editing a Record");
        messageBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        int deleteBoxReturn = messageBox.exec();

        switch (deleteBoxReturn)
        {
        case QMessageBox::Cancel:
        {
            for (int i = 0; i < 5; ++i)
            {
                userInputPointers[i]->clear();
            }
            editSelectionLastClicked = false;
            return;
        case QMessageBox::Ok:
            {
                editSelectionLastClicked = false;
            }

        }
        }
    }

    for (int i = 0; i < 5; ++i)
    {
        if (recordData[i] != "")
        {
            noText = false;
        }
    }

    if (noText == true)
    {
        QMessageBox messageBox;
        messageBox.setText("No record data to add");
        messageBox.setInformativeText("It looks like there is no text in the input boxes");
        messageBox.setWindowTitle("No Record Available");
        messageBox.exec();
        return;
    }

    else if (noText == false)
    {
        pointerToTable->insertRow(pointerToTable->rowCount());

        for (int i = 0; i < 5; ++i)
        {
            pointerToTable->setItem(pointerToTable->rowCount() - 1, i, new QTableWidgetItem(recordData[i]));

        }

        if (isEmpty == true)
        {
            pointerToTable->setItem(pointerToTable->rowCount() - 1, 5,
                                    new QTableWidgetItem(QString::number(1)));
        }

        else if (isEmpty == false)
        {
            pointerToTable->setItem(pointerToTable->rowCount() - 1, 5,
                                new QTableWidgetItem(QString::number(recordEntries.last().ID + 1)));
        }

        for (int i = 0; i < 5; ++i)
        {
            userInputPointers[i]->clear();
        }

        addNewRecordToList();
    }
}

void recordManager::addNewRecordToList()
{
    bool isEmpty = false;

    if (recordEntries.length() == 0)
    {
        isEmpty = _refToDBServInRecMgr.isTableEmpty();
    }


    if (isEmpty == true)
    {
        recordEntry.ID = 1;
        recordEntry.bName = recordData[0];
        recordEntry.aTitle = recordData[1];
        recordEntry.genre = recordData[2];
        recordEntry.year = recordData[3];
        recordEntry.recLabel = recordData[4];
    }

    else if (isEmpty == false)
    {
        recordEntry.ID = recordEntries.last().ID + 1;
        recordEntry.bName = recordData[0];
        recordEntry.aTitle = recordData[1];
        recordEntry.genre = recordData[2];
        recordEntry.year = recordData[3];
        recordEntry.recLabel = recordData[4];
    }

    recordEntries.append(recordEntry);

    _refToDBServInRecMgr.addNewRecordToDB(recordEntry);

}

void recordManager::editSelectionClicked()
{
    //Make it so if Edit Selection is the last button clicked, then clicking Add New will not work, and will generate
    //some sort of warning/informative pop-up telling the user that they just clicked Edit selection and will
    //need to clear their inputs and enter a new record

    editSelectionLastClicked = true;
    QList <QTableWidgetItem *> userSelectedItems;
    userSelectedItems = pointerToTable->selectedItems();
    QTableWidgetItem *firstItem, *nextItem;
    int firstItemRow, nextItemRow;
    QString textToEdit;
    bool singleRecord = true;

    //exception catching for when a user has no cells selected and presses Edit Selection button
    if (userSelectedItems.length() == 0)
    {
        QMessageBox messageBox;
        messageBox.setText("Please selet a single row (record) edit");
        messageBox.setWindowTitle("No record selected");
        messageBox.exec();
        singleRecord = false;
    }

    //excpetion catching for when a user as multiple cells from different rows (records) selected,
    //but does not have a single, whole row (record) selected
    else if (userSelectedItems.length() > 0 && userSelectedItems.length() < 6)
    {
        bool sameRow = true;
        firstItem = userSelectedItems.first();
        firstItemRow = firstItem->row();

        for (int i = 1; i < userSelectedItems.length(); ++i)
        {
            nextItem = userSelectedItems.at(i);
            nextItemRow = nextItem->row();
            if (nextItemRow != firstItemRow)
            {
                QMessageBox messageBox;
                messageBox.setText("Please select a single row (record) to edit");
                messageBox.setInformativeText("It looks like you have more than one record selected");
                messageBox.setWindowTitle("Multiple records selected");
                messageBox.exec();
                singleRecord = false;
                sameRow = false;
                break;
            }
        }
            //exception handling for when a user selects multiple cells from the same row, but not a whole row
            if (sameRow == true && userSelectedItems.length() != 5)
            {
                QMessageBox messageBox;
                messageBox.setText("Please select a single row (record) to edit");
                messageBox.setInformativeText("It looks like you don't have a whole record selected");
                messageBox.setWindowTitle("Partial record selected");
                messageBox.exec();
                singleRecord = false;
            }
}

    //exception catching for if a user has a whole row (record) and at least one more cell selected
    else if (userSelectedItems.length() >= 6)
    {
        QMessageBox messageBox;
        messageBox.setText("Please select a single row (record) to edit");
        messageBox.setInformativeText("It looks like you have more than one record selected");
        messageBox.setWindowTitle("Multiple records selected");
        messageBox.exec();
        singleRecord = false;
    }

    if (userSelectedItems.length() == 5 && singleRecord == true)
    {
        for (int i = 0; (i < userSelectedItems.length() - 1); ++i)
        {
            textToEdit = userSelectedItems[i]->text();
            editRow = pointerToTable->row(userSelectedItems[i]);
            userInputPointers[i]->setText(textToEdit);
        }
    }
}
void recordManager::saveChangesClicked()
{
    line_edit *editCapture;

    for (int i = 0; i < 5; i++)
    {
        editCapture = userInputPointers[i];
        recordData[i] = editCapture->text();
        pointerToTable->setItem(editRow, i, new QTableWidgetItem(recordData[i]));
    }

    recordEntry.bName = userInputPointers[0]->text();
    recordEntry.aTitle = userInputPointers[1]->text();
    recordEntry.genre = userInputPointers[2]->text();
    recordEntry.year = userInputPointers[3]->text();
    recordEntry.recLabel = userInputPointers[4]->text();
    recordEntry.ID = pointerToTable->item(editRow, 5)->text().toInt();

    _refToDBServInRecMgr.updateRecordInDB(recordEntry);

    for (int i = 0; i < 5; ++i)
    {
        userInputPointers[i]->clear();
    }

    QMessageBox messageBox;
    messageBox.setText("Record updated succesfully");
    messageBox.setWindowTitle(" ");
    messageBox.exec();

    editSelectionLastClicked = false;
}

void recordManager::deleteSelectionClicked()
{
    QList <QTableWidgetItem *> userSelectedItems;
    userSelectedItems = pointerToTable->selectedItems();
    QList <QTableWidgetSelectionRange> itemsToDelete;
    QTableWidgetItem *firstItem, *nextItem;
    int firstItemRow, nextItemRow;
    bool wholeRecord = true;

    if (editSelectionLastClicked == true)
    {
        QMessageBox messageBox;
        messageBox.setText("Cannot delete a record you are editing");
        messageBox.setInformativeText("We will clear the input fields so you can remove records");
        messageBox.setWindowTitle("Cannot Delete Record");
        messageBox.exec();
        for (int i = 0; i < 5; ++i)
        {
            userInputPointers[i]->clear();
        }
        editSelectionLastClicked = false;
        return;
    }

    if (userSelectedItems.length() == 0)
    {
        QMessageBox messageBox;
        messageBox.setText("Please select at least one single record (row) to delete.");
        messageBox.setWindowTitle("Cannot Delete Records");
        messageBox.exec();
        wholeRecord = false;
    }

    else if (userSelectedItems.length() > 0 && userSelectedItems.length() < 5)
    {
        QMessageBox messageBox;
        messageBox.setText("It looks like you have 1 or more cell selected, but not a whole row.\nPlease select at least one whole record (row) to delete");
        messageBox.setWindowTitle("Cannot Delete Records");
        messageBox.exec();
        wholeRecord = false;

    }

    else if (userSelectedItems.length() >= 5)
    {
        if (userSelectedItems.length() % 5 != 0)
        {
            QMessageBox messageBox;
            messageBox.setText("You can only delete whole records.\nPlease select one or more whole records (rows) to remove.");
            messageBox.setWindowTitle("Cannot Delete Records");
            messageBox.exec();
            wholeRecord = false;
        }

        else if (userSelectedItems.length() % 5 == 0)
        {
            firstItem = userSelectedItems.first();
            firstItemRow = firstItem->row();

            for (int n = 0; n < (userSelectedItems.length() / 5); ++n)
            {
                for (int i = 0; i < 5; ++i)
                {
                    nextItem = userSelectedItems.at(i);
                    nextItemRow = nextItem->row();
                    if (nextItemRow != firstItemRow)
                    {
                        QMessageBox messageBox;
                        messageBox.setText("You can only delete whole records.\nPlease select one or more whole records (rows) to remove.");
                        messageBox.setWindowTitle("Cannot Delete Records");
                        messageBox.exec();
                        wholeRecord = false;
                        break;
                    }
                }
            }
        }
    }

    if (wholeRecord == true)
    {
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setText("You are about to permanently delete records.");
        messageBox.setInformativeText("Do you wish to proceed?");
        messageBox.setWindowTitle("WARNING");
        messageBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        int deleteBoxReturn = messageBox.exec();

        switch (deleteBoxReturn)
        {
        case QMessageBox::Cancel:
           {
               return;
           }
        case QMessageBox::Ok:
           {
               for (int i = 0; i < (userSelectedItems.length() / 5); ++i)
               {

                   itemsToDelete = pointerToTable->selectedRanges();
                   deleteBottomRow(itemsToDelete);
               }
           }
        }
    }
}

void recordManager::deleteBottomRow(QList<QTableWidgetSelectionRange> selectedRange)
{
    int bRow = selectedRange.at(0).bottomRow();
    int ID = pointerToTable->item(bRow, 5)->text().toInt();
    std::cout << "in deleteBottomRow ID = " << ID << std::endl;
    pointerToTable->removeRow(bRow);

    _refToDBServInRecMgr.deleteRecordFromDB(ID);
}

void recordManager::clearEntriesClicked()
{
    for (int i = 0; i < 5; ++i)
    {
        userInputPointers[i]->clear();
    }

    editSelectionLastClicked = false;
}

void recordManager::searchClicked()
{
    if (searchResultsList.length() > 0)
    {
        searchResultsList.clear();
        searchResultsList.squeeze();
    }

    int searchFieldsIndex;
    line_edit* searchTermCapture;
    QString searchTerm, compareTerm;

    searchFieldsIndex = searchMenuPointer->currentIndex();

    if (searchFieldsIndex == 0)
    {
        QMessageBox messageBox;
        messageBox.setText("Please select a valid search field");
        messageBox.setInformativeText("It looks like you didn't select a column to search on in the dropdown menu");
        messageBox.setWindowTitle("Invalid Search Field");
        messageBox.exec();
        return;
    }

    searchTermCapture = searchTermPointer;
    searchTerm = searchTermCapture->text();
    std::cout << searchTerm.toStdString() << std::endl;

    switch (searchFieldsIndex)
    {
    case 1:
    {
        for (int i = 0; i < recordEntries.length(); ++i)
        {
            compareTerm = recordEntries[i].bName;
            if (compareTerm.contains(searchTerm, Qt::CaseInsensitive))
            {
                searchResultsList.append(recordEntries[i]);
            }
        }
        break;
    }
    case 2:
    {
        for (int i = 0; i < recordEntries.length(); ++i)
        {
            compareTerm = recordEntries[i].aTitle;
            if (compareTerm.contains(searchTerm, Qt::CaseInsensitive))
            {
                searchResultsList.append(recordEntries[i]);
            }
        }
        break;
    }
    case 3:
    {
        for (int i = 0; i < recordEntries.length(); ++i)
        {
            compareTerm = recordEntries[i].genre;
            if (compareTerm.contains(searchTerm, Qt::CaseInsensitive))
            {
                searchResultsList.append(recordEntries[i]);
            }
        }
        break;
    }
    case 4:
    {
        for (int i = 0; i < recordEntries.length(); ++i)
        {
            compareTerm = recordEntries[i].year;
            if (compareTerm.contains(searchTerm, Qt::CaseInsensitive))
            {
                searchResultsList.append(recordEntries[i]);
            }
        }
        break;
    }
    case 5:
    {
        for (int i = 0; i < recordEntries.length(); ++i)
        {
            compareTerm = recordEntries[i].recLabel;
            if (compareTerm.contains(searchTerm, Qt::CaseInsensitive))
            {
                searchResultsList.append(recordEntries[i]);
            }
        }
        break;
    }
    }

    displaySearchResults();
}

void recordManager::displaySearchResults()
{
    pointerToTable->clearContents();
    pointerToTable->setSortingEnabled(false);

    for (int i = pointerToTable->rowCount(); i >= 0; --i)
    {
        pointerToTable->removeRow(i);
    }

    for (int i = 0; i < searchResultsList.length(); ++i) //rows
    {
        pointerToTable->insertRow(pointerToTable->rowCount());
        pointerToTable->setItem(i, 0, new QTableWidgetItem(searchResultsList.at(i).bName));
        pointerToTable->setItem(i, 1, new QTableWidgetItem(searchResultsList.at(i).aTitle));
        pointerToTable->setItem(i, 2, new QTableWidgetItem(searchResultsList.at(i).genre));
        pointerToTable->setItem(i, 3, new QTableWidgetItem(searchResultsList.at(i).year));
        pointerToTable->setItem(i, 4, new QTableWidgetItem(searchResultsList.at(i).recLabel));
        pointerToTable->setItem(i, 5, new QTableWidgetItem(QString::number(searchResultsList.at(i).ID)));
    }

    pointerToTable->setSortingEnabled(true);

}

void recordManager::clearSearchResultsClicked()
{
    pointerToTable->setSortingEnabled(false);
    pointerToTable->clearContents();

    for (int i = searchResultsList.length(); i >= 0; --i)
    {
        pointerToTable->removeRow(i);
    }

    fillTable();
}




