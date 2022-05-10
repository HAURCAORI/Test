#include "DataIO.h"

#include <fcntl.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

IOManager::IOManager() {
    pagefiles.reserve(DefaultReserve);
}
IOManager::~IOManager() {
    printf("[Process] : Remove Pages\r\n");
    for(auto it = pagefiles.begin(); it != pagefiles.end(); it++) {
        unloadPage(*it);
    }
}
bool IOManager::createPage(PAGE id) {
    std::string address = (std::string)PATH + std::to_string(id);
    FILE *stream = fopen(address.c_str(), "wb");
    if(!stream) { return false; }

    int dim = SectorDimension;
    int temp = SectorSize;

    fwrite(&id, sizeof(int),1,stream);
    fwrite(&dim, sizeof(int),1,stream);
    for(int i = 0 ; i < dim; i++)
    {
        fwrite(&temp, sizeof(int),1,stream);
    }
    fclose(stream);
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

    off_t offset_to_map(0);
	size_t length_to_map(buf.st_size);

    void* memory_area = mmap(0, length_to_map, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset_to_map);
    if (memory_area == MAP_FAILED) {
        printf("error : map failed.\r\n");
        return false;
    }

    auto it = pagefiles.insert(PageFile(id, fd,std::move(memory_area),length_to_map));
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
    if(munmap(temp->memory_area, temp->size_mapped) < 0) {
        printf("error : munmap.\r\n");
        return false;
    }
    if(close(temp->fd)) {
        printf("error : closing file.\r\n");
        return false;
    }
    pagefiles.erase(PageFile(id));
    return true;
}

bool IOManager::unloadPage(PageFile pf) {
    if(pf.memory_area) {
        if(munmap(pf.memory_area,pf.size_mapped) < 0) {
            printf("error : munmap.\r\n");
            return false;
        }
    }
    if(pf.fd != -1) {
        if(close(pf.fd)) {
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

