#pragma once

template<class T>
class GNode {
	// key��value�ǵȼ۵ģ���key����value����value����key
public:
	T key;
	int value; // ��0��ʼ, ��valueΪ-1������ζ������ɾ�����õ㱾����Ӧ��dataӳ��ȫΪ0��
			   // ���ޱߵĽڵ���Ȼ���Ǳ�����ɾ���ˣ���������ڵ���˵��
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
static char * GNode<char *>::createKey(char * k) { // char * �� const char*���ڶ��ϴ���һ�����鷵����ָ����Ϊ��ʼ��
	int csize = -1;
	while (k[++csize] != 0);
	char * tmp = new char[csize + 1]; // ע������0
	for (int i = 0; i <= csize; i++)
		tmp[i] = k[i];
	return tmp;
}

template<>
static const char * GNode<const char *>::createKey(const char * k) {
	int csize = -1;
	while (k[++csize] != 0);
	char * tmp = new char[csize + 1]; // ע������0
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