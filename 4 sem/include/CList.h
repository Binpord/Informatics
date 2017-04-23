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
		
		bool operator=(const Elem&);	// for CList_exeption* IsOk();
		std::ostream& put(std::ostream&);
		std::ostream& operator<<(std::ostream&);
	};
	
	Elem *first, *last;
	unsigned int length;
public:
	CList();
	~CList();
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
		Iterator(const Iterator&);
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
