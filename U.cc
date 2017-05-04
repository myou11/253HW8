// Name: Maxwell You
// Date: 2017-04-29
// Purpose: Overload operators

#include "U.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Default ctor using initializer list
U::U() : charsRead("") { }	// initialized charsRead to be empty

// Copy ctor
U::U(const U & rhs) : charsRead(rhs.charsRead), charsReadVect(rhs.charsReadVect) { } 	// copy all rhs's class members (private vars) to

// Filename ctor
U::U(string filename) {
	charsRead = "";			// initialize string to empty string
	readfile(filename);		// call readfile to do the work
}

// Assignment operator=
const U & U::operator=(const U & rhs) {
	if (this != &rhs) {						// rhs by itself is the actual object, but with the & (& rhs), it get the pointer to rhs, which is also what this is (a pointer to the object)
		charsRead = rhs.charsRead;			// assign contents of rhs's charsRead to this object
		charsReadVect = rhs.charsReadVect; 	// assign contents of rhs's charsReadVect to this obj
	}
	return *this; 							// this is just a pointer, *this is the actual object
}

// Dtor
U::~U( ) { }								// default destructor

// Error check ifstream for failure
void U::streamFail(ifstream & in, int byteNum, string filename) const {
	if (in.fail()) {
		ostringstream oss;
		oss << "Byte " << byteNum << " was unable to be retrieved (doesn't exist) in file: " << filename;
		throw oss.str();
	}
}

// Error check for invalid continuation byte
void U::contByteFail(int byte) const {
	if ( (byte & 0xC0) != 0x80 ) {
		ostringstream oss;
		oss << "0x" << hex << byte << " was not a valid continuation byte";
		throw oss.str();
	}
}

// Determines byte length of character
int U::bytes(int byte) const {
	if ( (byte & 0x80) == 0 )
		return 1;
		
	else if ( (byte & 0xE0) == 0xC0 )
		return 2;
	
	else if ( (byte & 0xF0) == 0xE0 )
		return 3;

	else if ( (byte & 0xF8) == 0xF0 )
		return 4;

	else {	// means byte is not valid UTF8 leading byte
		ostringstream oss;
		oss << "Not a valid UTF8 character. Bad leading byte: 0x"<< hex << byte;
		throw oss.str();
	}
}

// Read and check for valid UTF8 characters
void U::readUTF(int byte1, ifstream & in, string filename) {
	char fbyte = byte1;	// used to concatenate first byte to the charsRead string
	char c; // to get the additional UTF bytes

	// 1 byte
	if (bytes(byte1) == 1)
		charsRead += fbyte; // add first byte of char to charsRead

	// 2 bytes
	else if (bytes(byte1) == 2) {
		charsRead += fbyte;	// add first byte of char to charsRead
		
		in.get(c);
		streamFail(in, 2, filename);	// check if the stream failed to get a character
		int byte2 = c;
		contByteFail(byte2);	// check if this is a valid continuation byte
		charsRead += c;
	}

	// 3 bytes
	else if (bytes(byte1) == 3) {
		charsRead += fbyte;
		
		in.get(c);
		streamFail(in, 2, filename);	
		int byte2 = c;
		contByteFail(byte2);
		charsRead += c;

		in.get(c);
		streamFail(in, 3, filename);
		int byte3 = c;
		contByteFail(byte3);
		charsRead += c;
	}

	// 4 bytes
	else if (bytes(byte1) == 4) {
		charsRead += fbyte;	// add first byte of char to charsRead

		in.get(c);
		streamFail(in, 2, filename);
		int byte2 = c;
		contByteFail(byte2);
		charsRead += c;

		in.get(c);
		streamFail(in, 3, filename);
		int byte3 = c;
		contByteFail(byte3);
		charsRead += c;

		in.get(c);
		streamFail(in, 4, filename);
		int byte4 = c;
		contByteFail(byte4);
		charsRead += c;
	}
}

