#pragma once
#include "GNode.h"

template<class T> // 主要针对char*
class GArray {
public:
	GArray() :head(new GNode<T>()), vSize(50), vCurrSize(0), data(new int[vSize]) {}
	GArray(const GArray<T>&);
	~GArray();

	bool addV(T, int w); //添加一个节点，参数为节点名称(下同)
	bool addE(T, T, int); //将参数里的两个节点名称对应的节点连接，若已存在则替换
	bool delV(T); //删除一个节点
	bool delE(T, T); //将参数里的两个节点名称对应的节点断绝

	int getE(T, T)const; //返回参数里的两个节点名称所对应的节点的连接边的权重
	int getE(T) const; //返回参数里的节点名称所对应的节点的连接边数

private:
	bool compare(T lhs, T rhs) {
		return lhs == rhs;
	} // 比较两个名称是否相同,用来特例化const char*
	void check(); // 用于检测data是否还有一定容量
	int find(T, T); // 找到两个节点名称的序号，返回其在data位置的映射
	int find(T); // 找到节点名称的序号

	GNode<T>* head; // 链表用于存储节点名称对应的序号

	int vCurrSize; // 当前节点个数
	int vSize; // 指明数据的大小[0,vSize)
	inline int getvHash(int i, int j); // 计算序号为i与j的节点的权重在data位置的映射
	int* data; // 数据大小为vSize*(vSize-1)/2,即vSize个节点两两之间相连所得最大边数


};




template<class T>
GArray<T>::GArray(const GArray<T>& rhs) :head(new GNode<T>()) {
	vSize = rhs.vSize;
	vCurrSize = rhs.vCurrSize;
	// 拷贝data
	int size = vSize * (vSize - 1) / 2;
	data = new int[size];
	for (int i = 0; i != size; i++)
		data[i] = 0;
	// 拷贝链表
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
void GArray<T>::check() { // 如果满了就扩大两倍
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
	GNode<T>* tmp = head; // 第0号开始
	int cnt = -1;
	int bst = -1; // 用来记录最前面的被删除的空，将之补充进来
	GNode<T>* bsttmp = 0;
	while (tmp->pNext != nullptr) {
		tmp = tmp->pNext;
		cnt++;
		if (bst == -1 && tmp->value == -1) {
			bsttmp = tmp;
			bst = cnt;
		}
		if (tmp->value != -1 && compare(vName, tmp->key))
			return false; // 已经存在相同名称的节点了
	}
	if (bst != -1) {
		T p = GNode<T>::createKey(vName); //创建一个存vName的新的堆
		GNode<T>::delKey(bsttmp->key);
		bsttmp->key = p;
		bsttmp->createKey(vName);
		bsttmp->value = bst;
		bsttmp->weight = w;
		return true;
	}
	tmp->pNext = new GNode<T>(vName, cnt + 1, w); // 暂时忽略new可能抛出的异常
	vCurrSize++;
	check();
	return true; // 成功加入
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
	if (vNo1 == -1 || vNo2 == -1) return -1; //找不到所对应的名称
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