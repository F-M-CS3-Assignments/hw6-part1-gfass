#include <iostream>
#include <cassert>
#include <random>
#include "RedBlackTree.h"

using namespace std;

void TestSimpleConstructor() {
	cout << "Testing Simple Constructor..." << endl;
	RedBlackTree rbt = RedBlackTree();

	// Tree should be empty initially
	assert(rbt.ToInfixString() == "");
	assert(rbt.ToPrefixString() == "");
	assert(rbt.ToPostfixString() == "");

	// Size should be zero
	assert(rbt.Size() == 0);

	// Contains should return false for any value
	assert(!rbt.Contains(0));
	assert(!rbt.Contains(100));

	// GetMin and GetMax should throw exceptions
	try {
		rbt.GetMin();
		assert(false); // Should not reach here
	} catch (invalid_argument &e) { }

	try {
		rbt.GetMax();
		assert(false); // Should not reach here
	} catch (invalid_argument &e) { }

	cout << "PASSED!" << endl << endl;
}

void TestConstructor() {
	cout << "Testing Constructor W/Int Input..." << endl;
	RedBlackTree rbt = RedBlackTree(15);

	// Check prefix
	assert(rbt.ToPrefixString() == " B15 ");

	// Check infix and postfix also
	assert(rbt.ToInfixString() == " B15 ");
	assert(rbt.ToPostfixString() == " B15 ");

	// Size should be 1
	assert(rbt.Size() == 1);

	// Contains should find 15, but not other numbers
	assert(rbt.Contains(15));
	assert(!rbt.Contains(0));
	assert(!rbt.Contains(100));

	// Min and Max should both be 15
	assert(rbt.GetMin() == 15);
	assert(rbt.GetMax() == 15);

	cout << "PASSED!" << endl << endl;
}

void TestInsertFirstNode() {
	cout << "Testing Insert One Node..." << endl;
	RedBlackTree rbt = RedBlackTree();
	rbt.Insert(30);

	// Check tree structure
	assert(rbt.ToPrefixString() == " B30 ");
	assert(rbt.ToInfixString() == " B30 ");
	assert(rbt.ToPostfixString() == " B30 ");

	// Size should now be 1
	assert(rbt.Size() == 1);

	// Contains should correctly find inserted node and not others
	assert(rbt.Contains(30));
	assert(!rbt.Contains(0));
	assert(!rbt.Contains(100));

	// Min and Max should both be 30
	assert(rbt.GetMin() == 30);
	assert(rbt.GetMax() == 30);

	// Try inserting a duplicate: should throw an exception
	try {
		rbt.Insert(30);
		assert(false); // Should not reach here
	} catch (invalid_argument &e) { }

	cout << "PASSED!" << endl << endl;
}

void TestInsertSecondNode() {
	cout << "Testing Insert Second Node..." << endl;

	// Test inserting a smaller value
	RedBlackTree *rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(15);

	// Tree should have black root and red left child
	assert(rbt->ToPrefixString() == " B30  R15 ");
	assert(rbt->ToInfixString() == " R15  B30 ");
	assert(rbt->ToPostfixString() == " R15  B30 ");
	assert(rbt->Size() == 2);
	assert(rbt->Contains(30));
	assert(rbt->Contains(15));
	assert(!rbt->Contains(100));
	assert(rbt->GetMin() == 15);
	assert(rbt->GetMax() == 30);

	// Try inserting a duplicate
	try {
		rbt->Insert(15);
		assert(false); // Should not reach here
	} catch (invalid_argument &e) { }

	delete rbt;

	// Test inserting a larger value
	rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(45);

	// Tree should have black root and red right child
	assert(rbt->ToPrefixString() == " B30  R45 ");
	assert(rbt->ToInfixString() == " B30  R45 ");
	assert(rbt->ToPostfixString() == " R45  B30 ");
	assert(rbt->Size() == 2);
	assert(rbt->Contains(30));
	assert(rbt->Contains(45));
	assert(!rbt->Contains(100));
	assert(rbt->GetMin() == 30);
	assert(rbt->GetMax() == 45);

	// Try inserting a duplicate
	try {
		rbt->Insert(45);
		assert(false); // Should not reach here
	} catch (invalid_argument &e) { }

	delete rbt;

	cout << "PASSED!" << endl << endl;
}

