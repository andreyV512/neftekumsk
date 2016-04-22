#pragma once
#pragma warning( disable : 4146 )
#import "c:\program files\common files\system\ado\msado15.dll" rename("EOF", "EndOfFile")
#import "c:\program files\common files\system\ado\msadox.dll" 

class MSBase
{	
public:
	ADODB::_ConnectionPtr  conn;
	MSBase(const wchar_t *path)
	{
		_bstr_t strcnn("File Name=");
		strcnn += path;
		try
		{
		  conn.CreateInstance(__uuidof(ADODB::Connection)); 
		  conn->Open( strcnn, L"", L"", NULL);
		}
		catch(...){}
	}

	~MSBase()
	{
		if(IsOpen())  conn->Close();
	}

	void Close()
	{
        if(IsOpen())  conn->Close();
	}

	bool IsOpen()
	{
		try
		{
			return conn->State == ADODB::adStateOpen;
		}
		catch(...)
		{
			return false;
		}
	}

	bool RecordSetOpen(wchar_t *strsql, ADODB::_RecordsetPtr &rst)
	{
		try
		{
			rst.CreateInstance(__uuidof(ADODB::Recordset));
			rst->Open(_bstr_t(strsql), conn.GetInterfacePtr(),
				ADODB::adOpenStatic, ADODB::adLockOptimistic, ADODB::adCmdText);
			return true;
		}
		catch(...)
		{
			return false;
		}
	}

	ADODB::_RecordsetPtr ConnectionSQL(wchar_t *strsql)
	{
		 return conn->Execute(strsql, NULL, ADODB::adExecuteNoRecords);
	}
	bool OpenTable(const wchar_t *table, ADODB::_RecordsetPtr &rec)
	{
		try
		{
			rec.CreateInstance(__uuidof(ADODB::Recordset));
			rec->Open(_variant_t(table), conn.GetInterfacePtr()
				, ADODB::adOpenStatic
				, ADODB::adLockOptimistic,ADODB::adCmdTable);
			return true;
		}
		catch(...)
		{
			return false;
		}
	}
};