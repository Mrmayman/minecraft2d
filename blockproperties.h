#include <map>
#include <string>

struct BlockProperties {
    bool isBlock;
    std::string texture;
    // Add more properties as needed
};

extern std::map<int, BlockProperties> blockProperties;

void initializeBlockProperties();
