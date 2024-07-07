#include "encode.hpp"

#include <fstream>
#include <cstdlib>
#include <cmath>
#include <time.h>

#include "fit_encode.hpp"
#include "fit_mesg_broadcaster.hpp"
#include "fit_file_id_mesg.hpp"
#include "fit_date_time.hpp"

// 2 * PI (3.14159265)
#define TWOPI 6.2831853

// Number of semicircles per meter at the equator
#define SC_PER_M 107.173

int FIT_FILE_ENCODER::EncodeWorkout(std::string filename, std::vector<std::time_t> timeStamps, std::vector<int> powerValues, std::vector<int> cadenceValues, std::vector<int> heartRateValues)
{
	try {
		int recordSize = std::min(std::min(timeStamps.size(), powerValues.size()), std::min(cadenceValues.size(), heartRateValues.size()));

		// Open the file
		std::fstream file;
		file.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);

		if (!file.is_open())
		{
			printf("Error opening file %s\n", filename.c_str());
			return EXIT_FAILURE;
		}

		// Create a FIT Encode object
		fit::Encode encode(fit::ProtocolVersion::V20);

		// Write the FIT header to the output stream
		encode.Open(file);

		// The starting timestamp for the activity
		fit::DateTime startTime(timeStamps.at(0));

		// Every FIT file MUST contain a File ID message
		fit::FileIdMesg fileIdMesg;
		fileIdMesg.SetType(FIT_FILE_ACTIVITY);
		fileIdMesg.SetManufacturer(FIT_MANUFACTURER_DEVELOPMENT);
		fileIdMesg.SetProduct(1);
		fileIdMesg.SetTimeCreated(startTime.GetTimeStamp());
		// You should create a serial number unique to your platform
		srand((unsigned int)time(NULL));
		fileIdMesg.SetSerialNumber(rand() % 10000 + 1);
		encode.Write(fileIdMesg);

		// Create the Developer Id message for the developer data fields.
		fit::DeveloperDataIdMesg developerIdMesg;
		// It is a BEST PRACTICE to use the same Guid for all FIT files created by your platform
		// 00010203-0405-0607-0809-0A0B0C0D0E0F
		FIT_UINT8 appId[] = { 0x03,0x02,0x01,0x00,0x05,0x04,0x07,0x06,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f };
		for (FIT_UINT8 i = 0; i < 16; i++)
		{
			developerIdMesg.SetApplicationId(i, appId[i]);
		}
		developerIdMesg.SetDeveloperDataIndex(0);
		developerIdMesg.SetApplicationVersion(110);
		encode.Write(developerIdMesg);

		// Create the Developer Data Field Descriptions

		/*fit::FieldDescriptionMesg powerFieldDescMesg;
		powerFieldDescMesg.SetDeveloperDataIndex(0);
		powerFieldDescMesg.SetFieldDefinitionNumber(1);
		powerFieldDescMesg.SetFitBaseTypeId(FIT_BASE_TYPE_UINT8);
		powerFieldDescMesg.SetFieldName(0, L"Power");
		powerFieldDescMesg.SetUnits(0, L"watts");
		powerFieldDescMesg.SetNativeFieldNum(fit::RecordMesg::FieldDefNum::Power);
		powerFieldDescMesg.SetNativeMesgNum(FIT_MESG_NUM_RECORD);
		encode.Write(powerFieldDescMesg);

		fit::FieldDescriptionMesg powerFieldDescMesg;
		powerFieldDescMesg.SetDeveloperDataIndex(0);
		powerFieldDescMesg.SetFieldDefinitionNumber(1);
		powerFieldDescMesg.SetFitBaseTypeId(FIT_BASE_TYPE_UINT8);
		powerFieldDescMesg.SetFieldName(0, L"Cadence");
		powerFieldDescMesg.SetUnits(0, L"rpm");
		powerFieldDescMesg.SetNativeFieldNum(fit::RecordMesg::FieldDefNum::Cadence);
		powerFieldDescMesg.SetNativeMesgNum(FIT_MESG_NUM_RECORD);
		encode.Write(powerFieldDescMesg);*/

		fit::FieldDescriptionMesg hrFieldDescMesg;
		hrFieldDescMesg.SetDeveloperDataIndex(0);
		hrFieldDescMesg.SetFieldDefinitionNumber(1);
		hrFieldDescMesg.SetFitBaseTypeId(FIT_BASE_TYPE_UINT8);
		hrFieldDescMesg.SetFieldName(0, L"Heart Rate");
		hrFieldDescMesg.SetUnits(0, L"bpm");
		hrFieldDescMesg.SetNativeFieldNum(fit::RecordMesg::FieldDefNum::HeartRate);
		hrFieldDescMesg.SetNativeMesgNum(FIT_MESG_NUM_RECORD);
		encode.Write(hrFieldDescMesg);

		// Timer Events are a BEST PRACTICE for FIT ACTIVITY files
		fit::EventMesg eventMesgStart;
		eventMesgStart.SetTimestamp(startTime.GetTimeStamp());
		eventMesgStart.SetEvent(FIT_EVENT_TIMER);
		eventMesgStart.SetEventType(FIT_EVENT_TYPE_START);
		encode.Write(eventMesgStart);

		// Every FIT ACTIVITY file MUST contain Record messages
		fit::DateTime timestamp(startTime);

		// Create one hour (3600 seconds) of Record data
		for (FIT_UINT16 i = 0; i < (FIT_UINT16)recordSize; i++)
		{
			// Create a new Record message and set the timestamp
			fit::RecordMesg recordMesg;
			recordMesg.SetTimestamp(timestamp.GetTimeStamp());

			// Fake Record Data of Various Signal Patterns
			//recordMesg.SetDistance(i); // Ramp
			//recordMesg.SetSpeed(1); // Flatline
			if (heartRateValues.at(i) != -1) {
				recordMesg.SetHeartRate((FIT_UINT8)heartRateValues.at(i));
			}
			if (cadenceValues.at(i) != -1) {
				recordMesg.SetCadence((FIT_UINT8)cadenceValues.at(i));
			}
			if (powerValues.at(i) != -1) {
				recordMesg.SetPower((FIT_UINT16)powerValues.at(i));
			}
			//recordMesg.SetAltitude((float)std::abs(((float)(i % 255)) - 127.0f)); // Triangle
			//recordMesg.SetPositionLat(0);
			//recordMesg.SetPositionLong((FIT_SINT32)(i * SC_PER_M));

			// Add a Developer Field to the Record Message
			fit::DeveloperField developerHrField(hrFieldDescMesg, developerIdMesg);
			developerHrField.AddValue((FIT_UINT8)((sin(TWOPI * (0.01 * i + 10)) + 1.0) * 127.0)); // Sine
			recordMesg.AddDeveloperField(developerHrField);

			// Write the Rercord message to the output stream
			encode.Write(recordMesg);

			// Increment the timestamp by one second
			timestamp.add(fit::DateTime((FIT_DATE_TIME)1));
		}

		// Timer Events are a BEST PRACTICE for FIT ACTIVITY files
		fit::EventMesg eventMesgStop;
		eventMesgStop.SetTimestamp(timestamp.GetTimeStamp());
		eventMesgStop.SetEvent(FIT_EVENT_TIMER);
		eventMesgStop.SetEventType(FIT_EVENT_TYPE_STOP);
		encode.Write(eventMesgStop);

		// Every FIT ACTIVITY file MUST contain at least one Lap message
		fit::LapMesg lapMesg;
		lapMesg.SetTimestamp(timestamp.GetTimeStamp());
		lapMesg.SetStartTime(startTime.GetTimeStamp());
		lapMesg.SetTotalElapsedTime((FIT_FLOAT32)(timestamp.GetTimeStamp() - startTime.GetTimeStamp()));
		lapMesg.SetTotalTimerTime((FIT_FLOAT32)(timestamp.GetTimeStamp() - startTime.GetTimeStamp()));
		encode.Write(lapMesg);

		// Every FIT ACTIVITY file MUST contain at least one Session message
		fit::SessionMesg sessionMesg;
		sessionMesg.SetTimestamp(timestamp.GetTimeStamp());
		sessionMesg.SetStartTime(startTime.GetTimeStamp());
		sessionMesg.SetTotalElapsedTime((FIT_FLOAT32)(timestamp.GetTimeStamp() - startTime.GetTimeStamp()));
		sessionMesg.SetTotalTimerTime((FIT_FLOAT32)(timestamp.GetTimeStamp() - startTime.GetTimeStamp()));
		sessionMesg.SetSport(FIT_SPORT_STAND_UP_PADDLEBOARDING);
		sessionMesg.SetSubSport(FIT_SUB_SPORT_GENERIC);
		sessionMesg.SetFirstLapIndex(0);
		sessionMesg.SetNumLaps(1);

		// Add a Developer Field to the Session message
		/*fit::DeveloperField doughnutsEarnedDevField(doughnutsFieldDescMesg, developerIdMesg);
		doughnutsEarnedDevField.AddValue(sessionMesg.GetTotalElapsedTime() / 1200.0f);
		sessionMesg.AddDeveloperField(doughnutsEarnedDevField);
		encode.Write(sessionMesg);*/

		// Every FIT ACTIVITY file MUST contain EXACTLY one Activity message
		fit::ActivityMesg activityMesg;
		activityMesg.SetTimestamp(timestamp.GetTimeStamp());
		activityMesg.SetNumSessions(1);
		int timezoneOffset = 12 * recordSize;
		activityMesg.SetLocalTimestamp((FIT_LOCAL_DATE_TIME)((int)timestamp.GetTimeStamp() + timezoneOffset));
		encode.Write(activityMesg);

		// Update the data size in the header and calculate the CRC
		if (!encode.Close())
		{
			printf("Error closing encode.\n");
			return EXIT_FAILURE;
		}

		// Close the file
		file.close();

		printf("Encoded FIT file %s.\n", filename.c_str());

		return EXIT_SUCCESS;
	}
	catch (...) {
		return EXIT_FAILURE;
	}

	return 0;
}