// Read characters from an input file
void U::readfile(string filename) {
	ifstream in(filename);

	if (!in) {
		ostringstream oss;
		oss << "Unable to read file: " << filename;
		throw oss.str();
	}

	char c;
	while(in.get(c)) {
		if (in.fail()) {
			ostringstream oss;
			oss << "Byte 1 was unable to be retrieved (doesn't exist) in file: " << filename;
			throw oss.str();
		}
		
		readUTF(c, in, filename);
	}

	in.close();		// close the file stream

	// Create vector from this accumulated string
	for (uint i = 0; i < charsRead.length(); /* leave incrementing work for body of loop */) {
		if (bytes(charsRead.at(i)) == 1) {						// evaluate # of bytes a character consists of
			charsReadVect.push_back(charsRead.substr(i, 1));	// add the char to the vector (no need to error check since readUTF already did)
			++i;												// incr once because char was 1 byte
		} else if (bytes(charsRead.at(i)) == 2) {
			charsReadVect.push_back(charsRead.substr(i, 2));
			i += 2;
		} else if (bytes(charsRead.at(i)) == 3) {
			charsReadVect.push_back(charsRead.substr(i, 3));
			i += 3;
		} else if (bytes(charsRead.at(i)) == 4) {
			charsReadVect.push_back(charsRead.substr(i, 4));
			i += 4;
		}
	}
}

void U::append(string extra) {
	string utfChar;		// stores all bytes of UTF8 character as a string
	for (uint i = 0; i < extra.length(); /* let loop body incr */) {

		/* These conditional will run convUTF based on the # of bytes a character is
		 * convUTF is passed the current character (which should be the start of a UTF8 charac)
		 * and it will pass it the rest of the character's bytes too
		 * The index, i, gets incremented accordingly based on # of bytes
		 */

		if (bytes(extra.at(i)) == 1) {			// evaluate # of bytes a character consists of
			utfChar = extra.substr(i, 1);		// get 1 character from string
			convUTF(extra.at(i), utfChar);		// use convUTF to check for invalid UTF8 char
			
			charsRead += utfChar;				// add the char to the accumulated string
			charsReadVect.push_back(utfChar);	// add the char to the vector

			++i;								// incr once because char was 1 byte
		} else if (bytes(extra.at(i)) == 2) {
			utfChar = extra.substr(i, 2);
			convUTF(extra.at(i), utfChar);

			charsRead += utfChar;
			charsReadVect.push_back(utfChar);

			i += 2;
		} else if (bytes(extra.at(i)) == 3) {
			utfChar = extra.substr(i, 3);
			convUTF(extra.at(i), extra.substr(i, 3));

			charsRead += utfChar;
			charsReadVect.push_back(utfChar);

			i += 3;
		} else if (bytes(extra.at(i)) == 4) {
			utfChar = extra.substr(i, 4);
			convUTF(extra.at(i), extra.substr(i, 4));

			charsRead += utfChar;
			charsReadVect.push_back(utfChar);

			i += 4;
		}
		// no need to error check since convUTF() and bytes() will take care of that
	}
}

// State how many characters read thus far
int U::size() const {
	return charsReadVect.size();		// return length of the vector
}

// Get all chars read thus far
string U::get() const {
	return charsRead;
}

// Get char at index
string U::get(int index) const {
	if (index > (this->size() - 1) || index < 0) {
		ostringstream oss;
		oss << "Invalid index: " << index << " (valid range: [0," << this->size() << "))";
		throw oss.str();
	}
	return charsReadVect.at(index);
}

// Get chars from start to end
string U::get(int start, int end) const {
	if (start < 0 || end > (this->size() - 1)) {
		ostringstream oss;
		if (start < 0 && end > this->size() - 1)
			oss << "Invalid indices: (" << start << ", " << end << ") (valid range: [0," << this->size() << "))";
		else if (start < 0)
			oss << "Invalid index: " << start << " (valid range: [0," << this->size() << "))";
		else 
			oss << "Invalid index: " << end << " (valid range: [0," << this->size() << "))";
		throw oss.str();
	}
	
	string res = "";
	for (int i = start; i < end; ++i) {
		res += charsReadVect.at(i);
	}

	return res;
}

