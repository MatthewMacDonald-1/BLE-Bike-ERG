#pragma once

#include <string>

namespace MattsUtils::Partitions {

	/// <summary>
	/// Returns the sum of a valid partition
	/// </summary>
	/// <param name="partition"></param>
	/// <returns></returns>
	long sumPartition(std::string partition);

	/// <summary>
	/// Checks wheather a partition is valid and returns the validity and a formatted partion if valid.
	/// </summary>
	/// <param name="partition">Input partition.</param>
	/// <param name="formattedOutput">Valid Formatted Output.</param>
	/// <returns>Wheather the partition is valid.</returns>
	bool processPartition(std::string partition, std::string* formattedOutput);

	/// <summary>
	/// Returns wheather a partition is valid or not.
	/// </summary>
	/// <param name="partition"></param>
	/// <returns></returns>
	bool isPartition(std::string partition);

	/// <summary>
	/// Return a formatted partition if valid otherwise an empty string.
	/// </summary>
	/// <param name="partition"></param>
	/// <returns></returns>
	std::string formatPartition(std::string partition);

	bool isPartitionSeperator(std::string line);

}