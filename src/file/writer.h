#pragma once
#include <fstream>
#include <string>

/** Class used for writing text into file */
class CWriter {
public:
    /** Throws FileException if an error during opening the file occurs **/
    CWriter(const std::string & filename);
    /**
     * @param str Text to be written in the file
     * @return False if an error during writing occurs, otherwise true
     */
    bool write(const std::string & str);
    /**
     *
     * @return False if an error during closing the file occurs, otherwise true
     */
    bool close();
private:
    std::ofstream m_File;
};