void TestInsertThirdNode() {
	cout << "Testing Insert Third Node..." << endl;

	// Test Left Left case (causes Right Rotation)
	RedBlackTree *rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(15);
	rbt->Insert(10); // Left Left
	assert(rbt->ToPrefixString() == " B15  R10  R30 ");
	assert(rbt->ToInfixString() == " R10  B15  R30 ");
	assert(rbt->ToPostfixString() == " R10  R30  B15 ");
	assert(rbt->Size() == 3);
	assert(rbt->Contains(10));
	assert(rbt->Contains(15));
	assert(rbt->Contains(30));
	assert(rbt->GetMin() == 10);
	assert(rbt->GetMax() == 30);
	delete rbt;

	// Test Right Left case (causes Right-Left Rotation)
	rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(15);
	rbt->Insert(25); // Right Left
	assert(rbt->ToPrefixString() == " B25  R15  R30 ");
	assert(rbt->ToInfixString() == " R15  B25  R30 ");
	assert(rbt->ToPostfixString() == " R15  R30  B25 ");
	assert(rbt->Size() == 3);
	assert(rbt->Contains(15));
	assert(rbt->Contains(25));
	assert(rbt->Contains(30));
	assert(rbt->GetMin() == 15);
	assert(rbt->GetMax() == 30);
	delete rbt;

	// Test Easy case (no rotation needed)
	rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(15);
	rbt->Insert(45);
	assert(rbt->ToPrefixString() == " B30  R15  R45 ");
	assert(rbt->ToInfixString() == " R15  B30  R45 ");
	assert(rbt->ToPostfixString() == " R15  R45  B30 ");
	assert(rbt->Size() == 3);
	assert(rbt->Contains(15));
	assert(rbt->Contains(30));
	assert(rbt->Contains(45));
	assert(rbt->GetMin() == 15);
	assert(rbt->GetMax() == 45);
	delete rbt;

	// Test Right Right case (Left Rotation)
	rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(45);
	rbt->Insert(60); // Right Right
	assert(rbt->ToPrefixString() == " B45  R30  R60 ");
	assert(rbt->ToInfixString() == " R30  B45  R60 ");
	assert(rbt->ToPostfixString() == " R30  R60  B45 ");
	assert(rbt->Size() == 3);
	assert(rbt->Contains(30));
	assert(rbt->Contains(45));
	assert(rbt->Contains(60));
	assert(rbt->GetMin() == 30);
	assert(rbt->GetMax() == 60);
	delete rbt;

	// Test Left Right case (Left-Right Rotation)
	rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(45);
	rbt->Insert(40); // Left Right
	assert(rbt->ToPrefixString() == " B40  R30  R45 ");
	assert(rbt->ToInfixString() == " R30  B40  R45 ");
	assert(rbt->ToPostfixString() == " R30  R45  B40 ");
	assert(rbt->Size() == 3);
	assert(rbt->Contains(30));
	assert(rbt->Contains(40));
	assert(rbt->Contains(45));
	assert(rbt->GetMin() == 30);
	assert(rbt->GetMax() == 45);
	delete rbt;

	cout << "PASSED!" << endl << endl;
}

void TestInsertFourthNode() {
	cout << "Testing Insert Fourth Node..." << endl;

	// Left-heavy tree (4 nodes)
	RedBlackTree *rbt = new RedBlackTree();
	rbt->Insert(50);
	rbt->Insert(30);
	rbt->Insert(70);
	rbt->Insert(20);

	assert(rbt->ToPrefixString() == " B50  B30  R20  B70 ");
	assert(rbt->ToInfixString() == " R20  B30  B50  B70 ");
	assert(rbt->ToPostfixString() == " R20  B30  B70  B50 ");
	assert(rbt->Size() == 4);

	assert(rbt->Contains(20));
	assert(rbt->Contains(30));
	assert(rbt->Contains(50));
	assert(rbt->Contains(70));
	assert(!rbt->Contains(100));

	assert(rbt->GetMin() == 20);
	assert(rbt->GetMax() == 70);

	delete rbt;

	// Right-heavy tree (4 nodes)
	rbt = new RedBlackTree();
	rbt->Insert(50);
	rbt->Insert(70);
	rbt->Insert(30);
	rbt->Insert(80);

	assert(rbt->ToPrefixString() == " B50  B30  B70  R80 ");
	assert(rbt->ToInfixString() == " B30  B50  B70  R80 ");
	assert(rbt->ToPostfixString() == " B30  R80  B70  B50 ");
	assert(rbt->Size() == 4);

	assert(rbt->Contains(30));
	assert(rbt->Contains(50));
	assert(rbt->Contains(70));
	assert(rbt->Contains(80));
	assert(!rbt->Contains(5));

	assert(rbt->GetMin() == 30);
	assert(rbt->GetMax() == 80);

	delete rbt;

	cout << "PASSED!" << endl << endl;
}

