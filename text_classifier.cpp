/*
 * text_classifier.cpp
 *
 *  Created on: May 4, 2011
 *      Author: ethan
 */


/**
 * Wrapper that reads input files, sends them to classifier,
 * and evaluates its predictions.
 *
 *
 * Command-line syntax:
 * class1-trainfile class2-trainfile class1-testfile class2-testfile
 *
 * Or, for 3-way classification (done as 3 1-vs-many classifiers):
 * class1-trainfile class2-trainfile class3-trainfile class1-testfile class2-testfile class3-testfile
 *
 * Input files can be any text. See getLinesFromFile() for preprocessing.
 */

#include "feature.hpp"
#include "naive_bayes_classifier.hpp"
#include "string_utilities.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>  // For max


using std::vector;
using std::string;
using std::ifstream;
using std::cerr;
using std::cout;
using std::endl;
using std::ifstream;

  // Extract strings from a text file, one string per line.
  vector<string> getLinesFromFile(string filename) {

    vector<string> lines;
    ifstream inFile(filename.c_str());
    if (!inFile) {
      cerr << "File does not exist: " << filename << endl;
      exit(0);
    }
    string line;

    while( getline(inFile,line) ) {

      line = removeNonAlpha(line); // remove all non-alphabetic characters
      line = trim(line);            // trim opening & trailing whitespace
      line = removeMultipleSpaces(line); // Remove multiple spaces
      // convert to upper case
      std::transform(line.begin(), line.end(),line.begin(), ::toupper);

      if (line.length() > 0) {  // If not an empty string
        lines.push_back(line);  // add it
      }
    }
    inFile.close();
    return lines;
  }

  NaiveBayesClassifier trainClassifier(string posExampleFile, string negExampleFile) {
    vector<string> posExamples = getLinesFromFile(posExampleFile);
    vector<string> negExamples = getLinesFromFile(negExampleFile);
    NaiveBayesClassifier nbc;

    for (vector<string>::iterator posEx = posExamples.begin();posEx != posExamples.end();posEx++) {
      nbc.addTrainingExample(*posEx, 1);
    }

    for (vector<string>::iterator negEx = negExamples.begin();negEx != negExamples.end();negEx++) {
      nbc.addTrainingExample(*negEx, 0);
    }

    return nbc;
  }

  void evaluatePerformance(NaiveBayesClassifier nbc, string posTestFile, string negTestFile) {
    vector<string> posExamples = getLinesFromFile(posTestFile);
    vector<string> negExamples = getLinesFromFile(negTestFile);

    // print out each test case, and calculate accuracy and squared error
    printf("\n%-30s%12s%12s%12s%10s\n", "string", "true class", "pred.class", "postr C=1", "correct?");
    printf("%-30s%12s%12s%12s%10s\n\n", "------", "----------", "----------", "---------", "--------");

    int numCorrect = 0;
    int numTried = 0;
    double squaredError = 0;

    for (vector<string>::iterator posEx = posExamples.begin();posEx != posExamples.end();posEx++) {
      double probClass1 = nbc.getPosteriorProbability(1, *posEx);//getProbabilityClass1(*posEx);
      int predictedClass = nbc.classify(*posEx);//probClass1 >= .5) ? 1 : 0;



      printf("%-30s%12d%12d%12f%10s\n",
             truncateString(*posEx).c_str(),   // %-30s
             1,                                // %12d
             predictedClass,                   // %12d
             probClass1,                       // %12f
             predictedClass == 1 ? "Y" : "N"); // %10s

      if (predictedClass == 1) {
        numCorrect++;
      }
      numTried++;
      double error = 1 - probClass1;
      squaredError += error * error;
    }

    for (vector<string>::iterator negEx = negExamples.begin();negEx != negExamples.end();negEx++) {

      double probClass1 = nbc.getPosteriorProbability(1, *negEx);
      int predictedClass = nbc.classify(*negEx);//(probClass1 >= .5) ? 1 : 0;

      printf("%-30s%12d%12d%12f%10s\n",
              truncateString(*negEx).c_str(),    // %-30s
              0,                                 // %12d
              predictedClass,                    // %12d
              probClass1,                        // %12f
              predictedClass == 0 ? "Y" : "N");  // %10s
      //System.out.format(negEx + "\t0\t%.4f\t" + (1 - predictedClass) + "\n", probClass1);
      if (predictedClass == 0) {
        numCorrect++;
      }
      numTried++;
      double error = probClass1;
      squaredError += error * error;
    }

    cout << "\nSummary of "
         << numTried << " test cases, " << numCorrect << " correct; accuracy = " <<( (double) numCorrect / (double)numTried) << endl
         << "Mean squared error: " << (squaredError / (double) numTried) << endl;
  }

  std::pair<int,int> evaluateMultiWay(NaiveBayesClassifier nbc1, NaiveBayesClassifier nbc2, NaiveBayesClassifier nbc3,
                                      string testFile, int trueClass) {

    int numCorrect = 0;
    int numTried = 0;

    vector<string> c1Examples = getLinesFromFile(testFile);
    for (vector<string>::iterator c1Ex = c1Examples.begin();c1Ex != c1Examples.end();c1Ex++) {

      double probClass1 = nbc1.getPosteriorProbability(1, *c1Ex);
      double probClass2 = nbc2.getPosteriorProbability(1, *c1Ex);
      double probClass3 = nbc3.getPosteriorProbability(1, *c1Ex);

      // find the highest prob (& its class)
      double max1 = std::max(probClass1, probClass2);
      int high1 = (probClass1 >= probClass2) ? 0 : 1;
      double max2 = std::max(max1, probClass3);
      int predictedClass = (max2 > max1) ? 2 : high1;

      printf("%-30s%12d%12d%12f%12f%12f%10s\n",
             truncateString(*c1Ex).c_str(),   // %-30s
             trueClass,                        // %12d
             predictedClass,                   // %12d
             probClass1,                       // %12f
             probClass2,                       // %12f
             probClass3,                       // %12f
             predictedClass == trueClass ? "Y" : "N"); // %10s

      if (predictedClass == trueClass) {
        numCorrect++;
      }
      numTried++;
    }
    return std::pair<int,int>(numCorrect,numTried);

  }

  void multiWayMain(int argc, char *argv[]) {
    cout << "multi-way classification" << endl;

    string c1TrainingFile(argv[1]);
    string c2TrainingFile(argv[2]);
    string c3TrainingFile(argv[3]);
    string c1TestFile(argv[4]);
    string c2TestFile(argv[5]);
    string c3TestFile(argv[6]);

    // Create 3 nbcs, one to recognize each class
    NaiveBayesClassifier nbc1 = trainClassifier(c1TrainingFile, c2TrainingFile);
    vector<string> negExamples = getLinesFromFile(c3TrainingFile);
    for (vector<string>::iterator negEx = negExamples.begin();negEx != negExamples.end();negEx++) {
      nbc1.addTrainingExample(*negEx, 0);
    }

    NaiveBayesClassifier nbc2 = trainClassifier(c2TrainingFile, c1TrainingFile);
    negExamples = getLinesFromFile(c3TrainingFile);
    for (vector<string>::iterator negEx = negExamples.begin();negEx != negExamples.end();negEx++) {
      nbc2.addTrainingExample(*negEx, 0);
    }

    NaiveBayesClassifier nbc3 = trainClassifier(c3TrainingFile, c2TrainingFile);
    negExamples = getLinesFromFile(c1TrainingFile);
    for (vector<string>::iterator negEx = negExamples.begin();negEx != negExamples.end();negEx++) {
      nbc3.addTrainingExample(*negEx, 0);
    }


    // print out each test case, and calculate accuracy and squared error
    printf("\n%-30s%12s%12s%12s%12s%12s%10s\n", "string", "true class", "pred.class", "postr C=1", "postr C=2", "postr C=3", "correct?");
    printf("%-30s%12s%12s%12s%12s%12s%10s\n\n", "------", "----------", "----------", "---------", "---------", "---------", "--------");

    //    cout << "string\ttrue class\tprediction\t3 probs\tcorrect?";

    std::pair<int,int> results = evaluateMultiWay(nbc1, nbc2, nbc3, c1TestFile, 0);
    std::pair<int,int> res2 = evaluateMultiWay(nbc1, nbc2, nbc3, c2TestFile, 1);
    std::pair<int,int> res3 = evaluateMultiWay(nbc1, nbc2, nbc3, c3TestFile, 2);

    int numTried = results.second + res2.second + res3.second;
    int numCorrect = results.first + res2.first + res3.first;

    cout << "\nSummary of "
         << numTried << " test cases, " << numCorrect << " correct; accuracy = " <<( (double) numCorrect / (double)numTried) << endl;
  }


  int main2(int argc, char *argv[]) {

    if (argc != 5 && argc != 7) {
      cout << endl << "Usage: " << endl << endl
           << "     $ ./textClassifier class1-trainfile class2-trainfile class1-testfile class2-testfile" << endl
           <<          "\nOr, for 3-way classification (done as 3 1-vs-many classifiers):" << endl << endl
           <<"      $ ./textClassifier class1-trainfile class2-trainfile class3-trainfile class1-testfile class2-testfile class3-testfile" << endl << endl;
      return -1;
    }

    bool multiWay = (argc >= 7);
    if (multiWay) {
      multiWayMain(argc,argv);
      return 0;
    }

    // open files, read strings, feed to classifier
    string posTrainingFile = argv[1];
    string negTrainingFile = argv[2];
    string posTestFile = argv[3];
    string negTestFile = argv[4];

    NaiveBayesClassifier nbc = trainClassifier(posTrainingFile, negTrainingFile);

    // how well does it do?
    evaluatePerformance(nbc, posTestFile, negTestFile);

    return 0;
  }

