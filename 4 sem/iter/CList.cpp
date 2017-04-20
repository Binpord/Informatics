#include "CList.h"

template<typename InfoType>
CList<InfoType>::CList()
{
	first = NULL;
	last = NULL;
	length = 0;
}

template<typename InfoType>
void CList<InfoType>::add(const InfoType& NewInfo)
{
	this->IsOk();
	CList<InfoType>::Elem *NewElem = new CList<InfoType>::Elem;	// mind that elements are created dinamically => need destructor (which will do cleaning)
	NewElem->info = NewInfo;
	NewElem->next = NULL;		// NewElem will be last in the list
	this->length++;
	
	if(this->length == 1)
	{
		this->first = NewElem;
		this->last = NewElem;
		NewElem->prev = NULL;
		return;
	}
	else
	{
		NewElem->prev = this->last;
		this->last = NewElem;
		return;
	}
}

template<typename InfoType>
typename CList<InfoType>::Elem* CList<InfoType>::end() const
{
	return this->last;
}

template<typename InfoType>
typename CList<InfoType>::Elem* CList<InfoType>::begin() const
{
	return this->first;
}

template<typename InfoType>
CList<InfoType>::Iterator::Iterator()
{
	cur_elem = NULL;
}

template<typename InfoType>
typename CList<InfoType>::Iterator* CList<InfoType>::Iterator::operator=(CList<InfoType>::Elem* elem) const
{
	cur_elem = elem;
}
