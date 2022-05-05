#include "DataIO.h"


bool loadPage(PAGE id) {
    if(pagefile.find(PageFile(id)) != pagefile.end()) { return false; }
    auto it = pagefile.insert(PageFile(id));
    return (it.second ? true : false);
}

bool unloadPage(PAGE id) {
    //fclose(pages.find({id,nullptr})->stream);
    pagefile.erase(PageFile(id));
    return true;
}

void printPage() {
    printf("[Page List]\r\n");
    for(auto it = pagefile.begin(); it != pagefile.end(); it++) {
        printf("Page id[%d] \r\n", it->id);
    }
}

