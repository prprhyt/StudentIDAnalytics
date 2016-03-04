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

class student_id_details {
public:
	int enter_ad;
	char enter_semester;
	char dept[3];
	int class_num;
	int seminar_num;
	int personal_num;

	int get_student_id_details(WCHAR[]);
};

#endif
