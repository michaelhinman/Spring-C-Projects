#ifndef OLIST_H
#define OLIST_H

#include "Node.h"
#include "DestructCommand.h"

class OList
{
public:
	// specialization
	OList(int numNodes);

	// Big four
	OList() = delete;
	~OList();
	OList(const OList&) = delete;
	OList& operator = (const OList&) = delete;

	// Destructors
	void SetDestructCommand(DestructCommand* pFunctor);

	// Accessors
	Node* GetHead();
	void SetHead(Node* pNode);

	// Class methods
	int GetCutoffLength();
	bool FindKey(unsigned int key, Node& foundNode);

	// Static Methods

	static bool Verify(const OList* const pNew, const OList* const pOld);
	static bool VerifySortOrder(const OList* const pList);

	void SetCutoffLength();
	OList(const char* const pFileName, int numNodes);
	void WriteToFile(const char* const pFileName);

	Node* mergeSort(Node* head);
	void InsertionSort();
	void insertionSortHelper(Node** head_ref);
	void sortedInsert(Node** head_ref, Node* newNode);
	void MergeSort();
	Node* split(Node* head);
	Node* merge(Node* first, Node* second);
	
	void MergeComboSort(int CutoffLength);

private:
	// methods
	void privCreateLinkedList(int numNodes);
	Node* privGetListHead();

	Node* pHead;
	DestructCommand* privFunctor;
	int  NumNodes;
	int  mCutoffLength;
};

#endif 
