#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include "Sentiment.h"

using namespace std;

/*
Name:Alex Ghasemi
Date: 3/10/2024
Program name: Sentiment Analysis
!!!!!!Note:the program wont run with all reviews, it stops before due to a memory issue. you can test the individual reviews by commenting out the reviews to where it only runs two at a time.
PSUEDO CODE:
Start with sentiment.txt and sort the positive and negative and sort into 3 vectors
read each reviews and find words that are positive and negative, and puts into a vector of those
prints orignal statment
prints positive words to make negative, and then prints revised statment
prints negative words to makie more positive and then prints statment
goes to the next review
Sourced: gpt for binary and some pos stuff
*/

int main() {
  //randomize results each time
  srand(time(0));
  //declare the vectors that have all the sentaments
  vector<wordList> words;
  vector<wordList> posList;
  vector<wordList> negList;
  //load the sentaments
  loadSentiment(words,posList,negList);
  //declare vector with all files

  vector<string> filestoreview;
  string fileName1 = "review1.txt",fileName2 = "review2.txt",fileName3 = "review3.txt",fileName4 = "review4.txt",fileName5 = "review5a.txt",fileName6 = "review6.txt",fileName7 = "review7.txt",fileName8 = "review8.txt";
  //load vector with file names
  filestoreview.push_back(fileName1);
  filestoreview.push_back(fileName2);
  filestoreview.push_back(fileName3);
  filestoreview.push_back(fileName4);
  filestoreview.push_back(fileName5);
  filestoreview.push_back(fileName6);
  filestoreview.push_back(fileName7);
  filestoreview.push_back(fileName8);
  ofstream outFile("outputFile.txt");
  //iterate thrugh reviews
  for (int i = 0; i < filestoreview.size(); i++) {
    ifstream inFile(filestoreview[i]);
    processFile(inFile, outFile, filestoreview[i],words,posList, negList);
    inFile.close();
  }
  outFile.close();
}