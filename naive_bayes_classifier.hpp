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

  double getPriorProbability(int classNumber) {
    /* YOUR CODE HERE
     *
     * Compute and return the prior probability that a string belongs to a class.
     * This is P(C=classNum).
     */

    return 0.0;  // stub, replace me!
  }

  double getLikelihood(int classNumber, string s)
  {
    /* YOUR CODE HERE
     *
     * Compute and return the likelihood of the class, given the features.
     * This is P(F1=f1 and F2=f2 and ... Fn=fn | C=classNumber)
     *   where f1, f2, ..., fn are 0 or 1 depending on the presence or absence
     *   of the feature in that string.
     */

    return 0.0;  // stub, replace me!
  }

public :
  NaiveBayesClassifier() {

    m_classTotals[0] = 0;  // initialize m_classTotals to 0;
    m_classTotals[1] = 0;

    /* YOUR CODE HERE
     *
     * Create the set of features that we'll use.
     * For part 1 of the assignment, there are 26 features:
     * one for each letter of the alphabet.  So, you need to
     * create each feature and add it to m_features.
     */
  }

  void addTrainingExample(string s, int classNumber) {
    /* YOUR CODE HERE
     *
     * Given a string and its correct classification, update:
     * (1) the counts that are used to compute the likelihoods.
     *     Feature.addTrainingExample(featureVal, classNumber) should take care of this,
     *     but you need to determine whether the feature is present for each feature.
     * (2) the counts that are used to compute the prior probability
     */

  }

  int classify(string s) {
    /* YOUR CODE HERE
     *
     * Compute and return the most probable class (0 or 1) that this string
     * belongs to, using the maximum a posteriori (MAP) decision rule.
     *
     * You can do this using calls to getPriorProbability and getLikelihood.
     * You do not need to call getPosteriorProbability.
     */

    return 0.0;  // stub, replace me!
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

    return 0.0;  // stub, replace me!
  }

};

#endif /* NAIVE_BAYES_CLASSIFIER_HPP_ */
