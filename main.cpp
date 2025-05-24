#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <cmath>
using namespace std;

class DataAnalyzer {
private:
    std::vector<std::vector<std::string>> data;
    std::vector<std::string> headers;
    std::string filename;

public:
    DataAnalyzer(const std::string& file) : filename(file) {}

    // Load CSV data
    bool loadData() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return false;
        }

        std::string line;
        bool isFirstLine = true;

        while (std::getline(file, line)) {
            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string cell;

            while (std::getline(ss, cell, ',')) {
                // Remove leading/trailing whitespace
                cell.erase(0, cell.find_first_not_of(" \t"));
                cell.erase(cell.find_last_not_of(" \t") + 1);
                row.push_back(cell);
            }

            if (isFirstLine) {
                headers = row;
                isFirstLine = false;
            } else {
                data.push_back(row);
            }
        }

        file.close();
        std::cout << "Data loaded successfully! " << data.size() << " rows, "
                  << headers.size() << " columns." << std::endl;
        return true;
    }

    // Display basic information about the dataset
    void displayInfo() {
        std::cout << "\n=== DATASET INFORMATION ===" << std::endl;
        std::cout << "File: " << filename << std::endl;
        std::cout << "Rows: " << data.size() << std::endl;
        std::cout << "Columns: " << headers.size() << std::endl;
        std::cout << "\nColumn Headers:" << std::endl;
        for (size_t i = 0; i < headers.size(); ++i) {
            std::cout << i + 1 << ". " << headers.at(i) << std::endl;
        }
    }

    // Display first few rows
    void displayHead(int n = 5) {
        std::cout << "\n=== FIRST " << n << " ROWS ===" << std::endl;

        // Print headers
        for (const auto& header : headers) {
            std::cout << std::setw(15) << header;
        }
        std::cout << std::endl;

        // Print separator
        for (size_t i = 0; i < headers.size(); ++i) {
            std::cout << std::setw(15) << "---------------";
        }
        std::cout << std::endl;

        // Print data rows
        for (int i = 0; i < std::min(n, static_cast<int>(data.size())); ++i) {
            for (const auto& cell : data.at(i)) {
                std::cout << std::setw(15) << cell;
            }
            std::cout << std::endl;
        }
    }

    // Check if a column contains numeric data
    bool isNumericColumn(int columnIndex) {
        if (columnIndex < 0 || columnIndex >= static_cast<int>(headers.size())) {
            return false;
        }

        for (const auto& row : data) {
            if (columnIndex < static_cast<int>(row.size())) {
                try {
                    std::stod(row[columnIndex]);
                } catch (const std::exception&) {
                    return false;
                }
            }
        }
        return true;
    }

    // Get numeric values from a column
    std::vector<double> getNumericColumn(int columnIndex) {
        std::vector<double> values;
        if (columnIndex < 0 || columnIndex >= static_cast<int>(headers.size())) {
            return values;
        }

        for (const auto& row : data) {
            if (columnIndex < static_cast<int>(row.size())) {
                try {
                    values.push_back(std::stod(row[columnIndex]));
                } catch (const std::exception&) {
                    // Skip non-numeric values
                }
            }
        }
        return values;
    }

    // Calculate statistical measures
    void analyzeColumn(int columnIndex) {
        if (columnIndex < 0 || columnIndex >= static_cast<int>(headers.size())) {
            std::cout << "Invalid column index!" << std::endl;
            return;
        }

        std::cout << "\n=== ANALYSIS FOR: " << headers.at(columnIndex) << " ===" << std::endl;

        if (isNumericColumn(columnIndex)) {
            auto values = getNumericColumn(columnIndex);
            if (values.empty()) {
                std::cout << "No numeric data found in this column." << std::endl;
                return;
            }

            // Sort for median calculation
            std::sort(values.begin(), values.end());

            // Calculate statistics
            double sum = std::accumulate(values.begin(), values.end(), 0.0);
            double mean = sum / values.size();

            double median = values.size() % 2 == 0 ?
                           (values[values.size()/2 - 1] + values[values.size()/2]) / 2.0 :
                           values[values.size()/2];

            // Calculate standard deviation
            double variance = 0.0;
            for (auto value : values) {
                variance += (value - mean) * (value - mean);
            }
            variance /= values.size();
            double stdDev = std::sqrt(variance);

            std::cout << std::fixed << std::setprecision(2);
            std::cout << "Count: " << values.size() << std::endl;
            std::cout << "Mean: " << mean << std::endl;
            std::cout << "Median: " << median << std::endl;
            std::cout << "Min: " << values.front() << std::endl;
            std::cout << "Max: " << values.back() << std::endl;
            std::cout << "Standard Deviation: " << stdDev << std::endl;
            std::cout << "Sum: " << sum << std::endl;

        } else {
            // Categorical data analysis
            std::map<std::string, int> frequency;
            int totalCount = 0;

            for (const auto& row : data) {
                if (columnIndex < static_cast<int>(row.size())) {
                    frequency[row[columnIndex]]++;
                    totalCount++;
                }
            }

            std::cout << "Categorical Data Analysis:" << std::endl;
            std::cout << "Total entries: " << totalCount << std::endl;
            std::cout << "Unique values: " << frequency.size() << std::endl;
            std::cout << "\nFrequency distribution:" << std::endl;

            // Sort by frequency (descending)
            std::vector<std::pair<std::string, int>> sortedFreq(frequency.begin(), frequency.end());
            std::sort(sortedFreq.begin(), sortedFreq.end(),
                     [](const auto& a, const auto& b) { return a.second > b.second; });

            for (const auto& pair : sortedFreq) {
                double percentage = (static_cast<double>(pair.second) / totalCount) * 100;
                std::cout << std::setw(20) << pair.first << ": "
                         << std::setw(5) << pair.second << " ("
                         << std::fixed << std::setprecision(1) << percentage << "%)" << std::endl;
            }
        }
    }

    // Find correlations between numeric columns
    void findCorrelations() {
        std::cout << "\n=== CORRELATION ANALYSIS ===" << std::endl;
        std::vector<int> numericColumns;

        for (int i = 0; i < static_cast<int>(headers.size()); ++i) {
            if (isNumericColumn(i)) {
                numericColumns.push_back(i);
            }
        }

        if (numericColumns.size() < 2) {
            std::cout << "Need at least 2 numeric columns for correlation analysis." << std::endl;
            return;
        }

        std::cout << "Numeric columns found: ";
        for (auto col : numericColumns) {
            std::cout << headers.at(col) << " ";
        }
        std::cout << std::endl << std::endl;

        // Calculate correlations
        for (size_t i = 0; i < numericColumns.size(); ++i) {
            for (size_t j = i + 1; j < numericColumns.size(); ++j) {
                auto values1 = getNumericColumn(numericColumns.at(i));
                auto values2 = getNumericColumn(numericColumns.at(j));

                if (values1.size() == values2.size() && !values1.empty()) {
                    double correlation = calculateCorrelation(values1, values2);
                    std::cout << headers[numericColumns.at(i)] << " <-> "
                             << headers[numericColumns.at(j)] << ": "
                             << std::fixed << std::setprecision(3) << correlation << std::endl;
                }
            }
        }
    }

    // Calculate Pearson correlation coefficient
    double calculateCorrelation(const std::vector<double>& x, const std::vector<double>& y) {
        if (x.size() != y.size() || x.empty()) return 0.0;

        double meanX = std::accumulate(x.begin(), x.end(), 0.0) / x.size();
        double meanY = std::accumulate(y.begin(), y.end(), 0.0) / y.size();

        double numerator = 0.0, sumX2 = 0.0, sumY2 = 0.0;

        for (size_t i = 0; i < x.size(); ++i) {
            double dx = x.at(i) - meanX;
            double dy = y.at(i) - meanY;
            numerator += dx * dy;
            sumX2 += dx * dx;
            sumY2 += dy * dy;
        }

        double denominator = std::sqrt(sumX2 * sumY2);
        return denominator == 0.0 ? 0.0 : numerator / denominator;
    }

    // Search for specific values
    void searchData(const std::string& searchTerm) {
        std::cout << "\n=== SEARCH RESULTS FOR: '" << searchTerm << "' ===" << std::endl;
        int matchCount = 0;

        for (size_t i = 0; i < data.size(); ++i) {
            bool rowMatches = false;
            for (size_t j = 0; j < data.at(i).size(); ++j) {
                if (data.at(i).at(j).find(searchTerm) != std::string::npos) {
                    if (!rowMatches) {
                        std::cout << "Row " << (i + 1) << ": ";
                        for (const auto& cell : data.at(i)) {
                            std::cout << std::setw(15) << cell;
                        }
                        std::cout << std::endl;
                        rowMatches = true;
                        matchCount++;
                    }
                }
            }
        }
        std::cout << "Found " << matchCount << " matching rows." << std::endl;
    }

    // Export filtered data
    void exportData(const std::string& outputFile, int columnIndex, const std::string& condition, double value) {
        std::ofstream file(outputFile);
        if (!file.is_open()) {
            std::cerr << "Error: Could not create output file " << outputFile << std::endl;
            return;
        }

        // Write headers
        for (size_t i = 0; i < headers.size(); ++i) {
            file << headers.at(i);
            if (i < headers.size() - 1) file << ",";
        }
        file << std::endl;

        // Write filtered data
        int exportedRows = 0;
        for (const auto& row : data) {
            if (columnIndex >= 0 && columnIndex < static_cast<int>(row.size())) {
                try {
                    double cellValue = std::stod(row[columnIndex]);
                    bool shouldExport = false;

                    if (condition == ">" && cellValue > value) shouldExport = true;
                    else if (condition == "<" && cellValue < value) shouldExport = true;
                    else if (condition == "=" && std::abs(cellValue - value) < 0.001) shouldExport = true;
                    else if (condition == ">=" && cellValue >= value) shouldExport = true;
                    else if (condition == "<=" && cellValue <= value) shouldExport = true;

                    if (shouldExport) {
                        for (size_t i = 0; i < row.size(); ++i) {
                            file << row[i];
                            if (i < row.size() - 1) file << ",";
                        }
                        file << std::endl;
                        exportedRows++;
                    }
                } catch (const std::exception&) {
                    // Skip non-numeric values
                }
            }
        }

        file.close();
        std::cout << "Exported " << exportedRows << " rows to " << outputFile << std::endl;
    }
};

