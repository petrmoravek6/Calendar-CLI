#include "writer.h"
#include "fileException.h"
#include <string>
using namespace std;

CWriter::CWriter(const string & filename) {
    if(m_File.is_open())
        throw FileException("Error during opening the file");
    m_File.open(filename.c_str());
    if (!m_File)
        throw FileException("Error during opening the file");
}

bool CWriter::write(const string & str) {
    if(!m_File.is_open())
        return false;
    m_File.write(str.c_str(), str.length());
    if(!m_File)
        return false;
    return true;
}

bool CWriter::close() {
    if(!m_File.is_open())
        return false;
    m_File.close();
    if(!m_File)
        return false;
    return true;
}
