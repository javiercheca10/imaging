/**
 * @file MPTools.h
 * @brief Compatibility utilities for NetBeans and MP standalone builds
 */

#ifndef MPTOOLS_H
#define MPTOOLS_H

#include <string>
#include <fstream>
#include <cstdlib>

const std::string SLASH = "/";

inline void mpSavePGMImage(const std::string& filename, const unsigned char* buffer, int width, int height) {
    std::ofstream fo(filename.c_str());
    if (!fo) return;
    fo << "P2\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < width * height; ++i) {
        fo << (int)buffer[i] << " ";
    }
}

inline void mpDisplayImage(const std::string& filename, const std::string& title) {
    // Utility display function (calls ImageMagick display if available in GUI environment)
    #if defined(__linux__) || defined(__APPLE__)
    std::string cmd = "which display >/dev/null 2>&1 && display -title \"" + title + "\" " + filename + " >/dev/null 2>&1 &";
    (void)!std::system(cmd.c_str());
    #endif
}

inline size_t mphash(const unsigned char* buffer, size_t size) {
    // djb2 hash algorithm
    size_t hash = 5381;
    for (size_t i = 0; i < size; ++i) {
        hash = ((hash << 5) + hash) + buffer[i];
    }
    return hash;
}

#endif // MPTOOLS_H
