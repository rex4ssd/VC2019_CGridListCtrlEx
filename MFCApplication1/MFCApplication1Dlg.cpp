
// MFCApplication1Dlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#include "..\CGridListCtrlEx\CGridColumnTraitDateTime.h"
#include "..\CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "..\CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "..\CGridListCtrlEx\CGridColumnTraitHyperLink.h"
#include "..\CGridListCtrlEx\CGridRowTraitXP.h"
#include "..\CGridListCtrlEx\ViewConfigSection.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg dialog



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CMFCApplication1Dlg::OnLvnItemchangedList2)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMFCApplication1Dlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMFCApplication1Dlg message handlers

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	// Give better margin to editors
	m_ListCtrl.SetCellMargin(1.2);
	CGridRowTraitXP* pRowTrait = new CGridRowTraitXP;
	m_ListCtrl.SetDefaultRowTrait(pRowTrait);
	m_ListCtrl.EnableVisualStyles(true);
	
	// Create Columns
	m_ListCtrl.InsertHiddenLabelColumn();	// Requires one never uses column 0


	TRACE("GetColCount = %d\n", m_DataModel.GetColCount());
	for (int col = 0; col < m_DataModel.GetColCount(); ++col)
	{
		const CString& title = m_DataModel.GetColTitle(col);
		TRACE(L"title = %s\n", m_DataModel.GetColTitle(col));
		CGridColumnTrait* pTrait = NULL;
		if (col == 0)	// Item
		{
			//pTrait = new CGridColumnTraitText;
			CGridColumnTraitCombo* pComboTrait = new CGridColumnTraitCombo;
			pComboTrait->SetShowDropDown(TRUE);//REX
			//pComboTrait->SetStyle(10);
			//pComboTrait->SetSingleClickEdit(TRUE);
			//pComboTrait->SetStyle(2);
			const vector<CString>& countries = m_DataModel.GetCountries();
			for (size_t i = 0; i < countries.size(); ++i) {
				pComboTrait->AddItem((DWORD_PTR)i, countries[i]);
				TRACE(L"i = %d, countries[i] = %s\n", i, countries[i]);
			}
			pTrait = pComboTrait;
		}
		if (col == 1)	// Context
		{
			pTrait = new CGridColumnTraitEdit;
			//CGridColumnTraitCombo* pComboTrait1 = new CGridColumnTraitCombo;
			//pTrait = pComboTrait1;
		}

		m_ListCtrl.InsertColumnTrait(col + 1, title, LVCFMT_LEFT, 500, col, pTrait);
		//m_ListCtrl.InsertColumnTrait()
		//m_ListCtrl.InsertItem()
	}

	// Insert data into list-control by copying from datamodel
	TRACE("GetRowIds = %d\n", m_DataModel.GetRowIds());

	int nItem = 0;
	for (size_t rowId = 0; rowId < m_DataModel.GetRowIds(); ++rowId)
	{
		TRACE(L"rowId= %d, InsertItem = %s\n", rowId, m_DataModel.GetCellText(rowId, 0));
		nItem = m_ListCtrl.InsertItem(++nItem, m_DataModel.GetCellText(rowId, 0));
		m_ListCtrl.SetItemData(nItem, rowId);
		for (int col = 0; col < m_DataModel.GetColCount(); ++col)
		{
			int nCellCol = col + 1;	// +1 because of hidden column
			const CString& strCellText = m_DataModel.GetCellText(rowId, col);
			m_ListCtrl.SetItemText(nItem, nCellCol, strCellText);
			TRACE(L"col = %d, nItem = %d, nCellCol = %d, strCellText = %s\n", col, nItem, nCellCol, strCellText);
		}
	}

	CViewConfigSectionWinApp* pColumnProfile = new CViewConfigSectionWinApp(_T("Sample List"));
	pColumnProfile->AddProfile(_T("Default"));
	pColumnProfile->AddProfile(_T("Special"));
	m_ListCtrl.SetupColumnConfig(pColumnProfile);
	

	TRACE("ShowAllDataModel go\n");
	m_DataModel.ShowAllDataModel();

	// CString s1, s2;
	// s1 = m_DataModel.GetColTitle(1);
	// s2 = m_DataModel.GetColTitle(2);
	// TRACE("GetColTitle go, %s, %s, %s\n", 
	// 	s1,
	// 	s2,
	// 	m_DataModel.GetColTitle(3));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CMFCApplication1Dlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CMFCApplication1Dlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();

	m_ListCtrl.OnSaveStateColumnPick();// OnSaveStateColumnPick();

	TRACE("OnBnClickedOk go\n");
	m_DataModel.ShowAllDataModel();
}
