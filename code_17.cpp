#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <algorithm> // For std::sort
#include <string>

class StockPriceFinder {
public:
    // Constructor to initialize the StockPriceFinder with a filename
    StockPriceFinder(const std::string& filename) {
        readPricesFromFile(filename);
        filterNegativeValues();
        std::sort(prices_.begin(), prices_.end()); // Ensure prices are sorted before using binary search
    }

     bool checkInt(const std::string& value) {
            int ret = 0;
            try
            {
                ret = std::stoi(value);
            }
            catch (std::exception& e)
            {
                return false;
            }
            return  true;
     }

    // Function to read prices from a file into a vector
    void readPricesFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open the file.");
        }

        std::string price;
        while (std::getline(file, price)) {
            if (checkInt(price))
            {
              int val = std::stoi(price);
              if(val > 0)
              prices_.push_back(val);
            }
        }

        file.close();
    }

    // Function to filter out negative values from the price list
    void filterNegativeValues() {
        prices_.erase(std::remove_if(prices_.begin(), prices_.end(), [](int price) {
            return price <= 0;
            }), prices_.end());
    }

    // Function to find the closest price below the specified threshold
    int findClosestPriceBelowThreshold(int threshold) {
        if (prices_.empty()) {
            throw std::runtime_error("The price list is empty.");
        }

        if (threshold <= 0) {
            throw std::invalid_argument("Threshold must be a positive number.");
        }

        int left = 0, right = prices_.size() - 1;
        int result = -1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (prices_[mid] < threshold) {
                result = prices_[mid]; // Store the price instead of index
                left = mid + 1; // Look for larger values that are still below the threshold
            }
            else {
                right = mid - 1;
            }
        }

        if (result == -1) {
            throw std::runtime_error("No price found below the threshold.");
        }

        return result; // Return the price, not the index
    }

private:
    std::vector<int> prices_;
};

