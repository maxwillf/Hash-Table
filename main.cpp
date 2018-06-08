#include "hash.hpp"

int main(int argc, char *argv[])
{

	HashTable<int,int> tbl;

	for (int i = 0; i < 200; ++i) {
		tbl.insert(i,i+1);	
	}

	for (int i = 60; i < 150; ++i) {
		tbl.remove(i);	
	}
		tbl.print();
	return 0;
}
