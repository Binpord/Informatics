#include "std.h"

template<typename InfoType>
class CList
{
protected:
	class Elem
	{
	public:
		InfoType info;
		Elem *next, *prev;
	};
	
	Elem *first, *last;
	int length;
public:
	CList();
	~CList();
	void add(const InfoType&);
	Elem* getFirst();
	Elem* getLast();
	void print();

	class Iterator
	{
	protected:
		Elem *cur_elem;
		int index;
	public:
		Iterator();
		Iterator* operator=(Elem*);
		Iterator* operator++(int);
		InfoType* operator->();
		InfoType operator*();
		bool operator!=(Elem*);
	};
};
