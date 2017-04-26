#include "std.h"
#include "image.h"

template<typename InfoType>
class CList
{
protected:
	class Elem
	{
	public:
		InfoType info;
		Elem *next, *prev;
		
		Elem();
		bool operator=(const Elem&);	// for CList_exeption* IsOk();
	};
	
	Elem *first, *last;
	unsigned int length;
public:
	CList();
	~CList();
	void add(const InfoType&);
	void del(const InfoType&);
	Elem* begin() const;
	Elem* end() const;
	InfoType operator[](unsigned int);

	class Iterator
	{
	protected:
		Elem *cur_elem;
	public:
		Iterator();
		Iterator(Elem*);
		Iterator& operator=(Elem*);
		Iterator* operator++(int);
		Iterator* operator--(int);
		InfoType* operator->() const;
		InfoType operator*() const;
		bool operator!=(Elem*) const;
	};

	// debug tools
#ifdef __DEBUG
	class CList_exeption
	{
	public:
		int ml_errno;
		CList_exeption();
		void print() const;
		bool operator!=(bool) const;
	};
	CList_exeption* IsOk() const;
#endif
};
