
// testdlgDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "testdlg.h"
#include "testdlgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CtestdlgDlg::CtestdlgDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTDLG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestdlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestdlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_BUTTON1, &CtestdlgDlg::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, &CtestdlgDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CtestdlgDlg 消息处理程序
#include "CAdDisplayDlg.h"
BOOL CtestdlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

  pAdImageWnd = new CAdImageWnd();

  CString strr2(R"(C:\Users\LZ\Desktop\im.png)");
  CSize adSize(300, 300);
  //CRect rc;
  //GetClientRect(&rc);
 // CSize adSize(rc.Width(), rc.Height());
  pAdImageWnd->CreateAdWindow(this, adSize, strr2);
  pAdImageWnd->ShowWindow(SW_SHOW);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestdlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestdlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtestdlgDlg::OnBnClickedButton1()
{
  CString strr(R"(C:\Users\LZ\Desktop\test\1.异形窗体\b.gif异形窗体\customDlg\customDlg\res\loading.gif)");
  if (!pAdImageWnd->m_hWnd)
  {
    CSize adSize(300, 120);
    pAdImageWnd->CreateAdWindow(this, adSize, strr);
    pAdImageWnd->ShowWindow(SW_SHOW);
  }
  else
  {
    pAdImageWnd->SetImage(strr);
  }
}

void CtestdlgDlg::OnBnClickedButton2()
{

  CString strr(R"(C:\Users\LZ\Desktop\test\1.异形窗体\b.gif异形窗体\customDlg\customDlg\res\1111.png)");
  if (!pAdImageWnd->m_hWnd)
  {
    CSize adSize(1200, 240);
    pAdImageWnd->CreateAdWindow(this, adSize, strr);
    pAdImageWnd->ShowWindow(SW_SHOW);
  }
  else
  {
    pAdImageWnd->SetImage(strr);
  }
}








ULONG_PTR gdiplusToken;

#pragma comment(lib, "gdiplus.lib")

BEGIN_MESSAGE_MAP(CAdImageWnd, CWnd)
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_WM_TIMER()
END_MESSAGE_MAP()

