/*
  CIS 22C Team Project, Team #3 - A Database of Cupertino Pet Store.
- Data Structure: binary search tree and hash table implementation
- Firstly, we will display a greeting message
- Secondly, we sill display the menu.
- Then, according to the menu,the user will enter some letters.
- The program is executed depend on the input letter.
- After the program is done, display a goodbye message.
- Written by: team #3, teamLeader: Gezheng Kang
- For this main.cpp file,
  -- unit 1 / Team Coordination done by Gezheng Kang
  -- unit 4 / Screen Output done by Katie Leong
*/

#include <iostream>
#include <string>
#include <cctype>  // for lowercase to uppercase
#include <fstream> // for dealing with files
#include <sstream> // for dealing with file
#include "Pet.h"
#include "StackADT.h" //need to include Stack ADT class for the undo delete method
#include "BinarySearchTree.h"
#include "HashTable.h"

using namespace std;

// Function Prototypes
void buildFun(const string , BinarySearchTree<string> &, HashTable<Pet> &);
void startMessage();
void searchManger(HashTable<Pet> &hash);
void menuManager(BinarySearchTree<string> &, HashTable<Pet> &, Stack<Pet> &);
void deleteFun(BinarySearchTree<string> &, HashTable<Pet> &, Stack<Pet> &);
void undoDelete(BinarySearchTree<string>&, HashTable<Pet> &, Stack<Pet> &);
void writeFile (HashTable<Pet>& , string );
void iDisplay(const string &, int );
void displayMenu();
void displayMembers();
void displayAnimal(int);
void endMessage();


int main()
{
   BinarySearchTree<string> bst; // declare bst, which only constains string keys
   HashTable<Pet> hash; // declare hash, which contains Pet objects
   Stack<Pet> deletedStack; // will need to be passed into undo Delete and menuManager
    
   startMessage(); // display a greeting message
     
   menuManager(bst, hash, deletedStack); // display the menu and promote the user to enter an ooption
    
   string fileName = "petDefaultCopy.txt"; // a back up file
   writeFile(hash, fileName);
   return 0;
}


/*
    menuManager
    Pre: bst - reference to binary search tree of type pet, passed in empty
         hash - reference to a hashtable to be used for searching
         deletedStack - reference to a stack that will hold deleted data for recovery
    Post: bst has data either directly from user or from an input file, data may be changed from original
          hash has data inputted from user or from an input file, data may be changed from original
    About: display menu options that the user can do to a data set stored in a bst and hashtable, data set
    is provided by user
*/
void menuManager(BinarySearchTree<string> &bst, HashTable<Pet> &hash,  Stack<Pet> &deletedStack)
{
  bool cont = true; // true if user wants to continue choosing menu options, false if they want to exit program
  char option;      // option user chooses from menu

  displayMenu(); // intial menu display

  // loop to keep asking user what they want to do
  while (cont)
  {
    cout << "Enter an option: ";
    cin >> option;

    option = toupper(option); // convert the letter to uppercase in case

    // take action depending on user input
    switch (option)
    {
        case 'A': // add data item
        {
            /*
             create a pointer to the object along wiht the object,
             insert object into hash table based on its key created from hash function
             inser pointer of object plus the key created into bst
             */
            // Variables required for pet object creation
            string owner = "", name = "", type = "";
            double age = 0.0, weight = 0.0;

            // asking user for the info on the pet
            cout << "Enter the Owner Name: ";
            cin >> owner;

            if (bst.search(owner,owner) == 1)
            {
                cout << "An error has occurred. You may enter duplicate data." << endl;
            }
            
            else
            {
                cout << "Enter the Name of the pet: ";
                cin >> name;
                
                cout << "Enter the Age of the pet: ";
                cin >> age;
                
                cout << "Enter the Pet type: ";
                cin >> type;
                
                cout << "Enter the Weight of the pet: ";
                cin >> weight;
                
                // creating pet object based on user input
                Pet newPet(owner, name, type, age, weight);
                
                // insert into bst and hash table
                // error message if the insert did not work
                if(hash.insert(newPet, key_to_index)!= 0)
                {
                    cout << "This is what you have added:" << endl;
                    vDisplay(newPet);
                }
            }
            continue;
    }
            
    case 'B': // add data from input file, call buildFunction
      {
          string fileName;
          cout << "Enter the file name: ";
          cin >> fileName; // pass the file name to fileio class

          buildFun(fileName, bst, hash);

          continue;
      }

    case 'C': // delete data
      /*
          delete data from bst and hash table, save item into the delete stack
          - if item not found, display error message, else display what has been deleted
      */
          deleteFun(bst,hash,deletedStack);
    break;

    case 'D': // find+display one element using primary key
      /*
          search manager: should ask user for item they want
          search, use the hashtable to retrieve item, display
      */
            searchManger(hash);
    break;

    case 'E': // list data sorted by primary key
      /*
          traverse bst inorder, print data
          *need to create a function displayPet
      */
          {
              bst.inOrder(); // one data per line
              break;
          }

    case 'F': // write data to a file
      /*
          File I/O: save to file, in hash table sequence
          Delete the stack that saves the deleted items
      */
        {
            cout << "Enter a name file to save data with hashtablbe sequence in : " ;
            string fileName;
            cin >> fileName;
            writeFile(hash,fileName);
            continue;
        }

    case 'G': // display statistics
    {
        cout << "Staticistics of hash table: " << endl;
        cout << "Load Factor: " << hash.getLoadFactor() << "%" << endl; // load factor of hash table
        cout << "Total number of collisions: " << hash.totalCollisions() << endl; // total number of collisions
        hash.longestCollisionPath(); // longest collision path
    break;
    }
    case 'H': // Help option, redisplay menu for user
      displayMenu();
      break;
            
    case 'Q':
        {
            cont = false;
            endMessage();
            break;
        }
            
    case 'Z':
      undoDelete(bst, hash, deletedStack);
      continue;

    case 'M': // Display member names
      displayMembers();
      continue;
            
    case 'P': // display tree as an indented list, pass the idisplay friend function to it.
      bst.printTree(iDisplay);
      continue;

    default: // user did not type a valid choice
      cout << "That is invalid input, try again" << endl;
      continue;
    }
  }
}

