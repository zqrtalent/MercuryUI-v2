#pragma once
#include "..\..\..\..\Serialize\Serializable.h"

class GMSMarketDataExporter : public Serializable
{
public:
	GMSMarketDataExporter();
	virtual ~GMSMarketDataExporter();

public:
	void	ExportInExcell();

protected: // Serialization.	
	INIT_RUNTIME_VARIABLE()
};