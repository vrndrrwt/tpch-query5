// query5.cpp

#include "query5.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <vector>
#include <map>
#include <getopt.h>
#include <string>
#include <iomanip>

std::mutex result_mutex;

bool parseArgs(int argc, char* argv[], std::string& r_name, std::string& start_date, std::string& end_date, int& num_threads, std::string& table_path, std::string& result_path) {
    const char* const short_opts = "";
    const option long_opts[] = {
        {"r_name", required_argument, nullptr, 'r'},
        {"start_date", required_argument, nullptr, 's'},
        {"end_date", required_argument, nullptr, 'e'},
        {"threads", required_argument, nullptr, 't'},
        {"table_path", required_argument, nullptr, 'p'},
        {"result_path", required_argument, nullptr, 'o'},
        {nullptr, no_argument, nullptr, 0}
    };

    while (true) {
        int opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        if (opt == -1) break;

        switch (opt) {
            case 'r': r_name = optarg; break;
            case 's': start_date = optarg; break;
            case 'e': end_date = optarg; break;
            case 't': num_threads = std::stoi(optarg); break;
            case 'p': table_path = optarg; break;
            case 'o': result_path = optarg; break;
            default:
                std::cerr << "Invalid arguments\n";
                return false;
        }
    }

    // Final check
    if (r_name.empty() || start_date.empty() || end_date.empty() || num_threads <= 0 || table_path.empty() || result_path.empty()) {
        std::cerr << "Missing required arguments\n";
        return false;
    }

    return true;
}


// --------------------- Helper: CSV Reading ---------------------
bool readCSV(const std::string& filename, std::vector<std::map<std::string, std::string>>& data) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line, cell;
    std::vector<std::string> headers;

    if (std::getline(file, line)) {
        std::stringstream headerStream(line);
        while (std::getline(headerStream, cell, '|')) {
            headers.push_back(cell);
        }
    }

    while (std::getline(file, line)) {
        std::map<std::string, std::string> row;
        std::stringstream lineStream(line);
        size_t i = 0;
        while (std::getline(lineStream, cell, '|') && i < headers.size()) {
            row[headers[i++]] = cell;
        }
        if (!row.empty()) data.push_back(row);
    }

    return true;
}

// --------------------- Read TPCH Data ---------------------
bool readTPCHData(const std::string& table_path,
                  std::vector<std::map<std::string, std::string>>& customer_data,
                  std::vector<std::map<std::string, std::string>>& orders_data,
                  std::vector<std::map<std::string, std::string>>& lineitem_data,
                  std::vector<std::map<std::string, std::string>>& supplier_data,
                  std::vector<std::map<std::string, std::string>>& nation_data,
                  std::vector<std::map<std::string, std::string>>& region_data) {
    return readCSV(table_path + "/customer.tbl", customer_data) &&
           readCSV(table_path + "/orders.tbl", orders_data) &&
           readCSV(table_path + "/lineitem.tbl", lineitem_data) &&
           readCSV(table_path + "/supplier.tbl", supplier_data) &&
           readCSV(table_path + "/nation.tbl", nation_data) &&
           readCSV(table_path + "/region.tbl", region_data);
}

// --------------------- Query 5 Execution ---------------------
// ... (includes and other parts unchanged)

void processPartition(int thread_id,
                      int num_threads,
                      const std::vector<std::map<std::string, std::string>>& orders_data,
                      const std::vector<std::map<std::string, std::string>>& lineitem_data,
                      const std::vector<std::map<std::string, std::string>>& supplier_data,
                      const std::vector<std::map<std::string, std::string>>& customer_data,
                      const std::vector<std::map<std::string, std::string>>& nation_data,
                      const std::vector<std::map<std::string, std::string>>& region_data,
                      const std::string& r_name,
                      const std::string& start_date,
                      const std::string& end_date,
                      std::map<std::string, double>& results) {

    std::map<std::string, double> local_results;

    for (size_t i = thread_id; i < orders_data.size(); i += num_threads) {
        const auto& order = orders_data[i];
        std::string order_date = order.at("o_orderdate");
        if (order_date < start_date || order_date > end_date) continue;

        std::string custkey = order.at("o_custkey");

        auto cust_it = std::find_if(customer_data.begin(), customer_data.end(),
                                    [&](const std::map<std::string, std::string>& row) {
                                        return row.at("c_custkey") == custkey;
                                    });
        if (cust_it == customer_data.end()) continue;

        std::string nationkey = cust_it->at("c_nationkey");

        auto nation_it = std::find_if(nation_data.begin(), nation_data.end(),
                                      [&](const std::map<std::string, std::string>& row) {
                                          return row.at("n_nationkey") == nationkey;
                                      });
        if (nation_it == nation_data.end()) continue;

        std::string regionkey = nation_it->at("n_regionkey");

        auto region_it = std::find_if(region_data.begin(), region_data.end(),
                                      [&](const std::map<std::string, std::string>& row) {
                                          return row.at("r_regionkey") == regionkey && row.at("r_name") == r_name;
                                      });
        if (region_it == region_data.end()) continue;

        std::string orderkey = order.at("o_orderkey");

        for (const auto& line : lineitem_data) {
            if (line.at("l_orderkey") != orderkey) continue;

            std::string suppkey = line.at("l_suppkey");
            auto supp_it = std::find_if(supplier_data.begin(), supplier_data.end(),
                                        [&](const std::map<std::string, std::string>& row) {
                                            return row.at("s_suppkey") == suppkey;
                                        });

            if (supp_it == supplier_data.end()) continue;

            std::string nationkey2 = supp_it->at("s_nationkey");
            auto nation_it2 = std::find_if(nation_data.begin(), nation_data.end(),
                                           [&](const std::map<std::string, std::string>& row) {
                                               return row.at("n_nationkey") == nationkey2;
                                           });
            if (nation_it2 == nation_data.end()) continue;

            std::string nation_name = nation_it2->at("n_name");

            double extended_price = std::stod(line.at("l_extendedprice"));
            double discount = std::stod(line.at("l_discount"));
            double revenue = extended_price * (1.0 - discount);

            local_results[nation_name] += revenue;
        }
    }

    std::lock_guard<std::mutex> lock(result_mutex);
    for (const auto& pair : local_results) {
        results[pair.first] += pair.second;
    }
}

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
                   std::map<std::string, double>& results) {
    results.clear();
    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(processPartition, i, num_threads,
                             std::ref(orders_data),
                             std::ref(lineitem_data),
                             std::ref(supplier_data),
                             std::ref(customer_data),
                             std::ref(nation_data),
                             std::ref(region_data),
                             std::ref(r_name),
                             std::ref(start_date),
                             std::ref(end_date),
                             std::ref(results));
    }

    for (auto& t : threads) {
        t.join();
    }

    return true;
}

bool outputResults(const std::string& result_path, const std::map<std::string, double>& results) {
    std::ofstream out(result_path);
    if (!out.is_open()) return false;

    out << "nation|revenue\n";
    for (const auto& pair : results) {
        out << pair.first << "|" << std::fixed << std::setprecision(2) << pair.second << "\n";
    }

    return true;
}
