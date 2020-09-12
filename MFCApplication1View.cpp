
// MFCApplication1View.cpp: CMFCApplication1View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <vector>
#include <iostream>


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_WM_KEYUP()
    ON_WM_CHAR()
END_MESSAGE_MAP()

// CMFCApplication1View 构造/析构

CLines g_Lines;

CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: 在此处添加构造代码
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//初始化画笔、点变量数组
	m_pen[0].CreatePen(PS_SOLID, 1, RGB(255, 0, 0));     //红色实线，1像素宽---参数：样式、宽度、颜色
	m_pen[1].CreatePen(PS_SOLID, 6, RGB(0, 255, 0));     //绿色实线，6像素宽
	m_pen[2].CreatePen(PS_DASH, 1, RGB(255, 0, 0));      //红色虚线，必须为一个像素宽
	m_pen[3].CreatePen(PS_DOT, 1, RGB(0, 0, 255));       //蓝色点线，必须为一个像素宽
	m_pen[4].CreatePen(PS_DASHDOTDOT, 1, RGB(255, 0, 0));//红色双点虚线，必须为一个像素宽

	//绘制多边形的点数组
	m_point[0].x = 10;
	m_point[0].y = 100;
	m_point[1].x = 10;
	m_point[1].y = 120;
	m_point[2].x = 100;
	m_point[2].y = 105;
	m_point[3].x = 170;
	m_point[3].y = 120;
	m_point[4].x = 170;
	m_point[4].y = 100;

    /*AllocConsole();
    freopen("CON", "r", stdin);
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);*/

    int b = 123;
    wchar_t a[MAX_PATH] = { 0 };
    wsprintf(a, L"%d######################################\n", b);
    OutputDebugString(a);

    m_Direction.m_x = 1.0f;
    m_Direction.m_y = 0.0f;

    g_unitAngle.getPoints(g_unitNum);
}

