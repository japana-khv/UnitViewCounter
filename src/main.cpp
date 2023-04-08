//============================================================================
// Name        : SearchTree.cpp
// Author      : rvk
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>

#include "GridCell.h"
#include "common.h"
#include "task.h"

using namespace std;

void test(){
	avx::GridCell::unitTest();
//	Task::testUnitTask();
	std::cerr << "Tests completed \n";
}

int main() {
	cout << "avx warper" << endl; // prints avx warper
	test();

	std::vector<unit> units;
	std::vector<int> result;

	int count = 0;

	// input
	std::ifstream in("input.txt");
	if (in.is_open()) {
		in >> count;
		unit t;
		for (int i = 0; i < count; i++) {
			in >> t.position.x >> t.position.y;
			in >> t.direction.x >> t.direction.y;
			in >> t.fov_deg >> t.distance;
			units.push_back(t);
		}
		in.close();
	}

	result.reserve(count);
	// invoke
	Task::checkVisible(units, result);
	{
		// output
		std::ofstream out;
		out.open("output_fast.txt");
		if (out.is_open()) {
			for (int i = 0; i < count; i++)
				out << result[i] << std::endl;
			out.close();
		}
	}

	std::vector<int> result2;
	result2.reserve(count);
	Task::checkVisible2(units, result2);
	{
		// output
		std::ofstream out;
		out.open("output_dumb.txt");
		if (out.is_open()) {
			for (int i = 0; i < count; i++)
				out << result2[i] << std::endl;
			out.close();
		}
	}

	if (result == result2){
		std::cout << "Fast and Dump methods results are the same." << std::endl;
	}else{
		std::cout << "ERROR!!! Fast and Dump methods results are not the same." << std::endl;
	}

	return 0;
}
