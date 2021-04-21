#pragma once


class SoundMan
{
protected:
	SoundMan();
	virtual ~SoundMan();

public:
	static SoundMan* GetInstance(){
		static SoundMan man;
		return &man;
		}

protected:

};