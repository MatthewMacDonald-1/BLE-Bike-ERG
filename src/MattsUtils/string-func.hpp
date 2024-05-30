#pragma once
#include <string>
#include <vector>

namespace MattsUtils::String {

	/// @brief Sourced from (https://stackoverflow.com/questions/5888022/split-string-by-single-spaces)
	/// @param txt String to be split
	/// @param strs Reference to the vector of strings
	/// @param ch The deliminator
	/// @return The size of the vector of strings
	size_t split(const std::string& txt, std::vector<std::string>& strs, char ch);

	/// @brief Sourced from (https://stackoverflow.com/questions/1798112/removing-leading-and-trailing-spaces-from-a-string) answer
	/// @param str 
	/// @param whitespace 
	/// @return 
	std::string trim(const std::string& str, const std::string& whitespace = " \t");

	/// <summary>
	/// Returns true if the value is in the array.
	/// </summary>
	/// <param name="value"></param>
	/// <param name="array"></param>
	/// <returns>If present.</returns>
	bool in_array(const std::string& value, const std::vector<std::string>& array);

	/// <summary>
	/// Finds the index of the value if present in the array.
	/// </summary>
	/// <param name="value"></param>
	/// <param name="array"></param>
	/// <returns>The index in the array otherwise -1.</returns>
	int findIndexInArray(const std::string& value, const std::vector<std::string>& array);
}
