#include <vector>
#include <string>
template <class T>
class Queue {
private:
	std::vector<T> container{};
	int size{ 0 };

public:
	Queue() {};

	Queue(int psize, T* elements) {
		size = psize;
		for (int i = 0; i < psize; ++i) {
			container.push_back(*(elements++));
		}
	}

	void enqueue(T elem) {
		container.push_back(elem);
		size += 1;
	}

	T dequeue() {
		T elem = container[0];
		container.erase(container.begin());
		size -= 1;
		return elem;
	}

	int get_lenght() {
		return size;
	}

	std::string print_queue_contents() {
		std::string ret{ "[" };
		for (T elem : container) {
			ret.append(elem);
			ret.append(", ");
		}
		ret.append("]");
		return ret;
	}
};