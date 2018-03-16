# Vinyl-Record-database-project

I love to collect vinyl records. The sound, tactile sensation, album artwork and act of going out and hunting for new records makes for a great time! However, if you're like me, you might sometimes lose track of what records you have in your collection. Thankfully, I've created this application, which I am dubbing 'AudioFile' (hope you get the pun), for those of you who also have a hard time keeping track of your record collection.

The application is written in C++. using the Qt framework for the user interface. the code uses the MySQL/C++ API to access a MySQL database which is hosted through the AWS Relational Database Service. 

Users can register as a new user, then login to begin managing their record collection through the Collection Manager UI. Each user is granted their own MySQL schema (or database) where they can create MySQL tables to manage various different collections. A MySQL table corresponds to a record library. Maybe you want to manage your classic rock and new wave albums in one library, and have a different library for your classical records. You can easily do this! Maybe you have one group of records that you're trying to sell. Create a different library for these, and manage the progress of your sales!

Once you create a library, simply click Open, and you can easily start adding records to your library. The provided fields are: Band Name, Album Title, Genre, Year Released and Record Label. You don't have to fill out all the fields for each record, and you can come back later to edit or update a particular entry. You can also delete as needed on a single record basis. 

There is also a simple search functionality, where you can search based on a column header. For example, maybe you want to find all your records that came out in 1965. You can do this by selecting "Year Released" as your search field, then typing "1965" as the criteria.

Anyways, that's the jist of the application. It's very simple, and the UI is what I would consider rudimentary, but it works! Please let me know if you find any bugs or things that aren't working properly. 

Things I learned in this project:

1) Qt and how to make responsive UI's
2) Object Oriented design and how important it is to think about architecture up front. For example, I had to refactor the loginmanager class because it became too cumbersome to manage all of the different layouts of the various dialogs, depending on if a user is logging in, registering or resetting their password. 
3) How to share a class instance amongst multiple classes.
  - The dbManager class does all things with the MySQL database. loginManager, collectionManager and recordManager all need access to the SAME instance of dbmanager, because dbmanger stores some information about users that is needed to securely access user data. At first, I thought this would be very tricky, but then I learned I could just instantiate the dbManager once, then pass a reference to the instance of dbManager whereever it's needed!
4) Downloading, building, installing and linking third party libraries. Needed to do this for the MySQL/C++ API. At first it was challenging, but I've since done this and other projects and have a much better understanding of what's happening because of this.
5) Leveraging the cloud with a desktop application. This application is meant to be run on a desktop or laptop, but the data is stored on the cloud! Pretty cool, I think.
6) Security. Things like don't store passwords in plaintext, how do you make sure one user can't access another's data and vice versa. I still have a lot to learn here, but my understanding of security and user authentication/authorization grew exponentially thanks to this project.
