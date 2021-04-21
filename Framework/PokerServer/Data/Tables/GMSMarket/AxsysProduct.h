#pragma once
#include "..\..\..\..\Serialize\Serializable.h"

class AxsysProduct : public Serializable
{
public:
	AxsysProduct();
	virtual ~AxsysProduct();

public:
	// AXSYS_PRODUCT
	int			m_nId;					// UID
	int			m_nStockId;				// STOCK_UID
	CString		m_sProductCode;			// PRODUCT_CODE
	CString		m_sProductName;			// PRODUCT_NAME
	CString		m_sProductDescription;	// PRODUCT_DESCRIPTION
	int			m_nUnitId;				// UNIT_UID
	double		m_dOutputPrice;			// OUTPUT_PRICE
	double		m_dCriticalCt;			// CRITICAL_COUNT

protected:
	virtual Serializable*						CreateSerializableObject(){return new AxsysProduct();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<AxsysProduct>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class AxsysProductOutput : public Serializable
{
public:
	AxsysProductOutput();
	virtual ~AxsysProductOutput();

public:
	CString			m_sProductName;
	CString			m_sProductCode;
	COleDateTime	m_dtDate;
	double			m_dOutputCount;
	double			m_dPayment;
	double			m_dRealPayment;
	double			m_dNativePrice;
	double			m_dOutputPrice;

protected:
	virtual Serializable*						CreateSerializableObject(){return new AxsysProductOutput();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<AxsysProductOutput>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class AxsysProductIncome : public Serializable
{
public:
	AxsysProductIncome();
	virtual ~AxsysProductIncome();

public:
	CString			m_sProductName;
	CString			m_sProductCode;
	COleDateTime	m_dtDate;
	double			m_dProductCount;
	double			m_dFullPrice;
	double			m_dNativePrice;
	double			m_dOutputPrice;

protected:
	virtual Serializable*						CreateSerializableObject(){return new AxsysProductIncome();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<AxsysProductIncome>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

class AxsysProductRemain : public Serializable
{
public:
	AxsysProductRemain();
	virtual ~AxsysProductRemain();

public:
	int				m_nId;
	CString			m_sProductName;
	double			m_dOutputPrice;
	double			m_dProductCount;
	double			m_dFullPrice;
	double			m_dNativePrice;
	

protected:
	virtual Serializable*						CreateSerializableObject(){return new AxsysProductRemain();};
	virtual EnumerableObject<Serializable>*		CreateEnumerableObject	(){return (EnumerableObject<Serializable>*)new EnumerableObject<AxsysProductRemain>();};

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};

