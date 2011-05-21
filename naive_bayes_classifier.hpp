/*
 * naive_bayes_classifier.hpp
 *
 *  Created on: May 3, 2011
 *      Author: ethan
 */

#ifndef NAIVE_BAYES_CLASSIFIER_HPP_
#define NAIVE_BAYES_CLASSIFIER_HPP_


/**
 * Two-class naive Bayes classifier.
 */

#include "feature.hpp"
#include <vector>


#define CLASS_DEBUG 1 

using std::vector;

class NaiveBayesClassifier {

  /* "m_features" maintains a list of all the features we are considering
   * in this classifier.
   *
   * "m_classTotals" counts how many training examples we've seen for each
   * of the two classes (0 and 1).
   */
protected :
  vector<Feature> m_features;
  int m_classTotals[2];
  
  // This is P(Y)
  double getPriorProbability(int classNumber) {
    /*
     * Compute and return the prior probability that a string belongs to a class.
     * This is P(C=classNum).
     */
    


    int total = m_classTotals[0] + m_classTotals[1];
    
    if(total == 0) total = 1;
    
    double result = (double)m_classTotals[classNumber]/total;  // stub, replace me!
    return result;
  }
  
  // This is P(Data | Y)
  double getLikelihood(int classNumber, string s)
  {
    /*
     * Compute and return the likelihood of the class, given the features.
     * This is P(F1=f1 and F2=f2 and ... Fn=fn | C=classNumber)
     *   where f1, f2, ..., fn are 0 or 1 depending on the presence or absence
     *   of the feature in that string.
     */
    
    double result = 1.0;
    
    // Go through every feature, figure out its probability given the classification, reflect it in the result.
    
    for(unsigned int i = 0; i < m_features.size(); i++){
      
      // Check if the feature is present
      int featurePresence = m_features[i].isFeaturePresent(s);
      double featureProb = m_features[i].getProbOfFeatureGivenClass(featurePresence, classNumber);
      
      /* DEBUG
      if(featureProb == 0.0)
		fprintf(stderr, "getLikelihood: featureProb is zero for featurePresence = %d, class Y = %d\n", featurePresence, classNumber);
      */

      result *= featureProb;
    }

    // Return the result
    return result;
  }

public :
  NaiveBayesClassifier() {

    m_classTotals[0] = 0;  // initialize m_classTotals to 0;
    m_classTotals[1] = 0;

    /* 
     * Create the set of features that we'll use.
     * For part 1 of the assignment, there are 26 features:
     * one for each letter of the alphabet.  So, you need to
     * create each feature and add it to m_features.
     */

    for(char i = 'A'; i <= 'Z'; i++){
      // For each character create a feature and add it to the vector
      string character(1,i);
      Feature f(character);
      
      m_features.push_back(f);
    }
    

    // DEBUG
    //    printf("Size of vector = %d\n", (int)m_features.size());
    
  }

  void addTrainingExample(string s, int classNumber) {
    /*
     * Given a string and its correct classification, update:
     * (1) the counts that are used to compute the likelihoods.
     *     Feature.addTrainingExample(featureVal, classNumber) should take care of this,
     *     but you need to determine whether the feature is present for each feature.
     * (2) the counts that are used to compute the prior probability
     */
    
    // For every feature, check if the training string has the feature present.
    // Then update accordingly
    
    for(unsigned int i = 0; i < m_features.size(); i++) {
      //      Feature f = m_features[i];
      
      
      // DEBUG
      //      if(CLASS_DEBUG)
      // fprintf(stderr, "Feature: %s address %p\n", m_features[i].getFeatureMatch().c_str(), &m_features[i]);
      
      if(m_features[i].isFeaturePresent(s)){
	m_features[i].addTrainingExample(1, classNumber);
      }else{
	m_features[i].addTrainingExample(0, classNumber);
      }
      
    }
    
    m_classTotals[classNumber]++;

  }

  int classify(string s) {
    /* 
     * Compute and return the most probable class (0 or 1) that this string
     * belongs to, using the maximum a posteriori (MAP) decision rule.
     *
     * You can do this using calls to getPriorProbability and getLikelihood.
     * You do not need to call getPosteriorProbability.
     */
    
    
    // Find out the chance that the class is 0
    double class_0_chance = this->getPriorProbability(0) * this->getLikelihood(0, s);
    
    
    // Find out the chance that the class is 1
    double class_1_chance = this->getPriorProbability(1) * this->getLikelihood(1, s);
    
    if(class_0_chance > class_1_chance) 
      return 0;
    else return 1;

  }

  double getPosteriorProbability(int classNumber, string s)
  {
    /* YOUR CODE HERE
     *
     * Compute and return the probability that the given string belongs to
     * class classNumber.
     * This is P(C=classNumber | F1=f1 and F2=f2 and ... Fn=fn)
     *   where f1, f2, ..., fn are 0 or 1 depending on the presence or absence
     *   of the feature in that string.
     *
     * The code will be similar to classify, except that you need to also
     * need to use Bayes' rule and the law of total probability.
     *
     * Note that this function is not necessary for a working classifier!
     * That is, you don't need to call getPosteriorProbability inside of classify.
     */
    

    // To get P(X) = P(X | Y = 0)P(Y = 0) + P(X | Y = 1) P( Y = 1)
    double px_equal_0 = this->getLikelihood(0, s) * this->getPriorProbability(0); 
    double px_equal_1 = this->getLikelihood(1, s) * this->getPriorProbability(1);

    // DEBUG
    // if(px_equal_0 == 0.0) 
    //   fprintf(stderr, "naive.getPosteriorProb: For feature %s | classNumber = %d | X = 0 is ZERO!!!\n",s.c_str(), classNumber);

    // if(px_equal_1 == 0.0) 
    //   fprintf(stderr, "naive.getPosteriorProb: For feature %s | classNumber = %d | X = 1 is ZERO!!!\n", s.c_str(), classNumber);
    


    double px = (double)px_equal_0 + px_equal_1;
   
    
    
    
    // Find out the chance that the class is 0
    double class_chance = this->getPriorProbability(classNumber) *
                                        this->getLikelihood(classNumber, s);

    if(CLASS_DEBUG && this->getPriorProbability(classNumber) == 0)
      fprintf(stderr, "naive.getPosteriorProb:For feature %s, classNumber = %d -> Prior Prob is 0\n", s.c_str(), classNumber);

    if(CLASS_DEBUG && this->getLikelihood(classNumber,s ) == 0)
      fprintf(stderr, "naive.getPosteriorProb:For feature %s, classNumber = %d -> LIKELIHOOD is 0\n", s.c_str(), classNumber);

    
    // Make sure don't have a divide by zero
    if(px == 0.0)
      {
	if(CLASS_DEBUG)
	  fprintf(stderr, "naive.getPosteriorProb: X is ZERO!!!\n");
	return 0.0;
      }
    else{
      double result = (double) (class_chance/px);

      if(result == 0.0 && CLASS_DEBUG)
	fprintf(stderr, "naive.getPosteriorProb:For feature %s, classNumber = %d -> is 0\n", s.c_str(), classNumber);
      
      return result;
    }

    
    
  }

};

#endif /* NAIVE_BAYES_CLASSIFIER_HPP_ */
