#include "DataIO.h"

#include <vector>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void writeFileData(FILE *stream) {
    int header = 4; 
    fwrite(&header, sizeof(int),1,stream);
    for(int i = 0 ; i < 1000; i++) {
        fwrite(&i, sizeof(int),1,stream);
    }
    /*
    int dim = SectorDimension;
    int temp = SectorSize;

    fwrite(&dim, sizeof(int),1,stream);
    for(int i = 0 ; i < dim; i++)
    {
        fwrite(&temp, sizeof(int),1,stream);
    }
    */
}

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

    writeFileData(stream);

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
    int dimension = 0;
    std::vector<int> dimSizes;
    if(read(fd, &offset_header, sizeof(int)) > 0) {
        dimension = offset_header - 1;
        dimSizes.reserve(dimension);
        int temp;
        for(int i = 0; i < dimension; i++) {
            if(read(fd, &temp, sizeof(int)) > 0) {
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
    fs.size_header = offset_header;
    fs.size_mapped = length_to_map;

    auto it = pagefiles.insert(PageFile(id, std::move(fs), dimension, dimSizes));
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
