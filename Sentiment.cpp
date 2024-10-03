#include "Sentiment.h"

//SUPPORT FUNCTION TO FIND THE CURRENT WORD IN THE SENTEMENT VECTOR & RETURN THE SENTIMENT VALUE
//PRE:   accepts a string (in) and the words list
//POST:  returns the sentiment of the word if found or the value 0 if not
/*double sentimentVal(string in, vector<wordList> &words) {
    double sentval = 0;
    for (int i = 0; i < words.size(); i++) {
      if (words[i].word == in) {

        //test if word is sent word then break out if it is
        sentval = words[i].value;
        break;
      }
    }
    return sentval;

}*/
//searching thrugh binary
double sentimentVal(string in, vector<wordList> &words) {
  int left = 0;
  int right = words.size() - 1;

  while (left <= right) {
      int mid = left + (right - left) / 2;

      if (words[mid].word == in) {
          return words[mid].value; // Target found
      } else if (words[mid].word < in) {
          left = mid + 1; // Target is in the right half
      } else {
          right = mid - 1; // Target is in the left half
      }
  }

  return 0; // Target not found
}

//PRE:   accepts an empty set of 3 <wordList> vectors
//POST:  loads all three vectors with data from sentiment.txt
void loadSentiment(vector<wordList>& sentList,
    vector<wordList>& posList,
    vector<wordList>& negList) {

    //open sentiment.txt
    //add the word to the sentList vector
    //if positive enough add to posList
    //if negative enough add to negList
    //close sentiment.txt
  ifstream inFile("sentiment.txt");
  string test;

  //continue to get lines till cant anymore
  while (getline(inFile, test)) {
    string word;
    int number,totallength;
    double value;
    totallength = test.length();
    //find where the comma is
    number = test.find(',');
    //put word in the test
    word = test.substr(0,number);
    //change the string to a double
    value = stod(test.substr(number + 1, totallength));
      wordList wordex;
    //set values into exsample
      wordex.word = word;
      wordex.value = value;
    //sort the word into the qualifying vectors
      if (value < -1.25) {
      negList.push_back(wordex);
    } else if (value > 1.25) {
      posList.push_back(wordex);
    }
      sentList.push_back(wordex);
    }

  inFile.close();
}

//PRE:  positive & negative word lists are sent
//POST: Open the input file & output file (infile+Output 
//      'Clean' the input by removing syntax and setting all to lower case
//       Get the sentiment value for the edited word
//        store unedited word in vector with sentiment value
//         if sentiment < -1  find a positive word to replace it
//         if sentiment > 1   find a negative word to replace

