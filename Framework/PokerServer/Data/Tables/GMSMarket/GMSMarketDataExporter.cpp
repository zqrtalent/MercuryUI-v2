#include "stdafx.h"
#include "GMSMarketDataExporter.h"
#include "..\..\..\..\Data\DataManager.h"
#include "AxsysProduct.h"

#include "..\..\..\..\Libxl\libxl.h"
using namespace libxl;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

GMSMarketDataExporter::GMSMarketDataExporter(){
	}

BEGIN_DECLARE_VAR(GMSMarketDataExporter, Serializable)
	DECLARE_VAR(_T("GMSMarketDataExporter"), _T(""),		VariableType::None, 0, false, true, false)
END_DECLARE_VAR()

GMSMarketDataExporter::~GMSMarketDataExporter(){
	}

void
GMSMarketDataExporter::ExportInExcell(){
	CString sConnStrinFb, sFbBaseFile = _T("D:\\GMSMARKET1.FDB");
	sConnStrinFb.Format("Driver={Firebird/InterBase(r) driver};Dbname=%s;CHARSET=NONE;PWD=masterkey;UID=SYSDBA;", sFbBaseFile);
	DataManager* pFbDataMan = new DataManager(sConnStrinFb);
	pFbDataMan->Open();

	Book* book = xlCreateBook();  
	if( !book ){
		delete pFbDataMan;
		return;
		}
	book->setKey("Vakhtang Tsvariani", "windows-2222200b03ceeb0267b3616ca6d6c7j5");
	
	Sheet* sheet = book->addSheet("Sheet1");
	if( !sheet ){
		delete pFbDataMan;
		return;
		}

	/*
	EnumerableObject<AxsysProduct>* pListProducts = new EnumerableObject<AxsysProduct>();
	pFbDataMan->LoadTable((EnumerableObject<Serializable>*)pListProducts,_T("where UID>0"), _T(""));

	for(int i=0; i<pListProducts->GetCount(); i++){
		AxsysProduct* pItem = pListProducts->GetAt(i);
		if( !pItem ) continue;

		sheet->writeStr(i, 0, pItem->m_sProductName); 
		sheet->writeStr(i, 1, pItem->m_sProductCode); 
		sheet->writeNum(i, 2, pItem->m_dOutputPrice); 
		}

	CString sXlsFile = _T("d:\\AxsysProducts.xls");
	book->save(sXlsFile);
	book->release();
	delete pListProducts;
	*/

	/* Output
	EnumerableObject<AxsysProductOutput>* pListOutputProducts = new EnumerableObject<AxsysProductOutput>();

	COleDateTime dtStart;
	dtStart.SetDate(2010, 1, 1);
	COleDateTime dtFinish;
	dtFinish.SetDate(2013, 7, 1);

	CString sWhereClause;
	sWhereClause.Format(
_T("inner join AXSYS_OUTPUT OUT on OUT.UID = PRO.OUTPUT_UID \
inner join AXSYS_PRODUCT PRD on PRD.UID = PRO.PRODUCT_UID \
WHERE PRO.OUTPUT_UID > 0 AND \
PRD.PRODUCT_NAME like '%%%%' AND \
OUT.OUTPUT_DATE >= '%02d/%02d/%04d' AND \
OUT.OUTPUT_DATE <= '%02d/%02d/%04d'"),
		dtStart.GetMonth(), dtStart.GetDay(), dtStart.GetYear(),
		dtFinish.GetMonth(), dtFinish.GetDay(), dtFinish.GetYear());



	pFbDataMan->LoadTable((EnumerableObject<Serializable>*)pListOutputProducts,sWhereClause, _T("order by OUT.OUTPUT_DATE"), _T(""), false);

	sheet->writeStr(0, 0, _T("Product Name")); 
	sheet->writeStr(0, 1, _T("Product Size")); 
	sheet->writeStr(0, 2, _T("Product Color")); 
	sheet->writeStr(0, 3, _T("Product Code")); 
	sheet->writeStr(0, 4, _T("Date")); 
	sheet->writeStr(0, 5, _T("Payment")); 
	sheet->writeStr(0, 6, _T("Real Payment")); 
	sheet->writeStr(0, 7, _T("Quantity")); 
	sheet->writeStr(0, 8, _T("Native Price")); 

	for(int i=0; i<pListOutputProducts->GetCount(); i++){
		AxsysProductOutput* pItem = pListOutputProducts->GetAt(i);
		if( !pItem ) continue;

		sheet->writeStr(i+1, 0, pItem->m_sProductName); 

		int nFind = pItem->m_sProductName.ReverseFind(_T(' '));
		if( nFind != -1 ){
			CString sSize = pItem->m_sProductName.Mid(nFind, pItem->m_sProductName.GetLength() - nFind);
			sheet->writeStr(i+1, 1, sSize); 
			pItem->m_sProductName.SetAt(nFind, '_');

			int nFind1 = pItem->m_sProductName.ReverseFind(_T(' '));
			if( nFind1 != -1 ){
				CString sColor = pItem->m_sProductName.Mid(nFind1, nFind - nFind1);
				sheet->writeStr(i+1, 2, sColor); 

				pItem->m_sProductName = pItem->m_sProductName.Left(nFind1);
				sheet->writeStr(i+1, 0, pItem->m_sProductName); 
				}
			}
		else{
			sheet->writeStr(i+1, 1, _T("")); 
			sheet->writeStr(i+1, 2, _T("")); 
			}

		sheet->writeStr(i+1, 3, pItem->m_sProductCode); 

		CString sDate;
		sDate.Format(_T("%02d/%02d/%04d"), pItem->m_dtDate.GetDay(), pItem->m_dtDate.GetMonth(), pItem->m_dtDate.GetYear());
		sheet->writeStr(i+1, 4, sDate); 

		double dSalePercent = pItem->m_dRealPayment / pItem->m_dPayment;

		sheet->writeNum(i+1, 5, pItem->m_dOutputPrice*pItem->m_dOutputCount); 
		sheet->writeNum(i+1, 6, pItem->m_dOutputPrice*pItem->m_dOutputCount*(dSalePercent)); 
		sheet->writeNum(i+1, 7, pItem->m_dOutputCount); 
		sheet->writeNum(i+1, 8, pItem->m_dNativePrice*pItem->m_dOutputCount); 
		}

	CString sXlsFile = _T("d:\\AxsysOutputProducts.xls");
	book->save(sXlsFile);
	book->release();
	delete pListOutputProducts;
	*/
	/* Income
	EnumerableObject<AxsysProductIncome>* pListIncomeProducts = new EnumerableObject<AxsysProductIncome>();

	COleDateTime dtStart;
	dtStart.SetDate(2010, 1, 1);
	COleDateTime dtFinish;
	dtFinish.SetDate(2013, 7, 1);

	CString sWhereClause = _T(" inner join AXSYS_PRODUCT PRD on PRD.UID = PRO.PRODUCT_UID \
inner join AXSYS_STOCK ST on ST.UID = PRD.STOCK_UID \
where PRO.INCOME_UID = 0 and PRO.OUTPUT_UID = 0 and PRO.BACK_DISTR_UID = 0 and PRO.BACK_CLIENT_UID = 0 and PRO.BACK_DELIVERY_UID = 0 and PRO.BACK_DOWN_UID = 0 and PRO.PRODUCT_UID > 0 and PRO.NEW_PRODUCT_COUNT = 0");
	

	pFbDataMan->LoadTable((EnumerableObject<Serializable>*)pListIncomeProducts,sWhereClause, _T("order by PRO.TRANS_DATE"), _T(""), _T(""), false);

	sheet->writeStr(0, 0, _T("Product Name")); 
	sheet->writeStr(0, 1, _T("Product Size")); 
	sheet->writeStr(0, 2, _T("Product Color")); 
	sheet->writeStr(0, 3, _T("Product Code")); 
	sheet->writeStr(0, 4, _T("Date")); 
	sheet->writeStr(0, 5, _T("Full Price")); 
	sheet->writeStr(0, 6, _T("Output Price")); 
	sheet->writeStr(0, 7, _T("Quantity")); 
	sheet->writeStr(0, 8, _T("Native Price")); 

	for(int i=0; i<pListIncomeProducts->GetCount(); i++){
		AxsysProductIncome* pItem = pListIncomeProducts->GetAt(i);
		if( !pItem ) continue;
		
		sheet->writeStr(i+1, 0, pItem->m_sProductName); 

		int nFind = pItem->m_sProductName.ReverseFind(_T(' '));
		if( nFind != -1 ){
			CString sSize = pItem->m_sProductName.Mid(nFind, pItem->m_sProductName.GetLength() - nFind);
			sheet->writeStr(i+1, 1, sSize); 
			pItem->m_sProductName.SetAt(nFind, '_');

			int nFind1 = pItem->m_sProductName.ReverseFind(_T(' '));
			if( nFind1 != -1 ){
				CString sColor = pItem->m_sProductName.Mid(nFind1, nFind - nFind1);
				sheet->writeStr(i+1, 2, sColor); 

				pItem->m_sProductName = pItem->m_sProductName.Left(nFind1);
				sheet->writeStr(i+1, 0, pItem->m_sProductName); 
				}
			}
		else{
			sheet->writeStr(i+1, 1, _T("")); 
			sheet->writeStr(i+1, 2, _T("")); 
			}

		sheet->writeStr(i+1, 3, pItem->m_sProductCode); 

		CString sDate;
		sDate.Format(_T("%02d/%02d/%04d"), pItem->m_dtDate.GetDay(), pItem->m_dtDate.GetMonth(), pItem->m_dtDate.GetYear());
		sheet->writeStr(i+1, 4, sDate); 

		sheet->writeNum(i+1, 5, pItem->m_dFullPrice); 
		sheet->writeNum(i+1, 6, pItem->m_dOutputPrice); 
		sheet->writeNum(i+1, 7, pItem->m_dProductCount); 
		sheet->writeNum(i+1, 8, pItem->m_dNativePrice*pItem->m_dProductCount); 
		}

	CString sXlsFile = _T("d:\\AxsysIncomeProducts.xls");
	book->save(sXlsFile);
	book->release();
	delete pListIncomeProducts;
	*/


	EnumerableObject<AxsysProductRemain>* pListRemain = new EnumerableObject<AxsysProductRemain>();

	CString sSQL = _T("select PRD.UID, \
	PRD.PRODUCT_NAME, \
	PRD.OUTPUT_PRICE,\
	(SELECT  FIRST 1 PRODUCT_COUNT FROM AXSYS_INCOME_PRODUCT WHERE TRANS_DATE <= '1/1/2013'  AND PRD.UID = PRODUCT_UID ORDER BY UID DESC) AS PRODUCT_COUNT, \
	(SELECT  FIRST 1 NATIVE_PRICE FROM AXSYS_INCOME_PRODUCT WHERE TRANS_DATE <= '1/1/2013'  AND PRD.UID = PRODUCT_UID ORDER BY UID DESC) AS NATIVE_PRICE, \
	(SELECT  FIRST 1 FULL_PRICE FROM AXSYS_INCOME_PRODUCT WHERE TRANS_DATE <= '1/1/2013'  AND PRD.UID = PRODUCT_UID ORDER BY UID DESC) AS FULL_PRICE\
	FROM AXSYS_PRODUCT PRD \
	WHERE PRD.PRODUCT_NAME like '%%' AND PRD.UID > 0 ");
	pFbDataMan->LoadTableBySql((EnumerableObject<Serializable>*)pListRemain, sSQL, true);

	sheet->writeStr(0, 0, _T("Product Name")); 
	sheet->writeStr(0, 1, _T("Output Price")); 
	sheet->writeStr(0, 2, _T("Product Quantity")); 
	sheet->writeStr(0, 3, _T("Full Price")); 
	sheet->writeStr(0, 4, _T("Native Price")); 

	for(int i=0; i<pListRemain->GetCount(); i++){
		AxsysProductRemain* pItem = pListRemain->GetAt(i);
		if( !pItem ) continue;
		
		sheet->writeStr(i+1, 0, pItem->m_sProductName); 

		sheet->writeNum(i+1, 1, pItem->m_dOutputPrice); 
		sheet->writeNum(i+1, 2, pItem->m_dProductCount); 
		sheet->writeNum(i+1, 3, pItem->m_dFullPrice); 
		sheet->writeNum(i+1, 4, pItem->m_dNativePrice); 
		}

	CString sXlsFile = _T("d:\\AxsysNashti.xls");
	book->save(sXlsFile);
	book->release();
	delete pListRemain;
	delete pFbDataMan;

	/* Output

	inner join AXSYS_OUTPUT OUT on OUT.UID = PRO.OUTPUT_UID 
	inner join AXSYS_PRODUCT PRD on PRD.UID = PRO.PRODUCT_UID 
	WHERE PRO.OUTPUT_UID > 0 AND 
	PRD.PRODUCT_NAME like '%[prd]%' AND 
	OUT.OUTPUT_DATE >= '[indat]' AND 
	OUT.OUTPUT_DATE <= '[mordat]' ;

	SELECT 
	PRO.NEW_PRODUCT_COUNT, 
	PRO.NEW_FULL_PRICE, 
	PRD.PRODUCT_NAME, 
	OUT.ORDER_N, 
	OUT.OUTPUT_DATE AS ACT_DATE, 
	PRD.PRODUCT_NAME, 
	CNT.NAME 
	FROM AXSYS_INCOME_PRODUCT PRO 
	inner join AXSYS_OUTPUT OUT on OUT.UID = PRO.OUTPUT_UID 
	inner join AXSYS_CONTACT CNT on CNT.UID = OUT.CLIENT_UID  
	inner join AXSYS_PRODUCT PRD on PRD.UID = PRO.PRODUCT_UID 
	WHERE PRO.OUTPUT_UID > 0 AND 
	PRD.PRODUCT_NAME like '%[prd]%' AND 
	OUT.OUTPUT_DATE >= '[indat]' AND 
	OUT.OUTPUT_DATE <= '[mordat]' ;
	*/
	/*
	SELECT 
	PRD.UID, 
	PRD.PRODUCT_NAME, 
	(SELECT  FIRST 1 PRODUCT_COUNT FROM AXSYS_INCOME_PRODUCT WHERE TRANS_DATE <= '[indat]'  AND PRD.UID = PRODUCT_UID ORDER BY UID DESC) AS OLD_COUNT, 
	(SELECT  FIRST 1 NATIVE_PRICE FROM AXSYS_INCOME_PRODUCT WHERE TRANS_DATE <= '[indat]'  AND PRD.UID = PRODUCT_UID ORDER BY UID DESC) AS OLD_NATIVE, 
	(SELECT  FIRST 1 FULL_PRICE FROM AXSYS_INCOME_PRODUCT WHERE TRANS_DATE <= '[indat]'  AND PRD.UID = PRODUCT_UID ORDER BY UID DESC) AS OLD_PRICE, 
	( SELECT SUM(NEW_PRODUCT_COUNT) FROM AXSYS_INCOME_PRODUCT WHERE INCOME_UID > 0 AND TRANS_DATE >= '[indat]' AND TRANS_DATE <= '[mordat]'  AND PRD.UID = PRODUCT_UID)AS INCOME_COUNT,
	( SELECT SUM(NEW_PRODUCT_COUNT) FROM AXSYS_INCOME_PRODUCT WHERE OUTPUT_UID > 0 AND TRANS_DATE >= '[indat]' AND TRANS_DATE <= '[mordat]'  AND PRD.UID = PRODUCT_UID)AS OUTPUT_COUNT,
	( SELECT SUM(NEW_PRODUCT_COUNT) FROM AXSYS_INCOME_PRODUCT WHERE BACK_CLIENT_UID > 0 AND TRANS_DATE >= '[indat]' AND TRANS_DATE <= '[mordat]'  AND PRD.UID = PRODUCT_UID) AS BACK_CLIENT_COUNT, 
	( SELECT SUM(NEW_PRODUCT_COUNT) FROM AXSYS_INCOME_PRODUCT WHERE BACK_DELIVERY_UID > 0 AND TRANS_DATE >= '[indat]' AND TRANS_DATE <= '[mordat]'  AND PRD.UID = PRODUCT_UID) AS BACK_DELIVERY_COUNT,
	( SELECT SUM(NEW_PRODUCT_COUNT) FROM AXSYS_INCOME_PRODUCT WHERE BACK_DOWN_UID > 0 AND TRANS_DATE >= '[indat]' AND TRANS_DATE <= '[mordat]'  AND PRD.UID = PRODUCT_UID) AS BACK_DOWN_COUNT,
	(SELECT  FIRST 1 PRODUCT_COUNT FROM AXSYS_INCOME_PRODUCT WHERE TRANS_DATE <= '[mordat]' AND PRD.UID = PRODUCT_UID  ORDER BY UID DESC) AS NEW_PRODUCT_COUNT, 
	(SELECT  FIRST 1 NATIVE_PRICE FROM AXSYS_INCOME_PRODUCT WHERE TRANS_DATE <= '[mordat]' AND PRD.UID = PRODUCT_UID  ORDER BY UID DESC) AS NEW_NATIVE, 
	(SELECT  FIRST 1 FULL_PRICE FROM AXSYS_INCOME_PRODUCT WHERE TRANS_DATE <= '[mordat]' AND PRD.UID = PRODUCT_UID  ORDER BY UID DESC) AS NEW_FULL_PRICE  
	FROM AXSYS_PRODUCT PRD 
	WHERE PRD.PRODUCT_NAME like '%[prd]%' AND PRD.UID > 0 
	*/


	/* Income
select ST.NAME, PRD.PRODUCT_NAME, PRD.PRODUCT_CODE, PRD.OUTPUT_PRICE, PRO.PRODUCT_COUNT, PRO.FULL_PRICE, PRO.NATIVE_PRICE, PRO.TRANS_DATE
from AXSYS_INCOME_PRODUCT PRO
inner join AXSYS_PRODUCT PRD on PRD.UID = PRO.PRODUCT_UID
inner join AXSYS_STOCK ST on ST.UID = PRD.STOCK_UID
where PRO.INCOME_UID = 0 and PRO.OUTPUT_UID = 0 and PRO.BACK_DISTR_UID = 0 and PRO.BACK_CLIENT_UID = 0 and PRO.BACK_DELIVERY_UID = 0 and PRO.BACK_DOWN_UID = 0 and PRO.PRODUCT_UID > 0 and PRO.NEW_PRODUCT_COUNT = 0
	*/

	/* Nashti
	 select PRD.UID, 
	PRD.PRODUCT_NAME, 
	PRD.OUTPUT_PRICE,
	(SELECT  FIRST 1 PRODUCT_COUNT FROM AXSYS_INCOME_PRODUCT WHERE TRANS_DATE <= '1/1/2013'  AND PRD.UID = PRODUCT_UID ORDER BY UID DESC) AS PRODUCT_COUNT, 
	(SELECT  FIRST 1 NATIVE_PRICE FROM AXSYS_INCOME_PRODUCT WHERE TRANS_DATE <= '1/1/2013'  AND PRD.UID = PRODUCT_UID ORDER BY UID DESC) AS NATIVE_PRICE, 
	(SELECT  FIRST 1 FULL_PRICE FROM AXSYS_INCOME_PRODUCT WHERE TRANS_DATE <= '1/1/2013'  AND PRD.UID = PRODUCT_UID ORDER BY UID DESC) AS FULL_PRICE
	FROM AXSYS_PRODUCT PRD
	WHERE PRD.PRODUCT_NAME like '%%' AND PRD.UID > 0 
	*/
	}