#pragma once

template<class T>
class GNode {
	// key和value是等价的，用key来找value，用value来找key
public:
	T key;
	int value; // 从0开始, 若value为-1，则意味着它被删除，该点本来对应的data映射全为0，
			   // 即无边的节点自然就是被视作删除了（相对其他节点来说）
	int weight;
	GNode* pNext;
	static T createKey(T k) { return k; }
	static void delKey(T k) {}
	GNode(const GNode& rhs) :key(GNode<T>::createKey(rhs.key)), value(rhs.value), pNext(rhs.pNext) {}
	GNode(T k, int v, int w, GNode<T>* pN = nullptr) :key(GNode<T>::createKey(k)), value(v), weight(w), pNext(pN) {}
	GNode() :key(0), value(0), weight(0), pNext(nullptr) {}
	~GNode() {}
};


template<>
GNode<char*>::GNode() :key(nullptr), value(0), weight(0), pNext(nullptr) {

}

template<>
GNode<const char*>::GNode() : key(nullptr), value(0), weight(0), pNext(nullptr) {

}

template<>
static void GNode<char *>::delKey(char * k) {
	delete[] k;
}

template<>
static void GNode<const char *>::delKey(const char * k) {
	delete[] k;
}

template<>
static char * GNode<char *>::createKey(char * k) { // char * 和 const char*就在堆上创建一个数组返回其指针作为初始化
	int csize = -1;
	while (k[++csize] != 0);
	char * tmp = new char[csize + 1]; // 注意最后的0
	for (int i = 0; i <= csize; i++)
		tmp[i] = k[i];
	return tmp;
}

template<>
static const char * GNode<const char *>::createKey(const char * k) {
	int csize = -1;
	while (k[++csize] != 0);
	char * tmp = new char[csize + 1]; // 注意最后的0
	for (int i = 0; i <= csize; i++)
		tmp[i] = k[i];
	return tmp;
}

template<>
GNode<char *>::~GNode() {
	delete[] key;
}

template<>
GNode<const char *>::~GNode() {
	delete[] key;
}