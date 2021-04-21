#include "stdafx.h"
#include "AxsysProduct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

AxsysProduct::AxsysProduct(){
	}

BEGIN_DECLARE_VAR(AxsysProduct, Serializable)
	DECLARE_VAR(_T("AxsysProduct"),			_T("AXSYS_PRODUCT"),		VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),					_T("UID"),					VariableType::Int32, offsetof(AxsysProduct,m_nId), false, false, false)
	DECLARE_VAR(_T("StockId"),				_T("STOCK_UID"),			VariableType::Int32, offsetof(AxsysProduct,m_nStockId), false, false, false)
	DECLARE_VAR(_T("ProductCode"),			_T("PRODUCT_CODE"),			VariableType::String, offsetof(AxsysProduct,m_sProductCode), false, false, false)
	DECLARE_VAR(_T("ProductName"),			_T("PRODUCT_NAME"),			VariableType::String, offsetof(AxsysProduct,m_sProductName), false, false, false)
	DECLARE_VAR(_T("ProductDescription"),	_T("PRODUCT_DESCRIPTION"),	VariableType::String, offsetof(AxsysProduct,m_sProductDescription), false, false, false)
	DECLARE_VAR(_T("UnitId"),				_T("UNIT_UID"),				VariableType::Int32, offsetof(AxsysProduct,m_nUnitId), false, false, false)
	DECLARE_VAR(_T("OutputPrice"),			_T("OUTPUT_PRICE"),			VariableType::Double, offsetof(AxsysProduct,m_dOutputPrice), false, false, false)
	DECLARE_VAR(_T("CriticalCt"),			_T("CRITICAL_COUNT"),		VariableType::Double, offsetof(AxsysProduct,m_dCriticalCt), false, false, false)

END_DECLARE_VAR()

AxsysProduct::~AxsysProduct(){
	}


AxsysProductOutput::AxsysProductOutput(){
	}

BEGIN_DECLARE_VAR(AxsysProductOutput, Serializable)
	DECLARE_VAR(_T("AxsysProductOutput"),		_T("AXSYS_INCOME_PRODUCT PRO"),		VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("PRODUCT_NAME"),				_T("PRD.PRODUCT_NAME"),				VariableType::String, offsetof(AxsysProductOutput,m_sProductName), false, false, false)
	DECLARE_VAR(_T("PRODUCT_CODE"),				_T("PRD.PRODUCT_CODE"),				VariableType::String, offsetof(AxsysProductOutput,m_sProductCode), false, false, false)
	DECLARE_VAR(_T("OUTPUT_DATE"),				_T("OUT.OUTPUT_DATE"),				VariableType::Date, offsetof(AxsysProductOutput,m_dtDate), false, false, false)
	DECLARE_VAR(_T("NATIVE_PRICE"),				_T("PRO.NATIVE_PRICE"),				VariableType::Double, offsetof(AxsysProductOutput,m_dNativePrice), false, false, false)
	DECLARE_VAR(_T("NEW_PRODUCT_COUNT"),		_T("PRO.NEW_PRODUCT_COUNT"),		VariableType::Double, offsetof(AxsysProductOutput,m_dOutputCount), false, false, false)
	DECLARE_VAR(_T("PAYMENT"),					_T("OUT.PAYMENT"),					VariableType::Double, offsetof(AxsysProductOutput,m_dPayment), false, false, false)
	DECLARE_VAR(_T("REAL_PAYMENT"),				_T("OUT.REAL_PAYMENT"),				VariableType::Double, offsetof(AxsysProductOutput,m_dRealPayment), false, false, false)
	DECLARE_VAR(_T("OUTPUT_PRICE"),				_T("PRD.OUTPUT_PRICE"),				VariableType::Double, offsetof(AxsysProductOutput,m_dOutputPrice), false, false, false)
END_DECLARE_VAR()

AxsysProductOutput::~AxsysProductOutput(){
	}


AxsysProductIncome::AxsysProductIncome(){
	}

BEGIN_DECLARE_VAR(AxsysProductIncome, Serializable)
	DECLARE_VAR(_T("AxsysProductIncome"),		_T("AXSYS_INCOME_PRODUCT PRO"),		VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("PRODUCT_NAME"),				_T("PRD.PRODUCT_NAME"),				VariableType::String, offsetof(AxsysProductIncome,m_sProductName), false, false, false)
	DECLARE_VAR(_T("PRODUCT_CODE"),				_T("PRD.PRODUCT_CODE"),				VariableType::String, offsetof(AxsysProductIncome,m_sProductCode), false, false, false)
	DECLARE_VAR(_T("TRANS_DATE"),				_T("PRO.TRANS_DATE"),				VariableType::Date, offsetof(AxsysProductIncome,m_dtDate), false, false, false)
	DECLARE_VAR(_T("NATIVE_PRICE"),				_T("PRO.NATIVE_PRICE"),				VariableType::Double, offsetof(AxsysProductIncome,m_dNativePrice), false, false, false)
	DECLARE_VAR(_T("PRODUCT_COUNT"),			_T("PRO.PRODUCT_COUNT"),			VariableType::Double, offsetof(AxsysProductIncome,m_dProductCount), false, false, false)
	DECLARE_VAR(_T("FULL_PRICE"),				_T("PRO.FULL_PRICE"),				VariableType::Double, offsetof(AxsysProductIncome,m_dFullPrice), false, false, false)
	DECLARE_VAR(_T("OUTPUT_PRICE"),				_T("PRD.OUTPUT_PRICE"),				VariableType::Double, offsetof(AxsysProductIncome,m_dOutputPrice), false, false, false)
END_DECLARE_VAR()

AxsysProductIncome::~AxsysProductIncome(){
	}

AxsysProductRemain::AxsysProductRemain(){
	}

BEGIN_DECLARE_VAR(AxsysProductRemain, Serializable)
	DECLARE_VAR(_T("AxsysProductRemain"),		_T(""),					VariableType::None, 0, false, true, false)
	DECLARE_VAR(_T("Id"),						_T("UID"),				VariableType::Int32, offsetof(AxsysProductRemain,m_nId), false, false, false)
	DECLARE_VAR(_T("ProductName"),				_T("PRODUCT_NAME"),		VariableType::String, offsetof(AxsysProductRemain,m_sProductName), false, false, false)
	DECLARE_VAR(_T("OutputPrice"),				_T("OUTPUT_PRICE"),		VariableType::Double, offsetof(AxsysProductRemain,m_dOutputPrice), false, false, false)
	DECLARE_VAR(_T("ProductCount"),				_T("PRODUCT_COUNT"),	VariableType::Double, offsetof(AxsysProductRemain,m_dProductCount), false, false, false)
	DECLARE_VAR(_T("FullPrice"),				_T("FULL_PRICE"),		VariableType::Double, offsetof(AxsysProductRemain,m_dFullPrice), false, false, false)
	DECLARE_VAR(_T("NativePrice"),				_T("NATIVE_PRICE"),		VariableType::Double, offsetof(AxsysProductRemain,m_dNativePrice), false, false, false)
END_DECLARE_VAR()

AxsysProductRemain::~AxsysProductRemain(){
	}