void TestInsertFifthNode() {
	cout << "Testing Insert Fifth Node..." << endl;

	RedBlackTree *rbt = new RedBlackTree();
	rbt->Insert(30);
	rbt->Insert(15);
	rbt->Insert(45);
	rbt->Insert(10);
	rbt->Insert(25);

	assert(rbt->ToPrefixString() == " B30  B15  R10  R25  B45 ");
	assert(rbt->ToInfixString() == " R10  B15  R25  B30  B45 ");
	assert(rbt->ToPostfixString() == " R10  R25  B15  B45  B30 ");
	assert(rbt->Size() == 5);

	// Check contains
	assert(rbt->Contains(10));
	assert(rbt->Contains(15));
	assert(rbt->Contains(25));
	assert(rbt->Contains(30));
	assert(rbt->Contains(45));
	assert(!rbt->Contains(100));

	// Check min and max
	assert(rbt->GetMin() == 10);
	assert(rbt->GetMax() == 45);

	// Try inserting a duplicate
	try {
		rbt->Insert(10);
		assert(false); // Should not reach here
	} catch (invalid_argument &e) { }

	delete rbt;

	// Another case: inserting to force more balancing
	rbt = new RedBlackTree();
	rbt->Insert(50);
	rbt->Insert(20);
	rbt->Insert(70);
	rbt->Insert(10);
	rbt->Insert(30);

	assert(rbt->ToPrefixString() == " B50  B20  R10  R30  B70 ");
	assert(rbt->ToInfixString() == " R10  B20  R30  B50  B70 ");
	assert(rbt->ToPostfixString() == " R10  R30  B20  B70  B50 ");
	assert(rbt->Size() == 5);

	assert(rbt->Contains(10));
	assert(rbt->Contains(20));
	assert(rbt->Contains(30));
	assert(rbt->Contains(50));
	assert(rbt->Contains(70));

	assert(rbt->GetMin() == 10);
	assert(rbt->GetMax() == 70);

	delete rbt;

	cout << "PASSED!" << endl << endl;
}

void TestToStrings() {
	cout << "Testing ToString Methods..." << endl;

	RedBlackTree rbt = RedBlackTree();
	rbt.Insert(12);
	rbt.Insert(11);
	rbt.Insert(15);
	rbt.Insert(5);
	rbt.Insert(13);
	rbt.Insert(7);

	// Basic check on the original tree
	assert(rbt.ToPrefixString() == " B12  B7  R5  R11  B15  R13 ");
	assert(rbt.ToInfixString() == " R5  B7  R11  B12  R13  B15 ");
	assert(rbt.ToPostfixString() == " R5  R11  B7  R13  B15  B12 ");
	assert(rbt.Size() == 6);

	// Check Contains
	assert(rbt.Contains(5));
	assert(rbt.Contains(7));
	assert(rbt.Contains(11));
	assert(rbt.Contains(12));
	assert(rbt.Contains(13));
	assert(rbt.Contains(15));
	assert(!rbt.Contains(100));

	assert(rbt.GetMin() == 5);
	assert(rbt.GetMax() == 15);

	// Another tree
	RedBlackTree rbt2 = RedBlackTree();
	rbt2.Insert(50);
	rbt2.Insert(25);
	rbt2.Insert(75);
	rbt2.Insert(10);
	rbt2.Insert(30);
	rbt2.Insert(60);
	rbt2.Insert(80);

	assert(rbt2.ToPrefixString() == " B50  B25  R10  R30  B75  R60  R80 ");
	assert(rbt2.ToInfixString() == " R10  B25  R30  B50  R60  B75  R80 ");
	assert(rbt2.ToPostfixString() == " R10  R30  B25  R60  R80  B75  B50 ");
	assert(rbt2.Size() == 7);

	assert(rbt2.Contains(10));
	assert(rbt2.Contains(25));
	assert(rbt2.Contains(30));
	assert(rbt2.Contains(50));
	assert(rbt2.Contains(60));
	assert(rbt2.Contains(75));
	assert(rbt2.Contains(80));
	assert(!rbt2.Contains(5));

	assert(rbt2.GetMin() == 10);
	assert(rbt2.GetMax() == 80);

	cout << "PASSED!" << endl << endl;
}

