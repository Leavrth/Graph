#include "GArray.h"

int main() {
	GArray<const char *> a; // 可用char*，此时下面的字符串用数组代替即可
	// 演示用const char *
	// 增加名为Label的作者，Size是影响力
	//  Label   Size
	//	 asd	 2
	a.addV("asd", 2);
	//   ppp     3
	a.addV("ppp", 3);
	//   ttt     4
	a.addV("ttt", 4);

	//将名为asd和ppp的作者用对应的权重连接边
	// Node1  Node2  Weight
	//  asd    ppp     3
	a.addE("asd", "ppp", 3);
	//  asd    ttt     5
	a.addE("asd", "ttt", 5);
	//  ppp    asd     1
	a.addE("ppp", "asd", 1);

	//删除ppp和ttt之间的边
	a.delE("ppp", "ttt");
	//删除节点ttt
	a.delV("ttt");

	//
	// 说明
	// （i）封装了一个GArray类，里面主要包含记录作者间合作关系的数组data
	// 以及用来记录作者的链表, 还有用来操作它们的成员函数和成员变量。
	// （ii）该链表记录了作者的“名称”(key)、影响力(weight)、在data中的排序(value)，
	// 被封装在了GNode.h文件中
	// （iii）类中vCurrSize记录了最后一个作者在链表的序号（这里是因为删除作者
	// 节点的函数并没有删除链表，而是将value设置为0，再初始化所有data中有关的边，从而
	// 会存在链表中某些部分已被删除）
	// (iv)vSize(50, 100, 200, 400, 800...)指出data最多可以放多少个节点, data大小即为
	// vSize * (vSize - 1) / 2（即vSize个节点两两连边所得的最大边数）
	// 这里用到了一个对应关系，假设有两个节点序号(i, j)（其中i和j都是来自链表中key（作者名称）对应
	// 的value），不妨让i<j，则其对应边数在data里的映射即为data[i * vSize - i * (i + 3) / 2 + j - 1]。
	// 而data中两个节点序号依次是(0,1),(0,2),(0,3),...,(0,vSize-1),(1,2),(1,3),...,(1,vSize-1),...,(vSize-2, vSize-1)
	// (v) 文件包含形式是GNode.h<-GArray.h<-main.cpp
	// 代码可能部分地方存在小小的漏洞，但总体来说是可以执行的。
	/*
	GArray功能：
		bool addV(T, int w); //添加一个节点，参数为节点名称(下同)
		bool addE(T, T, int); //将参数里的两个节点名称对应的节点连接，若已存在则替换
		bool delV(T); //删除一个节点
		bool delE(T, T); //将参数里的两个节点名称对应的节点断绝

		int getE(T, T)const; //返回参数里的两个节点名称所对应的节点的连接边的权重
		int getE(T) const; //返回参数里的节点名称所对应的节点的连接边数
	*/
	return 0;
}