CMFCApplication1View::~CMFCApplication1View()
{
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

void CMFCApplication1View::drawLine(CDC* pDC, CLine& line)
{
	if (!pDC)
		return;
    DrawEllipse(pDC, CPoint(line.m_begin.m_x, line.m_begin.m_y));
    DrawEllipse(pDC, CPoint(line.m_end.m_x ,line.m_end.m_y));
	pDC->MoveTo(CPoint(line.m_begin.m_x,line.m_begin.m_y));
	pDC->LineTo(CPoint(line.m_end.m_x, line.m_end.m_y));
}

//void CMFCApplication1View::drawLines(CDC* pDC, std::vector<CLine>& lines)
//{
//	if (!pDC)
//		return;
//	for (auto& line : lines)
//	{
//        drawLine(pDC, line);
//	}
//}

void CMFCApplication1View::drawLines(CDC* pDC, std::vector<CLine>& lines)
{
    if (!pDC)
        return;
    pDC->MoveTo(CPoint(lines[0].m_begin.m_x, lines[0].m_begin.m_y));
    for (auto& line : lines)
    {
        DrawEllipse(pDC, CPoint(line.m_begin.m_x, line.m_begin.m_y));
        pDC->LineTo(CPoint(line.m_begin.m_x, line.m_begin.m_y));
        pDC->MoveTo(CPoint(line.m_begin.m_x, line.m_begin.m_y));
    }
    
}


CLines g_headLines;
std::vector<CPoint> g_vPoint;
// CMFCApplication1View 绘图 

void CMFCApplication1View::OnDraw(CDC* pDC)
{
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    if (!pDoc) {
        std::cout << "err" << std::endl;
        return;
    }
 
	
    drawLines(pDC, g_Lines.m_lines);
    DrawArc(pDC);
     
    //CLine line0 = g_Lines.moveToIndex(m_ToIndex);
    CLine line0 = g_Lines.moveToIndexNew(m_ToIndex);
    //CLine line0 = g_Lines.moveAngle();

	g_vPoint.push_back(CPoint(line0.m_begin.m_x,line0.m_begin.m_y));
	drawPoits(pDC, g_vPoint);


	/*for (auto& line:g_Lines.m_lines)
	{
		g_vPoint.push_back(line.m_begin);
		drawPoits(pDC, g_vPoint);
	}*/

    
    //DrawAngleArc(pDC);
    std::cout << "--" << std::endl;

    /*wchar_t a[MAX_PATH] = { 0 };
    wsprintf(a, L"x:%d y:%d \n", line0.m_begin.x, line0.m_begin.y);
    OutputDebugString(a);*/

    Sleep(100);
    pDoc->UpdateAllViews(this);
    Invalidate();
    
}


// CMFCApplication1View 打印

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCApplication1View 诊断

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication1View 消息处理程序

//绘制直线函数
void CMFCApplication1View::DrawLine(CDC* pDC)
{
    CPen* oldPen = pDC->SelectObject(&m_pen[0]);//保存DC原始画笔
    for (int i = 0; i < 5; i++)
    {
        pDC->SelectObject(&m_pen[i]);        //将画笔选入DC
        pDC->MoveTo(20, 20 + i * 15);         //设置DC当前点
        pDC->LineTo(170, 20 + i * 15);            //绘制直线
    }
    pDC->SelectObject(oldPen);       //回复DC原画笔
}


void CMFCApplication1View::drawPoits(CDC* pDC, std::vector<CPoint> points)
{
    if (!pDC)
        return;
    if (points.size()==0)
    {
        return;
    }
    pDC->MoveTo(points[0].x,points[0].y);         //设置DC当前点
    for (int i=1;i<points.size();++i)
    {
        pDC->LineTo(points[i].x,points[i].y);            //绘制直线
    }
}

//绘制折线
void CMFCApplication1View::DrawPolyline(CDC* pDC)
{
    CPen* oldPen = pDC->SelectObject(&m_pen[0]);//保存DC原始画笔
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            m_point[j].y += 35;       //每次下移35个像素
        }
        pDC->SelectObject(&m_pen[i]);
        pDC->Polyline(m_point, 5);
    }
    pDC->SelectObject(oldPen);
}

//绘制多边形
void CMFCApplication1View::DrawPolygon(CDC* pDC)
{
    for (int j = 0; j < 5; j++)
        m_point[j].y += 75;   //数组中的点下移75个像素
    CPen* oldPen = pDC->SelectObject(&m_pen[0]);//保存DC原始画笔
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            m_point[j].y += 35;   //每次绘制图形下移35个像素
        }
        pDC->SelectObject(&m_pen[i]);
        pDC->Polygon(m_point, 5); //绘制多边形
    }
    pDC->SelectObject(oldPen);
}

//绘制矩形
void CMFCApplication1View::DrawRect(CDC* pDC)
{
    CRect rect(150, 20, 220, 80);
    CPen* oldPen = pDC->SelectObject(&m_pen[0]);
    for (int i = 0; i < 5; i++)
    {
        rect.OffsetRect(80, 0);//每次绘图右移80个像素
        pDC->SelectObject(&m_pen[i]);
        pDC->Rectangle(&rect);//绘制矩形
    }
    pDC->SelectObject(oldPen);
}

//绘制圆角矩形
void CMFCApplication1View::DrawRoundRect(CDC* pDC)
{
    CRect rect(150, 150, 220, 230);
    CPen* oldPen = pDC->SelectObject(&m_pen[0]);
    for (int i = 0; i < 5; i++)
    {
        rect.OffsetRect(80, 0);//每次绘图右移80个像素
        pDC->SelectObject(&m_pen[i]);
        pDC->RoundRect(&rect, CPoint(15, 15));
    }
    pDC->SelectObject(oldPen);
}

