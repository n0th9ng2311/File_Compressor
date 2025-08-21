//
// Created by n0ht9ng on 16-07-2025.
//

#ifndef COMMON_H
#define COMMON_H

#include<iostream>
#include<array>
#include<cstdint>
#include<cstring>
#include<vector>
#include<string>
#include<string_view>
#include<fstream>
#include<optional>
#include<memory>
#include<chrono>
#include<filesystem>
#include<map>
#include<unordered_map>
#include<algorithm>


//Function to print size of original and compressed file with the size of compressed file
//being displayed as a % of original size as well
namespace fs = std::filesystem ;
void inline printCompSize(const fs::path& input_path,const fs::path& output_path) {
    const auto inputF_size  = file_size(input_path);
    const auto outputF_size = file_size(output_path);
    const double reduction_percent  = (static_cast<double>(outputF_size) / inputF_size) * 100.0;

    std::cout<< "\nOriginal Size: "<<inputF_size << " bytes"
             << "\nCompressed Size: "<<outputF_size << " bytes"
             << "\nSize reduced to: "<< reduction_percent <<"% of original ";
}
#endif //COMMON_H