/*
- This function reads data about pets from a file and inserts them
  into BST and hash table. The list is sorted in ascending order by owner's name
- Relational operators have been overloaded in Pet.h
*/
void buildFun(const string filename, BinarySearchTree<string> & bst, HashTable<Pet> &hash)
{
  ifstream inFile;
  inFile.open(filename);
  if (!inFile)
  {
    cout << "Error opening the input file: \"" << filename << "\"" << endl;
    exit(EXIT_FAILURE);
  }

  string line;

  while (getline(inFile, line))
  {
    double age, weight;
    string owner, name, type;

    stringstream temp(line);

    getline(temp, owner, ':'); // get the owner until colon
    temp.ignore();             // ignore the space
    getline(temp, name, ',');  // get the pet's name until comma
    temp.ignore();             // ignore the space
    temp >> age;               // get the age until space
    temp.ignore();             // ignore the space
    temp >> type;              // get the type until space
    temp.ignore();             // ignore the space
    temp >> weight;            // get the weight until space

    Pet apet(owner, name, type, age, weight);
    bst.insert(owner);
    hash.insert(apet,key_to_index);
  }
  cout << "Successfully reading " << filename << ". "<< endl << endl;
  inFile.close();
}

/*
 This function receives an array, its size, and the file's name, which is written to.
 This function writes report in a file, whose name is fileName.
*/
void writeFile (HashTable<Pet> &hash, string fileName)
{
    Pet pet;
    ofstream outFile (fileName);
    hash.writeHashTableToFile(fileName);
    cout << "Data saved in: " << fileName << endl << endl;
    
}


/*
searchManager
- About: asks users what they want to search for given a key, returns the data associated
- with the key or message that item was not found
*/
void searchManger(HashTable<Pet> &hash)
{
  string owner;
  cout << "What is the name of the owner of the pet you are looking for? " << endl;
  cin >> owner;

  Pet petOut;
  Pet petHere(owner," "," ",0,0);
  if(hash.search(petOut,petHere, key_to_index) == -1)
   {
       cout << "An owner named " << owner << " was not found!" << endl << endl;
   }
   else
   {
       vDisplay(petOut);
   }
}

/*
  deleteFun
  Pre: bst - reference to a binary tree that holds data where specific item will be deleted from
       hash - reference to a hashtable where data will be deleted from
       deletedStack - reference to a stack that stores deleted item
  Post: remove an item from the bst and hash and add it to deletedStack
  About: delete function that will delete a certain node in bst and hash.
         Allow people undo by pushing data in to a stack.
*/
void deleteFun(BinarySearchTree<string> &bst, HashTable<Pet> &hash,  Stack<Pet> &deletedStack)
{
    string owner; // key for item to be deleted
    Pet deleted;
    cout << "Name of owner to be deleted: ";
    cin >> owner;
    
    Pet petHere(owner," "," ",0,0);
    
    
    bool h = hash.remove(deleted,petHere,key_to_index);
    bool b = bst.remove(owner);
    
    if(!h && !b)
        cout << "An error has occured, " << owner << " was not found" << endl;
    else
    {
        cout << deleted.getOwner() << " was deleted successfully. " << endl;
        deletedStack.push(deleted);
    }
}

