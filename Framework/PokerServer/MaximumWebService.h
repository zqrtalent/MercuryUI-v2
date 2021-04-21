#pragma once

struct MaximumUserDetails{
	int		nUserId;
    _String userName;
	_String firstName;
	_String lastName;
	_String address;
	_String email;
	_String phoneNumber;
	_String mobileNumber;
	_String docNumber;
	_String docPersonalNumber;
	};

class MaximumWebService
{
public:
	MaximumWebService();
	virtual ~MaximumWebService();

public:
	void	SetServiceURL		(_String sServiceURL){ m_sServiceURL = sServiceURL; };
	void	SetServicePort		(int nPort){m_nPort = nPort;}

	int		Authenticate		(_String sUserName, _String sPassword);
	bool	GetUserDetails		(_String sUserName, MaximumUserDetails& user);
	bool	DepositMoney		(int nUserId, int nAmount, int& nBalance, int& nEntryId);
	bool	WithdrawMoney		(int nUserId, int nAmount, int& nBalance, int& nEntryId);
	bool	DeleteTransferEntry	(int nEntryId);
	double	GetUserBalance		(int nUserId);

protected:
	_String		m_sServiceURL;
	int			m_nPort;
};
