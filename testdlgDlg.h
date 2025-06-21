
#pragma once
#include "vector"
#include <gdiplus.h>
using namespace Gdiplus;

class CAdImageWnd : public CWnd
{
public:
  CAdImageWnd();
  virtual ~CAdImageWnd();

  BOOL CreateAdWindow(CWnd* pParent, CSize szWindow, LPCTSTR imagePath);
  void SetImage(LPCTSTR imagePath);  // 切换图片（JPG 或 GIF）
protected:
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnTimer(UINT_PTR nIDEvent);

  DECLARE_MESSAGE_MAP()

private:
  void LoadImage(LPCTSTR path);
  void UpdateLayeredFrame();
  void RoundRegion(int width, int height, int radius);

  Image* m_pImage;
  UINT m_nFrameCount;
  UINT m_nCurrentFrame;
  std::vector<UINT> m_frameDelays;
  GUID m_guidFrameDimension;

  CPoint m_ptMouseDown;
  bool m_bLButtonDown;
  bool m_bDragging;

  CRect m_rcCloseBtn;
  bool m_bHoverClose;

  CRgn m_rgnRoundCorner;
};

class CtestdlgDlg : public CDialogEx
{
  // 构造
public:
  CtestdlgDlg(CWnd* pParent = nullptr);	// 标准构造函数

  // 对话框数据
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_TESTDLG_DIALOG };
#endif

protected:
  virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

  // 实现
protected:
  HICON m_hIcon;
  CAdImageWnd* pAdImageWnd;
  // 生成的消息映射函数
  virtual BOOL OnInitDialog();
  afx_msg void OnPaint();
  afx_msg HCURSOR OnQueryDragIcon();
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton2();
};

