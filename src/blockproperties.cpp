#include "blockproperties.h"

std::map<int, BlockProperties> blockProperties;

void initializeBlockProperties() {
    // Add block properties to the map
    blockProperties[0] = {false, "selector.png"};
    blockProperties[1] = {true, "grass.png"};
    blockProperties[2] = {true, "dirt.png"};
    blockProperties[3] = {true, "stone.png"};
    // Add more block properties as needed
}
