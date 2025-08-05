#ifndef QUERY5_HPP
#define QUERY5_HPP

#include <string>
#include <vector>
#include <map>

// Parses command-line arguments and fills the provided variables.
// Returns true if parsing succeeds, false otherwise.
bool parseArgs(int argc, char* argv[],
               std::string& r_name,
               std::string& start_date,
               std::string& end_date,
               int& num_threads,
               std::string& table_path,
               std::string& result_path);

// Reads TPCH tables from the given directory into structured vectors.
// Returns true if successful.
bool readTPCHData(const std::string& table_path,
                  std::vector<std::map<std::string, std::string>>& customer_data,
                  std::vector<std::map<std::string, std::string>>& orders_data,
                  std::vector<std::map<std::string, std::string>>& lineitem_data,
                  std::vector<std::map<std::string, std::string>>& supplier_data,
                  std::vector<std::map<std::string, std::string>>& nation_data,
                  std::vector<std::map<std::string, std::string>>& region_data);

// Executes TPCH Query 5 using multiple threads.
// Aggregates results into a map keyed by nation name.
bool executeQuery5(const std::string& r_name,
                   const std::string& start_date,
                   const std::string& end_date,
                   int num_threads,
                   const std::vector<std::map<std::string, std::string>>& customer_data,
                   const std::vector<std::map<std::string, std::string>>& orders_data,
                   const std::vector<std::map<std::string, std::string>>& lineitem_data,
                   const std::vector<std::map<std::string, std::string>>& supplier_data,
                   const std::vector<std::map<std::string, std::string>>& nation_data,
                   const std::vector<std::map<std::string, std::string>>& region_data,
                   std::map<std::string, double>& results);

// Writes query results to the given output file.
// Returns true if successful.
bool outputResults(const std::string& result_path,
                   const std::map<std::string, double>& results);

#endif // QUERY5_HPP
