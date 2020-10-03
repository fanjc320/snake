#pragma once
#include <vector>
#include <queue>
#include <cmath>
using namespace std;
static float g_NodeDistance = 12.0f;//node֮���ֱ�о��룬���߽г�ʼ����
static int g_bodyNum = 50;

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

	CMyPoint operator+(const CMyPoint& b) {
		CMyPoint box;
		box.m_x = this->m_x + b.m_x;
		box.m_y = this->m_y + b.m_y;
		return box;
	}
	//��Ҫд��this.m_x= ����ʽ�������-���������ı����ڵ�ֵ
	CMyPoint operator-(const CMyPoint& b) {
		CMyPoint box;
		box.m_x = this->m_x - b.m_x;
		box.m_y = this->m_y - b.m_y;
		return box;
	}
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


//����toP�Ƿ���p1��p2�н��ڣ����Ƿ�ɱ�p1��p2���
static bool bInAngle(CMyPoint toP, CMyPoint p1, CMyPoint p2)
{
	//https://wenku.baidu.com/view/f69f660976c66137ee061977.html
	//a*p1 + b*p2 = toP, a>=0,b>=0 ��˵��toP��p1��p2�н�����
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
	//a��b��>=0,�����
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


struct CNode {
	CNode(CMyPoint point) {
		m_point = point;
	}

	CMyPoint m_point;
	int m_curIndex = 0;//��ǰ����index
	int m_IndexGap = 4;
	bool bTurning = false;
	int m_flag = 0;

	//std::queue<CMyPoint> vHistoryPoints;
	//std::queue<CMyPoint> vHistoryPoints;
	std::deque<CMyPoint> vHistoryPoints;

	void pushToHis(CMyPoint point)
	{
		vHistoryPoints.push_back(point);
		CString str;
		/*
		str.Format(_T("----------------------------------------------------------------------------------------------------------------------flag:%d----pushToHis point-px:%f py:%f\n"),
			m_flag,point.m_x, point.m_y);
		OutputDebugString(str);*/
		while (vHistoryPoints.size()>3)
		{
			vHistoryPoints.pop_front();
		}
		
		/*for (auto it = vHistoryPoints.begin(); it != vHistoryPoints.end(); it++)
		{
			str.Format(_T(" ---p0x:%f p0y:%f "),
				it->m_x, it->m_y);
			OutputDebugString(str);
		}*/

		/*str.Format(_T("pushToHis print  --flag:%d  \n "),m_flag);
		OutputDebugString(str);*/

		if (vHistoryPoints.size()>=3)
		{

			CMyPoint vec1 = vHistoryPoints[1] - vHistoryPoints[0];
			CMyPoint vec2 = vHistoryPoints[2] - vHistoryPoints[1];
			if (fabs(vec1.m_x * vec2.m_y - vec1.m_y * vec2.m_x)<0.01f) //����
			{
				/*str.Format(_T("--------turning false---------------------------vec1x:%f vec1y:%f vec2x:%f vec2y:%f-------- \n"),
					vec1.m_x,vec1.m_y,vec2.m_x,vec2.m_y);
				OutputDebugString(str);*/
				bTurning = false;
			}
			else
			{
				/*str.Format(_T("----------------turning true-------vec1x:%f vec1y:%f vec2x:%f vec2y:%f--------- \n"),
					vec1.m_x, vec1.m_y, vec2.m_x, vec2.m_y);
				OutputDebugString(str);*/
				bTurning = true;
			}
		}
	}

	void moveToPoint(CMyPoint cp)
	{
		m_point = cp;
		pushToHis(m_point);
	}
	void moveVec(CMyPoint cp)
	{
		m_point.m_x += cp.m_x;
		m_point.m_y += cp.m_y;
		pushToHis(m_point);
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

	//ͷ���ڵ���Ϊ�����򲻶Ͽ���,�������ÿһ֡����һ��
	bool toIndex(int iToIndex)//toIndex �� m_curIndex Ҫ��֤���ǷǸ���
	{
		checkIndex(iToIndex);
		checkIndex(m_curIndex);
		if (iToIndex == m_curIndex)
		{
			bTurning = false;
			return true;//�ѵ���Ŀ��ת��
		}
		CString str;
		/*
		str.Format(_T("==================================================toIndex  curindex:%d toindex:%d \n"), m_curIndex, iToIndex);
		OutputDebugString(str);*/

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
			/*m_curIndex += m_IndexGap;
			str.Format(_T("===============toIndex  curindex:%d ++++  \n"), m_curIndex);*/
			OutputDebugString(str);
		}
		else
		{
			/*m_curIndex -= m_IndexGap;
			str.Format(_T("===============toIndex  curindex:%d ---- \n"), m_curIndex);*/
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
	//��ǰһ�ڵ��뱾�ڵ����˶��ǶȲ��(A��ǰһ�ڵ㣬B�Ǻ�һ�ڵ㣬A����Ŀ��index�˶���һ���˶��Ƕ�Ϊm_IndexGap��index,B�˶�(m_IndexGap-1)���Ƕ�),
	//B���˶�û��Aת����ô��,���Բ�������·��Ч�����ɴˣ�B��A�˶�������ͬ���߸��������£�B��A�����λ�û����,������������Ч��
	//��A���˶��Ƕ�ΪͻȻ��m_IndexGap��Ϊ0����ôB�ڵ���(m_IndexGap-1)����0��Ȼ�������ȫģ��A�Ĺ���,��toPreNode
	//C��׷��B�Ĺ��̣�ÿ��ת��(m_IndexGap-2�����Ƕȣ����Ҳ����(m_IndexGap-2������0
	//D��׷��C�Ĺ��̣�ÿ��ת��(m_IndexGap-3�����Ƕȣ����Ҳ����(m_IndexGap-3������0
	//...��m_IndexGap-n == 0 ��û��ת��ֻ�д�ģ��ǰһ�ڵ㣬��toPreNode
	//Ҳ����˵��ͷ���ڵ��ÿ��ת��index��󣬺���Ľڵ�ת���߶����εݼ���0
	//����ڵ�ÿ��ת��Ƕ���ȻС������ǰһ�ڵ�������룬�����˳�ת����̣�����ת��������࣬�ܽǶ�Ҳ��360�ȡ�
	//����ڵ�ת���γɵ�Բ����ǰһ�ڵ��С��������תȦȦ��СЧ��
	//�����и����⣬���ת���Ƕ���m_IndexGap-1����ʽ�����ôճ�360�ȣ�Ҳ����˵������ģ�Ҫ�ճ�360�ȣ�����������֮���
	//���ԣ����ﻹ������������
	bool toPreArrive(CMyPoint preArrive)//ǰһ�ڵ��ƶ��󵽴��λ��
	{

		return false;
	}

	//��ȫģ��ǰһ�ڵ�
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
		int iLength = g_NodeDistance;
		//int iLength = g_NodeDistance;
		for (int i = 0; i < g_bodyNum; ++i)
		{
			begin.m_x -= iLength;
			CNode node(begin);
			node.m_flag = i;
			m_nodes.push_back(node);
		}
	}

	std::vector<CNode> m_nodes;

	int m_distance = 50;

	int m_Angle = 5;//5����λ����
	int m_CurIndex = 0;// ����ͷ��ת�䵽�ڼ���index
	int m_moveNum = 0;


	CNode moveToIndexNew1(int iToIndex)
	{
		CNode& head = m_nodes[0];
		//int iDistance = 10;//ת��
		//int iDistance1 = 12;//ֱ��
		int size = g_unitAngle.m_vPoint.size();

		//CMyPoint mvVec;
		CString str;
		CMyPoint myP_last;//�ϸ������ƶ���λ��
		CMyPoint myP_last_old;//�ϸ������λ��
		CNode* pLastNode = &head;
		bool bFlag = false;
		/*if (iToIndex == 0)
		{
			str.Format(_T("moveToIndexNew1 0000000000 \n"));
			OutputDebugString(str);
		}
		else
		{
			str.Format(_T("moveToIndexNew1 1111111111 \n"));
			OutputDebugString(str);
		}*/
		myP_last_old = head.m_point;
		//head.pushToHis(head.m_point);
		head.moveForward(g_NodeDistance);
		

		myP_last = head.m_point;//ǰһ�ڵ��ƶ�����λ��
		bool bTurned = true;//trueΪֱ�У�falseΪת����
		if (iToIndex != head.m_curIndex)
		{
			bTurned = head.toIndex(iToIndex);//�����Ƿ񵽴�Ŀ��ת��
		}

		int iCnt = 4;
		int iInitIndex = head.m_curIndex;
		bool bTurning = false;//��һ�ڵ��Ƿ���ת��
		for (int i = 1; i < m_nodes.size(); ++i)
		{
			CNode& body = m_nodes[i];
			CMyPoint tmp = body.m_point;
			//body.pushToHis(body.m_point);

			CMyPoint direct;//ǰһ�ڵ�A�뵱ǰ�ڵ�B֮��Ĳ�����
			direct.m_x = myP_last.m_x - body.m_point.m_x;
			direct.m_y = myP_last.m_y - body.m_point.m_y;
			//ǰһ�ڵ�A�뵱ǰ�ڵ�B֮��Ĳ������Ĵ�С,������
			float distance = pow(pow(direct.m_x, 2) + pow(direct.m_y, 2), 0.5);

			if (distance >= g_NodeDistance)
			{
				/*str.Format(_T("moveToIndexNew1 -------22----distance:%d------- \n"),distance);
				OutputDebugString(str);*/
				//ת��ʱ���ɺ�ֱ��ʱ�ƶ���ͬ���룬�ɵ�����������,move�ĳ�����һ������g_NodeDistance
				CMyPoint move = CMyPoint(direct.m_x / distance * g_NodeDistance, direct.m_y / distance * g_NodeDistance);
				CMyPoint moveTo = CMyPoint(myP_last.m_x - move.m_x, myP_last.m_y - move.m_y);

				if (pLastNode->bTurning)
				{
					body.moveVec(move);
					/*str.Format(_T("moveToIndexNew1 --------11-----------index:%d \n"), i);
					OutputDebugString(str);*/
				}
				else
				{
					body.moveToPoint(moveTo);
					/*str.Format(_T("moveToIndexNew1 --------22-----------index:%d \n"), i);
					OutputDebugString(str);*/
				}

			}
			
			myP_last_old = tmp;
			myP_last = body.m_point;
			pLastNode = &body;
		}


		return head;
	}
};