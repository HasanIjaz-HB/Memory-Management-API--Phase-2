#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H
#include <string>
#include <iostream>
using namespace std;

struct node
{
	int id;
	int size;
	int index;
	node *next;

	node::node()
	{}

	node::node(int i, int s, int idx,  node * link)
		: id(i), size(s), index(idx), next(link)
	{}
};

class linkedlist
{
private:
	node *head;		//Head node, start of LinkedList
public:
	linkedlist();			//Constructor
	void printList();		//Prints the given LinkedList from head to end
	void addToEnd(int i,int s,int idx);	//Inserts new node at the end of the LinkedList with given data


};


linkedlist::linkedlist()
{
	head = NULL;
}

void linkedlist::printList()
{
	node * ptr = head;
	//List is empty
	if (head == NULL)
	{
		cout << "List is empty" << endl;
	}
	else
	{
		cout << "List:" << endl;
		while (ptr != NULL)
		{
		
			cout << "[" << ptr->id << "]" << "[" << ptr->size << "]" << "[" << ptr->index << "]";
			ptr = ptr->next;
		}
		cout << endl;
	}
}

void linkedlist::addToEnd(int i, int s, int idx)
{
	node *ptr = head;
	node *temp = new node(i,s,idx, NULL);
	//List is empty
	if (head == NULL)
	{
		head = temp;
	}
	else
	{
		while (ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = temp;
	}
}

//Delete Function could be implemented here.



#endif


