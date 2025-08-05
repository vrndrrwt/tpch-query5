# TPC-H Benchmark Implementation

This project is an implementation of the **TPC-H Query Benchmark**, a decision support benchmark consisting of a suite of business-oriented ad-hoc queries and concurrent data modifications.

## 📌 Overview

This repository implements **TPC-H Query 5** using C++ with multithreading support. It enables efficient querying on large datasets generated using the TPC-H data generator (`dbgen`).

## 📁 Project Structure

tpch-query5/
├── CMakeLists.txt # Build configuration
├── main.cpp # Entry point
├── query5.cpp # Implementation of TPC-H Query 5
├── query5.h # Header file
├── utils/ # Utility functions (e.g., CSV parser, file loader)
├── tpch_tables/ # Directory to store .tbl data files
├── build/ # Compiled binaries
└── README.md # This file

bash
Copy
Edit

## 🚀 Features

- ✅ TPC-H Query 5 implementation
- ✅ Multi-threaded execution using C++ threads
- ✅ Support for scale factor (via dbgen)
- ✅ CSV/pipe-delimited `.tbl` data loader
- ✅ Customizable filters: region, date range, threads

## ⚙️ Setup Instructions

### 1. Clone the Repository

```bash
git clone https://github.com/<your-username>/tpch-query5.git
cd tpch-query5
2. Generate TPC-H Data
Use the TPC-H dbgen tool to generate the .tbl files.

bash
Copy
Edit
git clone https://github.com/electrum/tpch-dbgen.git
cd tpch-dbgen
make
./dbgen -s 2     # Example: generates data for scale factor 2
mv *.tbl /path/to/tpch-query5/tpch_tables/
3. Build the Project
bash
Copy
Edit
mkdir build && cd build
cmake ..
make
4. Run the Query
bash
Copy
Edit
./tpch_query5 \
  --r_name=ASIA \
  --start_date=1994-01-01 \
  --end_date=1995-01-01 \
  --threads=4 \
  --table_path=/full/path/to/tpch_tables \
  --result_path=/full/path/to/output.txt
✅ Command Line Options
Flag	Description
--r_name	Region name (e.g., ASIA)
--start_date	Start date (e.g., 1994-01-01)
--end_date	End date (e.g., 1995-01-01)
--threads	Number of threads (e.g., 1, 4, etc.)
--table_path	Absolute path to folder with .tbl files
--result_path	File to save output results

🧪 Performance
You can compare performance with different thread counts:

bash
Copy
Edit
# Single-threaded
./tpch_query5 --threads=1 ...

# Multi-threaded (4 threads)
./tpch_query5 --threads=4 ...
📚 References
TPC-H Benchmark Specification

dbgen Tool

TPC-H Query 5 Description

🛠️ TODOs
 Implement more TPC-H queries (Q1–Q22)

 Optimize memory usage

 Add unit tests and benchmarks

 Add support for SQLite/PostgreSQL for comparison

👨‍💻 Author
Virendra Singh Rawat
