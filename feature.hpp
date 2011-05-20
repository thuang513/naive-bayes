/*
 * feature.hpp
 *
 *  Created on: May 3, 2011
 *      Author: Ethan L. Schreiber
 *      Adapted from java code written by Andrew McGregor
 */

#ifndef FEATURE_HPP_
#define FEATURE_HPP_

#include <string>
#include <cstring>

using std::string;
/**
 * feature.hpp
 *
 * Stores counts and computes probabilities for one boolean feature.
 */
class Feature {

  /* counts[][] is a 2x2 matrix that stores for each of the
   * 2 classes (0 and 1), the number of training examples we've
   * seen that don't have the feature (0) or have it (1).
   *
   * So counts[0][0] would be the number of training examples that fall
   * into class 0, that don't have the feature.
   *
   * totalPerClass[] stores the number of training examples we've seen
   * per class.  So totalPerClass[i] should always equal
   * counts[i][0] + counts[i][1].
   */

protected:
  
  // Class 0 = String Not found
  // Class 1 = String Found

  int m_counts[2][2];
  int m_totalPerClass[2];
  string m_toMatch;

public :
  Feature(string toMatch) : m_toMatch(toMatch) {

    // Initialize the counters to 0
    m_counts[0][0] = 0;
    m_counts[0][1] = 0;
    m_counts[1][0] = 0;
    m_counts[1][1] = 0;

    m_totalPerClass[0] = 0;
    m_totalPerClass[1] = 0;
  }




#define FOUND_CLASS 1
#define NOT_FOUND_CLASS 0


  int isFeaturePresent(string s) {
    /* YOUR CODE HERE
     *
     * Determine whether this feature occurs in the string.
     * Return 1 if the feature is present; 0 otherwise.
     */
    
    // Scan through the string to see if m_toMatch can be found in the string.
    size_t searchResult = s.find(m_toMatch);
    
    return searchResult != string::npos;
  }

  void addTrainingExample(int featurePresence, int classNumber) {
    /* YOUR CODE HERE
     *
     * Given the absence/presence of this feature for the given class,
     * update the counts used to compute the probabilities.
     *
     * classNumber = the number of the class for this example (0 or 1)
     * featurePresence = 0 or 1 stating the presence or absence of this feature
     */
    
    // Record the correct class
    m_counts[classNumber][featurePresence]++;
    
    // DEBUG
    fprintf(stderr, "Feature.addTrainingExample: FEATURE: %s | m_count[%d][%d] = %d\n", m_toMatch.c_str(),classNumber, featurePresence, m_counts[classNumber][featurePresence]);

    
    // DEBUG
    fprintf(stderr, "Feature.addTrainingExample: BEFORE FEATURE: %s | m_totalPerClass[%d] = %d\n", m_toMatch.c_str(),classNumber, m_totalPerClass[classNumber]);


    // Update the total count
    this->m_totalPerClass[classNumber] += 1;

    
    // DEBUG
    fprintf(stderr, "Feature.addTrainingExample: AFTER FEATURE: %s | m_totalPerClass[%d] = %d\n", m_toMatch.c_str(),classNumber, m_totalPerClass[classNumber]);

  }

  double getProbOfFeatureGivenClass(int featurePresence, int classNumber) {
    /* YOUR CODE HERE
     *
     * What is the probability of this feature being absent/present for this class?
     * Use your counts, but make sure to smooth it by adding 1 to avoid probabilities of
     * absolute zero or one.
     */
    
    // Find the total count, and class+feature count.
    double result =  (double) m_counts[classNumber][featurePresence]/(1+m_totalPerClass[classNumber]);
    
    return result;
    
    //.. you need to smooth it so there's never a 0 or 1 probability
    
  }
};

#endif /* FEATURE_HPP_ */
