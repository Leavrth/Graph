#pragma once
#include "GNode.h"

template<class T> // ��Ҫ���char*
class GArray {
public:
	GArray() :head(new GNode<T>()), vSize(50), vCurrSize(0), data(new int[vSize]) {}
	GArray(const GArray<T>&);
	~GArray();

	bool addV(T, int w); //���һ���ڵ㣬����Ϊ�ڵ�����(��ͬ)
	bool addE(T, T, int); //��������������ڵ����ƶ�Ӧ�Ľڵ����ӣ����Ѵ������滻
	bool delV(T); //ɾ��һ���ڵ�
	bool delE(T, T); //��������������ڵ����ƶ�Ӧ�Ľڵ�Ͼ�

	int getE(T, T)const; //���ز�����������ڵ���������Ӧ�Ľڵ�����ӱߵ�Ȩ��
	int getE(T) const; //���ز�����Ľڵ���������Ӧ�Ľڵ�����ӱ���

private:
	bool compare(T lhs, T rhs) {
		return lhs == rhs;
	} // �Ƚ����������Ƿ���ͬ,����������const char*
	void check(); // ���ڼ��data�Ƿ���һ������
	int find(T, T); // �ҵ������ڵ����Ƶ���ţ���������dataλ�õ�ӳ��
	int find(T); // �ҵ��ڵ����Ƶ����

	GNode<T>* head; // �������ڴ洢�ڵ����ƶ�Ӧ�����

	int vCurrSize; // ��ǰ�ڵ����
	int vSize; // ָ�����ݵĴ�С[0,vSize)
	inline int getvHash(int i, int j); // �������Ϊi��j�Ľڵ��Ȩ����dataλ�õ�ӳ��
	int* data; // ���ݴ�СΪvSize*(vSize-1)/2,��vSize���ڵ�����֮����������������


};




template<class T>
GArray<T>::GArray(const GArray<T>& rhs) :head(new GNode<T>()) {
	vSize = rhs.vSize;
	vCurrSize = rhs.vCurrSize;
	// ����data
	int size = vSize * (vSize - 1) / 2;
	data = new int[size];
	for (int i = 0; i != size; i++)
		data[i] = 0;
	// ��������
	GNode<T>* tmpr = rhs.head;
	GNode<T>* tmpl = head;
	while (tmpr->pNext != nullptr) {
		tmpr = tmpr->pNext;
		tmpl->pNext = new GNode<T>(tmpr->key, tmpr->value);
		tmp1 = tmp1->pNext;
	}
}

template<class T>
GArray<T>::~GArray() {
	delete[] data;
	GNode<T>* tmp = head->pNext;
	while (tmp != nullptr) {
		GNode<T>* ttmp = tmp;
		tmp = tmp->pNext;
		delete ttmp;
	}
	delete head;
}

template<class T>
void GArray<T>::check() { // ������˾���������
	if (vCurrSize == vSize) {
		int* tmp = data;
		int oldSize = vSize * (vSize - 1) / 2;
		vSize *= 2;
		int size = vSize * (vSize - 1) / 2;
		data = new int[size];
		int i;
		for (i = 0; i != oldSize; i++)
			data[i] = tmp[i];
		for (; i != size; i++)
			data[i] = 0;
		delete[] tmp;
	}
}

template<class T>
inline int GArray<T>::getvHash(int i, int j) {
	if (i < j)	return i * vSize - i * (i + 3) / 2 + j - 1;
	else return j * vSize - j * (j + 3) / 2 + i - 1;
}

