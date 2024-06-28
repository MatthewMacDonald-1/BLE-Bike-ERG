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


#if !defined(FIT_SET_MESG_HPP)
#define FIT_SET_MESG_HPP

#include "fit_mesg.hpp"

namespace fit
{

class SetMesg : public Mesg
{
public:
    class FieldDefNum final
    {
    public:
       static const FIT_UINT8 Timestamp = 254;
       static const FIT_UINT8 Duration = 0;
       static const FIT_UINT8 Repetitions = 3;
       static const FIT_UINT8 Weight = 4;
       static const FIT_UINT8 SetType = 5;
       static const FIT_UINT8 StartTime = 6;
       static const FIT_UINT8 Category = 7;
       static const FIT_UINT8 CategorySubtype = 8;
       static const FIT_UINT8 WeightDisplayUnit = 9;
       static const FIT_UINT8 MessageIndex = 10;
       static const FIT_UINT8 WktStepIndex = 11;
       static const FIT_UINT8 Invalid = FIT_FIELD_NUM_INVALID;
    };

    SetMesg(void) : Mesg(Profile::MESG_SET)
    {
    }

    SetMesg(const Mesg &mesg) : Mesg(mesg)
    {
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of timestamp field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsTimestampValid() const
    {
        const Field* field = GetField(254);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid();
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns timestamp field
    // Comment: Timestamp of the set
    ///////////////////////////////////////////////////////////////////////
    FIT_DATE_TIME GetTimestamp(void) const
    {
        return GetFieldUINT32Value(254, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set timestamp field
    // Comment: Timestamp of the set
    ///////////////////////////////////////////////////////////////////////
    void SetTimestamp(FIT_DATE_TIME timestamp)
    {
        SetFieldUINT32Value(254, timestamp, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of duration field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsDurationValid() const
    {
        const Field* field = GetField(0);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid();
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns duration field
    // Units: s
    ///////////////////////////////////////////////////////////////////////
    FIT_FLOAT32 GetDuration(void) const
    {
        return GetFieldFLOAT32Value(0, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set duration field
    // Units: s
    ///////////////////////////////////////////////////////////////////////
    void SetDuration(FIT_FLOAT32 duration)
    {
        SetFieldFLOAT32Value(0, duration, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of repetitions field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsRepetitionsValid() const
    {
        const Field* field = GetField(3);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid();
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns repetitions field
    // Comment: # of repitions of the movement
    ///////////////////////////////////////////////////////////////////////
    FIT_UINT16 GetRepetitions(void) const
    {
        return GetFieldUINT16Value(3, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set repetitions field
    // Comment: # of repitions of the movement
    ///////////////////////////////////////////////////////////////////////
    void SetRepetitions(FIT_UINT16 repetitions)
    {
        SetFieldUINT16Value(3, repetitions, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of weight field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsWeightValid() const
    {
        const Field* field = GetField(4);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid();
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns weight field
    // Units: kg
    // Comment: Amount of weight applied for the set
    ///////////////////////////////////////////////////////////////////////
    FIT_FLOAT32 GetWeight(void) const
    {
        return GetFieldFLOAT32Value(4, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set weight field
    // Units: kg
    // Comment: Amount of weight applied for the set
    ///////////////////////////////////////////////////////////////////////
    void SetWeight(FIT_FLOAT32 weight)
    {
        SetFieldFLOAT32Value(4, weight, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of set_type field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsSetTypeValid() const
    {
        const Field* field = GetField(5);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid();
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns set_type field
    ///////////////////////////////////////////////////////////////////////
    FIT_SET_TYPE GetSetType(void) const
    {
        return GetFieldUINT8Value(5, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set set_type field
    ///////////////////////////////////////////////////////////////////////
    void SetSetType(FIT_SET_TYPE setType)
    {
        SetFieldUINT8Value(5, setType, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of start_time field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsStartTimeValid() const
    {
        const Field* field = GetField(6);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid();
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns start_time field
    // Comment: Start time of the set
    ///////////////////////////////////////////////////////////////////////
    FIT_DATE_TIME GetStartTime(void) const
    {
        return GetFieldUINT32Value(6, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set start_time field
    // Comment: Start time of the set
    ///////////////////////////////////////////////////////////////////////
    void SetStartTime(FIT_DATE_TIME startTime)
    {
        SetFieldUINT32Value(6, startTime, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns number of category
    ///////////////////////////////////////////////////////////////////////
    FIT_UINT8 GetNumCategory(void) const
    {
        return GetFieldNumValues(7, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of category field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsCategoryValid(FIT_UINT8 index) const
    {
        const Field* field = GetField(7);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid(index);
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns category field
    ///////////////////////////////////////////////////////////////////////
    FIT_EXERCISE_CATEGORY GetCategory(FIT_UINT8 index) const
    {
        return GetFieldUINT16Value(7, index, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set category field
    ///////////////////////////////////////////////////////////////////////
    void SetCategory(FIT_UINT8 index, FIT_EXERCISE_CATEGORY category)
    {
        SetFieldUINT16Value(7, category, index, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns number of category_subtype
    ///////////////////////////////////////////////////////////////////////
    FIT_UINT8 GetNumCategorySubtype(void) const
    {
        return GetFieldNumValues(8, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of category_subtype field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsCategorySubtypeValid(FIT_UINT8 index) const
    {
        const Field* field = GetField(8);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid(index);
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns category_subtype field
    // Comment: Based on the associated category, see [category]_exercise_names
    ///////////////////////////////////////////////////////////////////////
    FIT_UINT16 GetCategorySubtype(FIT_UINT8 index) const
    {
        return GetFieldUINT16Value(8, index, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set category_subtype field
    // Comment: Based on the associated category, see [category]_exercise_names
    ///////////////////////////////////////////////////////////////////////
    void SetCategorySubtype(FIT_UINT8 index, FIT_UINT16 categorySubtype)
    {
        SetFieldUINT16Value(8, categorySubtype, index, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of weight_display_unit field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsWeightDisplayUnitValid() const
    {
        const Field* field = GetField(9);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid();
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns weight_display_unit field
    ///////////////////////////////////////////////////////////////////////
    FIT_FIT_BASE_UNIT GetWeightDisplayUnit(void) const
    {
        return GetFieldUINT16Value(9, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set weight_display_unit field
    ///////////////////////////////////////////////////////////////////////
    void SetWeightDisplayUnit(FIT_FIT_BASE_UNIT weightDisplayUnit)
    {
        SetFieldUINT16Value(9, weightDisplayUnit, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of message_index field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsMessageIndexValid() const
    {
        const Field* field = GetField(10);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid();
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns message_index field
    ///////////////////////////////////////////////////////////////////////
    FIT_MESSAGE_INDEX GetMessageIndex(void) const
    {
        return GetFieldUINT16Value(10, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set message_index field
    ///////////////////////////////////////////////////////////////////////
    void SetMessageIndex(FIT_MESSAGE_INDEX messageIndex)
    {
        SetFieldUINT16Value(10, messageIndex, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Checks the validity of wkt_step_index field
    // Returns FIT_TRUE if field is valid
    ///////////////////////////////////////////////////////////////////////
    FIT_BOOL IsWktStepIndexValid() const
    {
        const Field* field = GetField(11);
        if( FIT_NULL == field )
        {
            return FIT_FALSE;
        }

        return field->IsValueValid();
    }

    ///////////////////////////////////////////////////////////////////////
    // Returns wkt_step_index field
    ///////////////////////////////////////////////////////////////////////
    FIT_MESSAGE_INDEX GetWktStepIndex(void) const
    {
        return GetFieldUINT16Value(11, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

    ///////////////////////////////////////////////////////////////////////
    // Set wkt_step_index field
    ///////////////////////////////////////////////////////////////////////
    void SetWktStepIndex(FIT_MESSAGE_INDEX wktStepIndex)
    {
        SetFieldUINT16Value(11, wktStepIndex, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
    }

};

} // namespace fit

#endif // !defined(FIT_SET_MESG_HPP)
