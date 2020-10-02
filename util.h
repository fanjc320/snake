#pragma once
#include <vector>
#include <cmath>

//static int g_Id = 0;
static bool g_bTurning = false;//转向中 
static float g_NodeDistance = 12.0f;//node之间的直行距离，或者叫初始距离

struct CMyPoint
{
	CMyPoint() {};
	CMyPoint(float x, float y)
	{
		m_x = x;
		m_y = y;
	}
	float m_x = 0.0f;
	float m_y = 0.0f;
};
struct CUnitAngle
{
	std::vector<CMyPoint> m_vPoint;
	void getPoints(int iNum)
	{
		m_vPoint.clear();
		float angle = 360.0f / iNum;
		for (int i = 0; i < iNum; ++i)
		{
			float x = cos(angle * i / 180 * 3.1415926);
			float y = sin(angle * i / 180 * 3.1415926);
			CMyPoint p;
			p.m_x = x;
			p.m_y = y;
			m_vPoint.push_back(p);
		}
	}
	void printPoints()
	{
		for (auto p : m_vPoint)
		{
			//std::cout << p.m_x << p.m_y << std::endl;
		}
	}
	CMyPoint getDirect(int index)
	{
		return m_vPoint[index];
	}
};
static CUnitAngle g_unitAngle;
static int g_unitNum = 128;

static void checkIndex(int iIndex)
{
	if (iIndex < 0 || iIndex >= g_unitNum)
	{
		MessageBox(NULL, L"index error!", L"hello", MB_OK);
	}
}

//单位向量，用来表示角度
struct CAngle
{
	float distancePoint(float x1, float y1, float x2, float y2)
	{
		//return  pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
		float all = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
		float s = sqrt(all);//2次开根bai号函数,必须du包含头文件zhicmath.
		return s;
	}

	CAngle(float x, float y)//输入目标向量
	{
		//int distance = (int)distancePoint(0, 0, x, y);
		float distance = distancePoint(x, y, 0, 0);
		if (distance < 0.0001)
		{
			distance = 0.0001;
		}
		//求差向量
		x -= distance;
		distance = distancePoint(x, y, 0, 0);
		if (distance < 0.0001)
		{
			distance = 0.0001;
		}
		m_x = x / distance;
		m_y = y / distance;

	}
	/*CAngle(CMyPoint point)
	{
		CAngle(point.x, point.y);
	}*/
	float m_x = 1;
	float m_y = 0;
};

struct CVec// 在angle的基础上加上长度
{
	CAngle m_angle = CAngle(1, 0);
	//CAngle m_angle(1,0);
	float m_fLength = 0;
	CMyPoint m_ang_point = CMyPoint(1, 1);
	float m_x = 0;
	float m_y = 0;

	float distancePoint(float x1, float y1, float x2, float y2)
	{
		return  pow(pow(x1 - x2, 2) + pow(y1 - y2, 2), 0.5);
	}
	CVec(float x, float y, float fLength)
	{
		m_angle = CAngle(x, y);
		m_fLength = fLength;

		m_x = m_angle.m_x * fLength;
		m_y = m_angle.m_y * fLength;
	}
	CVec(CAngle angle, float fLength)
	{
		CVec(angle.m_x, angle.m_y, fLength);
	}
	void setXY(CAngle angle, float fLength)
	{
		m_angle = angle;
		m_x = m_angle.m_x * fLength;
		m_y = m_angle.m_y * fLength;
	}
	CVec(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

};


//向量toP是否在p1和p2夹角内，即是否可被p1和p2组合
static bool bInAngle(CMyPoint toP, CMyPoint p1, CMyPoint p2)
{
	//https://wenku.baidu.com/view/f69f660976c66137ee061977.html
	//a*p1 + b*p2 = toP, a>=0,b>=0 则说明toP在p1和p2夹角以内
	//a*p1.m_x+b*p2.m_x = toP.m_x;
	//a*p1.m_y+b*p2.m_y = toP.m_y;

	//a*p1.m_x+b*p2.m_x - toP.m_x = 0;
	//a*p1.m_y+b*p2.m_y - toP.m_y = 0;
	//A1x+B1y+C1=0
	//A2x+B2y+C2=0
	//x=(B1C2-B2C1)/(A1B2-A2B1);y=(C1A2-C2A1)/(A1B2-A2B1);
	float A1 = p1.m_x; float B1 = p2.m_x; float C1 = -toP.m_x;
	float A2 = p1.m_y; float B2 = p2.m_y; float C2 = -toP.m_y;
	float fGap = 0.0001f;
	//a和b都>=0,则存在
	if (A1 * B2 - A2 * B1 < fGap || A1 * B2 - A2 * B1 < fGap)
	{
		return false;
	}
	float a = (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1);
	float b = (C1 * A2 - C2 * A1) / (A1 * B2 - A2 * B1);
	if (a < 0 || b < 0)
	{
		return false;
	}

	return true;
}

struct CLine {
	CLine(CMyPoint begin, CMyPoint end) {
		m_begin = begin;
		m_end = end;
		m_mid = CMyPoint((m_begin.m_x + m_end.m_x) / 2, (m_end.m_y + m_end.m_y) / 2);
	}
	CLine()
	{
	}

