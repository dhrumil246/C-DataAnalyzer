# C++ Data Analyzer

A comprehensive command-line data analysis tool built in C++ that can process CSV files and provide statistical insights.

## Features

### 📊 **Core Analytics**
- **Dataset Overview**: Display basic information about your data
- **Statistical Analysis**: Calculate mean, median, standard deviation, min/max for numeric columns
- **Categorical Analysis**: Frequency distribution and percentage breakdowns
- **Correlation Analysis**: Find relationships between numeric variables

### 🔍 **Data Exploration**
- **Data Preview**: View first N rows of your dataset
- **Search Functionality**: Find specific values across all columns
- **Data Filtering**: Export subsets based on conditions

### 📈 **Output Features**
- **Formatted Display**: Clean, tabular output for easy reading
- **Export Capability**: Save filtered results to new CSV files
- **Interactive Menu**: User-friendly command-line interface

## Building the Project

```bash
# Compile using the Makefile
make

# Or compile manually
g++ -std=c++17 -Wall -Wextra -O2 -o data_analyzer main.cpp
```

## Usage

1. **Run the program**:
   ```bash
   ./data_analyzer
   ```

2. **Enter your CSV filename** when prompted

3. **Use the interactive menu** to explore your data:
   - View dataset information
   - Analyze specific columns
   - Find correlations
   - Search for specific values
   - Export filtered data

## Sample Data

The project includes `sample_data.csv` with employee information to test the analyzer. You can use your own CSV files as well.

## Example Analysis Output

```
=== ANALYSIS FOR: Salary ===
Count: 10
Mean: 56800.00
Median: 56500.00
Min: 45000.00
Max: 70000.00
Standard Deviation: 8234.56
Sum: 568000.00

=== CORRELATION ANALYSIS ===
Age <-> Salary: 0.743
Age <-> Years_Experience: 0.856
Salary <-> Performance_Score: 0.621
```

## Project Impact

### 🎯 **Technical Skills Demonstrated**
- **Object-Oriented Programming**: Clean class design with encapsulation
- **File I/O**: CSV parsing and data export
- **STL Usage**: Vectors, maps, algorithms, and iterators
- **Mathematical Computing**: Statistical calculations and correlations
- **Memory Management**: Efficient data structures
- **Error Handling**: Robust input validation

### 🚀 **Real-World Applications**
- Business data analysis
- Research data processing
- Financial data insights
- Performance metrics analysis
- Quality assurance reporting

### 📚 **Learning Outcomes**
- Data structure selection for performance
- Algorithm implementation for statistics
- User interface design for CLI applications
- Code organization and maintainability
- Testing with real datasets

## Extending the Project

**Next Steps to Make It Even More Impactful**:
1. Add data visualization (ASCII charts)
2. Implement machine learning algorithms (linear regression)
3. Support for JSON and other data formats
4. Multi-threading for large datasets
5. Database connectivity
6. Web interface using C++ web frameworks

## Author
**dhrumil246** - Created on 2025-05-24

---
*This project demonstrates practical C++ skills while solving real data analysis problems.*