void TestInsertRandomTests() {
	cout << "Testing ToString Methods..." << endl;

	RedBlackTree rbt = RedBlackTree();
	rbt.Insert(12);
	rbt.Insert(11);
	rbt.Insert(15);
	rbt.Insert(5);
	rbt.Insert(13);
	rbt.Insert(7);

	// Basic check
	assert(rbt.ToPrefixString() == " B12  B7  R5  R11  B15  R13 ");
	assert(rbt.ToInfixString() == " R5  B7  R11  B12  R13  B15 ");
	assert(rbt.ToPostfixString() == " R5  R11  B7  R13  B15  B12 ");
	assert(rbt.Size() == 6);

	// Contains checks
	assert(rbt.Contains(5));
	assert(rbt.Contains(7));
	assert(rbt.Contains(11));
	assert(rbt.Contains(12));
	assert(rbt.Contains(13));
	assert(rbt.Contains(15));
	assert(!rbt.Contains(100));

	assert(rbt.GetMin() == 5);
	assert(rbt.GetMax() == 15);

	// Insert more
	rbt.Insert(14);
	rbt.Insert(6);

	assert(rbt.Size() == 8);
	assert(rbt.Contains(6));
	assert(rbt.Contains(14));
	assert(rbt.GetMin() == 5);
	assert(rbt.GetMax() == 15);

	// Make sure ToString still work
	string pre = rbt.ToPrefixString();
	string in = rbt.ToInfixString();
	string post = rbt.ToPostfixString();
	assert(!pre.empty());
	assert(!in.empty());
	assert(!post.empty());

	// Build another different structure
	RedBlackTree rbt2 = RedBlackTree();
	rbt2.Insert(50);
	rbt2.Insert(25);
	rbt2.Insert(75);
	rbt2.Insert(10);
	rbt2.Insert(30);
	rbt2.Insert(60);
	rbt2.Insert(80);

	assert(rbt2.ToPrefixString() == " B50  B25  R10  R30  B75  R60  R80 ");
	assert(rbt2.ToInfixString() == " R10  B25  R30  B50  R60  B75  R80 ");
	assert(rbt2.ToPostfixString() == " R10  R30  B25  R60  R80  B75  B50 ");
	assert(rbt2.Size() == 7);

	assert(rbt2.Contains(10));
	assert(rbt2.Contains(25));
	assert(rbt2.Contains(30));
	assert(rbt2.Contains(50));
	assert(rbt2.Contains(60));
	assert(rbt2.Contains(75));
	assert(rbt2.Contains(80));
	assert(!rbt2.Contains(5));

	assert(rbt2.GetMin() == 10);
	assert(rbt2.GetMax() == 80);

	// Right-heavy tree
	RedBlackTree rbt3 = RedBlackTree();
	rbt3.Insert(10);
	rbt3.Insert(20);
	rbt3.Insert(30);
	rbt3.Insert(40);
	rbt3.Insert(50);

	assert(!rbt3.ToPrefixString().empty());
	assert(!rbt3.ToInfixString().empty());
	assert(!rbt3.ToPostfixString().empty());
	assert(rbt3.Size() == 5);
	assert(rbt3.GetMin() == 10);
	assert(rbt3.GetMax() == 50);

	// Left-heavy tree
	RedBlackTree rbt4 = RedBlackTree();
	rbt4.Insert(50);
	rbt4.Insert(40);
	rbt4.Insert(30);
	rbt4.Insert(20);
	rbt4.Insert(10);

	assert(!rbt4.ToPrefixString().empty());
	assert(!rbt4.ToInfixString().empty());
	assert(!rbt4.ToPostfixString().empty());
	assert(rbt4.Size() == 5);
	assert(rbt4.GetMin() == 10);
	assert(rbt4.GetMax() == 50);

	cout << "PASSED!" << endl << endl;
}

void TestCopyConstructor() {
	cout << "Testing Copy Constructor..." << endl;

	RedBlackTree rbt1 = RedBlackTree();
	rbt1.Insert(11);
	rbt1.Insert(23);
	rbt1.Insert(9);
	rbt1.Insert(52);
	rbt1.Insert(31);
	rbt1.Insert(4);

	assert(rbt1.ToPrefixString() == " B11  B9  R4  B31  R23  R52 ");
	assert(!rbt1.ToInfixString().empty());
	assert(!rbt1.ToPostfixString().empty());
	assert(rbt1.Size() == 6);

	RedBlackTree rbt2 = RedBlackTree(rbt1);

	assert(rbt2.ToPrefixString() == rbt1.ToPrefixString());
	assert(rbt2.ToInfixString() == rbt1.ToInfixString());
	assert(rbt2.ToPostfixString() == rbt1.ToPostfixString());
	assert(rbt2.Size() == rbt1.Size());

	assert(rbt2.Contains(4));
	assert(rbt2.Contains(9));
	assert(rbt2.Contains(11));
	assert(rbt2.Contains(23));
	assert(rbt2.Contains(31));
	assert(rbt2.Contains(52));
	assert(!rbt2.Contains(100));

	assert(rbt2.GetMin() == rbt1.GetMin());
	assert(rbt2.GetMax() == rbt1.GetMax());

	// Mutate original
	rbt1.Insert(200);
	rbt1.Insert(1);

	assert(rbt1.Size() == 8);
	assert(rbt2.Size() == 6);
	assert(rbt1.ToPrefixString() != rbt2.ToPrefixString());
	assert(rbt1.Contains(200));
	assert(!rbt2.Contains(200));
	assert(rbt1.Contains(1));
	assert(!rbt2.Contains(1));

	cout << "PASSED!" << endl << endl;
}

