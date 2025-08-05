#include "query5.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

// TODO: Include additional headers as needed

int main(int argc, char* argv[]) {
    std::string r_name, start_date, end_date, table_path, result_path;
    int num_threads;

    if (!parseArgs(argc, argv, r_name, start_date, end_date, num_threads, table_path, result_path)) {
        std::cerr << "Invalid arguments\n";
        return 1;
    }

    std::vector<std::map<std::string, std::string>> customer_data, orders_data, lineitem_data, supplier_data, nation_data, region_data;

    if (!readTPCHData(table_path, customer_data, orders_data, lineitem_data, supplier_data, nation_data, region_data)) {
        std::cerr << "Error reading TPCH data\n";
        return 1;
    }

    std::map<std::string, double> results;
    if (!executeQuery5(r_name, start_date, end_date, num_threads, customer_data, orders_data, lineitem_data, supplier_data, nation_data, region_data, results)) {
        std::cerr << "Error executing query\n";
        return 1;
    }

    if (!outputResults(result_path, results)) {
        std::cerr << "Error writing results\n";
        return 1;
    }

    std::cout << "Query 5 completed successfully.\n";
    return 0;
}
