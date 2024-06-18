#include "user-data.hpp"

int UserData::ftp = 0;

void UserData::LoadUserData()
{
	ftp = 243;
}

int UserData::GetFTP()
{
	return ftp;
}
