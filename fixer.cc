#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <exiv2/exiv2.hpp>

struct DateTime {
  unsigned year;
  unsigned month;
  unsigned day;
  unsigned hour;
  unsigned minute;
  unsigned second;
};

static DateTime AddHours(DateTime date_time, int hours) {
}

static void TestAddHours() {
}

int main(int argc, char** argv) {
  if (argc != 3 && argc != 4) {
    printf("usage: %s <image> <offset> [commit]\n", argv[0]);
    printf("  received:");
    for (int i = 0; i < argc; i++) {
      printf(" %s", argv[i]);
    }
    printf("\n");
    return 0;
  }
  int offset = atoi(argv[2]);
  bool commit = false;
  if (argc > 3 && !strcmp(argv[3], "commit")) {
    commit = true;
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

  printf("fixing %s, offset: %d, commit: %d\n", argv[1], offset, commit);

  const char* tag_names[] = {
    "Exif.Image.DateTime",
    "Exif.Photo.DateTimeOriginal",
    "Exif.Photo.DateTimeDigitized"};

  for (int i = 0; i < 3; i++) {
    const char* tag_name = tag_names[i];
    Exiv2::Exifdatum& tag = exif_data[tag_name];
    std::string date = tag.toString();
    std::string old_date;
    old_date += date;

    const char date_format[] = "%04u:%02u:%02u %02u:%02u:%02u";
    unsigned year = 0, month = 0, day = 0, hours = 0, minutes = 0, seconds = 0;
    sscanf(date.c_str(), date_format, &year, &month, &day, &hours, &minutes, &seconds);

    int new_hours_int = ((int)hours) + offset;
    if (new_hours_int < 0) {
      printf("HOURS UNDERFLOW: %d\n", new_hours_int);
      day--;
      hours = hours + (24 + offset);

    } else if (new_hours_int >= 24) {
      printf("HOURS OVERFLOW: %d\n", new_hours_int);
      day++;
      hours = hours + (offset - 24);

    } else {
      hours += offset;
    }

    sprintf((char*)date.c_str(), date_format, year, month, day, hours, minutes, seconds);
    printf("%30s old: %s\n", tag_name, old_date.c_str());
    printf("%30s new: %s\n", tag_name, date.c_str());

    if (commit) {
      tag.setValue(date);
    }
  }

  image->setExifData(exif_data);
  image->writeMetadata();

  return 0;
}
