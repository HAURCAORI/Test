#include "DataIO.h"
#include "Calculate.h"
#include "Mapping.h"
#include <vector>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//#define Point(i,j,k) (header + sizeof(Neuron)*(i + dimSizes[0]*j + dimSizes[0]*dimSizes[1]*k))

namespace DataIO
{

IOManager::IOManager() {
    pagefiles.reserve(DefaultReserve);
}
IOManager::~IOManager() {
    for(auto it = pagefiles.begin(); it != pagefiles.end(); it++) {
        unloadPage(*it);
    }
    printf("[Process] : Remove Pages\r\n");
}
bool IOManager::createPage(PAGE id) {
    std::string address = (std::string)PATH + std::to_string(id);
    FILE *stream = fopen(address.c_str(), "wb");
    if(!stream) { return false; }

    const int header = (1+SectorDimension)*4; //4byte(사이즈 1, 차원 3)
    std::vector<unsigned int> dimSizes{10,10,10}; //첫 번째 원소가 첫번째 차원 값

    unsigned int data_size = 1;
    fwrite(&header, sizeof(int),1,stream);
    for(unsigned int x : dimSizes) {
        fwrite(&x, sizeof(unsigned int), 1, stream);
        data_size *= x;
    }

    Neuron temp = Mapping::createNeuron();
    fwrite(&temp, sizeof(Neuron), data_size, stream);

    fclose(stream);
    return true;
}

bool IOManager::deletePage(PAGE id) {
    std::string address = (std::string)PATH + std::to_string(id);
    if(remove(address.c_str()) < 0) {
        printf("error : delete file.\r\n");
        return false;
    }
    return true;
}
bool IOManager::loadPage(PAGE id) {
    std::string address = (std::string)PATH + std::to_string(id);
    int fd = open(address.c_str(), O_RDWR);
    if(fd == -1) {
        printf("error : opening file.\r\n");
        return false;
    }

    struct stat buf;
    if (fstat(fd, &buf) < 0) {
        printf("error : stat file.\r\n");
        return false;
    }

    
    off_t offset_header = 0;
	size_t length_to_map(buf.st_size);
    unsigned int dimension = 0;
    std::vector<unsigned int> dimSizes;
    if(read(fd, &offset_header, sizeof(unsigned int)) > 0) {
        dimension = (offset_header - 4) / 4; //bytes
        dimSizes.reserve(dimension);
        unsigned int temp;
        for(unsigned int i = 0; i < dimension; i++) {
            if(read(fd, &temp, sizeof(unsigned int)) > 0) {
                dimSizes.push_back(temp);
            }
        }
    }

    void* memory_area = mmap(0, length_to_map, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memory_area == MAP_FAILED) {
        printf("error : map failed.\r\n");
        return false;
    }

    FileStruct fs;
    fs.fd = fd;
    fs.memory_area = std::move(memory_area);
    fs.size_header = offset_header; //bytes
    fs.size_mapped = length_to_map;

    DataStruct ds;
    ds.dimension = dimension;
    ds.dimSizes = std::move(dimSizes);

    auto it = pagefiles.insert(PageFile(id, std::move(fs), std::move(ds)));
    return (it.second ? true : false);
    /*
    if(pagefiles.find(PageFile(id)) != pagefiles.end()) { return false; }
    std::string address = (std::string)PATH + std::to_string(id);
    FILE *stream = fopen(address.c_str(), "r+");
    if(!stream) { return false; }
    auto it = pagefiles.insert(PageFile(id,std::move(stream)));
    return (it.second ? true : false);
    */
}

bool IOManager::unloadPage(PAGE id) {
    //fclose(pages.find({id,nullptr})->stream);
    auto temp = pagefiles.find(id);
    if(munmap(temp->fs.memory_area, temp->fs.size_mapped) < 0) {
        printf("error : munmap.\r\n");
        return false;
    }
    if(close(temp->fs.fd)) {
        printf("error : closing file.\r\n");
        return false;
    }
    pagefiles.erase(PageFile(id));
    return true;
}

bool IOManager::unloadPage(PageFile pf) {
    if(pf.fs.memory_area) {
        if(munmap(pf.fs.memory_area,pf.fs.size_mapped) < 0) {
            printf("error : munmap.\r\n");
            return false;
        }
    }
    if(pf.fs.fd != -1) {
        if(close(pf.fs.fd)) {
            printf("error : closing file.\r\n");
            return false;
        }
    }
    pagefiles.erase(pf);
    return true;
}

void IOManager::printPage() {
    printf("[Page List]\r\n");
    for(auto it = pagefiles.begin(); it != pagefiles.end(); it++) {
        printf("Page id[%d] \r\n", it->id);
    }
}

} // namespace DataIO