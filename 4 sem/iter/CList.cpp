#include "CList.h"

// ============================================================================================
// CList::Elem part

template<typename InfoType>
bool CList<InfoType>::Elem::operator=(const Elem& comp)
{
	this->prev = comp.prev;
	this->next = comp.next;
	this->info = comp.info;
	
	return true;
}

template<typename InfoType>
std::ostream& CList<InfoType>::Elem::put(std::ostream& stream)
{
	std::cout << info;
	return stream;
}

template<typename InfoType>
std::ostream& CList<InfoType>::Elem::operator<<(std::ostream& stream)
{
	return this->put(stream);
}

// ============================================================================================
// CList part

template<typename InfoType>
CList<InfoType>::CList()
{
	first = NULL;
	last = NULL;
	length = 0;
}

template<typename InfoType>
CList<InfoType>::~CList()
{
#ifdef __DEBUG
	CList<InfoType>::CList_exeption* status = this->IsOk();
	if(*status != true)
	{
		status->print();
		delete status;
		exit(1);
	}
	delete status;
#endif
	
	CList<InfoType>::Elem *cur_elem = this->first, *prev_elem = NULL;

	for(int i = 0; i < this->length; i++)
	{
		prev_elem = cur_elem;
		cur_elem = cur_elem->next;
		delete prev_elem;
	}

	this->length = 0;
}

template<typename InfoType>
void CList<InfoType>::add(const InfoType& NewInfo)
{
#ifdef __DEBUG
	CList<InfoType>::CList_exeption* status = this->IsOk();
	if(*status != true)
	{
		status->print();
		delete status;
		exit(1);
	}
	delete status;
#endif

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
		this->last->next = NewElem;
		NewElem->prev = this->last;
		this->last = NewElem;
		return;
	}
}

template<typename InfoType>
typename CList<InfoType>::Elem* CList<InfoType>::begin() const
{
#ifdef __DEBUG
	CList<InfoType>::CList_exeption* status = this->IsOk();
	if(*status != true)
	{
		status->print();
		delete status;
		exit(1);
	}
	delete status;
#endif

	return this->first;
}

template<typename InfoType>
typename CList<InfoType>::Elem* CList<InfoType>::end() const
{
#ifdef __DEBUG
	CList<InfoType>::CList_exeption* status = this->IsOk();
	if(*status != true)
	{
		status->print();
		delete status;
		exit(1);
	}
	delete status;
#endif

	return this->last;
}

template<typename InfoType>
void CList<InfoType>::print() const
{
#ifdef __DEBUG
	CList<InfoType>::CList_exeption* status = this->IsOk();
	if(*status != true)
	{
		status->print();
		delete status;
		exit(1);
	}
	delete status;
#endif

	std::cout << "CList print:" << std::endl;
	std::cout << "length = " << this->length << ";" << std::endl << "list:" << std::endl;

	std::cout << "NULL";
	CList<InfoType>::Iterator i;
	for(i = this->begin(); i != this->end(); i++)
		std::cout << " <-> " << *i;
	std::cout << " <-> " << *i << " <-> NULL" << std::endl;
}

// ============================================================================================
// CList::Iterator part

template<typename InfoType>
CList<InfoType>::Iterator::Iterator()
{
	this->cur_elem = NULL;
}

template<typename InfoType>
CList<InfoType>::Iterator::Iterator(const Iterator& in_iterator)
{
	this->cur_elem = in_iterator.cur_elem;
}

template<typename InfoType>
CList<InfoType>::Iterator::Iterator(CList<InfoType>::Elem* elem)
{
	this->cur_elem = elem;
}

template<typename InfoType>
typename CList<InfoType>::Iterator& CList<InfoType>::Iterator::operator=(CList<InfoType>::Elem* elem)
{
	this->cur_elem = elem;
	return *this;
}

template<typename InfoType>
typename CList<InfoType>::Iterator* CList<InfoType>::Iterator::operator++(int)
{
	if(this->cur_elem != NULL)
		this->cur_elem = cur_elem->next;

	return this;
}

template<typename InfoType>
typename CList<InfoType>::Iterator* CList<InfoType>::Iterator::operator--(int)
{
	if(this->cur_elem != NULL)
		this->cur_elem = cur_elem->prev;

	return this;
}

template<typename InfoType>
InfoType* CList<InfoType>::Iterator::operator->() const
{
	if(this->cur_elem != NULL)
		return &(this->cur_elem->info);
	else
		return NULL;
}

template<typename InfoType>
InfoType CList<InfoType>::Iterator::operator*() const
{
	if(this->cur_elem != NULL)
		return this->cur_elem->info;
	else
		return NULL;
}

template<typename InfoType>
bool CList<InfoType>::Iterator::operator!=(CList<InfoType>::Elem* elem) const
{
	return cur_elem != elem;
}

// ============================================================================================
// CList_exeption part
// need this only in debug mode
// need standart error messages in massive
#ifdef __DEBUG

std::string Error_Messages[] =		\
	{
		"CList contains no errors (or I cannot detect one).\n"						// 0
		"CList has length set to 0, but it's pointer(s) is(are) set.\n",				// 1
		"CList has non-zero length but it's pointer(s) is(are) pointing to NULL.\n",			// 2
		"One or more of CList elements contains invalid pointers to next or previous element.\n"	// 3
	};

template<typename InfoType>
CList<InfoType>::CList_exeption::CList_exeption()
{
	ml_errno = 0;			// default stade means "everything is normal"
}

template<typename InfoType>
void CList<InfoType>::CList_exeption::print() const
{
	std::cout << Error_Messages[this->ml_errno];
	std::cout << "If you don't know what it is, please try responding an error to the CList creator (current e-mail is 'vadimsh853@gmail.com')." << std::endl;
}

template<typename InfoType>
bool CList<InfoType>::CList_exeption::operator!=(bool comp) const
{
	return ml_errno == comp;			// this is because errno == 0 if everything is all right
}

template<typename InfoType>
typename CList<InfoType>::CList_exeption* CList<InfoType>::IsOk() const
{
	CList<InfoType>::CList_exeption* res = new CList<InfoType>::CList_exeption;		// need to delete it after checking
	if(length == 0)
	{
		if(this->first == NULL && this->last == NULL)
			return res;
		else
		{
			res->ml_errno = 1;
			return res;
		}
	}
	else if(this->first == NULL || this->last)
	{
		res->ml_errno = 2;
		return res;
	}

	CList<InfoType>::Elem *cur_elem = this->first;
	CList<InfoType>::Elem *prev_elem = NULL;

	for(int i = 0; i < this->length - 1; i++)			// - 1 for optimisatio (cur_elem == NULL is valid for the last element)
	{
		if(cur_elem->prev != prev_elem || cur_elem->next == NULL)
		{
			res->ml_errno = 3;
			return res;
		}

		prev_elem = cur_elem;
		cur_elem = cur_elem->next;
	}
	// still last element to go
	if(cur_elem->prev != prev_elem || cur_elem->next != NULL || this->last != cur_elem)
	{
		res->ml_errno = 3;
		return res;
	}

	return res;
}

#endif

// ============================================================================================
// creating exact templates
template class CList<std::string>;
