// hdecode.cc 
// Rachel Friedman
// 12/10/12

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include "hnode.h"

using namespace std;

void usage()
{
  cout << "USAGE: hencode FILENAME" << endl;
  exit(1);
}

Hnode * readTree(ifstream & istr)
{
  char type = istr.get();
    if (not istr)
      return NULL;
  if (type == 'L'){
    return new Hnode(istr.get());
  }
  return new Hnode(readTree(istr), readTree(istr));
}

string uchar2bitstring(unsigned char c)
{
  string bitstring;
  size_t total = 0;
  size_t i;
  unsigned char val;
  
  // converts a character to its bitstring equivalent
  for(i=0, val = 128; i < 8; i++, val/=2){
    if (total + val <= c){
      bitstring+= '1';
      total += val;
    }
    else
      bitstring+= '0';
  }
  return bitstring;
}

unsigned char decodeLetter(Hnode * curr, ifstream & file, string & bitstring)
{
  if (curr->left == NULL){  // if leaf, return the char
    return curr->data;
  }

  if (bitstring == ""){ // if the bitstring is empty, gets another char
    char c;
    file.get(c);
    bitstring+=uchar2bitstring(c); // converts the char to a bitstring
  }
 
  if (bitstring[0] == '0'){
    bitstring.erase(0, 1);
    return decodeLetter(curr->left, file, bitstring); // continues left
  }

  else {
    bitstring.erase(0, 1);
    return decodeLetter(curr->right, file, bitstring); // continues right
  }
}

void output(Hnode * root, int size, ofstream & decode, ifstream & file)
{
  string bitstring = "";

  // writes the decoded characters to the file (does this for the num of
  // characters in the original file to make sure there are no extra chars)

  for (int i=0; i < size; i++){
    decode << decodeLetter(root, file, bitstring);
  }
}

int main(int argc, char **argv)
{
  if (argc !=2)
    usage();

  string fname = argv[1];

  // prevent me from deleting files by mistake
  if (fname == "hencode.cc" or fname == "hdecode.cc")
    usage();

  ifstream file(fname.c_str()); // input file
  string decodedName = fname.substr(0, fname.size()-4);
  ofstream decoded(decodedName.c_str()); // file to output to

  int size;
  file >> size; // num of chars in the original file

  Hnode * tree = readTree(file); // creates a tree from the encoded tree
  output(tree, size, decoded, file); // outputs decoded chars to output file
  remove(fname.c_str()); // erases input file
}
