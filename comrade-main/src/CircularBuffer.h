#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <vector>

namespace Comrade {
	namespace IrisFoundation {

#define DEFAULT_BUFFER_SIZE 4

template <typename Generic>
class CircularBuffer
{
protected:	std::vector<Generic> c_queue;
			int index;	//this needs to be added for the code to compile. it may be an inappropriate hack, but i really don't know.
			int write_index;
			int size;

public:		CircularBuffer(int sz=DEFAULT_BUFFER_SIZE);

			void add(Generic item);
			int return_size();
			int return_index();
			Generic& at(int pos);

			~CircularBuffer();
};
	
template <typename Generic>
CircularBuffer<Generic>::CircularBuffer(int sz)
{
	size=sz;
	c_queue.resize(size);
}

template <typename Generic>
void CircularBuffer<Generic>::add(Generic item)
{
	c_queue[index]=item;
	++index;
	index%=size;
}

template <typename Generic>
int CircularBuffer<Generic>::return_size()
{
	return size;
}

template <typename Generic>
int CircularBuffer<Generic>::return_index()
{
	return index;
}

template <typename Generic>
Generic& CircularBuffer<Generic>::at(int pos)
{
	return c_queue[index];
}

template <typename Generic>
CircularBuffer<Generic>::~CircularBuffer()
{}

}
}

#endif

