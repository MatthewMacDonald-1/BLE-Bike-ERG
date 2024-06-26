

class UserSettings {
public:
	static void LoadUserSettings();

	static int GetFTP();
	
	static int GetPowerAveragePeriod();
	
	static int GetCadenceAveragePeriod();

private:
	static int ftp;
	static int powerAveragePeriod;
	static int cadenceAveragePeriod;
};