#ifndef BYTE_OBJECT_CONVERTOR_H
#define BYTE_OBJECT_CONVERTOR_H

#include <vector>

namespace Comrade {
	namespace Osiris {

template <typename Some>
class ByteObjectConvertor
{
protected:

public:		static std::vector<char> object_to_byte(Some* obj);
			static Some byte_to_object(std::vector<char>* vect);
};

template <typename Some>
std::vector<char> ByteObjectConvertor<Some>::object_to_byte(Some* obj)
{
	std::vector<char> vect;
	char* ptr=reinterpret_cast<char*>(obj);
	unsigned int index=0;

	while(index<=sizeof(*obj)-1)
	{
		vect.push_back(*ptr);
		++ptr;
		++index;
	}

	return vect;
}

template <typename Some>
Some ByteObjectConvertor<Some>::byte_to_object(std::vector<char>* vect)
{
	char* arr=new char[vect->size()];

	for (unsigned int i=0; i<=vect->size()-1; ++i)
	{
		arr[i]=(*vect)[i];
	}

	Some* ptr=reinterpret_cast<Some*>(arr);

	return *ptr;
}

}
}

#endif


