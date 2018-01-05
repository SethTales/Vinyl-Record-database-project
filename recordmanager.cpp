#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <iostream>
#include <string>
#include <vector>
#include <QVariant>

#include "recordmanager.h"
#include "button.h"
#include "dropdown.h"
#include "line_edit.h"
#include "tablewidget.h"

recordManager::recordManager(QWidget *parent)
    : QWidget(parent)
{
    QWidget::showMaximized(); //launch the app maximized
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

    searchFields *searchFieldsDropdown = createDropdown();

    Button *addNewRecord = createButton("Add New");
    Button *saveChanges = createButton("Save Changes");
    Button *deleteRecords = createButton("Delete");
    Button *search = createButton("Search");
    Button *editSelection = createButton("Edit Selection");

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
    std::cout << "deleteSelectionEntered" << std::endl;

    QList <QTableWidgetItem *> userSelectedItems;
    userSelectedItems = pointerToTable->selectedItems();
    QList <QTableWidgetSelectionRange> itemsToDelete;
    QTableWidgetItem *firstItem, *nextItem;
    int firstItemRow, nextItemRow, firstColumn, nextColumn;
    bool wholeRecord = true;

    if (userSelectedItems.length() == 0)
    {
        std::cout << "First if statement entered" << std::endl;
        std::cout << "Please select a single row (record) edit" << std::endl;
        wholeRecord = false;
    }

    else if (userSelectedItems.length() > 0 && userSelectedItems.length() < 5)
    {
        std::cout << "Second if statement entered" << std::endl;
        std::cout << "Please select at least one single row (record) to delete" << std::endl;
        wholeRecord = false;
        std::cout << "wholeRecord = " << wholeRecord << std::endl;

    }

    else if (userSelectedItems.length() >= 5)
    {
        std::cout << "Third if statement entered" << std::endl;

        if (userSelectedItems.length() % 5 != 0)
        {
            std::cout << "You can only delete whole records. Please select one or more whole rows to remove" << std::endl;
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
                        std::cout << "You can only delete whole records. Please select one or more whole rows to remove" << std::endl;
                        wholeRecord = false;
                        break;
                    }
                }
            }
        }
    }

    if (wholeRecord == true)
    {
       std::cout << "Entered nested if statement" << std::endl;

       for (int i = 0; i < (userSelectedItems.length() / 5); ++i)
       {

           std::cout << "userSelectedItems length = " << userSelectedItems.length() << std::endl;
           itemsToDelete = pointerToTable->selectedRanges();
           deleteBottomRow(itemsToDelete);

       }
    }
}

void recordManager::deleteBottomRow(QList<QTableWidgetSelectionRange> selectedRange)
{
    std::cout << "entered deleteBottomRow " << std::endl;
    int bRow = selectedRange.at(0).bottomRow();
    std::cout << "bottom row = " << bRow << std::endl;
    pointerToTable->removeRow(bRow);

}

void recordManager::searchClicked()
{

}




