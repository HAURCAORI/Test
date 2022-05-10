#include "DataIO.h"

#include <fcntl.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>


bool memoryMap(std::string file_name) {
    std::string address = (std::string)PATH + file_name;
    int fd = open(address.c_str(), O_RDONLY);
    if(fd == -1) {
        return false;
    }

    struct stat buf;
    if (fstat(fd, &buf) < 0) {
        return false;
    }

    off_t offset_to_map(0);
	size_t length_to_map(buf.st_size);

    void* memory_area = mmap(address)
}

IOManager::IOManager() {
    pagefiles.reserve(DefaultReserve);
}
IOManager::~IOManager() {
    for(auto it = pagefiles.begin(); it != pagefiles.end(); it++) {
        fclose(it->stream);
    }
}
bool IOManager::createPage(PAGE id) {
    std::string address = (std::string)PATH + std::to_string(id);
    FILE *stream = fopen(address.c_str(), "wb");
    if(!stream) { return false; }

    int dim = SectorDimension;
    int temp = SectorSize;

    fwrite(&dim, sizeof(int),1,stream);
    for(int i = 0 ; i < dim; i++)
    {
        fwrite(&temp, sizeof(int),1,stream);
    }
    fclose(stream);
    return true;
}
bool IOManager::loadPage(PAGE id) {
    if(pagefiles.find(PageFile(id)) != pagefiles.end()) { return false; }
    std::string address = (std::string)PATH + std::to_string(id);
    FILE *stream = fopen(address.c_str(), "r+");
    if(!stream) { return false; }
    auto it = pagefiles.insert(PageFile(id,std::move(stream)));
    return (it.second ? true : false);
}

bool IOManager::unloadPage(PAGE id) {
    //fclose(pages.find({id,nullptr})->stream);
    pagefiles.erase(PageFile(id));
    return true;
}

void IOManager::printPage() {
    printf("[Page List]\r\n");
    for(auto it = pagefiles.begin(); it != pagefiles.end(); it++) {
        printf("Page id[%d] \r\n", it->id);
    }
}

