#pragma once

/***
Integers nonnegative
***/
void insertion_sort(unsigned int* list, const int size) {
	int temp, j;

	for (int i = 1; i < size; i++)
	{
		temp = list[i];

		for (j = i - 1; j >= 0 && list[j] > temp; j--)
			list[j + 1] = list[j];

		list[j + 1] = temp;
	}
}


void sort4(unsigned int* list, const int size) {
	const int mask = 255;
	//build histogram based on first 8 bits
	int histogram[256] = {}; //ommited value, set to 0

	for (int i = 0; i < size; ++i) {
		++histogram[list[i] & mask];
	}

	//markers
	unsigned int* marker[256];
	marker[0] = list;
	for (int j = 0; j < 255; ++j) {
		marker[j + 1] = marker[j] + histogram[j];
	}

	int left = histogram[0];
	int index;
	int tmp;
	while (left-- > 0) {

		index = *marker[0] & mask;

		while (index != 0) {
			tmp = *marker[index];
			*marker[index]++ = *marker[0];
			*marker[0] = tmp;
			index = *marker[0] & mask;
		}
		++marker[0];
	}

	for (int k = 1; k < 256; ++k) {

		left = histogram[k] - (marker[k] - marker[k - 1]);

		while (left-- > 0) {
			index = *marker[k] & mask;

			while (index != k) {
				tmp = *marker[index];
				*marker[index]++ = *marker[k];
				*marker[k] = tmp;
				index = *marker[k] & mask;
			}

			++marker[k];
		}

	}
};

void sort3(unsigned int* list, const int size) {
	const int mask = 255;
	const int shift = 8;
	//build histogram based on first 8 bits
	int histogram[256] = {}; //ommited value, set to 0

	for (int i = 0; i < size; ++i) {
		++histogram[(list[i] >> shift) & mask];
	}

	//markers
	unsigned int* marker[256];
	marker[0] = list;
	for (int j = 0; j < 255; ++j) {
		marker[j + 1] = marker[j] + histogram[j];
	}

	int left = histogram[0];
	int index;
	int tmp;
	while (left-- > 0) {

		index = (*marker[0] >> shift) & mask;

		while (index != 0) {
			tmp = *marker[index];
			*marker[index]++ = *marker[0];
			*marker[0] = tmp;
			index = (*marker[0] >> shift) & mask;
		}
		++marker[0];
	}

	for (int k = 1; k < 256; ++k) {

		left = histogram[k] - (marker[k] - marker[k - 1]);

		while (left-- > 0) {
			index = (*marker[k] >> shift) & mask;

			while (index != k) {
				tmp = *marker[index];
				*marker[index]++ = *marker[k];
				*marker[k] = tmp;
				index = (*marker[k] >> shift) & mask;
			}

			++marker[k];
		}

	}

	//initialy all markers where in the beggining of sections, after sorting each is in the end of section it was pointing too
	for (int l = 255; l >= 0; --l) {
		if (histogram[l] > 1) {
			if (histogram[l] > 32) {
				//radix sort
				sort4(marker[l] - histogram[l], histogram[l]);
			}
			else {
				insertion_sort(marker[l] - histogram[l], histogram[l]);
			}
		}
	}
};

//stable; in place; linear
void sort2(unsigned int* list, const int size) {
	const int mask = 255;
	const int shift = 16;
	//build histogram based on first 8 bits
	int histogram[256] = {}; //ommited value, set to 0

	for (int i = 0; i < size; ++i) {
		++histogram[(list[i] >> shift) & mask];
	}

	//markers
	unsigned int* marker[256];
	marker[0] = list;
	for (int j = 0; j < 255; ++j) {
		marker[j + 1] = marker[j] + histogram[j];
	}

	int left = histogram[0];
	int index;
	int tmp;
	while (left-- > 0) {

		index = (*marker[0] >> shift) & mask;

		while (index != 0) {
			tmp = *marker[index];
			*marker[index]++ = *marker[0];
			*marker[0] = tmp;
			index = (*marker[0] >> shift) & mask;
		}
		++marker[0];
	}

	for (int k = 1; k < 256; ++k) {

		left = histogram[k] - (marker[k] - marker[k - 1]);

		while (left-- > 0) {
			index = (*marker[k] >> shift) & mask;

			while (index != k) {
				tmp = *marker[index];
				*marker[index]++ = *marker[k];
				*marker[k] = tmp;
				index = (*marker[k] >> shift) & mask;
			}

			++marker[k];
		}

	}

	//initialy all markers where in the beggining of sections, after sorting each is in the end of section it was pointing too
	for (int l = 255; l >= 0; --l) {
		if (histogram[l] > 1) {
			if (histogram[l] > 32) {
				//radix sort
				sort3(marker[l] - histogram[l], histogram[l]);
			}
			else {
				//insertion sort
				insertion_sort(marker[l] - histogram[l], histogram[l]);
			}
		}
	}
};