#pragma once
#include <stdio.h>


struct pqnode {
	int priority;
	void *value;
	pqnode() {
		priority = -1;
		value = NULL;
	}
};

class priority_queue {
public:
	priority_queue(int MAX) {
		body = new pqnode[maxsize = MAX + 1];
		numnodes = 0;
	}

	~priority_queue() {
		delete[] body;
	}

	void heapify(int index) {
		for (;;) {
			int left = index + index; int right = left + 1;
			// Кто больше, [index], [left], [right]?
			int largest = index;
			if (left <= numnodes &&
				body[left].priority < body[index].priority)
				largest = left;
			if (right <= numnodes &&
				body[right].priority < body[largest].priority)
				largest = right;
			if (largest == index) break;
			swap(index, largest);
			index = largest;
		}
	}

	void swap(int n1, int n2) {
		pqnode t = body[n1];
		body[n1] = body[n2];
		body[n2] = t;
	}

	int insert(pqnode const &node) {
		if (numnodes > maxsize) {
			return -1; // или расширяем.
		}
		body[++numnodes] = node;
		for (int i = numnodes; i > 1 &&
			body[i].priority < body[i / 2].priority;
			i /= 2) {
			swap(i, i / 2);
		}
		return 0;
	}

	pqnode fetchMin() {
		return numnodes == 0 ? pqnode() : body[1];
	}

	int removeMin() {
		if (numnodes == 0) return -1;
		body[1] = body[numnodes--];
		heapify(1);
		return 0;
	}

	void print() {
		printf("max=%d size=%d elems={ ", maxsize, numnodes);
		for (int i = 0; i < numnodes; i++) {
			printf("%d ", body[i].priority);
		}
		printf("}\n");
	}

	int size() {
		return numnodes;
	}
private:
	pqnode *body;
	int numnodes;
	int maxsize;
};