int U::convUTF(int byte1, string charac) const {	// index is passed by reference because it will change the actual value of index
																			// in the loop this function will be called in
	char c; // to get the additional UTF bytes

	if ( (byte1 & 0x80) == 0 ) 
		return (byte1 & 0x7F);

	if ( (byte1 & 0xE0) == 0xC0 ) {
		c = charac.at(1);
		int byte2 = c;
		contByteFail(byte2);

		byte1 = (byte1 << 6) & 0x7C0;
		byte2 = byte2 & 0x3F;

		return (byte1 | byte2);
	}

	if ( (byte1 & 0xF0) == 0xE0) {
		c = charac.at(1);
		int byte2 = c;
		contByteFail(byte2);

		c = charac.at(2);
		int byte3 = c;
		contByteFail(byte3);

		byte1 = (byte1 << 12) & 0xF000; // shift to bits 15-12, mask upper 4 bits
		byte2 = (byte2 << 6) & 0xFC0;   // shift to bits 11-6, mask bits 12-7
		byte3 = byte3 & 0x3F;			// shift to bits 5-0, mask bits 5-0

		return (byte1 | byte2 | byte3); // combine the 3 bytes to get unicode
	}

	if ( (byte1 & 0xF8) == 0xF0 ) {
		c = charac.at(1);
		int byte2 = c;
		contByteFail(byte2);

		c = charac.at(2);
		int byte3 = c;
		contByteFail(byte3);

		c = charac.at(3);
		int byte4 = c;
		contByteFail(byte4);


		byte1 = (byte1 & 0x07) << 18; // get lower 3 bits of 1st byte, shift to bits 20-18
		byte2 = (byte2 & 0x3F) << 12; // get lower 6 bits of 2nd byte, shift to bits 17-12
		byte3 = (byte3 & 0x3F) << 6;  // get lower 6 bits of 3rd byte, shift to bits 11-6
		byte4 = byte4 & 0x3F;		  // get lower 6 bits of 4th byte, shift to bits 5-0
		
		return (byte1 | byte2 | byte3 | byte4); // combine the 4 bytes to get unicode
	}

	return -1; // return -1 if not a valid unicode character
}

// Return the codepoint at charsRead[index]
int U::codepoint(int index) const {
	string charac = get(index);
	return convUTF(charac.at(0), charac);
}

// Return true if charsRead is empty
bool U::empty() const {
	return charsReadVect.empty();
}

// Clear charsRead
void U::clear() {
	charsRead = "";
	charsReadVect.clear();
}

// Operator Overloading
// Assuming u is a U, p is a P, and s is a string in code below

// Assignment
U & U::operator=(const string & s) {
	this->clear();		// clear the object because we are about to overwrite it
	this->append(s);	// append the new string 
	return *this;		// return lhs(this)
}

// Append
U & U::operator+=(const string & s) {
	this->append(s);	// append will add the accumulated string in rhs to our accumulated string and vector
	return *this;		// return lhs(this)
}

U & U::operator+=(const U & rhs) {
	U temp(rhs);					// make a copy of rhs to ensure we won't modify it
	this->append(temp.charsRead);	// append will add the accumulated string in rhs to our accumulated string and vector
	return *this;					// return the lhs(this)
}

// Concatenation

// u + u
const U U::operator+(const U & rhs) const {	// 
	U result(*this);	// make a copy of the lhs(this) to ensure we won't modify it
	result += rhs;		// let operator+=(U) to do the work
	return result;		// return the result
}

// u + s
const U U::operator+(const string & s) const {
	U result(*this);	// make a copy of the lhs(this) to ensure we won't modify it
	result += s;		// let operator+=(string) do the work
	return result;		// return the result
}

// s + u
const U operator+(const string & s, const U & rhs) {
	U result;			// create a U to hold the string
	result.append(s);	// add string to result
	result += rhs;		// let operator+=(U) do the work
	return result;		// return the new string obj
}

// Subscripting

// u[index]
string U::operator[](int index) const {
	return get(index);	// let get(index) do the work
}

// Send u to output stream

// cout << u
ostream & operator<<(ostream & out, const U & rhs) {
	out << rhs.get();	// get the accumulated string and send it to the output stream
	return out;
}

// Boolean evaluation
U::operator bool() const {
	return !(empty());
}

// Comparison

// u == u
bool U::operator==(const U & rhs) const {
	return charsRead == rhs.charsRead;	// compare the accumulated strings of the two objs
}

// u == s
bool U::operator==(const string & s) const {
	return charsRead == s;	// compare the accumulated string and s
}

// s == u
bool operator==(const string & s, const U & rhs) {
	return s == rhs.charsRead;	// compare the s and the accumulated string
}

// u != u
bool U::operator!=(const U & rhs) const {
	return !(*this == rhs);	// negate the result of u == u
}

// u != s
bool U::operator!=(const string & s) const {
	return !(*this == s);	// negate the result of u == s
}

// s != u
bool operator!=(const string & s, const U & rhs) {
	return !(s == rhs);	// negate the result of s == u
}
