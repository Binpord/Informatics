#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>

#define BAD_IN -1
#define FAIL -2

template<class T> std::ostream& operator<<(std::ostream& s, T& a)
{
	return a.put(s);
}
