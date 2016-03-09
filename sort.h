#pragma once

#include <atomic>
#include <thread>
#include <iostream>
#include <mutex>
#include <functional>
#include <cassert>
#include <omp.h>
#include "radix_sort.h"

using namespace std;

inline
void count(unsigned* list, int size, int* histogram) {
	for (int i = 0; i < size; ++i) {
		++histogram[(list[i]>>24)];
	}
}

void srt(int* histogram, unsigned** marker) {
	static const int bin_size = 256;
	int left = histogram[0];
	int index;
	int tmp;
	while (left-- > 0) {
		index = *marker[0] >> 24;
		while (index != 0) {
			tmp = *marker[index];
			*marker[index]++ = *marker[0];
			*marker[0] = tmp;
			index = *marker[0] >> 24;
		}
		++marker[0];
	}

	for (int k = 1; k < bin_size; ++k) {
		left = histogram[k]-(marker[k] - marker[k - 1]);
		while (left-- > 0) {
			index = *marker[k] >> 24;
			while (index != k) {
				tmp = *marker[index];
				*marker[index]++ = *marker[k];
				*marker[k] = tmp;
				index = *marker[k] >> 24;
			}
			++marker[k];
		}
	}
}

inline
void go_srt(int* histogram, unsigned** marker, mutex& m, int& nr, const int max_nr) {
	int index;
	m.lock();
	index = nr;
	++nr;
	m.unlock();
	int size = 0;
	while (index < max_nr) {
		if (histogram[index]>64) {
			sort2((marker[index] - histogram[index]), histogram[index]);
		}else if (histogram[index] > 1) {
			insertion_sort((marker[index] - histogram[index]), histogram[index]);
		}
		m.lock();
		index = nr;
		++nr;
		m.unlock();
	}
}


void parallel_sort(unsigned* arr, int size) {
    	//Build histogram
    	static const int bin_size = 256;
    	int histogram[bin_size] = { 0 };

    	count(arr, size, histogram);

    	unsigned int* marker[bin_size];
    	unsigned int* previous = arr;
    	for ( int i = 0; i < bin_size; ++i ) {
    		marker[i] = previous;
    		previous += histogram[i];
    	}
    	
    	srt(histogram, marker);
		mutex M;
		int nr = 0;
		const int max_nr = 256;
		
		thread t1(go_srt, histogram, marker, std::ref(M), std::ref(nr), max_nr);
		thread t2(go_srt, histogram, marker, std::ref(M), std::ref(nr), max_nr);
		thread t3(go_srt, histogram, marker, std::ref(M), std::ref(nr), max_nr);
		go_srt(histogram, marker, std::ref(M), std::ref(nr), max_nr);
		t1.join();
		t2.join();
		t3.join();
		
}