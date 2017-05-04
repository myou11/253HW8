// Name: Maxwell You
// Date: 2017-04-29
// Purpose: Overload operators

#ifndef U_H
#define U_H

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

class U {

	public:

		// Ctors
		U();
		U(const U &rhs);
		U(std::string filename);

		// Assignment operator=
		const U &operator=(const U &rhs);

		// Dtor
		~U();

		// Error check ifstream for failure
		void streamFail(std::ifstream &in, int byteNum, std::string filename) const;

		// Error check for invalid continuation byte
		void contByteFail(int byte) const;

		// Determines byte length of character
		int bytes(int index) const;

		// Modified convUTF for reading from a stream and checking valid chars
		void readUTF(int byte1, std::ifstream &in, std::string filename);

		// Read characters from an input file
		void readfile(std::string filename);

		// Adds string to accumulated string
		void append(std::string extra);

		// State how many characters read thus far
		int size() const;

		// Get all chars read thus far
		std::string get() const;

		// Get char at index
		std::string get(int index) const;

		// Get chars from start to end
		std::string get(int start, int end) const;

		// Convert UTF8 encoding to Unicode
		int convUTF(int byte1, std::string charac) const;

		// Return the Unicode codepoint at the index in the accumulated string
		int codepoint(int index) const;

		// Return true if accumulated string is empty
		bool empty() const;

		// Removes all data from the obj
		void clear();

		// Operator Overloading

		// Assignment
		U &operator=(const std::string & s);

		// Append
		
		// u += s
		U & operator+=(const std::string & s);

		// u += u
		U & operator+=(const U & rhs);

		// Concatenation

		// u + u
		const U operator+(const U & rhs) const;

		// u + s
		const U operator+(const std::string & s) const;

		// s + u
		friend const U operator+(const std::string & s, const U & rhs);

		// Subscripting

		// u[index]
		std::string operator[](int index) const;

		// Send a U to the output stream

		// cout << u
		friend std::ostream & operator<<(std::ostream & out, const U & rhs);

		// Boolean evaluation
		operator bool() const;

		// Comparison

		// u == u
		bool operator==(const U & rhs) const;

		// u == s
		bool operator==(const std::string & s) const;

		// s == u
		friend bool operator==(const std::string & s, const U & rhs);

		// u != u
		bool operator!=(const U & rhs) const;

		// u != s
		bool operator!=(const std::string & s) const;

		// s != u
		friend bool operator!=(const std::string & s, const U & rhs);

	private:

		std::string charsRead; 						// stores all characters read thus far
		std::vector<std::string> charsReadVect;		// stores UTF8 characters in each index
};

// s + u
const U operator+(const std::string & s, const U & rhs);

// cout << u
std::ostream & operator<<(std::ostream & out, const U & rhs);

// s == u
bool operator==(const std::string & s, const U & rhs);

// s != u
bool operator!=(const std::string & s, const U & rhs);

#endif
