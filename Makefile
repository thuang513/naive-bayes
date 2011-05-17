CC=g++

FLAGS = -Wall

all: textClassifier

textClassifier : feature.hpp naive_bayes_classifier.hpp string_utilities.hpp text_classifier.cpp  
	$(CC) $(FLAGS) text_classifier.cpp -o $@

clean: 
	rm -f textClassifier
	
