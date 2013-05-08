// hencode.cc
// Rachel Friedman
// 11/16/12

#include <iostream>
#include <cassert>
#include <fstream>
#include "heap.h"

using namespace std;

size_t Counts[256];
string codes[256];

void usage()
{
  cout << "USAGE: hencode FILENAME" << endl;
  exit(1);
}

void init()
{
  // initializes all the counts to zero and the codes to empty strings
  for (size_t i=0; i < 256; i++){
    Counts[i] = 0;
    codes[i] = "";
  }
}

void prependAll(char digit, Hnode * tree)
{
  // if it's a leaf adds to the code for the letter
  if (not tree->left)
    codes[(unsigned char)tree->data] = digit+codes[(unsigned char)tree->data];
  else {
    prependAll(digit, tree->left);
    prependAll(digit, tree->right);
  }
}
void rliot(Hnode * tree, size_t depth)
{
  if (!tree) return;

  rliot(tree->right, depth+1);
  for (size_t i=0; i<depth; i++){
    cout << "   ";
  }
  if (tree->data) cout << tree->data;
  else cout << "*";
  cout << endl;
  rliot(tree->left, depth+1);

}

unsigned char bitstring2number(string s)
{
  // converts a bitstring to a unsigned char to add to the output file
  assert (s.size() > 0);
  if (s.size() == 1)
    return s[0] - '0';
  else {
    string next = s.substr(0, s.size()-1);
    return 2 * bitstring2number(next) + s[s.size()-1] - '0';
  }
}

void writeTree(Hnode * tree, ostream & ostr)
{
  // Outputs characters symbolizing the tree to the output file
  if(!tree->left)
    ostr << 'L' << tree->data;
  else {
    ostr << 'I';
    writeTree(tree->left, ostr);
    writeTree(tree->right, ostr);
  }
}

string encode(ifstream & fin)
{
  char c;
  string bitstring; // will contain all of the character codes combined

  while(1) {
    fin.get(c);
    if (not fin)
      break;
    bitstring+= codes[(unsigned char)c]; // adds the code of the character
  }                                      // to the bitstring
  return bitstring;
}

void writeToFile(string bitstring, ofstream & fout)
{
  string substring;

  // outputs the unsigned character created by a conversion of 8 chars from
  // the bitstring and then erases the characters from the string
  while (bitstring.size() >= 8){
    substring = bitstring.substr(0, 8);
    bitstring.erase(0, 8);
    fout <<  bitstring2number(substring);
  }
  // if the bitstring size is less than 8 the bitstring is padded by 0's
    while(bitstring.size() < 8)
      bitstring+= '0';

    fout << bitstring2number(bitstring);
}

int main(int argc, char **argv)
{

  size_t fileSize = 0;
  if (argc != 2)
    usage();

  string fname = argv[1];

  if (fname == "hencode.cc" or fname == "hdecode.cc") // safety check for me
    usage();

  string outName = fname + ".huf";
  ifstream file(fname.c_str()); // input file
  ofstream fileOut(outName.c_str()); // file to hold the tree/character info

  init(); // initialized counts and codes

  char c;

  while (1) {
    file.get(c);
    if (not file)
      break;
    Counts[(unsigned char)c]++; // updates the counts of the characters
    fileSize+=1; // keeps track of the total encoded file size
  }

  fileOut << fileSize; // adds the file size to the output file

  Heap queue; // creates a heap to be used as the priority queue

  for(size_t i=0; i < 256; i++){
    if(Counts[i]){
      Hnode * p = new Hnode((char)i, Counts[i]); // creates a hnode pointer
      queue.add(p);         // to a hnode with the char and its count and adds
    }                        // to the queue
  }

  size_t queueSize = queue.getSize(); // gets the heap size

  for(size_t x = 0; x<queueSize-1; x++)
    {
      Hnode * left = queue.remove();  // removes the smallest two items
      Hnode * right = queue.remove();
      prependAll('0', left);
      prependAll('1', right);

      // creates a new hnode pointer that points to an hnode that's left
      // and right pointers are initialized to the two hnodes removed last
      Hnode * newborn = new Hnode(0, left->count + right->count, left, right);
      queue.add(newborn); // adds the new hnode pointer to the heap
      
    }
  Hnode * tree = queue.remove(); // at this point the heap only contains one
                                 // pointer which represents the whole tree
  writeTree(tree, fileOut); // writes the tree to the file

  file.close(); // closes and reopens the file for bitstring encoding
  file.open(fname.c_str());

  string bitstring = encode(file); // creates the bitstring
  writeToFile(bitstring, fileOut); // writes the bitstring to the file
  remove(fname.c_str()); // deletes the original file

}