int main() {

    {
        try {
            StockPriceFinder finder("prices.txt");
            int result = finder.findClosestPriceBelowThreshold(500000);
            std::cout << "Closest price below 500000: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    }

    {
        // Happy path test case 1
        std::ofstream file("prices1.txt");
        file << 100 << std::endl;
        file << 200 << std::endl;
        file << 150 << std::endl;
        file << 50 << std::endl;
        file << 250 << std::endl;
        file << 300 << std::endl;
        file.close();

        try {
            StockPriceFinder finder("prices1.txt");
            int result = finder.findClosestPriceBelowThreshold(200);
            std::cout << "Closest price below 200: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    }


    // Expected output: Closest price below 200: 150
    {
        // Happy path test case 2
        std::ofstream file("prices2.txt");
        file << "10" << std::endl;
        file << "20" << std::endl;
        file << "30" << std::endl;
        file << "40" << std::endl;
        file << "50" << std::endl;
        file << "60" << std::endl;
        file.close();

        try {
            StockPriceFinder finder("prices2.txt");
            int result = finder.findClosestPriceBelowThreshold(35);
            std::cout << "Closest price below 35: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    }
    // Expected output: Closest price below 35: 30

    {
        // Edge case test 1: All negative prices
        std::ofstream file("prices3.txt");
        file << "-10" << std::endl;
        file << "-20" << std::endl;
        file << "-30" << std::endl;
        file << "-40" << std::endl;
        file << "-50" << std::endl;
        file.close();

        try {
            StockPriceFinder finder("prices3.txt");
            int result = finder.findClosestPriceBelowThreshold(10);
            std::cout << "Closest price below 10: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    }

    // Expected output: Exception: The price list is empty.
    {
        // Edge case test 2: Threshold lower than any price
        std::ofstream file("prices4.txt");
        file << "100" << std::endl;
        file << "200" << std::endl;
        file << "300" << std::endl;
        file << "400" << std::endl;
        file << "500" << std::endl;
        file.close();

        try {
            StockPriceFinder finder("prices4.txt");
            int result = finder.findClosestPriceBelowThreshold(50);
            std::cout << "Closest price below 50: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    }

    {
        try {
            std::ofstream file("empty_prices.txt");
            file.close();
            StockPriceFinder finder("empty_prices.txt"); // Assume "empty_prices.txt" is empty
            int result = finder.findClosestPriceBelowThreshold(100);
           
        }
        catch (const std::exception& e) {
            std::cout << "Test Passed! Error: " << e.what() << std::endl;  // Expected exception
        }
    }

    // Threshold Equal to Zero
    {
        std::ofstream file("prices_zero_threshold.txt");
        file << "100" << std::endl;
        file << "200" << std::endl;
        file << "300" << std::endl;
        file << "150" << std::endl;
        file << "50" << std::endl;
        file << "70" << std::endl;
        file.close();

        try {
            StockPriceFinder finder("prices_zero_threshold.txt");
            int result = finder.findClosestPriceBelowThreshold(0);
            std::cout << "Closest price below 50: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    }

    {
        std::ofstream file("prices_equal_price_in_the_list.txt");
        file << "100" << std::endl;
        file << "200" << std::endl;
        file << "300" << std::endl;
        file << "150" << std::endl;
        file << "50" << std::endl;
        file << "70" << std::endl;
        file.close();

        try {
            StockPriceFinder finder("prices_equal_price_in_the_list.txt");
            int result = finder.findClosestPriceBelowThreshold(150);
            std::cout << "Closest price below 150: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    }


    {
        std::ofstream file("prices_all_same.txt");
        file << "100" << std::endl;
        file << "100" << std::endl;
        file << "100" << std::endl;
        file << "100" << std::endl;
        file << "100" << std::endl;
        file << "100" << std::endl;
        file.close();

        try {
            StockPriceFinder finder("prices_all_same.txt");
            int result = finder.findClosestPriceBelowThreshold(100);
            std::cout << "Closest price below 100: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    }


    {
        std::ofstream file("prices_all_negative.txt");
        file << "-100" << std::endl;
        file << "-100" << std::endl;
        file << "-100" << std::endl;
        file << "-100" << std::endl;
        file << "-100" << std::endl;
        file << "-100" << std::endl;
        file.close();

        try {
            StockPriceFinder finder("prices_all_negative.txt");
            int result = finder.findClosestPriceBelowThreshold(100);
            std::cout << "Closest price below 100: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    }

    {
        std::ofstream file("prices_all_0s.txt");
        file << "0" << std::endl;
        file << "0" << std::endl;
        file << "0" << std::endl;
        file << "0" << std::endl;
        file << "0" << std::endl;
        file << "0" << std::endl;
        file.close();

        try {
            StockPriceFinder finder("prices_all_0s.txt");
            int result = finder.findClosestPriceBelowThreshold(100);
            std::cout << "Closest price below 100: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    }


    {
        std::ofstream file("prices_one.txt");
        file << "10" << std::endl;
        file.close();

        try {
            StockPriceFinder finder("prices_one.txt");
            int result = finder.findClosestPriceBelowThreshold(100);
            std::cout << "Closest price below 100: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    }

    {
        std::ofstream file("prices_not_integer.txt");
        file << "100.12" << std::endl;
        file << "0.3" << std::endl;
        file << "3.3" << std::endl;
        file << "1.2" << std::endl;
        file.close();

        try {
            StockPriceFinder finder("prices_not_integer.txt");
            int result = finder.findClosestPriceBelowThreshold(100);
            std::cout << "Closest price below 100: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    }

    {
        std::ofstream file("prices_mixed.txt");
        file << "100.12" << std::endl;
        file << "3" << std::endl;
        file << "33" << std::endl;
        file << "1.2" << std::endl;
        file.close();

        try {
            StockPriceFinder finder("prices_mixed.txt");
            int result = finder.findClosestPriceBelowThreshold(100);
            std::cout << "Closest price below 100: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    }

    return 0;

}