	CMyPoint m_begin;
	CMyPoint m_end;
	CMyPoint m_mid;
	//CAngle m_moveAng = CAngle(1.0f, 0.0f);
	int m_angleCnt = 0;
	int m_curIndex;//当前方向index

	void moveToPoint(CMyPoint cp)
	{
		m_mid = cp;
		m_begin.m_x = cp.m_x - 0.05f;
		m_begin.m_y = cp.m_y;
		m_end.m_x = cp.m_x + 0.05f;
		m_end.m_y = cp.m_y;
	}
	void moveVec(CMyPoint p)
	{
		m_mid.m_x += p.m_x;
		m_begin.m_x += p.m_x;
		m_end.m_x += p.m_x;

		m_mid.m_y += p.m_x;
		m_begin.m_y += p.m_y;
		m_end.m_y += p.m_y;
	}

	void moveForward(float fDistance)
	{
		/*g_unitNum
		CMyPoint mm();*/
		CMyPoint pp = g_unitAngle.getDirect(m_curIndex);
		moveVec(CMyPoint(pp.m_x * fDistance, pp.m_y * fDistance));
	}

	void setDirection(int index)
	{
		m_curIndex = index;
	}

};

struct CNode {
	CNode(CMyPoint point) {
		m_point = point;
	}

	CMyPoint m_point;
	int m_curIndex = 0;//当前方向index
	int m_IndexGap = 4;

	void moveToPoint(CMyPoint cp)
	{
		m_point = cp;
	}
	void moveVec(CMyPoint cp)
	{
		m_point.m_x += cp.m_x;
		m_point.m_y += cp.m_y;
	}

	void moveForward(float fDistance)
	{
		CMyPoint pp = g_unitAngle.getDirect(m_curIndex);
		moveVec(CMyPoint(pp.m_x * fDistance, pp.m_y * fDistance));
		/*CString str;
		str.Format(_T("moveForward curindex:%d ppx:%f ppy:%f\n"), m_curIndex, pp.m_x, pp.m_y);
		OutputDebugString(str);*/
	}

	void setIndex(int index)
	{
		m_curIndex = index;
	}

	//头部节点行为，方向不断靠近,这里可以每一帧调用一次
	bool toIndex(int iToIndex)//toIndex 和 m_curIndex 要保证都是非负数
	{
		checkIndex(iToIndex);
		checkIndex(m_curIndex);
		if (iToIndex == m_curIndex)
		{
			return true;//已到达目的转向，
		}
		CString str;
		str.Format(_T("==================================================toIndex  curindex:%d toindex:%d \n"), m_curIndex, iToIndex);
		OutputDebugString(str);

		int iA = 0;
		int iB = 0;
		if (m_curIndex < iToIndex)
		{
			iA = iToIndex - m_curIndex;
			m_curIndex += g_unitNum;
			iB = m_curIndex - iToIndex;
		}
		else
		{
			iB = m_curIndex - iToIndex;
			iToIndex += g_unitNum;
			iA = iToIndex - m_curIndex;
		}

		if (iA < iB)
		{
			m_curIndex += m_IndexGap;
			str.Format(_T("===============toIndex  curindex:%d ++++  \n"), m_curIndex);
			OutputDebugString(str);
		}
		else
		{
			m_curIndex -= m_IndexGap;
			str.Format(_T("===============toIndex  curindex:%d ---- \n"), m_curIndex);
			OutputDebugString(str);
		}

		if (m_curIndex < 0)
		{
			m_curIndex += g_unitNum;
		}

		m_curIndex = m_curIndex % g_unitNum;
		return false;
	}
	
