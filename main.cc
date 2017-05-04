#include "U.h"
#include "P.h"
#include <iostream>
using namespace std;
const string pub="/s/bach/a/class/cs253/pub/";   // ~ only works in shells
int main() {
	try {
		U u;
		u.append("a³+b³≠c³");
		P p(pub+"UnicodeData.txt");
		
		for (int i=0; i<u.size(); i++)
			p.count(u.codepoint(i));
		
		/* -------------- U testing ----------------*/
		
		cout << "\n\n------------- U testing ----------------\n\n";

		// size and empty
		cout << "Should be 8: " << u.size() << '\n'
			<< "Should be false: " << u.empty() << '\n';
		
		// get
		cout << "Should be a³+b³≠c³: " << u.get() << '\n'
			<< "Should be ≠: " << u.get(5) << '\n'
			<< "Should be b³: " << u.get(3,5) << '\n';
		try {
			u.get(-1);
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}
		try {
			u.get(9);
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}
		try {
			u.get(-1,9);
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}
		
		// append
		cout << "Before: " << u.get() << '\n';
		u.append("marvεlΩus");
		cout << "After: " << u.get() << '\n';

		// readfile
		cout << "Before: " << u.get() << '\n';
		u.readfile("story.txt");
		cout << "After: " << u.get() << '\n';
		try {
			u.readfile("nothere.txt");
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}
		try {
			u.readfile("/bogus");
		} catch (const string &msg) {
			cout << "I expected this: " << msg << '\n';
		}

		// codepoint
		cout << "Should be ≠: " << u.get(5) << '\n';
		cout << "Should be 2260 converted to dec (8800): " << u.codepoint(5) << '\n';
		cout << "Should be 97: " << u.codepoint(0) << '\n';
		cout << "Should be 179: " << u.codepoint(1) << '\n';
		cout << "Should be 99: " << u.codepoint(6) << '\n';
		try {
			u.codepoint(-1);
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}
		try {
			u.codepoint(8);
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}

		// clear
		cout << "Clearing u...\n";
		u.clear();
		cout << "Size should be 0: " << u.size() << '\n';
		cout << "Should be empty (true): " << u.empty() << '\n';

		/* -------------- P testing ----------------*/
		
		cout << "\n\n------------- P testing ----------------\n\n";
		
		// readfile (should be error because already read a propfile)
		try {
			p.readfile("UnicodeDataV2.txt");
		} catch (const string &s) {
			cout << "Should be error: " << s << '\n';
		}

		// count(string)
		cout << "Should be 2: " << p.count("Sm") << '\n'
			<< "Should be 3: " << p.count("Ll") << '\n';

		// count(int)
		cout << "# Ll's before: " << p.count("Ll") << '\n';
		p.count(122);
		cout << "# Ll's after: " << p.count("Ll") << '\n';
		
		// props
		set<string> props = p.props();
		for (auto &s : props)
			cout << s << ' ';
		cout << '\n';

		// size
		cout << "Should be 29: " << p.size() << '\n';

		// empty
		cout << "Should be false: " << p.empty() << '\n';

		// clear
		cout << "Clearing p...\n";
		p.clear();
		cout << "Size should be 0: " << p.size() << '\n';
		cout << "Should be empty (true): " << p.empty() << '\n';

		// testing hamlet
		cout << "Testing hamlet\n";
		U ham = U(pub + "hamlet.txt");
		cout << "Should be 182568: " << ham.size() << '\n';

		
		/* -------------- Op Overloading tests ----------------*/
		
		cout << "\n\n------------- Op Overloading tests ----------------\n\n";

		U badFruit;	// used to test invalid cases

		/* 	
		==================
		Assignment (u = s)
		================== 	
		*/

		// testing with a const string
		U apple;
		apple.append("peanut butter");
		cout << "apple should be peanut butter: " << apple.get() << '\n';
		const string honey = "honey";
		apple = honey;
		cout << "Changed apple, should be honey: " << apple.get() << '\n';

		// testing with invalid string
		try {
			badFruit = "\xFF";
		} catch (const string & s) {
			cout << "Tried to set badFruit = bad string, should be error: " << s << '\n';
		}
		
		/*
		======
		Append
		======
		*/

		// u += u
		
		// testing with a const obj
		cout << "apple should be honey: " << apple.get() << '\n';
		U banana;
		banana.append("butter");
		const U constBanana(banana);
		apple += constBanana;
		cout << "apple += const U, should be honeybutter: " << apple.get() << '\n';

		// u += s
		
		// testing with a const string
		const string good= " is good";
		apple += good;
		cout << R"(apple += const string, should be honeybutter is good: )" << apple.get() << '\n';

		/*
		=============
		Concatenation
		=============
		*/
		
		// u + s
		
		// testing with a const string
		U bananaMilk;
		const string  milk = " is from churning milk";
		bananaMilk =  banana + milk;
		cout << "banana + const string, should be butter is from churning milk: " << bananaMilk.get() << '\n';

		// u + u
		
		//testing with 2 const objs
		U straw;
		straw.append("straw");
		const U constStraw(straw);
		U berry;
		berry.append("berry");
		const U constBerry(berry);
		U strawberry = straw + berry;
		cout << "const U + const U, should be: " << strawberry.get() << '\n';

		// s + u

		// testing with a const string
		const string chocolate = "chocolate";
		U fondue;
		fondue.append("fondue");
		const U constFondue(fondue);
		fondue = chocolate + constFondue;
		cout << "const string + const U, should be chocolatefondue: " << fondue.get() << '\n';

		/*
		============
		Subscripting
		============
		*/

		// u[index]

		// testing with const obj
		cout << "constBanana[4], should be 'e': " << constBanana[4] << '\n';
		
		// testing with invalid index
		try {
			apple[-1];
		} catch (const string &s) {
			cout << "Should be invalid index error: " << s << '\n';
		}

		/*
		====================
		Sending u to ostream
		====================
		*/

		// cout << u

		// testing with const obj
		cout << "constBanana should be butter: " << constBanana << '\n';

		// testing with non-empty, then empty obj
		U blackberry;
		blackberry = "cheesecake";
		cout << "blackberry should be cheesecake: " << blackberry << '\n';
		blackberry.clear();
		cout << "blackberry was cleared, should be '':" << blackberry << '\n';

		/*
		====================
		Sending p to ostream
		====================
		*/

		// cout << p

		// testing with const obj
		const P mango(p);	// copy the properties of p from above 
		cout << "mango properties and counts are: " << mango << '\n';	// should be empty since p was cleared
		
		// testing with non const obj
		P orange(pub + "UnicodeData.txt");
		cout << "orange properties and counts are: " << orange << '\n';	// should be all properties

		/*
		==================
		Boolean evaluation
		==================
		*/

		// if (p)

		// testing with const obj
		if (mango)
			cout << "mango is not empty!" << '\n';
		else if (!mango)
			cout << "boolean evaluation worked, mango is empty!" << '\n';

		// while (!u)
		U grapes;
		int count = 0;
		while (!grapes) {
			if (count == 3) {
				grapes = "wine";
				cout << "grapes is now " << grapes << ", loop will end now\n";
			}
			else {
				cout << "grapes is still empty\n";
				++count;
			}
		}

		/*
		==========
		Comparison
		==========
		*/

		// u == u

		// testing with const obj == obj
		U rWine;
		const string wine = "redWine";
		rWine = wine;
		const U redWine(rWine);
		cout << "Comparing const redWine and grapes, should be false: "  << (redWine == grapes) << '\n';

		// testing with const obj == const obj
		const U redWineDup(redWine);
		cout << "Comparing const redWine and const redWineDup, should be true: " << (redWine == redWineDup) << '\n';

		// u == s
		
		// testing with const obj == s
		cout << R"(Comparing const redWine and "redwine", should be false: )" << (redWine == "redwine") << '\n';
		cout << R"(Comparing const redWine and "redWine", should be true: )" << (redWine == "redWine") << '\n';

		// s == u

		// testing with const s == const obj
		const string wWine = "whiteWine";
		cout << R"(Comparing const "whiteWine" with const redWine, should be false: )" << (wWine == redWine) << '\n';
		cout << R"(Comparing const "redWine" with const redWine, should be true: )" << (wine == redWine) << '\n';

		// u != u
		
		// testing const obj != const obj
		cout << "const redWine != const redWineDup, should be false: " << (redWine != redWineDup) << '\n';

		// testing const obj != obj
		cout << "const redWine != apple, should be true: " << (redWine != apple) << '\n';

		// u != s

		// testing with const obj != const s
		cout << R"(const constBanana != "butter", should be false: )" << (constBanana != "butter") << '\n';
		cout << R"(const constBanana != "peanut", should be true: )" << (constBanana != "peanut") << '\n';

		// s != u
		cout << R"("butter" != constBanana, should be false: )" << ("butter" != constBanana) << '\n';
		cout << R"("peanut" != constBanana, should be true: )" << ("peanut" != constBanana) << '\n';
		
		return 0;
	}
	catch (const string &msg) {
		cout << "Unexpected error: " << msg << '\n';
	}
}
