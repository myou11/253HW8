// Name: Maxwell You
// Date: 2017-04-29
// Purpose: Overload operators

#ifndef U_H
#define U_H

#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

class UIter;

class U {

	public:

		// typedef UIter as iterator
		typedef UIter iterator;

		// Ctors
		U();
		U(const U &rhs);
		U(std::string filename);
		
		// Templated iterator ctor
		template <typename T>
		U(T it1, T it2) {
			while (it1 != it2) {
				charsRead += *it1;
				++it1;
			}
			createUTFVect();
		} 

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

		// Create vector to hold UTF8 chars from accumulated string
		void createUTFVect();

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

		// Iterator methods
		
		// Returns iterator "pointing" to beginning of accumulated string
		iterator begin() const;

		// Returns iterator "pointing" to one index past accumulated string
		iterator end() const;

		// Returns first int codepoint by value
		int front() const;

		// Returns last int codepoint by value
		int back() const;

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

class UIter {
	public:
		// Ctors

		// Default ctor
		UIter(const U *u = nullptr, int i = 0) : parent(u), index(i) { }
		
		// Copy ctor
		UIter(const UIter & rhs) = default;	// use default b/c it will copy class members already

		// Assignment operator
		UIter & operator=(const UIter & rhs) = default;	// use default b/c it will copy class members already

		// Dtor
		~UIter() = default;	// TODO check for memory leaks later

		// Indirection
		int operator*() const {
			if (!parent) 	// if this iterator is not associated with a U object
				throw std::string("Attempting to indirect uninitialized iterator");
			if (index == parent->size())	// if the index of the current iterator is one past the elements
				throw std::string("Attempting to indirect end() iterator");
			
			return parent->codepoint(index);	// use codepoint of the U obj and the current iterator's index to get codepoint
		}

		// operator==
		bool operator==(const UIter & rhs) const {
			if (!parent || !rhs.parent)	// both iterators must not be nullptr
				throw std::string("Attempting to compare uninitialized iterator");
			if (parent != rhs.parent)	// both iterators must be associated with the same U object
				throw std::string("Attempting to compare iterators from different U objects");
			return index == rhs.index;	// compare indices of the iterators
		}

		bool operator!=(const UIter & rhs) const {
			return !(*this == rhs);	// negate the result of operator==
		}

		UIter & operator++() {	// pre-incr b/c no dummy int arg
			if (!parent)	// if iterator is uninitialized
				throw std::string("Attempting to increment uninitialized iterator");
			if (index == parent->size())	// can't incr if iterator is already at end (one past last item)
				throw std::string("Attempting to increment end() iterator");

			++index;		// incr the index
			return *this;	// return the altered iter
		}

		UIter operator++(int) {	// post-incr b/c of dummy int arg
			const auto save = *this;	// save the state
			++*this;					// incr the iter
			return save;				// return the prev state
		}

		UIter & operator--() {	// pre-decr b/c no dummy int arg
			if (!parent)	// if iterator is uninitialized
				throw std::string("Attempting to decrement uninitialized iterator");
			if (index == 0)	// can't decr if iterator is at beginning
				throw std::string("Attempting to decrement begin() iterator");

			--index;		// decrement the index
			return *this;	// return the altered iter
		}

		UIter operator--(int) {	// post-decr b/c of dummy int arg
			const auto save = *this;	// save the state
			--*this;					// decr the iter
			return *this;				// return the prev state
		}

	private:
		const U *parent;	// parent U
		int index;	// index into accumulated string
};

#endif
