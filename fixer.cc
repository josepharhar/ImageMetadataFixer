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

  Exiv2::ExifData exif_data = image->exifData();
  if (exif_data.empty()) {
    printf("no exif data found in file\n");
    return 1;
  }

  /*for (Exiv2::ExifData::const_iterator it = exif_data.begin(); it != exif_data.end(); it++) {
    if (it->key() == "Exif.Image.DateTime"
        || it->key() == "Exif.Photo.DateTimeOriginal"
        || it->key() == "Exif.Photo.DateTimeDigitized") {
      printf("%30s %s\n", it->key().c_str(), it->value().c_str());
      const char* type_name = it->typeName();
      std::cout << it->key();
      printf(" 0x%lX %s %d", it->tag(), type_name ? type_name : "Unknown", it->count());
      std::cout << it->value();
      printf("\n");
    }
  }*/

  const char* tag_names[] = {
    "Exif.Image.DateTime",
    "Exif.Photo.DateTimeOriginal",
    "Exif.Photo.DateTimeDigitized"};

  for (int i = 0; i < 3; i++) {
    const char* tag_name = tag_names[i];
    Exiv2::Exifdatum& tag = exif_data[tag_name];
    std::string date = tag.toString();
    printf("%30s tostring: %s\n", tag_name, date.c_str());

    const char date_format[] = "%04u:%02u:%02u %02u:%02u:%02u";
    unsigned year = 0, month = 0, day = 0, hours = 0, minutes = 0, seconds = 0;
    sscanf(date.c_str(), date_format, &year, &month, &day, &hours, &minutes, &seconds);
    //printf("  year: %u, month: %u, day: %u, hours: %u, minutes: %u, seconds: %u\n", year, month, day, hours, minutes, seconds);

    hours -= 2;

    //printf("  year: %u, month: %u, day: %u, hours: %u, minutes: %u, seconds: %u\n", year, month, day, hours, minutes, seconds);
    sprintf((char*)date.c_str(), date_format, year, month, day, hours, minutes, seconds);

    printf("%30s      new: %s\n", tag_name, date.c_str());

    tag.setValue(date);
  }

  image->setExifData(exif_data);
  image->writeMetadata();

  return 0;
}
