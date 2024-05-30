#include "ferrersBoard.hpp"
#include "string-func.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

std::string MattsUtils::FerrersBoard::performMove(std::string ferrersBoard, unsigned int column)
{
	std::string formattedPartition;
	bool valid = MattsUtils::Partitions::processPartition(ferrersBoard, &formattedPartition);

	if (!valid) {
		std::cerr << "ERROR: invalid partition passed to FerrersBoard perform move (" << ferrersBoard << ")!" << std::endl;
		return "";
	}

	std::vector<std::string> rows;
	MattsUtils::String::split(formattedPartition, rows, ' ');

	std::vector<int> rowSizes;
	for (const std::string& row : rows) {
		// Convert to int
		char* endptr;
		long num = strtol(row.c_str(), &endptr, 10);
		if (endptr == row.c_str()) {
			// No digits found
		}
		else if (*endptr != '\0') {
			// Error  Invalid input
		}
		rowSizes.push_back(num);
	}

	// For each row size greater then the column number decrease in size by 1.
	int newRowCount = 0;
	for (size_t i = 0; i < rowSizes.size(); i++) {
		if (rowSizes.at(i) > column) {
			rowSizes.at(i) -= 1;
			newRowCount++;
		}
	}

	rowSizes.push_back(newRowCount);

	std::sort(rowSizes.rbegin(), rowSizes.rend());

	// Format as a string
	std::string partitionOutput;
	for (const int& row : rowSizes) {
		if (row > 0) {
			partitionOutput.append(std::to_string(row));
			partitionOutput.append(" ");
		}
	}
	partitionOutput = MattsUtils::String::trim(partitionOutput);

	return partitionOutput;
}