//绘制椭圆
void CMFCApplication1View::DrawEllipse(CDC* pDC, CPoint mid)
{
    int radius = 5;
    CRect rect(mid.x- radius, mid.y- radius, mid.x+ radius, mid.y+ radius);
    CPen* oldPen = pDC->SelectObject(&m_pen[0]);
    
        rect.OffsetRect(8, 0);
        pDC->SelectObject(&m_pen[1]);
        pDC->Ellipse(&rect);
     
    pDC->SelectObject(oldPen);
}

//绘制弧线
void CMFCApplication1View::DrawArc(CDC* pDC)
{
    CRect rect(100, 100, 220, 220);
    CPoint ptStart(170, 440);
    CPoint ptEnd(210, 400);
    CPen* oldPen = pDC->SelectObject(&m_pen[0]);
    for (int i = 0; i < 5; i++)
    {
        rect.OffsetRect(180, 0);
        ptStart += CPoint(80, 0);
        ptEnd += CPoint(80, 0);
        pDC->SelectObject(&m_pen[i]);
        pDC->Arc(&rect, ptStart, ptEnd);
    }
    pDC->SelectObject(oldPen);
}

//绘制直线与弧线
void CMFCApplication1View::DrawAngleArc(CDC* pDC)
{
    CRect rect(150, 500, 220, 560);
    CPoint ptCenter(185, 600);
    CPoint ptStart(150, 630);
    int r = 30;
    float angleStart = 0;
    float angleEnd = 180;
    CPen* oldPen = pDC->SelectObject(&m_pen[0]);
    for (int i = 0; i < 5; i++)
    {
        rect.OffsetRect(80, 0);
        ptStart += CPoint(80, 0);
        pDC->MoveTo(ptStart);
        pDC->SelectObject(&m_pen[i]);
        pDC->AngleArc(ptCenter.x, ptCenter.y, r, angleStart, angleEnd);
    }
    pDC->SelectObject(oldPen);
}

//无效
void CMFCApplication1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default
    if (nChar == VK_SHIFT)//判断Shift键是否被按下
    {
        //AfxMessageBox("dd");
        /*bShiftdown = true;
        bShiftup = false;*/
        Invalidate(true);//显示信息
    }
    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMFCApplication1View::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CView::OnKeyUp(nChar, nRepCnt, nFlags);
	if (nChar == 83)//w
	{
		//MessageBox(L"ww");
		m_Direction.m_x = 0.0f;
		m_Direction.m_y = 1.0f;
        m_ToIndex = g_unitNum / 4 * 3;
	}
	else if(nChar == 65)//a
	{
		m_Direction.m_x = -1.0f;
		m_Direction.m_y = 0.0f;
        m_ToIndex = g_unitNum / 4 * 2;
	}
	else if (nChar == 87)//s
	{
		m_Direction.m_x = 0.0f;
		m_Direction.m_y = -1.0f;
        m_ToIndex = g_unitNum / 4 * 1;
	}
	else if (nChar == 68)//d
	{
		m_Direction.m_x = 1.0f;
		m_Direction.m_y = 0.0f;
        m_ToIndex = 0;
	}
    
        /*CLine line;
        line.m_curIndex = 2;
        line.toDirection(10);
        line.toDirection(30);
        line.m_curIndex = 30;
        line.toDirection(20);
        line.toDirection(2);*/
   
}


BOOL CMFCApplication1View::PreTranslateMessage(MSG* pMsg)
{
	//https://www.cnblogs.com/lujin49/archive/2015/10/29/4919900.html
	/*SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	return 0;*/
    // TODO: 在此添加专用代码和/或调用基类
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_RETURN)//回车键
        {
            //OnBnClickedButtonInput();
            return TRUE;
        }
        if (pMsg->wParam == VK_SPACE)//空格键
        {
            //OnBnClickedButtonStartStop();
        }


    }
    return CView::PreTranslateMessage(pMsg);
}


void CMFCApplication1View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CView::OnChar(nChar, nRepCnt, nFlags);
	if (nChar =='w')
	{
		MessageBox(L"w");
	}
}