void processFile(ifstream& inFile, ofstream& outFile, string fileName,
                  vector<wordList>& words,// vectors containing the word & sentiment
                  vector<wordList>& posList, 
                  vector<wordList>& negList) {

  string inWord, newWord;
  char newChar;
  vector<wordList> origWords, negWords, posWords;
  vector<replaceList> negWordsreplace, posWordsreplace;
  double totalorigsent = 0;
  //list the review its on
  outFile << "PROCESSING FILE: " << fileName << endl << endl;
  //check if the file was able to open
  if (!inFile.is_open()) {
    cout << "file did not open" << endl;
  }
  // reset all vectors & values before processing inFile
  //   read and edit the initial file
  //   remove any unnecessary characters
  while (inFile >> inWord) {
      //  the inFile read will read a word up to the next space.
      //  there may be some unwanted characters
      //  read char by char and only use alphabetic characters
      //  and change all uppercase to lowercase
      newWord = "";
      for (int i = 0, len = inWord.size(); i < len; i++) {
        //put get the word without extra characters
          if (isalpha(inWord[i])) {
              newChar = tolower(inWord[i]);
              newWord += newChar;
          }
      }
    //find the sentvalue for the word|0 if not present
      double sentv = sentimentVal(newWord, words);
      wordList tempw;
      replaceList tempv;
      //get a random value for replace variable
      int num = rand() % negList.size();
    //set the word and sent
      tempw.word = newWord;
      tempw.value = sentv;
      if (sentv > 1) {
        //if positive set replace values
        tempv.origWord = tempw.word;
        tempv.origValue = tempw.value;
        tempv.newWord = negList[num].word;
        tempv.newValue = negList[num].value;
        negWordsreplace.push_back(tempv);
      } else if (sentv < -1) {
        //if negative set replace values
        tempv.origWord = tempw.word;
        tempv.origValue = tempw.value;
        tempv.newWord = posList[num].word;
        tempv.newValue = posList[num].value;
        posWordsreplace.push_back(tempv);
        
      }
      //put the word into a vector of all words and add sent value
      totalorigsent += tempw.value;
      origWords.push_back(tempw);
  }
      // add the 'clean' word to a temp location & add original word & sentiment value to origWords

      //CHECK TO MAKE SENTIMENT MORE NEGATIVE
      //store current word before checking to replace
      //add original word or replacement to negVector
      
      
      //CHECK TO MAKE SENTIMENT MORE POSITIVE
      //store current word before checking to replace
      //add original word or replacement to posVector
  
  //OUTPUT UPDATES
  //EDIT ORIGINAL WORDS VECTOR FOR OUTPUT
  //check to see if the length of the words written so far
  outFile << "FORMATTED REVIEW: " << endl;
  int lineLength = 0;
  for (unsigned int i = 0; i < origWords.size(); i++) {
      lineLength += origWords[i].word.length() + 1;
      if (lineLength > 80) {
          outFile << "\n";
          lineLength = origWords[i].word.length() + 1;
      }
      outFile << origWords[i].word + " ";
  }
  
  
  outFile << "\n\nORIGINAL SENTIMENT: " << totalorigsent << endl;

  //EDIT MORE NEGATIVE VECTOR FOR OUTPUT
  //CHECK THAT SOMETHING HAS BEEN UPDATED BEFORE BEGINNING PRINT
  //IF THE ORIGINAL SENTMENT & NEGATIVE SENTIMENT ARE EQUAL, PRINT "REVIEW NOT UPDATED, THE SENTIMENT REMAINS: "
  
   outFile << endl << endl << "WORDS UPDATED TO BE MORE NEGATIVE:" << endl;
  if (negWordsreplace.size() == 0) {
    outFile << "REVIEW NOT UPDATED. THE SENTIMENT REMAINS: " << totalorigsent << endl;
  } else {
  double origsent = 0, updatedsent = 0;
  for (int i = 0; i < negWordsreplace.size(); i++) {
    //output the words to be changed to negative
    outFile << setw(15) << right << negWordsreplace[i].origWord << " " << setw(6) << fixed << setprecision(2) << negWordsreplace[i].origValue << " " << setw(15) << right << negWordsreplace[i].newWord << " " << setw(6) << negWordsreplace[i].newValue << endl;
    origsent += negWordsreplace[i].origValue;
    updatedsent += negWordsreplace[i].newValue;

  }
  outFile << setw(15) << right << "TOTALS: " << setw(6) << origsent << " " << setw(21) << updatedsent << endl << endl << endl;  

  //output the statement that is revised to be more negative
  outFile << "UPDATED REVIEW (MORE NEGATIVE): " << endl;
  lineLength = 0;
  double negsentval = 0;
  for (unsigned int i = 0; i < origWords.size(); i++) {
      bool valid = false;
      int indnum;
      lineLength += origWords[i].word.length() + 1;
      if (lineLength > 80) {
          outFile << "\n";
          lineLength = origWords[i].word.length() + 1;
      }
      for (int j = 0; j < negWordsreplace.size(); j++) {
        if (negWordsreplace[j].origWord == origWords[i].word) {
          valid = true;
          indnum = j;
        }
          
        }
    
    if (valid) {
      outFile << negWordsreplace[indnum].newWord + " ";
      negsentval += sentimentVal(negWordsreplace[indnum].newWord,words);
    } else {
      outFile << origWords[i].word + " ";
      negsentval += sentimentVal(origWords[i].word,words);
    }
    
      
  }
  outFile << "\n\nUPDATED SENTIMENT: " << negsentval << endl << endl;
  }
  //EDIT MORE POSITIVE VECTOR FOR OUTPUT
  //CHECK THAT SOMETHING HAS BEEN UPDATED BEFORE BEGINNING PRINT
  //IF THE ORIGINAL SENTMENT & NEW POSITIVE SENTIMENT ARE EQUAL, PRINT "REVIEW NOT UPDATED, THE SENTIMENT REMAINS: "
  outFile << "\n\nWORDS UPDATED TO BE MORE POSITIVE:" << endl << endl;
  if (posWordsreplace.size() == 0) {
    outFile << "\nREVIEW NOT UPDATED. THE SENTIMENT REMAINS: " << totalorigsent << endl << endl << endl;
  } else {
    double origsent = 0, updatedsent = 0;
    for (int i = 0; i < posWordsreplace.size(); i++) {
      //output the words to be changed more positive
      outFile << setw(15) << right << posWordsreplace[i].origWord << " " << setw(6) << fixed << setprecision(2) << posWordsreplace[i].origValue << " " << setw(15) << right << posWordsreplace[i].newWord << " " << setw(6) << right << posWordsreplace[i].newValue << endl;
      //calculate the total sent for orig and new
      origsent += posWordsreplace[i].origValue;
      updatedsent += posWordsreplace[i].newValue;
    }
    outFile << setw(15) << "TOTALS: " << origsent << " " << setw(21) << updatedsent << endl << endl << endl;  

    //output the statment that is more positve
    outFile << "UPDATED REVIEW (MORE POSITIVE): " << endl;
    lineLength = 0;
    double possentval = 0;
    for (unsigned int i = 0; i < origWords.size(); i++) {
        bool valid = false;
        int indnum;
        lineLength += origWords[i].word.length() + 1;
        if (lineLength > 80) {
            outFile << "\n";
            lineLength = origWords[i].word.length() + 1;
        }
        for (int j = 0; j < posWordsreplace.size(); j++) {
          if (posWordsreplace[j].origWord == origWords[i].word) {
            valid = true;
            indnum = j;
          }

          }
        //if it is a word that is to be changed is changed
      if (valid) {
        outFile << posWordsreplace[indnum].newWord + " ";
        possentval += sentimentVal(posWordsreplace[indnum].newWord,words);
      } else {
        outFile << origWords[i].word + " ";
        possentval += sentimentVal(origWords[i].word,words);
      }


    }
    outFile << "\n\nUPDATED SENTIMENT: " << possentval << endl;
    }
  posWords.clear();
  negWords.clear();
  origWords.clear();
  negWordsreplace.clear();
  posWordsreplace.clear();
}