// Kuan Hao Yu     33115939
// Kimberly Nguy   86242309
// Pafoua Fang     84351619
// Kuanping Change 71018021

#include "Parse.h"

/**
 * Helper function for splitting up a message by a delimiter
 */
std::vector<std::string> Parse::split(std::string message, char delimiter) {
	std::vector<std::string> result = std::vector<std::string>();
	std::stringstream stream(message);
	std::string token;
	while (getline(stream, token, delimiter)) {
		result.push_back(token);
	}
	return result;
}