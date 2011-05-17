/*
 * string_utilities.hpp
 *
 *  Created on: May 4, 2011
 *      Author: ethan
 */

#ifndef STRING_UTILITIES_HPP_
#define STRING_UTILITIES_HPP_

#include <string>

using std::string;


// remove leading and trailing spaces
inline string trim(const string& source, const string& t = " ")
{
  string str = source;
  str.erase( str.find_last_not_of(t) + 1);
  str.erase(0 , source.find_first_not_of(t) );
  return str;
}

// remove all non-alphabetic characters from the string
inline string removeNonAlpha(const string& source) {
  string str = source;
  for (string::iterator it=str.end()-1; it>=str.begin(); --it) {
      if ( !isalpha(*it)) {
          str.erase(it);
      }
  }
  return str;
}

// Remove multiple white spaces in a row
inline string removeMultipleSpaces(const string& source)
{
  string str = source;
  string search = "  "; // this is 2 spaces
  size_t index;

  // remove 1 character from the string at index
  while( (index = str.find(search)) != string::npos ){
    str.erase(index,1);
  }

  return str;
}

string truncateString(string s) {
  if (s.size() > 30) {
    s = s.substr(0,27);     // Change it to the first 27 chars
    s.append("...",3);  // + "..."
  }
  return s;
}




/*
  // Extract strings from a text file, tokenized by white space
  // (This isn't used, but the method is here if you want to use it for anything in part 4)
  protected static ArrayList<String> getWordsFromFile(String fileName) {

    ArrayList<String> lines = new ArrayList<String>();
    try {
      BufferedReader in = new BufferedReader(new FileReader(fileName));
      String line;
      while ((line = in.readLine()) != null) {
        line = line.replaceAll("[^a-zA-Z\\s]+", ""); // remove all non-alphabetic characters
        line = line.replaceAll("^\\s+", "");   // trim opening & trailing whitespace
        line = line.replaceAll("\\s+$", "");
        line = line.replaceAll("\\s\\s", "\\s"); // Remove double spaces
        if (line.length() > 0) {
          String[] pieces = line.split("\\s+");  // split on spaces
          for (String piece : pieces) {
            lines.add(piece.toUpperCase());
          }
        }

      }
      in.close();

    } catch (IOException e) {
      e.printStackTrace();

    }
    return lines;
  }
  */

#endif /* STRING_UTILITIES_HPP_ */
