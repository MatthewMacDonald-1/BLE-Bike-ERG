/////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2024 Garmin International, Inc.
// Licensed under the Flexible and Interoperable Data Transfer (FIT) Protocol License; you
// may not use this file except in compliance with the Flexible and Interoperable Data
// Transfer (FIT) Protocol License.
/////////////////////////////////////////////////////////////////////////////////////////////
// ****WARNING****  This file is auto-generated!  Do NOT edit this file.
// Profile Version = 21.141.0Release
// Tag = production/release/21.141.0-0-g2aa27e1
/////////////////////////////////////////////////////////////////////////////////////////////


#if !defined(FIT_HSA_HEART_RATE_DATA_MESG_HPP)
#define FIT_HSA_HEART_RATE_DATA_MESG_HPP

#include "fit_mesg.hpp"

namespace fit
{

class HsaHeartRateDataMesg : public Mesg
{
public:
    class FieldDefNum final
    {
    public:
       static const FIT_UINT8 Timestamp = 253;
       static const FIT_UINT8 ProcessingInterval = 0;
       static const FIT_UINT8 Status = 1;
       static const FIT_UINT8 HeartRate = 2;
       static const FIT_UINT8 Invalid = FIT_FIELD_NUM_INVALID;
    };

    HsaHeartRateDataMesg(void) : Mesg(Profile::MESG_HSA_HEART_RATE_DATA)
    {
    }

    HsaHeartRateDataMesg(const Mesg &mesg) : Mesg(mesg)
    {
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of timestamp field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsTimestampValid() const
    {
        const Field* field = GetField(253);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid();
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns timestamp field
    // Units: s
    ///////////////////////////////////////////////////////////////////////
    FIT_DATE_TIME GetTimestamp(void) const
    {
        return GetFieldUINT32Value(253, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set timestamp field
    // Units: s
    ///////////////////////////////////////////////////////////////////////
    void SetTimestamp(FIT_DATE_TIME timestamp)
    {
        SetFieldUINT32Value(253, timestamp, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of processing_interval field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsProcessingIntervalValid() const
    {
        const Field* field = GetField(0);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid();
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns processing_interval field
    // Units: s
    // Comment: Processing interval length in seconds
    ///////////////////////////////////////////////////////////////////////
    FIT_UINT16 GetProcessingInterval(void) const
    {
        return GetFieldUINT16Value(0, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set processing_interval field
    // Units: s
    // Comment: Processing interval length in seconds
    ///////////////////////////////////////////////////////////////////////
    void SetProcessingInterval(FIT_UINT16 processingInterval)
    {
        SetFieldUINT16Value(0, processingInterval, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of status field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsStatusValid() const
    {
        const Field* field = GetField(1);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid();
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns status field
    // Comment: Status of measurements in buffer - 0 indicates SEARCHING 1 indicates LOCKED
    ///////////////////////////////////////////////////////////////////////
    FIT_UINT8 GetStatus(void) const
    {
        return GetFieldUINT8Value(1, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set status field
    // Comment: Status of measurements in buffer - 0 indicates SEARCHING 1 indicates LOCKED
    ///////////////////////////////////////////////////////////////////////
    void SetStatus(FIT_UINT8 status)
    {
        SetFieldUINT8Value(1, status, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns number of heart_rate
    ///////////////////////////////////////////////////////////////////////
    FIT_UINT8 GetNumHeartRate(void) const
    {
        return GetFieldNumValues(2, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of heart_rate field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsHeartRateValid(FIT_UINT8 index) const
    {
        const Field* field = GetField(2);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid(index);
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns heart_rate field
    // Units: bpm
    // Comment: Beats / min
    ///////////////////////////////////////////////////////////////////////
    FIT_UINT8 GetHeartRate(FIT_UINT8 index) const
    {
        return GetFieldUINT8Value(2, index, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set heart_rate field
    // Units: bpm
    // Comment: Beats / min
    ///////////////////////////////////////////////////////////////////////
    void SetHeartRate(FIT_UINT8 index, FIT_UINT8 heartRate)
    {
        SetFieldUINT8Value(2, heartRate, index, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

};

} // namespace fit

#endif // !defined(FIT_HSA_HEART_RATE_DATA_MESG_HPP)