/*
    undoDelete
    Pre: bst - reference to binary search tree where item was deleted from
         hash - reference to a hashtable that will be updated if there was an item deleted from it
         deletedStack - reference to a stack that holds deleted items
    Post: most recent item deleted has been reinserted into the bst and hashtable
    About: uses a stack to keep track of all the items removed from the bst/hashtable
*/
void undoDelete(BinarySearchTree<string> &bst, HashTable<Pet> &hash, Stack<Pet> &deletedStack)
{
   if(!deletedStack.isEmpty())
   {
       Pet item = deletedStack.pop();

       cout << item.getOwner() << " has been put back." << endl;

       hash.insert(item, key_to_index); // insert item back into hashtable and bst
       bst.insert(item.getOwner());

   }
   else //Error Message, there is nothing to undo as nothing has been deleted
   {
      cout << "Error: nothing to undo as an item has not been deleted from the data set." << endl;
   }
}

// iDisplay
// on one line, including the level number and
// ".." for each indentation level
void iDisplay(const string &item, int level)
{
    for (int i = 1; i < level; i++)
        cout << "..";
    cout << level << "). " << item << std::endl;
}


// Display the menu options
void displayMenu()
{
  cout << endl;
  cout << "---------- Show Menu ----------" << endl;
  cout << " A. Add a new data item" << endl;
  cout << " B. Add data from an input file" << endl;    // get name of file from user
  cout << " C. Delete Data" << endl;                    // one item, also everytime item deleted, push onto undo stack
  cout << " D. Search element using primary key" << endl; // display also
  cout << " E. Display data sorted by the owner name" << endl;
  cout << " P. Display tree as an indented tree" << endl;
  cout << " F. Write data to a file" << endl;
  cout << " G. Display statistics of hash table" << endl; // pt2
  cout << " H. Display the menu for help" << endl;
  cout << " Q. Quit the entire program" << endl;
  // cout << " M. Display team members of this program" << endl;  // hidden option
  cout << " Z. Undo previous delete" << endl;  // undo delete option
  cout << "---------- End Menu ----------" << endl;
  cout << endl;
}

// Display a starting message to the user
void startMessage()
{
  cout << endl << endl;
  displayAnimal(1); // display a cat
  cout << "Hi, this is Cupertino Pet Store program execution. " << endl;
  displayAnimal(4); // display 2 dogs
}

// Display a final message to the user
void endMessage()
{
  cout << endl << endl;
  displayAnimal(2); // display cat and parakeets
  cout << "Thanks for watching our program execution. Goodbye! " << endl;
  displayAnimal(3); // display a dog and bunny
}

// Display the member names
void displayMembers()
{
  cout << "T3 Members: Sungjun An, Anita Huang, Gezheng Kang, Katie Leong, Juhwan Park" << endl;
}



// Display ascii art of an animal given a specific number
void displayAnimal(int num)
{
  switch (num)
  {
  case 1:
    cout << "           __..--''``---....___   _..._    __" << endl;
    cout << " /// //_.-'    .-/\\\";  `        ``<._  ``.''_ `. / // /" << endl;
    cout << "///_.-' _..--.'_    \\                    `( ) ) // //" << endl;
    cout << "/ (_..-' // (< _     ;_..__               ; `' / ///" << endl;
    cout << " / // // //  `-._,_)' // / ``--...____..-' /// / //" << endl;
    break;

  case 2:
    cout << "      |\\      _,,,---,,_               \\\\" << endl;
    cout << "ZZZzz /,`.-'`'    -.  ;-;;,_    \\\\      (o>" << endl;
    cout << "     |,4-  ) )-,_. ,\\ (  `'-'  (o>     //\\ " << endl;
    cout << "    '---''(_/--'  `-'\\_)      _(()_____V_/_____" << endl;
    cout << "            Felix Lee           ||      ||" << endl;
    cout << "                                        ||" << endl;
    break;

  case 3:
    cout << "    ___                              ,\\" << endl;
    cout << " __/_  `.  .-\"\"\"-.                   \\\\\\,_" << endl;
    cout << " \\_,` | \\-'  /   )`-')                 \\` ,\\" << endl;
    cout << "  \"\") `\"`    \\  ((`\"`             __,.-\" =__)" << endl;
    cout << " ___Y  ,    .'7 /|                .\"        )" << endl;
    cout << "(_,___/...-` (_/_/             ,_/   ,    \\/\\_" << endl;
    cout << "                                 \\_|    )_-\\ \\_-`" << endl;
    cout << "                             jgs    `-----` `--`" << endl;
    break;

  case 4:
    cout << "          __           __      _   " << endl;
    cout << " \\ ______/ V`-,     o'')}____//   " << endl;
    cout << "  }        /~~        `_/      )   " << endl;
    cout << " /_)^ --,r'           (_(_/-(_/    " << endl;
    cout << "|b      |b                         " << endl;
    break;

    break;
  }
}

