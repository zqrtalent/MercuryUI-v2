#if !defined(PLATFORMDATETIME_H_INCLUDED)
#define      PLATFORMDATETIME_H_INCLUDED

#include "PlatformCompat.h"
#include <time.h>
//#include <ATLComTime.h> // temporary for compatibility

class DateTime
{
public:
	DateTime();
	DateTime(bool initializeWithCurrentTime);
	DateTime(time_t time);
	DateTime(int year, int month, int day);
	DateTime(int year, int month, int day, int hour, int minute, int second);

	DateTime& operator= (time_t t);
	//DateTime& operator= (COleDateTime dt);
	operator time_t () {return (status_ == Status::valid) ? time_ : 0; };
	bool operator== (DateTime& dt);
	bool operator> (DateTime& dt);
	bool operator< (DateTime& dt);

	inline operator time_t() const { return ((status_ == Status::valid) ? time_ : 0); };

	int	 GetDifferenceInSeconds(DateTime& dtAfter);

	virtual ~DateTime();

	enum Status
	{
		valid = 0,
		invalid,
		null
	};


	DateTime::Status GetStatus(){return status_;};
	void SetStatusNull(){ status_ = Status::null; }

	int GetYear			(){return (status_ == Status::valid) ? (t_.tm_year + 1900) : 0;};
	int GetMonth		(){return (status_ == Status::valid) ? (t_.tm_mon + 1) : 0;};
	int GetDay			(){return (status_ == Status::valid) ? t_.tm_mday : 0;};
	int GetHour			(){return (status_ == Status::valid) ? t_.tm_hour : 0;};
	int GetMinute		(){return (status_ == Status::valid) ? t_.tm_min : 0;};
	int GetSecond		(){return (status_ == Status::valid) ? t_.tm_sec : 0;};

	void SetCurrentTime	();
	bool SetDate		(int year, int month, int day);
	bool SetDateTime	(int year, int month, int day, int hour, int minute, int second);
	bool ParseDateTime	(LPCSTR lpszDateTime);
	bool ParseDate		(LPCSTR lpszDate);
	bool ParseTime		(LPCSTR lpszTime);

	bool AddSeconds		(int seconds);
	bool AddMinutes		(int seconds);
	bool AddHours		(int seconds);

protected:
	Status		status_;		// Current status.
	time_t		time_;			// Time object.
	tm			t_;
};

#endif // !defined(PLATFORMDATETIME_H_INCLUDED)

