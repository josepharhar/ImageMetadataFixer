#include <stdio.h>

#include <iostream>

#include <exiv2/exiv2.hpp>

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: %s <image>\n", argv[0]);
    return 0;
  }

  Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(argv[1]);
  if (!image.get()) {
    printf("!image.get()\n");
    return 1;
  }
  image->readMetadata();

  Exiv2::ExifData& exif_data = image->exifData();
  if (exif_data.empty()) {
    printf("no exif data found in file\n");
    return 1;
  }

  for (Exiv2::ExifData::const_iterator it = exif_data.begin(); it != exif_data.end(); it++) {
    const char* type_name = it->typeName();
    printf("%s 0x%lX %s %d", it->key(), it->tag(), type_name ? type_name : "Unknown", it->count());
    std::cout << it->value();
    printf("\n");
  }

  return 0;
}
