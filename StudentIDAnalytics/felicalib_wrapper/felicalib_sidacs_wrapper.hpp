#pragma once
#ifndef STUDENT_ID_ANALYTICS_FELICALIB_SIDACS_WRAPPER_HPP
#define STUDENT_ID_ANALYTICS_FELICALIB_SIDACS_WRAPPER_HPP
#include<cstdio>
#include<iterator>
#include<vector>
#include<algorithm>
#include<Windows.h>
#include <imagehlp.h>
#pragma comment(lib,"imagehlp.lib")
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
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

	//再帰用
	char details_char[6][16];
	int get_student_id_details(WCHAR[]);
	int count;
};

class student_id_data_node {
public:
	std::vector<student_id_data_node*> nodes;
	int child_node_num;
	char label[16];

	std::vector<int> personal_id;
};

class student_id_data_tree {
private:
	int add_tree_node(student_id_data_node*, student_id_details);//ツリーにノードを追加する
	void add_personal_num_list(student_id_data_node*, int);
	int count_student_id_from_tree(student_id_data_node *node, student_id_details sids);
	std::vector<std::wstring> enumerate_student_id_from_tree(student_id_data_node *node, student_id_details sids);
	std::vector<std::wstring> enumerate_elements_from_tree(student_id_data_node *node, student_id_details sids);
public:
	student_id_data_node* create_node(char[]);
	student_id_data_node* delete_node(student_id_data_node*);
	void add_tree_student_id(student_id_data_node*,WCHAR[]);
	int get_number_of_student_id_by_word(student_id_data_node*,WCHAR[]);
	std::vector<std::wstring> get_list_of_student_id_by_word(student_id_data_node *node, WCHAR rcvdata[]);
	std::vector<std::wstring> get_list_of_elements_name_by_word(student_id_data_node *node, WCHAR rcvdata[]);
	int store_student_id_data(student_id_data_node*);
	int restore_student_id_data(student_id_data_node*);
};

#endif