CAdImageWnd::CAdImageWnd()
  : m_pImage(nullptr), m_nFrameCount(0), m_nCurrentFrame(0),
  m_bLButtonDown(false), m_bDragging(false), m_bHoverClose(false)
{
  GdiplusStartupInput gdiplusStartupInput;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

CAdImageWnd::~CAdImageWnd()
{
  if (m_pImage) delete m_pImage;
  Gdiplus::GdiplusShutdown(gdiplusToken);
}

void CAdImageWnd::RoundRegion(int width, int height, int radius)
{
  CRect rc;
  GetClientRect(&rc);
  if (m_rgnRoundCorner.GetSafeHandle() == NULL)
  {
    m_rgnRoundCorner.CreateRoundRectRgn(0, 0, width + 1, height + 1, radius, radius);
  }
  SetWindowRgn(m_rgnRoundCorner, TRUE);
}

BOOL CAdImageWnd::CreateAdWindow(CWnd* pParent, CSize szWindow, LPCTSTR imagePath)
{
  if (!pParent)
    return FALSE;

  CRect rcParent;
  pParent->GetWindowRect(&rcParent);

  // 计算右下角位置（向内偏移 20 像素）
  int x = rcParent.right - szWindow.cx - 20;
  int y = rcParent.bottom - szWindow.cy - 40;

  CString className = AfxRegisterWndClass(0);
  BOOL bCreated = CreateEx(
    WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST,  // 分层 + 不在任务栏显示
    className,
    _T(""),
    WS_POPUP,
    x, y, szWindow.cx, szWindow.cy,
    NULL, NULL
  );

  if (bCreated)
  {
    SetOwner(pParent);

    LoadImage(imagePath);
    UpdateLayeredFrame();

    if (m_nFrameCount > 1 && !m_frameDelays.empty())
    {
      SetTimer(1, m_frameDelays[0], NULL);
    }
  }

  RoundRegion(szWindow.cx, szWindow.cy, 5);

  return bCreated;
}

void CAdImageWnd::SetImage(LPCTSTR imagePath)
{
  // 清除旧图像
  if (m_pImage)
  {
    delete m_pImage;
    m_pImage = nullptr;
  }

  m_nCurrentFrame = 0;
  m_nFrameCount = 0;
  m_frameDelays.clear();

  KillTimer(1);  // 停止原来的 GIF 动画

  // 加载新图片
  m_pImage = Image::FromFile(imagePath);

  if (!m_pImage || m_pImage->GetLastStatus() != Ok)
  {
    AfxMessageBox(_T("加载新图片失败"));
    delete m_pImage;
    m_pImage = nullptr;
    return;
  }

  // 检查是否是动画 GIF
  m_nFrameCount = m_pImage->GetFrameCount(&FrameDimensionTime);
  m_guidFrameDimension = FrameDimensionTime;

  if (m_nFrameCount > 1)
  {
    UINT size = m_pImage->GetPropertyItemSize(PropertyTagFrameDelay);
    if (size)
    {
      PropertyItem* pItem = (PropertyItem*)malloc(size);
      if (pItem && m_pImage->GetPropertyItem(PropertyTagFrameDelay, size, pItem) == Ok)
      {
        UINT* pDelays = (UINT*)pItem->value;
        for (UINT i = 0; i < m_nFrameCount; ++i)
        {
          m_frameDelays.push_back(pDelays[i] * 10);
        }
      }
      free(pItem);
    }

    if (!m_frameDelays.empty())
    {
      SetTimer(1, m_frameDelays[0], NULL);
    }
  }

  // 立即刷新显示
  UpdateLayeredFrame();
}


void CAdImageWnd::LoadImage(LPCTSTR path)
{
  m_pImage = Gdiplus::Image::FromFile(path, FALSE);
 // m_pImage = Image::FromFile(path);
  m_nFrameCount = m_pImage->GetFrameCount(&FrameDimensionTime);
  m_guidFrameDimension = FrameDimensionTime;

  m_frameDelays.clear();
  if (m_nFrameCount > 1)
  {
    PropertyItem* pItem = nullptr;
    UINT size = m_pImage->GetPropertyItemSize(PropertyTagFrameDelay);
    if (size)
    {
      pItem = (PropertyItem*)malloc(size);
      m_pImage->GetPropertyItem(PropertyTagFrameDelay, size, pItem);
      UINT* pDelays = (UINT*)pItem->value;
      for (UINT i = 0; i < m_nFrameCount; ++i)
      {
        m_frameDelays.push_back(pDelays[i] * 10); // GIF delay 单位是 1/100s
      }
      free(pItem);
    }
  }
}

void CAdImageWnd::UpdateLayeredFrame()
{
  if (!m_pImage) return;

  CRect rect;
  GetClientRect(&rect);
  int width = rect.Width();
  int height = rect.Height();

  HDC hScreenDC = ::GetDC(NULL);
  HDC hMemDC = ::CreateCompatibleDC(hScreenDC);

  BITMAPINFO bmi = {};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = width;
  bmi.bmiHeader.biHeight = -height;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  void* pvBits = nullptr;
  HBITMAP hBitmap = ::CreateDIBSection(hScreenDC, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
  HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hMemDC, hBitmap);

  Graphics graphics(hMemDC);

  graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
  graphics.SetSmoothingMode(SmoothingModeHighQuality);
  graphics.SetPixelOffsetMode(PixelOffsetModeHighQuality);
  graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
  graphics.Clear(Color(0, 0, 0, 0));

  m_pImage->SelectActiveFrame(&m_guidFrameDimension, m_nCurrentFrame);
  graphics.DrawImage(m_pImage, Rect(0, 0, width, height));

  // 关闭按钮区域
  int btnSize = 30;
  m_rcCloseBtn = CRect(width - btnSize - 5, 5, width - 5, 5 + btnSize);

  SolidBrush btnBrush(m_bHoverClose ? Color(200, 255, 100, 100) : Color(150, 200, 200, 200));
  graphics.FillEllipse(&btnBrush, m_rcCloseBtn.left, m_rcCloseBtn.top, btnSize, btnSize);

  Pen pen(Color(255, 0, 0, 0), 2);
  graphics.DrawLine(&pen, m_rcCloseBtn.left + 8, m_rcCloseBtn.top + 8, m_rcCloseBtn.right - 8, m_rcCloseBtn.bottom - 8);
  graphics.DrawLine(&pen, m_rcCloseBtn.right - 8, m_rcCloseBtn.top + 8, m_rcCloseBtn.left + 8, m_rcCloseBtn.bottom - 8);

  POINT ptSrc = { 0, 0 };
  SIZE sizeWnd = { width, height };
  BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

  ::UpdateLayeredWindow(GetSafeHwnd(), hScreenDC, NULL, &sizeWnd, hMemDC, &ptSrc, 0, &blend, ULW_ALPHA);

  ::SelectObject(hMemDC, hOldBitmap);
  ::DeleteObject(hBitmap);
  ::DeleteDC(hMemDC);
  ::ReleaseDC(NULL, hScreenDC);
}

void CAdImageWnd::OnTimer(UINT_PTR nIDEvent)
{
  if (nIDEvent == 1 && m_nFrameCount > 1)
  {
    m_nCurrentFrame = (m_nCurrentFrame + 1) % m_nFrameCount;
    UpdateLayeredFrame();
    SetTimer(1, m_frameDelays[m_nCurrentFrame], NULL);
  }
}

void CAdImageWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
  m_bLButtonDown = true;
  m_bDragging = false;
  m_ptMouseDown = point;
  SetCapture();
}

void CAdImageWnd::OnMouseMove(UINT nFlags, CPoint point)
{
  if (m_bLButtonDown)
  {
    CPoint delta = point - m_ptMouseDown;
    if (!m_bDragging && (abs(delta.x) > 4 || abs(delta.y) > 4))
    {
      m_bDragging = true;
      ReleaseCapture();
      PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
    }
  }

  bool hover = m_rcCloseBtn.PtInRect(point);
  if (hover != m_bHoverClose)
  {
    m_bHoverClose = hover;
    UpdateLayeredFrame();
  }
}

void CAdImageWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
  if (m_bLButtonDown)
  {
    m_bLButtonDown = false;
    ReleaseCapture();

    if (!m_bDragging)
    {
      if (m_rcCloseBtn.PtInRect(point))
      {
        PostMessage(WM_CLOSE);
      }
      else
      {
        MessageBox(_T("1"), _T("提示"), MB_OK);
      }
    }
  }
}

