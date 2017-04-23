#include "std.h"
#include "CList.h"

template<typename InfoType>
void getList(CList<InfoType>& storage)
{
	int in_length;
	InfoType info;

	std::cin >> in_length;
	for(int i = 0; i < in_length; i++)
	{
		std::cin >> info;
		storage.add(info);
	}
}

int main()
{
	CList<std::string> test_list;
	getList(test_list);

	test_list.print();

	return 0;
}