template<class T>
bool GArray<T>::addV(T vName, int w) {
	GNode<T>* tmp = head; // ��0�ſ�ʼ
	int cnt = -1;
	int bst = -1; // ������¼��ǰ��ı�ɾ���Ŀգ���֮�������
	GNode<T>* bsttmp = 0;
	while (tmp->pNext != nullptr) {
		tmp = tmp->pNext;
		cnt++;
		if (bst == -1 && tmp->value == -1) {
			bsttmp = tmp;
			bst = cnt;
		}
		if (tmp->value != -1 && compare(vName, tmp->key))
			return false; // �Ѿ�������ͬ���ƵĽڵ���
	}
	if (bst != -1) {
		T p = GNode<T>::createKey(vName); //����һ����vName���µĶ�
		GNode<T>::delKey(bsttmp->key);
		bsttmp->key = p;
		bsttmp->createKey(vName);
		bsttmp->value = bst;
		bsttmp->weight = w;
		return true;
	}
	tmp->pNext = new GNode<T>(vName, cnt + 1, w); // ��ʱ����new�����׳����쳣
	vCurrSize++;
	check();
	return true; // �ɹ�����
}

template<class T>
int GArray<T>::find(T vName1, T vName2) {
	int vNo1 = -1, vNo2 = -1;
	GNode<T>* tmp = head->pNext;
	int cnt = -1;
	while (tmp != nullptr) {
		if (vNo1 != -1 && vNo2 != -1)break;
		cnt = tmp->value;
		if (tmp->value != -1 && compare(tmp->key, vName1))vNo1 = cnt;
		else if (tmp->value != -1 && compare(tmp->key, vName2))vNo2 = cnt;
		tmp = tmp->pNext;
	}
	if (vNo1 == -1 || vNo2 == -1) return -1; //�Ҳ�������Ӧ������
	return getvHash(vNo1, vNo2);
}

template<class T>
int GArray<T>::find(T vName1) {
	int vNo = -1;
	GNode<T>* tmp = head->pNext;
	while (tmp != nullptr) {
		if (tmp->value != -1 && compare(tmp->key, vName)) {
			vNo = tmp->value;
			break;
		}
		tmp = tmp->pNext;
	}
	return vNo;
}

template<class T>
bool GArray<T>::addE(T vName1, T vName2, int w) {
	int vHash = find(vName1, vName2);
	if (vHash == -1)return false;
	data[vHash] = w;
	return true;
}

template<class T>
bool GArray<T>::delV(T vName) {
	GNode<T>* tmp = head->pNext;
	int vNo = -1;
	while (tmp != nullptr) {
		if (tmp->value != -1 && compare(tmp->key, vName)) {
			vNo = tmp->value;
			tmp->value = -1;
			break;
		}
		tmp = tmp->pNext;
	}
	if (vNo == -1)return false;
	for (int i = 0; i != vNo; i++)
		data[getvHash(i, vNo)] = 0;
	for (int j = vNo + 1; j != vCurrSize; j++)
		data[getvHash(j, vNo)] = 0;
	return true;
}

template<class T>
bool GArray<T>::delE(T vName1, T vName2) {
	int vHash = find(vName1, vName2);
	if (vHash == -1)return false;
	data[vHash] = 0;
	return true;
}

template<class T>
int GArray<T>::getE(T vName1, T vName2) const {
	int vHash = find(vName1, vName2);
	if (vHash == -1)return vHash;
	return data[vHash];
}

template<class T>
int GArray<T>::getE(T vName)const {
	int vNo = find(vName);
	if (vNo == -1)return vNo;
	int cnt = 0;
	for (int i = 0; i != vNo; i++)
		if (data[getvHash(i, vNo)] != 0)cnt++;
	for (int j = vNo + 1; j != vCurrSize; j++)
		if (data[getvHash(j, vNo)] != 0)cnt++;
	return cnt;
}

template<>
bool GArray<const char*>::compare(const char* lhs, const char* rhs) {
	for (;; lhs++, rhs++) {
		if (*(unsigned char *)lhs != *(unsigned char *)rhs)
			return false;
		if (*lhs == 0)
			return true;
	}
}

template<>
bool GArray<char*>::compare(char* lhs, char* rhs) {
	for (;; lhs++, rhs++) {
		if (*(unsigned char *)lhs != *(unsigned char *)rhs)
			return false;
		if (*lhs == 0)
			return true;
	}
}