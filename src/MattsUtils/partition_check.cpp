#include "partition_check.hpp"
#include "string-func.hpp"
#include <algorithm>
#include <iostream>
#include "parsing.hpp"


long MattsUtils::Partitions::sumPartition(std::string partition)
{
    std::vector<long> numbers = MattsUtils::Number::parseLongArray(partition, ' ');
    long sum = 0;

    for (long num : numbers) {
        sum += num;
    }


    return sum;
}

bool MattsUtils::Partitions::processPartition(std::string partition, std::string* formattedOutput) {
    std::string originalLine = MattsUtils::String::trim(partition);
    *formattedOutput = ""; // Set the formatted output as an empty string.

    if (partition.length() < 1) {
        return false;
    }

    // Check validity
    // Finally check for a partition
    if (originalLine.at(0) >= '0' && originalLine.at(0) <= '9') {
        // The first char is numeric but that doesn't mean we are in the clear

        // Check that all chars are numeric or ',' or ' '.
        std::string inlineComment;
        std::string rawPartitionLine = originalLine;


        bool valid = true;
        for (size_t i = 0; i < originalLine.length(); i++) {
            if ((originalLine.at(i) >= '0' && originalLine.at(i) <= '9') || originalLine.at(i) == ' ' || originalLine.at(i) == ',') {
                // do nothing
            }
            else {
                valid = false;
                break;
            }
        }

        if (!valid) {
            return false;
        }

        // ToDo: Check for separator consistency and create a string with only a single space between the numbers
        std::vector<char> cleanedChars;
        char separatorChar = '\0';
        char lastSeenNonWhiteSpace = '\0';
        size_t sinceLastNumber = 0;

        int errorType = -1;
        for (size_t i = 0; i < rawPartitionLine.length(); i++) {
            if (rawPartitionLine.at(i) >= '0' && rawPartitionLine.at(i) <= '9') {
                // add to cleaned chars
                cleanedChars.push_back(rawPartitionLine.at(i));
                sinceLastNumber = 0;
                continue;
            }
            else if (rawPartitionLine.at(i) != separatorChar) {
                if (separatorChar == '\0' && (rawPartitionLine.at(i) == ' ' || rawPartitionLine.at(i) == ',')) {
                    separatorChar = rawPartitionLine.at(i);
                }
                else if (rawPartitionLine.at(i) != separatorChar && lastSeenNonWhiteSpace != separatorChar) {
                    valid = false;
                    errorType = 1;
                    break;
                }
            }

            if ((rawPartitionLine.at(i) == separatorChar) && sinceLastNumber == 0) {
                // If the char is the separator or a whitespace then only add whitespace once
                cleanedChars.push_back(' ');
                sinceLastNumber++;
            }
            else if (sinceLastNumber == 0 && rawPartitionLine.at(i) != separatorChar) {
                valid = false;
                errorType = 1;
                break;
            }
            else if (separatorChar == ',' && rawPartitionLine.at(i) == separatorChar && sinceLastNumber > 0) {
                valid = false;
                errorType = 2;
            }

            if (rawPartitionLine.at(i) != ' ') {
                lastSeenNonWhiteSpace = rawPartitionLine.at(i);
            }
        }


        rawPartitionLine = MattsUtils::String::trim(rawPartitionLine);

        if (separatorChar == ',' && rawPartitionLine.at(rawPartitionLine.length() - 1) == ',') {
            errorType = 3;
            valid = false;
        }

        if (!valid) {
            return false;
        }

        std::string cleanFormatted(cleanedChars.begin(), cleanedChars.end());

        cleanFormatted = MattsUtils::String::trim(cleanFormatted); // Trim the white space to reduce extra number as zero from whitespace error

        // ToDo: Convert the string to integer array
        std::vector<long> partitionNumbers;
        std::vector<std::string> cleanFormattedSubStrings;

        MattsUtils::String::split(cleanFormatted, cleanFormattedSubStrings, ' ');

        for (const std::string& subString : cleanFormattedSubStrings) {
            // Check that number does not have a leading zero
            if (subString.at(0) == '0') {
                valid = false;
                break;
            }

            // Convert to int and push to partition numbers
            long num = MattsUtils::Number::parseLong(subString);
            partitionNumbers.push_back(num);
        }

        if (!valid) {
            return false;
        }

        // ToDo: Sort array from largest (start) to smallest (end)
        std::sort(partitionNumbers.rbegin(), partitionNumbers.rend());

        // ToDo: check that the array doesn't contain any number less then or equal to zero
        for (const long& number : partitionNumbers) {
            if (number <= 0) {
                valid = false;
                break;
            }
        }

        if (!valid) {
            return false;
        }

        // ToDo: return the array space deliminated as string
        std::string partitionFinal;
        for (const long& number : partitionNumbers) {
            partitionFinal.append(std::to_string(number));
            partitionFinal.append(" ");
        }

        partitionFinal = MattsUtils::String::trim(partitionFinal);
        
        *formattedOutput = partitionFinal;
        return true;
    }

    return false;
}

bool MattsUtils::Partitions::isPartition(std::string partition)
{
    std::string output = "";
    //std::cout << "Processing Partition for Validity (" << partition << ")" << std::endl;
    bool res = processPartition(partition, &output);
    //std::cout << "Output: " << res << std::endl;
    return res;
}

std::string MattsUtils::Partitions::formatPartition(std::string partition)
{
    std::string output = "";
    //std::cout << "Processing Partition for formatting (" << partition << ")" << std::endl;
    processPartition(partition, &output);
    //std::cout << "Output: '" << output << "'" << std::endl;
	return output;
}

bool MattsUtils::Partitions::isPartitionSeperator(std::string line)
{
    if (line.at(0) == '-') 
    {
        // may be a separator

        // Check that there is at least 3 -'s and that they are all dashes
        size_t count = 0;
        bool valid = true;
        for (char& character : line) 
        {
            if (character == '-') 
            {
                count++;
            }
            else 
            {
                valid = false;
                break;
            }
        }

        // If valid return the standard separator
        if (valid && count >= 3) 
        {
            return true;
        }
        else 
        {
            return false;
        }
    }
    else 
    {
        return false;
    }
}
