#include <iostream>
#include <string>
#include <fstream>

using namespace std;


//LIST OF FILES NODES
class ListOfFilesNode {
    friend class ListOfFiles;
    private:
        int numberFile;
        ListOfFilesNode *next;
};

//LIST OF FILES
class ListOfFiles{
    public:
        ListOfFiles(){headFile = 0;}
        bool SearchFiles(const int&);
        void DisplayFiles();
        void InsertFile(const int&);
    private:
        ListOfFilesNode* headFile;
};


bool ListOfFiles::SearchFiles(const int& file)
{
    if(headFile==0)
    {
        //list is empty
        return false;
    }
    else
    {
        //search for the file in ListOfFiles
        ListOfFilesNode* current = headFile;
        while(current!=0)
        {
            if(current->numberFile==file)
            {
                //file found
                return true;
            }
            else
            {
                //go to the next node
                current = current->next;
            }
        }
    }
    //file was not found
    return false;
}

void ListOfFiles::DisplayFiles()
{
    //current is set to the first node and iterate through the list to display the files
    ListOfFilesNode* current = headFile;
    while(current!=0)
    {
        cout<< current->numberFile;
        if(current->next!=0)
        {
            cout<< ",";    
        }
        current = current->next;
    }
    cout<<endl;
}

void ListOfFiles::InsertFile(const int& file) {
    //new file node to insert 
    ListOfFilesNode* newNode = new ListOfFilesNode();
    newNode->numberFile = file;

    //insert at head
    if (headFile == nullptr || file < headFile->numberFile) 
    {
        newNode->next = headFile;
        headFile = newNode;
    } 
    else 
    {
        //find the correct position to insert
        ListOfFilesNode* current = headFile;
        while (current->next != nullptr && current->next->numberFile < file)
        {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}



class ListOfWords;
class ListOfWordsNode;

//LIST OF WORDS
class ListOfWords{
    public:
        ListOfWords(){head = 0;}
        ListOfWordsNode* SearchWord(const std::string&);
        ListOfWordsNode* InsertWord(const std::string&);
    private:
        ListOfWordsNode* head;
};

//LIST OF WORDS NODES
class ListOfWordsNode{
    friend class ListOfWords;
    friend ostream& operator<<(ostream& os, const ListOfWordsNode& node);
private:
    string word;
    int sum;
    ListOfFiles* LOF;
    ListOfWordsNode* next;
public:
    ListOfWordsNode(string word);
    ListOfFiles* getLOF(){return LOF;}
    string getWord(){return word;}
    int getSum(){return sum;}
    void setSum(){this->sum++;}

};

//overload << to display words
std::ostream& operator<<(std::ostream& os, const ListOfWordsNode& node)
{
    os << node.word;
    return os;
}


//constuctor of ListOfWordsNode
ListOfWordsNode::ListOfWordsNode(string w):word(w),next(0){LOF = new ListOfFiles();}

ListOfWordsNode* ListOfWords::SearchWord(const string& word)
{
    if(head==0)
    {
        // List is empty
        return nullptr;
    }
    else
    {
        //Search for the name in list
        ListOfWordsNode* current = head;
        while(current!=0)
        {
            if(current->word==word)
            {
                //word was found
                return current;
            }
            else
            {
                //current is set to next node
                current = current->next;
            }
        }
    }
    //word was not found
    return nullptr;
}

ListOfWordsNode* ListOfWords::InsertWord(const string& word)
{
    ListOfWordsNode* newNode = new ListOfWordsNode(word);
    if(head==0)
    {
        //List is empty
        head = newNode;
        return head;
    }
    else
    {
        //current has the current node, trail has the previous node 
        ListOfWordsNode* current = head;
        ListOfWordsNode* trail = 0;

        while(current!=0)
        {
            if(current->word>=newNode->word)
            {
                break;
            }
            else
            {
                trail = current;
                current = current->next;
            }
        }
        //Insert at head
        if(current == head)
        {
            newNode->next = head;
            head = newNode;
            return head;
        }
        else
        {
            //insert after the head
            newNode -> next = current;
            trail->next = newNode;
            return newNode;
        }
    }
}



int main()
{
    int orderOfFiles[] = {26, 94, 22, 40, 43, 24, 87, 44, 31, 45, 6, 49, 14, 53, 58, 33, 74, 13, 19, 93, 55, 71, 39, 29, 10, 80, 86, 77, 79, 82, 9, 92, 17, 96, 50, 2, 65, 90, 89, 11, 57, 51, 68, 83, 66, 54, 27, 95, 48, 81, 70, 25, 4, 60, 21, 76, 75, 61, 15, 37, 30, 62, 34, 56, 88, 59, 1, 32, 3, 73, 78, 42, 16, 36, 7, 47, 12, 67, 85, 64, 28, 52, 84, 98, 69, 41, 63, 72, 46, 20, 18, 100, 97, 99, 23, 38, 35, 91, 8, 5};
    //Open every file with orderInFiles order and read every word from each file
    ListOfWords* LOW = new ListOfWords();
    const int numOfFiles = 100;
    string inputDir= "input/";
    for (int i=0; i<numOfFiles ; i++)
    {
        string fileName =inputDir + to_string(orderOfFiles[i]) + ".txt";
        ifstream inFile(fileName);
        if (inFile.is_open())
        {
            string word;
            while(inFile >> word)
            {
                //Search if the word already exists in LOW
                ListOfWordsNode* node = LOW->SearchWord(word);
                if(node==nullptr)
                {
                    //word not found
                    node = LOW->InsertWord(word);
                    node->getLOF()->InsertFile(orderOfFiles[i]);
                }
                else
                {
                    //word found
                    if(node->getLOF()->SearchFiles(orderOfFiles[i]))
                    {
                        //file found
                    }
                    else
                    {
                        //file not found
                        node->setSum();
                        node->getLOF()->InsertFile(orderOfFiles[i]);
                    }
                }
            }
            inFile.close();
        }
        else
        {
            cout<<"Couldnt open file " << fileName << endl;
            return 1;
        }
    }
    string input;
    while(true)
    {
        cout<<"Enter the word to Search in the list: ";
        cin>>input;
        if(input=="q")
        {
            break;
        }
        ListOfWordsNode* lowNode = LOW->SearchWord(input);
        if (lowNode==nullptr)
        {
            cout<<"The word " << input << " was not found in the list" << endl;
        }
        else
        {
            cout<<"The word " << lowNode->getWord() << " appears in " << lowNode->getSum() + 1 << " documents.These are : ";
            lowNode->getLOF()->DisplayFiles();
        }
    }
    delete LOW;
    return 0;
}




