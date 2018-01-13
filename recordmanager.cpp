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

recordManager::recordManager(QWidget *parent)
    : QWidget(parent)
{
    QWidget::showMaximized(); //launch the app maximized
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

    searchFields *searchFieldsDropdown = createDropdown();

    Button *addNewRecord = createButton("Add New");
    addNewRecord->setToolTip("Adds a new record to the bottom row of the table.");
    Button *saveChanges = createButton("Save Changes");
    saveChanges->setToolTip("Saves changes to an edited record.");
    Button *deleteRecords = createButton("Delete");
    deleteRecords->setToolTip("Deletes one or more selected records.");
    Button *search = createButton("Search");
    Button *editSelection = createButton("Edit Selection");
    editSelection->setToolTip("Allows editing of one selected record.");

    line_edit *bandName = createDisplay("Band Name", SLOT(addNewClicked()), addNewRecord);
    userInputPointers.append(bandName);
    line_edit *albumTitle = createDisplay("Album Title", SLOT(saveChangesClicked()), saveChanges);
    userInputPointers.append(albumTitle);
    line_edit *genre = createDisplay("Genre", SLOT(deleteSelectionClicked()), deleteRecords);
    userInputPointers.append(genre);
    line_edit *yearReleased = createDisplay("Year Released", SLOT(searchClicked()), editSelection);
    userInputPointers.append(yearReleased);
    line_edit *recordLabel = createDisplay("Record Label", SLOT(addNewRecordToTable()), addNewRecord);
    userInputPointers.append(recordLabel);
    line_edit *searchTerm = createDisplay("Search", SLOT(searchClicked()), search);

    recordTable *newTable = createTable(SLOT(editSelectionClicked()), editSelection);
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
    mainLayout->addWidget(deleteRecords, 2, 20, 1, 4);
    mainLayout->addWidget(search, 1, 8, 1, 4);
    mainLayout->addWidget(editSelection, 2, 0, 1, 4);

    //add drop down menu
    mainLayout->addWidget(searchFieldsDropdown, 1, 0, 1, 2);

    //add table widget
    mainLayout->addWidget(newTable, 3, 0, 20, 20);

    setLayout(mainLayout);

    dbService = new databaseService;
    recordEntries = dbService->readRecordsFromDB();
    fillTable();
}

recordManager::~recordManager()
{
    delete dbService;
}

Button *recordManager::createButton(const QString &text)
{
    Button *button = new Button(text);
    return button;
}

recordTable *recordManager::createTable(const char *member, Button *button)
{
    int rows = 0;
    int columns = 5;
    QStringList columnHeaders;
    columnHeaders << "Artist Name" << "Album Title" << "Genre" << "Year Released" << "Label";
    recordTable *table = new recordTable(rows, columns);
    QObject::connect(button, SIGNAL(clicked()), this, member);
    table->setHorizontalHeaderLabels(columnHeaders);
    return table;

}

searchFields *recordManager::createDropdown()
{
    QStringList searchFieldsList =(QStringList() << "Artist Name" << "Album Title" << "Genre" << "Year Released" << "Label");
    searchFields *dropDown = new searchFields(searchFieldsList);
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

        std::cout << "i = " << i << std::endl;
    }
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
    pointerToTable->insertRow(pointerToTable->rowCount());

    for (int i = 0; i < 5; ++i)
    {
        pointerToTable->setItem(pointerToTable->rowCount() - 1, i, new QTableWidgetItem(recordData[i]));

    }

    addNewRecordToList();

}

void recordManager::addNewRecordToList()
{

    recordEntry.bName = recordData[0];
    recordEntry.aTitle = recordData[1];
    recordEntry.genre = recordData[2];
    recordEntry.year = recordData[3];
    recordEntry.recLabel = recordData[4];

    recordEntries.append(recordEntry);

    dbService->addNewRecordToDB(recordEntry);

}

void recordManager::editSelectionClicked()
{
    //Make it so if Edit Selection is the last button clicked, then clicking Add New will not work, and will generate
    //some sort of warning/informative pop-up telling the user that they just clicked Edit selection and will
    //need to clear their inputs and enter a new record

    QList <QTableWidgetItem *> userSelectedItems;
    userSelectedItems = pointerToTable->selectedItems();
    QTableWidgetItem *firstItem, *nextItem;
    int firstItemRow, nextItemRow;
    QString textToEdit;
    bool singleRecord = true;

    //exception catching for when a user has no cells selected and presses Edit Selection button
    //change this to a dialag box warning
    if (userSelectedItems.length() == 0)
    {
        std::cout << "Please selet a single row (record) edit" << std::endl;
        singleRecord = false;
    }

    //excpetion catching for when a user as multiple cells from different rows (records) selected,
    //but does not have a single, whole row (record) selected
    //change this to a dialog box warning
    else if (userSelectedItems.length() > 0 && userSelectedItems.length() < 6)
    {
        firstItem = userSelectedItems.first();
        firstItemRow = firstItem->row();

        for (int i = 1; i < userSelectedItems.length(); ++i)
        {
            nextItem = userSelectedItems.at(i);
            nextItemRow = nextItem->row();
            if (nextItemRow != firstItemRow)
            {
                std::cout << "Please select a single row (record) to edit" << std::endl;
                singleRecord = false;
                break;
            }
        }
    }

    //exception catching for if a user has a whole row (record) and at least one more cell selected
    //change this to a dialog box warning
    else if (userSelectedItems.length() >= 6)
    {
        std::cout << "You can only edit 1 record at a time." << std::endl;
        singleRecord = false;
    }

    //statement to replace input text boxes with the current selection if a single, whole row is selected
    //maybe generate a message saying "Selection Edited Succesfully" or something like that
    if (userSelectedItems.length() == 5 && singleRecord == true)
    {
        std::cout << "Entered last else statement" << std::endl;
        for (int i = 0; i < userSelectedItems.length(); ++i)
        {
            textToEdit = userSelectedItems[i]->text();
            editRow = pointerToTable->row(userSelectedItems[i]);
            userInputPointers[i]->setText(textToEdit);
        }

    std::cout << "End of editSelectionClicked " << std::endl;
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
}

void recordManager::deleteSelectionClicked()
{
    QList <QTableWidgetItem *> userSelectedItems;
    userSelectedItems = pointerToTable->selectedItems();
    QList <QTableWidgetSelectionRange> itemsToDelete;
    QTableWidgetItem *firstItem, *nextItem;
    int firstItemRow, nextItemRow;
    bool wholeRecord = true;

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
    pointerToTable->removeRow(bRow);

}

void recordManager::searchClicked()
{

}




