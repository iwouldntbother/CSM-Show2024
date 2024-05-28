//
// Created by williamwestwood on 24/05/24.
//

#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include <memory>
#include <mutex>

#include "DataContainer.h"

class SharedData {
private:
    std::shared_ptr<DataContainer> dataContainer;
    std::mutex dataMutex;

public:
    SharedData() : dataContainer(std::make_shared<DataContainer>()) {}

    std::shared_ptr<DataContainer> getDataContainer() {
        std::lock_guard<std::mutex> lock(dataMutex);
        return dataContainer;
    }
};

#endif //SHAREDDATA_H
