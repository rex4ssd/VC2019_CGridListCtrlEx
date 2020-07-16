#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct CListCtrl_DataRecord
{
	CListCtrl_DataRecord()
	{}

	CListCtrl_DataRecord(const CString& city, const CString& country )
		:m_City(city)
		,m_Country(country)
	{}

	CString	m_City;
	CString	m_Country;

	CString GetCellText_Struct(int col, bool title) const
	{
		switch(col)
		{
		case 0: { static const CString title0(_T("Item")); return title ? title0 : m_Country; }
		case 1: { static const CString title1(_T("Context")); return title ? title1 : m_City; }
		default:{ static const CString emptyStr; return emptyStr; }
		}
	}

	int  GetColCount() const { return 2; } //set column number, Rex
};

class CListCtrl_DataModel
{
	vector<CListCtrl_DataRecord> m_Records;
	int	m_LookupTime;
	int m_RowMultiplier;

public:
	CListCtrl_DataModel()
		:m_RowMultiplier(0)
		,m_LookupTime(0)
	{
		InitDataModel();
	}

	void InitDataModel()
	{
		m_Records.clear();
		m_Records.push_back( CListCtrl_DataRecord(_T("Copenhagen................"), _T("Denmark................")));
		m_Records.push_back( CListCtrl_DataRecord(_T("Berlin"), _T("Germany")));
		m_Records.push_back( CListCtrl_DataRecord(_T("Paris"), _T("France")));
		m_Records.push_back( CListCtrl_DataRecord(_T("Athen"), _T("Greece")));
		m_Records.push_back( CListCtrl_DataRecord(_T("Stockholm"), _T("Sweden")) );
		m_Records.push_back( CListCtrl_DataRecord(_T("Madrid"), _T("Spain")) );

		if (m_RowMultiplier > 0)
		{
			vector<CListCtrl_DataRecord> rowset(m_Records);
			m_Records.reserve((m_RowMultiplier-1) * rowset.size());
			for(int i = 0 ; i < m_RowMultiplier ; ++i)
			{
				m_Records.insert(m_Records.end(), rowset.begin(), rowset.end());
			}
		}
	}

	void ShowAllDataModel()
	{
		CString s0, s1, s2;
		
		for (int i = 0; i < m_Records.size(); i++) {
			s0 = m_Records.at(i).GetCellText_Struct(0, false);
			s1 = m_Records.at(i).GetCellText_Struct(1, false);
			s2 = m_Records.at(i).GetCellText_Struct(2, false);
			//m_Records.at(i).m_City
			TRACE( L"%d = %s, %s, %s\n", i, s0, s1, s2
				);
		}
	}

	CString GetCellText(size_t lookupId, int col) const
	{
		if (lookupId >= m_Records.size())
		{
			static CString oob(_T("Out of Bound"));
			return oob;
		}
		// How many times should we search sequential for the row ?
		for(int i=0; i < m_LookupTime; ++i)
		{
			for(size_t rowId = 0; rowId < m_Records.size(); ++rowId)
			{
				if (rowId==lookupId)
					break;
			}
		}
		return m_Records.at(lookupId).GetCellText_Struct(col, false);
	}

	vector<CString> GetCountries() const
	{
		vector<CString> countries;
		for(size_t rowId = 0 ; rowId < m_Records.size(); ++rowId)
			countries.push_back( m_Records[rowId].m_Country );
		sort(countries.begin(), countries.end());
		countries.erase(unique(countries.begin(), countries.end()), countries.end());
		return countries;
	}

	size_t GetRowIds() const { return m_Records.size(); }
	int GetColCount() const { return CListCtrl_DataRecord().GetColCount(); }
	CString GetColTitle(int col) const { return CListCtrl_DataRecord().GetCellText_Struct(col, true); }

	vector<CListCtrl_DataRecord>& GetRecords() { return m_Records; }
	void SetLookupTime(int lookupTimes) { m_LookupTime = lookupTimes; }
	void SetRowMultiplier(int multiply) { if (m_RowMultiplier != multiply ) { m_RowMultiplier = multiply; InitDataModel(); } }
};