#include "pch.h"
#include <iostream>
#include "list.h"
#include "hashtable.h"

using namespace mySTL;
using namespace std;

<<<<<<< HEAD
/* ×¢Òâ£ºlist µÄµü´úÆ÷ÎªË«Ïòµü´úÆ÷Ã»ÓĞ +n ²Ù×÷£¬Ö»ÄÜ ++ -- */
=======
<<<<<<< HEAD
/* ×¢Òâ£ºlist µÄµü´úÆ÷ÎªË«Ïòµü´úÆ÷Ã»ÓĞ +n ²Ù×÷£¬Ö»ÄÜ ++ -- */
=======
/* æ³¨æ„ï¼šlist çš„è¿­ä»£å™¨ä¸ºåŒå‘è¿­ä»£å™¨æ²¡æœ‰ +n æ“ä½œï¼Œåªèƒ½ ++ -- */
>>>>>>> update
>>>>>>> update

int main() {
	list<int> ilist;
	list<int>::iterator it;

	ilist.push_back(9);
	ilist.push_back(1);
	ilist.push_back(5);
	ilist.push_back(8);
	ilist.push_back(3);
	ilist.push_back(7);
	ilist.push_back(5);
	ilist.push_back(4);
	ilist.push_back(8);
	ilist.push_back(6);
	ilist.push_back(2);
	ilist.push_back(5);
<<<<<<< HEAD
	cout << "Ô­Ê¼×´¿ö: ilist.size() = " << ilist.size() << endl;
=======
<<<<<<< HEAD
	cout << "Ô­Ê¼×´¿ö: ilist.size() = " << ilist.size() << endl;
=======
	cout << "åŸå§‹çŠ¶å†µ: ilist.size() = " << ilist.size() << endl;
>>>>>>> update
>>>>>>> update
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	ilist.sort();
<<<<<<< HEAD
	cout << "ÅÅĞòºó: ilist.size() = " << ilist.size() << endl;
=======
<<<<<<< HEAD
	cout << "ÅÅĞòºó: ilist.size() = " << ilist.size() << endl;
=======
	cout << "æ’åºå: ilist.size() = " << ilist.size() << endl;
>>>>>>> update
>>>>>>> update
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	if ((it = find(ilist.begin(), ilist.end(), 5)) != ilist.end()) {
		ilist.insert(it, 99);
<<<<<<< HEAD
		cout << "ÔÚ5Ö®Ç°²åÈë99: ilist.size() = " << ilist.size() << endl;
=======
<<<<<<< HEAD
		cout << "ÔÚ5Ö®Ç°²åÈë99: ilist.size() = " << ilist.size() << endl;
=======
		cout << "åœ¨5ä¹‹å‰æ’å…¥99: ilist.size() = " << ilist.size() << endl;
>>>>>>> update
>>>>>>> update
		for (it = ilist.begin(); it != ilist.end(); ++it) {
			cout << *it << " ";
		}
		cout << endl << endl;
	}


	ilist.unique();
<<<<<<< HEAD
	cout << "È¥³ıÁ¬ĞøÖØ¸´Ïî: ilist.size() = " << ilist.size() << endl;
=======
<<<<<<< HEAD
	cout << "È¥³ıÁ¬ĞøÖØ¸´Ïî: ilist.size() = " << ilist.size() << endl;
=======
	cout << "å»é™¤è¿ç»­é‡å¤é¡¹: ilist.size() = " << ilist.size() << endl;
>>>>>>> update
>>>>>>> update
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	ilist.reverse();
<<<<<<< HEAD
	cout << "·´×ªºó: ilist.size() = " << ilist.size() << endl;
=======
<<<<<<< HEAD
	cout << "·´×ªºó: ilist.size() = " << ilist.size() << endl;
=======
	cout << "åè½¬å: ilist.size() = " << ilist.size() << endl;
>>>>>>> update
>>>>>>> update
	for (it = ilist.begin(); it != ilist.end(); ++it) {
		cout << *it << " ";
	}
	cout << endl << endl;


	return 0;
<<<<<<< HEAD
}
=======
<<<<<<< HEAD
}
=======
}
>>>>>>> update
>>>>>>> update
