#pragma once
#include <vector>
#include <cmath>

//static int g_Id = 0;

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
static int g_unitNum = 32;

static void checkIndex(int iIndex)
{
	if (iIndex < 0 || iIndex >= g_unitNum)
	{
		MessageBox(NULL,L"index error!", L"hello", MB_OK);
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
	int m_curIndex;//当前方向index

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
	}

	void setIndex(int index)
	{
		m_curIndex = index;
	}

	//头部节点行为，方向不断靠近,这里可以每一帧调用一次
	void toDirection(int iToIndex)//toIndex 和 m_curIndex 要保证都是非负数
	{
		checkIndex(iToIndex);
		checkIndex(m_curIndex);
		if (iToIndex == m_curIndex)
		{
			return;//已到达目的转向，
		}
		/*CString str;
		str.Format(_T("===============toDirection  curindex:%d toindex:%d \n"), m_curIndex, iToIndex);
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
			++m_curIndex;
			/*str.Format(_T("===============toDirection ++++ \n"));
			OutputDebugString(str);*/
		}
		else
		{
			--m_curIndex;
			/*str.Format(_T("===============toDirection ----- \n"));
			OutputDebugString(str);*/
		}
		if (m_curIndex < 0)
		{
			m_curIndex += g_unitNum;
		}

		m_curIndex = m_curIndex % g_unitNum;
	}

};

struct CLines {
	CLines()
	{
		CMyPoint begin(500, 200);
		int iLength = 8;
		CMyPoint end = begin;
		for (int i = 0; i < 50; ++i)
		{
			CMyPoint end(begin.m_x - iLength, begin.m_y);
			CLine line(begin, end);
			m_lines.push_back(line);
			begin = end;
		}
	}

	std::vector<CLine> m_lines;

	void addLine(CLine line)
	{
		m_lines.push_back(line);
	}

	void moveTo(CLine& line, int iToIndex)
	{

	}

	CLine moveToIndex(int iToIndex)
	{
		CLine& line0 = m_lines[0];
		int iDistance = 10;
		int size = g_unitAngle.m_vPoint.size();

		CMyPoint mvVec;
		CString str;
		CMyPoint myP_last;
		bool bFlag = false;
		for (int i = 0; i < m_lines.size(); ++i)
		{
			CLine& line = m_lines[i];

			int index = 0;
			int index_next = 0;
			CMyPoint mp;
			if (m_moveNum + m_Angle - i > 0)
			{
				index = (m_moveNum + m_Angle - i) % size;
				index_next = (m_moveNum + m_Angle - i + 1) % size;
				mp = g_unitAngle.m_vPoint[index];//一个单位向量，表示方向
				CMyPoint mp_next = g_unitAngle.m_vPoint[index_next];//一个单位向量，表示方向

				/*str.Format(_T("moveAngle index:%d index_next:%d mpx:%6f mpy:%6f mpnx:%6f mpny:%6f \n"),
					index, index_next, mp.m_x, mp.m_y, mp_next.m_x, mp_next.m_y);
				OutputDebugString(str);*/

				if (index == iToIndex)
				{
					str.Format(_T("===============moveAngle index:%d toindex:%d mpx:%6f mpy:%6f mpnx:%6f mpny:%6f===============\n"),
						index, iToIndex, mp.m_x, mp.m_y, mp_next.m_x, mp_next.m_y);
					OutputDebugString(str);
					bFlag = true;
				}
			}

			if (i == 0)
			{
				mvVec = CMyPoint(mp.m_x, mp.m_y);
				mvVec.m_x = mp.m_x * iDistance;
				mvVec.m_y = mp.m_y * iDistance;
				str.Format(_T("moveAngle index:%d x:%6f y:%6f \n"), index, mp.m_x, mp.m_y);
				OutputDebugString(str);

				line.moveVec(mvVec);
				myP_last.m_x = mvVec.m_x;
				myP_last.m_y = mvVec.m_y;
			}
			else
			{
				CMyPoint vec_now;
				vec_now.m_x = mp.m_x * iDistance;
				vec_now.m_y = mp.m_y * iDistance;
				//str.Format(_T("moveAngle index:%d x:%6f y:%6f \n"), index, mp.m_x, mp.m_y);
				//OutputDebugString(str);

				CMyPoint now;
				now.m_x = myP_last.m_x - vec_now.m_x;
				now.m_y = myP_last.m_y - vec_now.m_y;

				line.moveToPoint(now);
			}
			myP_last.m_x = line.m_mid.m_x;
			myP_last.m_y = line.m_mid.m_y;
		}
		if (bFlag)
		{

		}
		else
		{
			++m_moveNum;
		}
		return line0;
	}

	int m_distance = 50;

	int m_Angle = 5;//5个单位向量
	int m_CurIndex = 0;// 控制头部转弯到第几个index
	int m_moveNum = 0;

	//
	CLine moveToIndexNew(int iToIndex)
	{
		CLine& line0 = m_lines[0];
		int iDistance = 10;//转弯
		int iDistance1 = 12;//直行
		int size = g_unitAngle.m_vPoint.size();

		CMyPoint mvVec;
		CString str;
		CMyPoint myP_last;
		bool bFlag = false;

		if (iToIndex < m_CurIndex)
		{
			iToIndex += g_unitNum;
		}
		if (iToIndex == m_CurIndex)
		{
			//已到达目的转向，
		}
		else if (iToIndex - m_CurIndex < g_unitNum / 2)
		{
			++m_CurIndex;
		}
		else
		{
			--m_CurIndex;
		}

		m_CurIndex = m_CurIndex % g_unitNum;

		for (int i = 0; i < m_lines.size(); ++i)
		{
			CLine& line = m_lines[i];
			CMyPoint mp;

			//if (m_CurIndex + m_Angle - i > 0)
			//{
			//	int index = (m_CurIndex + m_Angle - i) % size;
			//	mp = g_unitAngle.m_vPoint[index];//一个单位向量，表示方向
			//	
			//	if (index == iToIndex)
			//	{
			//		str.Format(_T("===============moveAngle index:%d toindex:%d mpx:%6f mpy:%6f ===============\n"),
			//			index, iToIndex, mp.m_x, mp.m_y);
			//		OutputDebugString(str);
			//	}
			//}

			if (i == 0)
			{
				mp = g_unitAngle.m_vPoint[m_CurIndex];//一个单位向量，表示方向
				mvVec = CMyPoint(mp.m_x, mp.m_y);
				mvVec.m_x = mp.m_x * iDistance;
				mvVec.m_y = mp.m_y * iDistance;
				str.Format(_T("moveAngle index:%d x:%6f y:%6f \n"), m_CurIndex, mp.m_x, mp.m_y);
				OutputDebugString(str);

				line.moveVec(mvVec);
				myP_last.m_x = mvVec.m_x;
				myP_last.m_y = mvVec.m_y;
			}
			else
			{
				CMyPoint vec_now;
				vec_now.m_x = mp.m_x * iDistance;
				vec_now.m_y = mp.m_y * iDistance;

				CMyPoint now;
				now.m_x = myP_last.m_x - vec_now.m_x;
				now.m_y = myP_last.m_y - vec_now.m_y;

				line.moveToPoint(now);
			}
			myP_last.m_x = line.m_mid.m_x;
			myP_last.m_y = line.m_mid.m_y;
		}


		return line0;
	}
};