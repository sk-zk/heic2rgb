#include "heif.h"
#include "ISOBMFF.hpp"

void getHeader(std::shared_ptr<ISOBMFF::File>& file, std::vector<uint8_t>& buffer) {
    auto meta = file->GetTypedBox<ISOBMFF::META>("meta");
    auto iprp = meta->GetTypedBox<ISOBMFF::ContainerBox>("iprp");
    auto ipco = iprp->GetTypedBox<ISOBMFF::IPCO>("ipco");
    auto hvcC = ipco->GetTypedBox<ISOBMFF::HVCC>("hvcC");
    auto arrays = hvcC->GetArrays();
    for (auto& array : arrays) {
        auto data = array->GetNALUnits()[0]->GetData();
        buffer.push_back((uint8_t)0);
        buffer.push_back((uint8_t)0);
        buffer.push_back((uint8_t)0);
        buffer.push_back((uint8_t)1);
        buffer.insert(buffer.end(), data.begin(), data.end());
    }
}

void getImageData(std::vector<uint8_t>& fileBuffer, std::shared_ptr<ISOBMFF::File>& file, std::vector<uint8_t>& buffer)
{
    auto meta = file->GetTypedBox<ISOBMFF::META>("meta");
    auto iloc = meta->GetTypedBox <ISOBMFF::ILOC>("iloc");

    auto items = iloc->GetItems();
    auto& mainImg = items[0];
    auto extents = mainImg->GetExtents();
    uint64_t offset = extents[0]->GetOffset();
    uint64_t length = extents[0]->GetLength();

    auto headerSize = buffer.size();
    buffer.push_back((uint8_t)0);
    buffer.push_back((uint8_t)0);
    buffer.push_back((uint8_t)0);
    buffer.push_back((uint8_t)1);
    auto begin = fileBuffer.begin() + offset + 4;
    buffer.insert(buffer.end(), begin, fileBuffer.end());
}