void printMenu() {
    std::cout << "\n=== DATA ANALYZER MENU ===" << std::endl;
    std::cout << "1. Display dataset information" << std::endl;
    std::cout << "2. Show first few rows" << std::endl;
    std::cout << "3. Analyze specific column" << std::endl;
    std::cout << "4. Find correlations" << std::endl;
    std::cout << "5. Search data" << std::endl;
    std::cout << "6. Export filtered data" << std::endl;
    std::cout << "7. Exit" << std::endl;
    std::cout << "Choose an option: ";
}

int main() {
    std::cout << "=== C++ DATA ANALYZER ===" << std::endl;
    std::cout << "Created by: dhrumil246" << std::endl;
    std::cout << "Date: 2025-05-24" << std::endl;

    std::string filename;
    std::cout << "\nEnter CSV filename: ";
    std::getline(std::cin, filename);

    DataAnalyzer analyzer(filename);

    if (!analyzer.loadData()) {
        return 1;
    }

    int choice;
    while (true) {
        printMenu();
        std::cin >> choice;
        std::cin.ignore(); // Clear the newline

        switch (choice) {
            case 1:
                analyzer.displayInfo();
                break;
            case 2: {
                int n;
                std::cout << "How many rows to display? ";
                std::cin >> n;
                analyzer.displayHead(n);
                break;
            }
            case 3: {
                int column;
                std::cout << "Enter column number to analyze: ";
                std::cin >> column;
                analyzer.analyzeColumn(column - 1); // Convert to 0-based index
                break;
            }
            case 4:
                analyzer.findCorrelations();
                break;
            case 5: {
                std::string searchTerm;
                std::cout << "Enter search term: ";
                std::cin.ignore();
                std::getline(std::cin, searchTerm);
                analyzer.searchData(searchTerm);
                break;
            }
            case 6: {
                std::string outputFile, condition;
                int column;
                double value;
                std::cout << "Enter output filename: ";
                std::cin >> outputFile;
                std::cout << "Enter column number for filtering: ";
                std::cin >> column;
                std::cout << "Enter condition (>, <, =, >=, <=): ";
                std::cin >> condition;
                std::cout << "Enter value: ";
                std::cin >> value;
                analyzer.exportData(outputFile, column - 1, condition, value);
                break;
            }
            case 7:
                std::cout << "Thank you for using the Data Analyzer!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    }

    return 0;
}