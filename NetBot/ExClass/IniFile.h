//////////////////////////////////////////////////////////////////////////
//IniFile.h
//ini�ļ���д��
//������Ȩ����
//2005��7��6��
//��������ʹ�á��޸ĺʹ��������뱣�����߰�Ȩ��Ϣ
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_INIFILE_H__B5C0D7F7_8353_4C93_AAA4_38A688CA253C__INCLUDED_)
#define AFX_INIFILE_H__B5C0D7F7_8353_4C93_AAA4_38A688CA253C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIniFile  
{
public:
         CIniFile();
         virtual ~CIniFile();
    
         BOOL SetPath(CString strPath);//����ini�ļ�·��,�ɹ�TRUE;����FALSE   

		 BOOL Createini();//����m_strPath�е�ini�ļ�

         BOOL SectionExist(CString strSection);//���section�Ƿ����,����TRUE;����FALSE,��section����keyҲ�������ڴ�������FALSE

         CString GetKeyValue(CString strSection,CString strKey);//��ָ����Section��Key��ȡKeyValue         
         
         void SetKeyValue(CString strSection,CString strKey,CString strKeyValue);//����/�޸�Section��Key�Լ�KeyValue����Section����Key�������򴴽�

         void DeleteKey(CString strSection,CString strKey);//ɾ��ָ��Section�µ�һ��Key

         void DeleteSection(CString strSection);//ɾ��ָ����Section�Լ����µ�����Key  

         int GetAllSections(CStringArray& strArrSection);//������е�Section,����Section��Ŀ

         
         int GetAllKeysAndValues (CString strSection,CStringArray&strArrKey,CStringArray&strArrKeyValue);//����ָ��Section�õ����µ�����Key��KeyValue,����Key����Ŀ
         
         void DeleteAllSections();//ɾ������Section

private:
         //ini�ļ�·��
         CString m_strPath;
};

#endif // !defined(AFX_INIFILE_H__B5C0D7F7_8353_4C93_AAA4_38A688CA253C__INCLUDED_)
//////////////////////////////////////////////////////////////////////////
//ʵ��CString���͵�int���͵�ת��,����CString str1="5";int i=atoi(str1);
//ʵ��CString���͵�double���͵�ת��,����CString str2="5";double j=atoi(str2);		
//////////////////////////////////////////////////////////////////////////