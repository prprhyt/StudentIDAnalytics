#pragma once
#ifndef STUDENT_ID_ANALYTICS_FELICALIB_SIDACS_WRAPPER_HPP
#define STUDENT_ID_ANALYTICS_FELICALIB_SIDACS_WRAPPER_HPP
#include<cstdio>
#include<Windows.h>
#include "../felicalib/felicalib.h"

#define _T TEXT

class felicalib_wrapper {
private:
	pasori *p;
	felica *f;
public:

	int init_felica();

	int read_data(WCHAR[]);

	void destroy_felica();

};
#endif