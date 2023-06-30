#include "WGraph.hpp"
#include <stdexcept>

//construct the heap array
MinHeap::MinHeap(int size)
{
	arr = new Edge*[size];
	capacity = size;
	count = 0;
}
//destroy array
MinHeap::~MinHeap()
{
	delete[] arr;
}
//add new node
void MinHeap::pushItem(Edge* edge)
{
	int value = edge->weight;
	if (count >= capacity)
	{
		resize();
	}
	arr[count] = edge;
	count++;
}
//resize the array
void MinHeap::resize()
{
	int oldCapacity = capacity; //original size
	capacity *= 2; //doubling size
	Edge** temp;

	//allocating new array
	temp = new Edge*[capacity];

	//copying elements
	for (int i = 0; i < oldCapacity; i++)
	{
		temp[i] = arr[i];
	}

	delete[] arr;
	arr = temp;
}
//get and remove a node
Edge* MinHeap::popItem()
{
	int index = 0; //will be index to delete

	if (count <= 0)
	{
		throw std::out_of_range("Empty Array");
	}
	Edge* minItem = arr[0];

	for (int i = 0; i < count; i++) //retrieve minimum
	{
		if (minItem->weight > arr[i]->weight)
		{
			minItem = arr[i];
			index = i;
		}
	}
	if (index < count) //if not at end of array
	{
		for (int i = index; i < count; i++)
		{
			arr[i] = arr[i + 1]; //shift items to fill gap
		}
	}

	count--; //removing node

	return minItem;
}

// Return the next item in minheap
Edge* MinHeap::peekItem() {
	if (!isEmpty()) {
		return arr[count-1];
	}
}

bool MinHeap::isEmpty() {
	if (count <= 0) {
		return true;
	}
	else {
		return false;
	}
}