	//...->D->C->B-A
	//当前一节点与本节点有运动角度差别(A是前一节点，B是后一节点，A向着目标index运动，一次运动角度为m_IndexGap个index,B运动(m_IndexGap-1)个角度),
	//B的运动没有A转向那么大,可以产生抄近路的效果，由此，B和A运动距离相同或者更大的情况下，B和A的相对位置会更近,造成蛇身的缩短效果
	//当A的运动角度为突然由m_IndexGap变为0，那么B节点由(m_IndexGap-1)进入0，然后进入完全模仿A的过程,即toPreNode
	//C在追逐B的过程，每次转向(m_IndexGap-2）个角度，最后也是由(m_IndexGap-2）进入0
	//D在追逐C的过程，每次转向(m_IndexGap-3）个角度，最后也是由(m_IndexGap-3）进入0
	//...当m_IndexGap-n == 0 ，没有转向，只有纯模仿前一节点，即toPreNode
	//也就是说，头部节点的每次转动index最大，后面的节点转动尺度依次递减到0
	//后面节点每次转向角度虽然小，但比前一节点提早进入，较晚退出转向过程，所以转向次数更多，总角度也是360度。
	//后面节点转向形成的圆，比前一节点更小，所以有转圈圈缩小效果
	//这里有个问题，如果转动角度用m_IndexGap-1的形式，不好凑成360度，也就是说是问题的，要凑成360度，可能有跳变之类的
	//所以，这里还是用向量减法
	bool toPreArrive(CMyPoint preArrive)//前一节点移动后到达的位置
	{

		return false;
	}

	//完全模仿前一节点
	bool toPreNode(CNode pre)
	{
		*this = pre;
		return true;
	}

};


struct CLinesNew {
	CLinesNew()
	{
		CMyPoint begin(500, 200);
		int iLength = 12;
		//int iLength = g_NodeDistance;
		for (int i = 0; i < 35; ++i)
		{
			begin.m_x -= -iLength;
			CNode node(begin);

			m_nodes.push_back(node);
		}
	}

	std::vector<CNode> m_nodes;

	int m_distance = 50;

	int m_Angle = 5;//5个单位向量
	int m_CurIndex = 0;// 控制头部转弯到第几个index
	int m_moveNum = 0;


	CNode moveToIndexNew1(int iToIndex)
	{
		CNode& head = m_nodes[0];
		//int iDistance = 10;//转弯
		//int iDistance1 = 12;//直行
		int size = g_unitAngle.m_vPoint.size();

		//CMyPoint mvVec;
		CString str;
		CMyPoint myP_last;//上个点已移动的位置
		CMyPoint myP_last_old;//上个点的老位置
		bool bFlag = false;
		if (iToIndex == 0)
		{
			str.Format(_T("moveToIndexNew1 0000000000 \n"));
			OutputDebugString(str);
		}
		else
		{
			str.Format(_T("moveToIndexNew1 1111111111 \n"));
			OutputDebugString(str);
		}
		myP_last_old = head.m_point;
		head.moveForward(g_NodeDistance);

		myP_last = head.m_point;//前一节点移动到的位置
		bool bTurned = true;//true为直行，false为转弯中
		if (iToIndex != head.m_curIndex)
		{
			bTurned = head.toIndex(iToIndex);//返回是否到达目标转向
		}

		int iCnt = 4;
		int iInitIndex = head.m_curIndex;
		for (int i = 1; i < m_nodes.size(); ++i)
		{
			CNode& body = m_nodes[i];
			CMyPoint tmp = body.m_point;

			CMyPoint direct;//前一节点A与当前节点B之间的差向量
			direct.m_x = myP_last.m_x - body.m_point.m_x;
			direct.m_y = myP_last.m_y - body.m_point.m_y;
			//前一节点A与当前节点B之间的差向量的大小,即距离
			float distance = pow(pow(direct.m_x, 2) + pow(direct.m_y, 2), 0.5);
			//可替换解决方案,本节点与前一节点，前一节点已到达的节点 共线，说明前一节点的移动是直行
			if (bTurned)//直行
			{
				//body.moveToPoint(myP_last_old);//重复上一次运动有个问题,就是如果上一节点A的移动距离和 A与本节点B初始的距离F 不等， 则移动后，F会改变,导致蛇身长度正常直行时与初始不同
				//if (distance > g_NodeDistance)
				{
					str.Format(_T("moveToIndexNew1 -------00----------- \n"));
					OutputDebugString(str);
					body.moveToPoint(myP_last_old);//直行时，只有当与前一节点的距离超过初始距离，本节点才动，这样就可以在直行时重新伸开身体
				}
				str.Format(_T("moveToIndexNew1 --------11----------- \n"));
				OutputDebugString(str);
			}
			else
			{
				str.Format(_T("moveToIndexNew1 -------22----------- \n"));
				OutputDebugString(str);
				//转弯时依旧和直行时移动相同距离，可导致蛇身收缩
				CMyPoint move = CMyPoint(direct.m_x / distance * g_NodeDistance, direct.m_y / distance * g_NodeDistance);
				body.moveVec(move);
			}
			
			myP_last_old = tmp;
			myP_last = body.m_point;

		}


		return head;
	}
};