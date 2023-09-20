// Dictionary.cpp

// This is a correct but horribly inefficient implementation of Dictionary
// functionality.  Your job is to change DictionaryImpl to a more efficient
// correct implementation.  You can change it any way you like, subject to
// restrictions in the spec (e.g., that the only C++ library container you
// are allowed to use are vector, list, stack, and queue (and string); if you
// want anything fancier, implement it yourself).

#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <utility>  // for swap
#include <functional>//for hash
#include <algorithm>
#include <vector>
using namespace std;

void removeNonLetters(string& s);
void generateNextPermutation(string& permutation);
bool isAnagram(string word1, string word2){
    sort(word1.begin(), word1.end());
    sort(word2.begin(), word2.end());
    if(word1==word2)return true;
    return false;
}
unsigned long getHashValue(string word){
    hash<string> hasher;
    sort(word.begin(), word.end()); //turns it into sorted string
    unsigned long h = hasher(word);//calls hash function on the word
    return h;
}
//struct Node{
//    string m_value;
//    Node* m_left;
//    Node* m_right;
//    Node(string val, Node* right, Node* left):m_value(val), m_left(left), m_right(right){
//    }
//};


  // This class does the real work of the implementation.

class DictionaryImpl
{
  public:
    DictionaryImpl(int  maxBuckets =50000 ) { //default value 5000
        m_words= new vector<list<string>>[maxBuckets]();//creates array of pointers to lists of maxbuckets size, all initialied to null at first.
        m_maxBuckets=maxBuckets;
    }
    //redo this damn.
    ~DictionaryImpl() {
//        for(int i=0; i<m_maxBuckets; i++){
//            delete m_words[i]; //deletes all of whats pointed to by the pointers
//        }
        delete [] m_words; //deletes whole dynamic array
    }
    void insert(string word);
    void lookup(string letters, void callback(string)) const;
  private:
   // list<string> m_words; ok were gonnna do dynamic array of pointers to lists of nodes
    //array of vectors of nodes
    vector<list<string>>* m_words;//vector of string list., pointers
    int m_maxBuckets;
};


void DictionaryImpl::insert(string word) //adds a word stripped of any nonletters to the dictionary
{
   
    //step one is sort the word
    //step 2 is call hash function on the word
    removeNonLetters(word);
    if (word.empty()) return;
    string originalWord=word;
    sort(word.begin(), word.end());
        unsigned long hashvalue =getHashValue(word); //so same anagrams have same hashvalue
    unsigned long index=hashvalue%m_maxBuckets;
    //now actually insert
    //ok iterate through vector and see if theres a list with an anagram
    bool inserted=false;
    //iterate through vector
    vector<list<string>>::iterator v; //so *v would be a list of strings. ok
    //iterate through the vector.
    for(v=m_words[index].begin(); v!=m_words[index].end();v++){
        if(isAnagram(v->front(), originalWord)){
            //traverse through linked list, every word in linked list should be called.
            v->push_back(originalWord);
            inserted=true;
            break;
            }
        }
    //means theres no linked list for this anagram yet
    if(!inserted){
        list<string> newOne;
        newOne.push_back(originalWord);
        m_words[index].push_back(newOne);
    }

    }
  
        

void DictionaryImpl::lookup(string letters, void callback(string)) const //The lookup function takes a string and a callback function, and for every word in the dictionary that is an anagram of the letters in the string, it calls the callback function with that word as an argument.
{
    if (callback == nullptr)
        return;
    removeNonLetters(letters);
    if (letters.empty())
        return;
    sort(letters.begin(), letters.end()); //sorts letters
    unsigned long index = getHashValue(letters)%m_maxBuckets;
    if(m_words[index].empty()) return; //nothing in vector
    //iterate through the vector at the index
    vector<list<string>>::iterator v; //so *v would be a list of strings. ok v is like a pointer to a list.
    for(v=m_words[index].begin(); v!=m_words[index].end();v++){
        if(isAnagram(v->front(), letters)){
            //traverse through linked list, every word in linked list should be called.
            list<string>::iterator l=v->begin();
            while(l!=v->end()){
                callback(*l);
                l++;
            }
        }
    }
}
   

//this is correct anyway
void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop everything off from "to" to end.
}

  // Each successive call to this function generates the next permutation of the
  // characters in the parameter.  For example,
  //    string s = "eel";
  //    generateNextPermutation(s);  // now s == "ele"
  //    generateNextPermutation(s);  // now s == "lee"
  //    generateNextPermutation(s);  // now s == "eel"
  // You don't have to understand this function's implementation.
void generateNextPermutation(string& permutation)
{
    string::iterator last = permutation.end() - 1;
    string::iterator p;

    for (p = last; p != permutation.begin()  &&  *p <= *(p-1); p--)
        ;
    if (p != permutation.begin())
    {
        string::iterator q;
        for (q = p+1; q <= last  &&  *q > *(p-1); q++)
            ;
        swap(*(p-1), *(q-1));
    }
    for ( ; p < last; p++, last--)
        swap(*p, *last);
}

//******************** Dictionary functions ******************************

// These functions simply delegate to DictionaryImpl's functions
// You probably don't want to change any of this code

Dictionary::Dictionary(int maxBuckets)
{
    m_impl = new DictionaryImpl(maxBuckets);
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters,callback);
}