void TestContains() {
	cout << "Testing Contains..." << endl;

	// Test Contains on empty tree
	RedBlackTree *rbt = new RedBlackTree();
	assert(!rbt->Contains(6));
	assert(!rbt->Contains(0));
	assert(!rbt->Contains(-100));
	delete rbt;

	// Insert multiple nodes and test
	rbt = new RedBlackTree();
	rbt->Insert(40);
	rbt->Insert(22);
	rbt->Insert(15);
	rbt->Insert(31);
	rbt->Insert(55);
	rbt->Insert(12);
	rbt->Insert(17);
	rbt->Insert(29);
	rbt->Insert(34);

	assert(rbt->Contains(12));
	assert(rbt->Contains(15));
	assert(rbt->Contains(17));
	assert(rbt->Contains(22));
	assert(rbt->Contains(29));
	assert(rbt->Contains(31));
	assert(rbt->Contains(34));
	assert(rbt->Contains(40));
	assert(rbt->Contains(55));

	assert(!rbt->Contains(0));
	assert(!rbt->Contains(1));
	assert(!rbt->Contains(99));
	assert(!rbt->Contains(100));

	delete rbt;

	// Edge case: Insert just one element
	rbt = new RedBlackTree();
	rbt->Insert(100);
	assert(rbt->Contains(100));
	assert(!rbt->Contains(99));
	delete rbt;

	cout << "PASSED!" << endl << endl;
}

void TestGetMinimumMaximum() {
	cout << "Testing Get Minimum and Get Maximum..." << endl;

	// Empty tree should throw exception
	RedBlackTree *rbt = new RedBlackTree();
	try {
		rbt->GetMin();
		assert(false);
	} catch (invalid_argument &e) { }

	try {
		rbt->GetMax();
		assert(false);
	} catch (invalid_argument &e) { }
	delete rbt;

	// Single-node tree
	rbt = new RedBlackTree();
	rbt->Insert(42);
	assert(rbt->GetMin() == 42);
	assert(rbt->GetMax() == 42);
	delete rbt;

	// Balanced tree
	rbt = new RedBlackTree();
	rbt->Insert(50);
	rbt->Insert(30);
	rbt->Insert(70);
	rbt->Insert(20);
	rbt->Insert(40);
	rbt->Insert(60);
	rbt->Insert(80);

	assert(rbt->GetMin() == 20);
	assert(rbt->GetMax() == 80);
	delete rbt;

	// Right-heavy tree
	rbt = new RedBlackTree();
	rbt->Insert(10);
	rbt->Insert(20);
	rbt->Insert(30);
	rbt->Insert(40);
	rbt->Insert(50);

	assert(rbt->GetMin() == 10);
	assert(rbt->GetMax() == 50);
	delete rbt;

	// Left-heavy tree
	rbt = new RedBlackTree();
	rbt->Insert(50);
	rbt->Insert(40);
	rbt->Insert(30);
	rbt->Insert(20);
	rbt->Insert(10);

	assert(rbt->GetMin() == 10);
	assert(rbt->GetMax() == 50);
	delete rbt;

	cout << "PASSED!" << endl << endl;
}

void TestPrivateMethods() {
	cout << "Testing Private Methods..." << endl;
	RedBlackTree rbt;
	rbt.PrivateTests();
	cout << "PASSED!" << endl << endl;
}

int main() {
	TestSimpleConstructor();
	TestConstructor();

	TestInsertFirstNode();
	TestInsertSecondNode();
	TestInsertThirdNode();
	TestInsertFourthNode();
	TestInsertFifthNode();

	TestToStrings();
	TestInsertRandomTests();

	TestCopyConstructor();

	TestContains();
	TestGetMinimumMaximum();

	TestPrivateMethods();

	cout << "ALL TESTS PASSED!!" << endl;
	return 0;
}
