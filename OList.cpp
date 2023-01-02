#include "OList.h"
#include "Node.h"

void OList::SetCutoffLength()
{
	// Yes it's a HARD coded cutoff... Trial & Error
	this->mCutoffLength = 32;
}

void OList::MergeSort()
{
	this->pHead = mergeSort(this->GetHead());
}

Node* OList::split(Node* head)
{
	Node* fast = head, * slow = head;
	while (fast->pNext && fast->pNext->pNext)
	{
		fast = fast->pNext->pNext;
		slow = slow->pNext;
	}
	Node* temp = slow->pNext;
	slow->pNext = NULL;
	return temp;
}

Node* OList::merge(Node* first, Node* second)
{
	// If first linked list is empty
	if (!first)
		return second;

	// If second linked list is empty
	if (!second)
		return first;

	// Pick the smaller value
	if (first->key < second->key)
	{
		first->pNext = merge(first->pNext, second);
		first->pNext->pPrev = first;
		first->pPrev = NULL;
		return first;
	}
	else
	{
		second->pNext = merge(first, second->pNext);
		second->pNext->pPrev = second;
		second->pPrev = NULL;
		return second;
	}
}

// Function to do merge sort
Node* OList::mergeSort(Node* head)
{
	if (!head || !head->pNext)
		return head;
	Node* second = split(head);

	// Recur for left and right halves
	head = mergeSort(head);
	second = mergeSort(second);

	// Merge the two sorted halves
	return merge(head, second);
}

void OList::InsertionSort()
{
	Node* head_ref = this->GetHead();
	insertionSortHelper((Node**)&head_ref);
}

void OList::insertionSortHelper(Node** head)
{
	Node* sorted = NULL;

	Node* current = *head;
	while (current != NULL)
	{
		// Store next for next iteration 
		Node* next = current->pNext;

		// removing all the links so as to create 'current' 
		// as a new node for insertion 
		current->pPrev = current->pNext = NULL;

		// insert current in 'sorted' doubly linked list 
		sortedInsert(&sorted, current);

		// Update current 
		current = next;
	}

	// Update head_ref to point to sorted doubly linked list 
	this->pHead = sorted;
	/*
	//For printing...
	Node* tmp = *head;
	while (tmp != NULL)
	{
		tmp = tmp->pNext;
	}
	*/
}

void OList::sortedInsert(Node** head_ref, Node* newNode)
{
	Node* current;
	// if list is empty 
	if (*head_ref == NULL)
	{
		*head_ref = newNode;
	}
	// if the node is to be inserted at the beginning 
	// of the doubly linked list 
	else if ((*head_ref)->key >= newNode->key)
	{
		newNode->pNext = *head_ref;
		newNode->pNext->pPrev = newNode;
		*head_ref = newNode;
	}
	else
	{
		current = *head_ref;
		// locate the node after which the new node 
		// is to be inserted 
		while (current->pNext != NULL &&
			current->pNext->key < newNode->key)
			current = current->pNext;

		//Make the appropriate links /
		newNode->pNext = current->pNext;

		// if the new node is not inserted 
		// at the end of the list 
		if (current->pNext != nullptr)
			newNode->pNext->pPrev = newNode;
		current->pNext = newNode;
		newNode->pPrev = current;
	}
}

void OList::MergeComboSort(int CutoffLength)
{
	if (this->NumNodes < CutoffLength)
		this->InsertionSort();
	else
		this->MergeSort();
}

void OList::WriteToFile(const char * const pFileName)
{
	FILE *pFileHandle = nullptr;
	assert(pFileName);
	errno_t status;
	status = fopen_s(&pFileHandle,pFileName, "wb"); // write binary
	assert(pFileHandle);
	assert(status == 0);
 
	Node* tmp = this->GetHead();
	while (tmp != nullptr)
	{
		fwrite(&tmp->pNext, 1, sizeof(int), pFileHandle);
		fwrite(&tmp->pPrev, 1, sizeof(int), pFileHandle);
		fwrite(&tmp->x, 1, sizeof(int), pFileHandle);
		fwrite(&tmp->y, 1, sizeof(int), pFileHandle);
		fwrite(&tmp->z, 1, sizeof(int), pFileHandle);
		fwrite(&tmp->key, 1, sizeof(int), pFileHandle);
		tmp = tmp->pNext;
	}
 	fclose(pFileHandle);
}

OList::OList(const char * const pFileName, int numNodes)
{
	this->SetCutoffLength();
	this->pHead = nullptr;
	this->mCutoffLength = this->GetCutoffLength();
	this->NumNodes = numNodes;
	this->privFunctor = nullptr;

 	FILE *pFileHandle;
	if (fopen_s(&pFileHandle, pFileName, "rb") != 0)
	{
		printf("ERROR: Cannot Open File!);
		return;
	}

	// Grab a contiguous block of memory
	char* p = new char[(unsigned int)numNodes * sizeof(Node)];
	fread(p, sizeof(Node) * numNodes , 1, pFileHandle);
	
 	// Fix head ptrs first...
	Node* headstart = (Node*)p;
	Node* tmp = headstart;
	
	headstart->pNext = headstart + 1;
	headstart->pPrev = nullptr;
	tmp++;
	
	//FIX pointers...
	for (int i = 1; i < numNodes - 1; i++)
	{
		//this sets the next pointer
		tmp->pNext = tmp + 1;
		//this calculates the address of where the prev should point to...
		tmp->pPrev = tmp - 1;
		tmp++;
	}
	// this is the last node...
	tmp->pNext = nullptr;
	tmp->pPrev = tmp - 1;

	// Set pHead
	this->pHead = headstart;
	fclose(pFileHandle);
}


