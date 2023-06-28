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
void MinHeap::addItem(Edge* edge)
{
	int value = edge->weight;
	if (count + 1 >= capacity)
	{
		resize();
	}
	arr[count + 1] = edge;
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
Edge* MinHeap::getItem()
{
	int index = 0; //will be index to delete

	if (count <= 0)
	{
		throw std::out_of_range("Empty Array");
	}
	Edge* minItem = arr[1];

	for (int i = 1; i < count + 1; i++) //retrieve minimum
	{
		if (minItem->weight > arr[i]->weight)
		{
			minItem = arr[i];
			index = i;
		}
	}
	if (index < count + 1) //if not at end of array
	{
		for (int i = index; i < count + 1; i++)
		{
			arr[i] = arr[i + 1]; //shift items to fill gap
		}
	}

	count--; //removing node

	return minItem;
}