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
	bool IsOk() const;
	void add(const InfoType&);
	Elem* begin() const;
	Elem* end() const;
	void print() const;

	class Iterator
	{
	protected:
		Elem *cur_elem;
	public:
		Iterator();
		Iterator* operator=(Elem*) const;
		Iterator* operator++(int);
		InfoType* operator->() const;
		InfoType operator*() const;
		bool operator!=(Elem*) const;
	};
};
